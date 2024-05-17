// Copyright 2024 Steven Buytaert

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include <codec-int.h>
#include <hci-types-5.4.h>

#define NUM(A) (sizeof(A) / sizeof(A[0]))

typedef struct CodecCtx_t * cctx_t;

typedef struct Fixup_t {
  const uint8_t *       src;      // From where to copy the data.
  void * *              ref;      // Where to write the destination pointer.
  uint16_t              num;      // Number of bytes to copy.
  uint8_t               delayed;  // Non zero when a delayed fix.
} Fixup_t;

typedef struct Stream_t * stream_t;

typedef struct Stream_t {
  const uint8_t * const start;    // Start of the block.
  uint8_t *             cur;      // Current position in the block.
  const uint8_t * const limit;    // First byte outside of the block.
  uint16_t *            status;
} Stream_t;

typedef void (*ccopy_t)(void * dst, const void * src, uint32_t sz);

typedef struct CodecCtx_t {       // Codec Context.
  Stream_t              Src;
  Stream_t              Dst;
  uint8_t *             befskip;  // Last read src position before skip (encoding).
  uint8_t               decoding; // Non zero when decoding.
  uint8_t               fixcap;   // Fixup capacity.
  uint8_t               bitsset;  // When non zero; use bitset.
  uint8_t               numc;     // Codec instruction stream length.
  codec_t               codec;    // Codec instruction stream.
  ccopy_t               ccopy;    // memcpy for code; either dummy or real.
  te_t                  entry;    // Entry with codec information.
  Fixup_t *             Fixup;    // Fixups[fixcap].
} CodecCtx_t;

static te_t search4te(ctab_t tab, te_t key, cmpte_t cf) {

  int32_t low = 0;                                          // Must be signed !
  int32_t mid;                                              // Must be signed !
  int32_t high = tab->numtab - 1;                           // Must be signed !
  te_t    e = NULL;
  int32_t com;

  while (low <= high) {                                     // Simple binary search
    mid = (low + high) / 2;                                 // Divide space to be searched in two parts
    assert(mid >= 0 && mid < (int) tab->numtab);            // Let's check anyway, we told them they must be s
    com = cf(key, & tab->table[mid]);                       // Calculate comparison value
    if (0 > com) {
      high = mid - 1;                                       // Search lower half
    }
    else if (0 < com) {
      low = mid + 1;                                        // Search higher half
    }
    else {
      e = & tab->table[mid]; break;                         // Found it, stop searching
    }
  }
  
  return e;

}

static uint32_t ru16(const uint8_t pkt[]) {                 // Read an uint16_t size from the packet.
  return (uint32_t) ((pkt[1] << 8) | pkt[0]);               // Little endian encoding.
}

static ctab_t opcode2tab(const HCI_Opcode_t * opc) {

  if (opc->OGF && opc->OGF <= 8) {                          // OGF must be sane.
    return ctabs[opc->OGF];                                 // OGF determines the table.
  }

  return NULL;

}

static ctab_t tab4codec(const void * sop, uint32_t e4d, uint8_t tr[1]) {

  const uint8_t * u08 = (const uint8_t *) sop;
  uint8_t         type = u08 ? u08[0] : 0xff;               // Get the proper type first.
  HCI_Opcode_t    Opc;

  tr[0] = type;

  if (type && type <= type_ISO) {
    switch (type) {
      case type_CMD: {
        if (e4d) { Opc.opcode = ru16(& u08[1]); }           // Decoding: get it from the unaligned stream.
        else { Opc.opcode = ((HCI_Cmd_t *) u08)->opcode; }  // Encoding: get it from the structure.
        return opcode2tab(& Opc);
        break;
      }
      
      case type_EVT: {
        return ctabs[0];
        break;
      }
      
      default: break;
    }
  }

  return NULL;

}

typedef HCI_Command_Complete_Evt_t CCEvt_t;                 // Shorthand notation.

static te_t codec4any(const uint8_t any[], uint32_t e4d, ctab_t tr[1]) {  // Search proper entry for encoding or decoding.

  uint8_t        type;
  ctab_t         ctab = tab4codec(any, e4d, & type);
  TE_t           Key;
  te_t           fnd = NULL;
  HCI_Opcode_t   Opc;
  HCI_Opcode_t * opc = & Opc;
  
  if (ctab) {
    tr[0] = ctab;
    if (type_EVT == type) {
      const HCI_Evt_t * evt = (const HCI_Evt_t *) any;
      if (0x0e == evt->code) {                              // Command Complete Event.
        Opc.opcode = ru16(& any[4]);
        Key.OCF = Opc.OCF;
        Key.isret = 1;                                      // Search for corresponding return structure.
        ctab = opcode2tab(& Opc);
        if (ctab) {
          tr[0] = ctab;
          fnd = search4te(ctab, & Key, cmpcmd);
        }
      }
      else if (0x3e == evt->code) {                         // Search on the subevent.
        Key.code = evt->subevent[0];
        Key.issub = 1;
        fnd = search4te(ctab, & Key, cmpevt);
        assert(! fnd || fnd->issub);
      }
      else {                                                // Search on the event code.
        Key.code = evt->code;
        Key.issub = 0;
        fnd = search4te(ctab, & Key, cmpevt);
        assert(! fnd || ! fnd->issub);
      }
    }
    else if (type_CMD == type) {
      if (e4d) { opc->opcode = ru16(& any[1]); }            // Decoding: get it from the unaligned stream.
      else { opc->opcode = ((HCI_Cmd_t *) any)->opcode; }   // Encoding: get it from the structure.
      Key.OCF = opc->OCF;
      Key.isret = 0;                                        // Otherwise would be an event.
      fnd = search4te(ctab, & Key, cmpcmd);
    }
  }
  
  return fnd;

}

static te_t codec4enc(const void * s2e, ctab_t tr[1]) {     // Search proper entry for encoding.
  return codec4any(s2e, 0, tr);
}

static te_t codec4dec(const uint8_t pkt[], ctab_t tr[1]) {  // Search proper entry for decoding.
  return codec4any(pkt, 1, tr);
}

static void nocopy(void * dst, const void * src, uint32_t sz) { }

static void docopy(void * dst, const void * src, uint32_t sz) {

  if (sz) {
    assert(dst && src);
    memcpy(dst, src, sz);
  }

}

static uint32_t isActive(cctx_t ctx) {
  return (ctx->ccopy == docopy) ? 1 : 0;
}

__attribute__((no_sanitize("undefined")))
static void check4ReadFrom(cctx_t ctx, stream_t src, const uint8_t * from, uint32_t num) {

  if (num && isActive(ctx)) {
    if (! from) {
      ctx->ccopy = nocopy;
      src->status[0] = CReq_null_ptr;
    }
    else if (from + num > src->limit) {
      ctx->ccopy = nocopy;
      src->status[0] = CReq_too_Short;
    }
    else if (from < src->start) {
      ctx->ccopy = nocopy;
      src->status[0] = CReq_OOB;
    }
  }

}

static void check4Read(cctx_t ctx, stream_t src, uint32_t num) {

  if (src->cur + num > src->limit) {                        // Reading can go up to and include limit.
    ctx->ccopy = nocopy;
    src->status[0] = CReq_too_Short;
  }

}

static void check4Write(cctx_t ctx, stream_t dst, uint32_t num) {

  if (isActive(ctx)) {                                      // Only when active.
    if (dst->cur + num >= dst->limit) {                     // Writing can NOT go beyond limit.
      ctx->ccopy = nocopy;
      dst->status[0] = CReq_OOB;
    }
  }

}

static uint32_t CoI2u16(const CoI_t * CoI) {                // Read an uint16_t size from the stream.
  return (uint32_t) ((CoI[1].MSB << 8) | CoI[0].LSB);       // Little endian encoding.
}

__attribute__((no_sanitize("undefined")))                   // For counting, this function adds to the NULL pointer.
static void copy(cctx_t ctx, uint32_t b2c, uint32_t skip) { // Copy and skip if needed; checking for overflow.

  check4Read(ctx, & ctx->Src, b2c);
  check4Write(ctx, & ctx->Dst, b2c);

  ctx->ccopy(ctx->Dst.cur, ctx->Src.cur, b2c);

  ctx->Dst.cur += b2c;
  ctx->Src.cur += b2c;

  if (ctx->decoding) {
#if ! defined(NDEBUG)                                       // If assertions are enabled for testing ...
    for (uint32_t n = 0; isActive(ctx) && n < skip; n++) {  // ... also clear the padding bytes.
      ctx->Dst.cur[n] = 0;
    }
#endif
    ctx->Dst.cur += skip;
  }
  else {                                                    // Encoding
    ctx->befskip = ctx->Src.cur;                            // Record last usefull byte before skip.
    ctx->Src.cur += skip;
  }

}

typedef struct Loop_t {
  const CoI_t *   start;          // Start of the loop.
  uint16_t        count;          // Loop counter.
} Loop_t;

static void intdecode(cctx_t ctx) {                         // Internal decoder.

  codec_t    codec = ctx->codec;
  codec_t    end = codec + ctx->numc;
  CoI_t      CoI;
  uint32_t   count;
  Fixup_t *  fixup;
  uint8_t    numfix = 0;                                    // Current number of fixes.
  Loop_t     Loop[2];
  uint8_t    actloop = 0xff;                                // Active loop index when not 0xff.
  
  for (CoI = *codec; codec < end; codec++, CoI = *codec) {  // Go over the instruction stream.
    if (ctx->Src.status[0] || ctx->Dst.status[0]) break;    // Stop at error.
    if (CoI.inst) {                                         // An action to perform.
      switch (CoI.action) {
        case modif: {
          if (0 == CoI.arg) { ctx->bitsset = 1; }           // Count modifier.
          break;
        }
        
        case inlinefix: {
          count = *(ctx->Src.cur - 1);                      // Previous src byte is count.
          fixup = & ctx->Fixup[numfix++];                   // Allocate a fixup.
          assert(numfix < ctx->fixcap);                     // Otherwise increase capacity.
          fixup->num = count * CoI.arg;                     // Number of bytes to copy; arg is size in bytes.
          fixup->ref = (void **) ctx->Dst.cur;              // Record where to write the pointer.
          fixup->src = ctx->Src.cur;
          check4Read(ctx, & ctx->Src, fixup->num);
          fixup->delayed = 0;                               // It is an inline fixup.
          check4Write(ctx, & ctx->Dst, sizeof(void *));
          ctx->Dst.cur += sizeof(void *);                   // Allocate space for the reference.
          ctx->Src.cur += fixup->num;                       // Skip over the data portion.
          if (isActive(ctx)) { *fixup->ref = NULL; }        // Clear it already when active.
          if (0 == count) numfix--;                         // If nothing to copy, release fixup.
          ctx->bitsset = 0;                                 // Reset in any case.
          break;
        }

        case laterfix: {
          count = *(ctx->Src.cur - 1);                      // Previous src byte is count.
          fixup = & ctx->Fixup[numfix++];                   // Allocate a fixup.
          assert(numfix < ctx->fixcap);                     // Otherwise increase capacity.
          fixup->num = count * CoI.arg;                     // Number of bytes to copy; arg is size in bytes.
          fixup->ref = (void **) ctx->Dst.cur;              // Record where to write the pointer.
          check4Write(ctx, & ctx->Dst, sizeof(void *));
          fixup->delayed = 1;
          ctx->Dst.cur += sizeof(void *);                   // Allocate space for the reference.
          if (isActive(ctx)) { *fixup->ref = NULL; }        // Clear it already when active.
          if (0 == count) numfix--;                         // If nothing to copy, release fixup.
          ctx->bitsset = 0;                                 // Reset in any case.
          break;
        }

        case loop: {
          count = *(ctx->Src.cur - 1);                      // Previous src byte is count.
          actloop++;                                        // Go to next loop level.
          assert(actloop < NUM(Loop));                      // Increase local capacity.
          Loop[actloop].count = count;
          Loop[actloop].start = codec;
          ctx->bitsset = 0;                                 // Reset in any case.
          if (0 == count) {                                 // Nothing to loop, skip until endloop.
            actloop--;
            for ( ; codec < end; codec++, CoI = *codec) {
              if (endloop == CoI.action) { break; }         // Ended 0 count loop.
            }
          }
          break;
        }
        
        case endloop: {
          if (! Loop[actloop].count) {                      // There must be an active loop.
            ctx->Src.status[0] = CReq_bad_code;
            return;                                         // Immediate stop.
          }
          Loop[actloop].count--;                            // Decrement active loop count.
          if (Loop[actloop].count) {                        // If count not yet zero, ...
            codec = Loop[actloop].start;                    // ... restart the loop.
          }
          else {
            actloop--;                                      // Loop done, go to previous active loop, if any.
          }
          break;
        }

        case copyws: {
          copy(ctx, CoI2u16(codec + 1), CoI.arg);           // Next 2 bytes is size, argument is skip count.
          codec += 2;                                       // Skip size bytes.
          break;
        }
        
        default: {
          ctx->Src.status[0] = CReq_bad_code;
          return;                                           // Immediate stop.
        }
      }
    }
    else {
      copy(ctx, CoI.num, CoI.skip);
    }
  }

  for (uint32_t i = 0; i < numfix; i++) {
    fixup = & ctx->Fixup[i];
    check4Write(ctx, & ctx->Dst, fixup->num);
    if (fixup->delayed) {
      check4Read(ctx, & ctx->Src, fixup->num);
      ctx->ccopy(ctx->Dst.cur, ctx->Src.cur, fixup->num);
      ctx->Src.cur += fixup->num;
    }
    else {                                                  // Data was inline, copy it now.
      ctx->ccopy(ctx->Dst.cur, fixup->src, fixup->num);     // Copy the data.
    }
    if (isActive(ctx)) {
      *fixup->ref = (void *) ctx->Dst.cur;                  // Write the reference.
    }
    ctx->Dst.cur += fixup->num;
  }

}

static void intencode(cctx_t ctx) {                         // Internal encoder.

  codec_t    codec = ctx->codec;
  codec_t    end = codec + ctx->numc;
  CoI_t      CoI;
  uint32_t   count;
  Loop_t     Loop[2];
  uint8_t    actloop = 0xff;                                // Active loop index when not 0xff.
  Fixup_t *  fixup;
  uint8_t    numfix = 0;                                    // Current number of fixes.
  uint32_t   num;
  void *     from;
  uint32_t   add2cur = 0;
  
  for (CoI = *codec; codec < end; codec++, CoI = *codec) {  // Go over the instruction stream.
    if (ctx->Src.status[0] || ctx->Dst.status[0]) break;    // Stop at error.
    if (CoI.inst) {                                         // An action to perform.
      switch (CoI.action) {
        case modif: {
          if (0 == CoI.arg) { ctx->bitsset = 1; }           // Count modifier.
          break;
        }

        case inlinefix: {
          count = *(ctx->befskip - 1);                      // Just before skip, count was written.
          from = ((void **) ctx->Src.cur)[0];
          num = count * CoI.arg;                            // Number of bytes to copy; arg is size in bytes.
          check4ReadFrom(ctx, & ctx->Src, from, num);
          check4Write(ctx, & ctx->Dst, num);
          if (num) {
            ctx->ccopy(ctx->Dst.cur, from, num);
          }
          ctx->Dst.cur += num;
          add2cur += num;                                   // Will be added later to source cursor.
          ctx->Src.cur += sizeof(void *);
          ctx->bitsset = 0;                                 // Reset in any case.
          break;
        }

        case laterfix: {
          fixup = & ctx->Fixup[numfix++];                   // Allocate a fixup.
          count = *(ctx->befskip - 1);                      // Just before skip, count was written.
          fixup->src = ((void **) ctx->Src.cur)[0];
          fixup->num = count * CoI.arg;                     // Number of bytes to copy; arg is size in bytes.
          check4ReadFrom(ctx, & ctx->Src, fixup->src, fixup->num);
          if (0 == count) numfix--;                         // If nothing to copy, release fixup.
          fixup->delayed = 1;                               // Implicit; only 1 type in encoding.
          ctx->Src.cur += sizeof(void *);
          ctx->bitsset = 0;                                 // Reset in any case.
          break;
        }

        case loop: {
          count = *(ctx->befskip - 1);                      // Just before skip, count was written.
          actloop++;                                        // Go to next loop level.
          assert(actloop < NUM(Loop));                      // Increase local capacity.
          Loop[actloop].count = count;
          Loop[actloop].start = codec;
          ctx->bitsset = 0;                                 // Reset in any case.
          if (0 == count) {                                 // Nothing to loop, skip until endloop.
            actloop--;
            for ( ; codec < end; codec++, CoI = *codec) {
              if (endloop == CoI.action) { break; }         // Ended 0 count loop.
            }
          }
          break;
        }
        
        case endloop: {
          if (! Loop[actloop].count) {                      // There must be an active loop.
            ctx->Src.status[0] = CReq_bad_code;
            return;                                         // Immediate stop.
          }
          Loop[actloop].count--;                            // Decrement active loop count.
          if (Loop[actloop].count) {                        // If count not yet zero, ...
            codec = Loop[actloop].start;                    // ... restart the loop.
          }
          else {
            actloop--;                                      // Loop done, go to previous active loop, if any.
          }
          break;
        }

        case copyws: {
          copy(ctx, CoI2u16(codec + 1), CoI.arg);           // Next 2 bytes is size, argument is skip count.
          codec += 2;                                       // Skip size bytes.
          break;
        }
                
        default: {                                          // Unknown action.
          ctx->Src.status[0] = CReq_bad_code;
          return;                                           // Immediate stop.
        }
      }
    }
    else {
      copy(ctx, CoI.num, CoI.skip);
    }
  }

  for (num = 0; num < numfix; num++) {
    fixup = & ctx->Fixup[num];
    check4Write(ctx, & ctx->Dst, fixup->num);
    ctx->ccopy(ctx->Dst.cur, fixup->src, fixup->num);
    ctx->Dst.cur += fixup->num;
    add2cur += fixup->num;
  }

  ctx->Src.cur += add2cur;                                  // Purely for consistency and during testing.

}

uint32_t pkt2struct(codecreq_t req) {

   Fixup_t    Fixup[32];
   ctab_t     ctab;

   CodecCtx_t Ctx = {
    .Src = {
      .start  = req->Pkt.buf,
      .cur    = req->Pkt.buf,
      .limit  = req->Pkt.buf + req->Pkt.sz,
      .status = & req->Pkt.status,
    },
    .Dst = {
      .start  = req->Struct.buf,
      .cur    = req->Struct.buf,
      .limit  = req->Struct.buf ? req->Struct.buf + req->Struct.sz : (void *) 1,
      .status = & req->Struct.status,
    },
    .entry = codec4dec(req->Pkt.buf, & ctab),
    .numc     = 0,
    .codec    = NULL,
    .Fixup    = Fixup,
    .fixcap   = NUM(Fixup),
    .decoding = 1,
    .ccopy    = req->Struct.buf ? docopy : nocopy,
  };

  req->Pkt.status = 0;
  req->Struct.status = 0;

  if (Ctx.entry) {
    assert(Ctx.entry->numcoi);                              // Must have at least 1 instruction.
    Ctx.numc = Ctx.entry->numcoi;
    Ctx.codec = & ctab->CoI[Ctx.entry->coistart];
    intdecode(& Ctx);

    if (Ctx.Src.status[0]) { return 0; }
    if (Ctx.Dst.status[0]) { return 0; }

    return (uint32_t) (Ctx.Dst.cur - Ctx.Dst.start);        // Size of the decoded structure.
  }
  
  req->Pkt.status = CReq_Not_Found;
  return 0;

}

uint32_t struct2pkt(codecreq_t req) {

   Fixup_t    Fixup[32];
   ctab_t     ctab;
   uint32_t   encsize;

// Same as decode, just Dec/Enc reversed and a different search function

   CodecCtx_t Ctx = {
    .Src = {
      .start  = req->Struct.buf,
      .cur    = req->Struct.buf,
      .limit  = req->Struct.buf + req->Struct.sz,
      .status = & req->Struct.status,
    },
    .Dst = {
      .start  = req->Pkt.buf,
      .cur    = req->Pkt.buf,
      .limit  = req->Pkt.buf ? req->Pkt.buf + req->Pkt.sz : (void *) 1,
      .status = & req->Pkt.status,
    },
    .entry = codec4enc(req->Struct.buf, & ctab), // and a different search
    .numc     = 0,
    .codec    = NULL,
    .Fixup    = Fixup,
    .fixcap   = NUM(Fixup),
    .decoding = 0,
    .ccopy    = req->Pkt.buf ? docopy : nocopy,
  };

  req->Pkt.status = 0;
  req->Struct.status = 0;

  if (Ctx.entry) {
    assert(Ctx.entry->numcoi);                              // Must have at least 1 instruction.
    Ctx.numc = Ctx.entry->numcoi;
    Ctx.codec = & ctab->CoI[Ctx.entry->coistart];
    intencode(& Ctx);

    encsize = (uint32_t) (Ctx.Dst.cur - Ctx.Dst.start);     // Size of the encoded packet.

    if (isActive(& Ctx)) {
      if (type_EVT == req->Pkt.buf[0]) {
        ((uint8_t *) req->Pkt.buf)[2] = encsize - 3;        // Type|code|length
      }
      else if (type_CMD == req->Pkt.buf[0]) {
        ((uint8_t *) req->Pkt.buf)[3] = encsize - 4;        // Type|OPC|OPC|Length
      }
    }

    if (Ctx.Src.status[0]) { return 0; }
    if (Ctx.Dst.status[0]) { return 0; }

    return encsize;

  }
  
  req->Struct.status = CReq_Not_Found;

  return 0;

}
