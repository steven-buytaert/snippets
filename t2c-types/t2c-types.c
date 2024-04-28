// Copyright 2023-2024 Steven Buytaert

#define  _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdalign.h>

#include <t2c-types.h>

typedef struct t2c_Ctx_t *     ctx_t;    // Internally used shorthands.
typedef struct t2c_XRef_t *    xref_t;
typedef struct t2c_Type_t *    type_t;
typedef struct t2c_Member_t *  member_t;
typedef struct t2c_Alt_t *     alt_t;
typedef struct t2c_TGSpec_t *  spec_t;
typedef struct t2c_OffMap_t *  omap_t;
typedef struct t2c_MapUnit_t * offu_t;

const uint32_t t2ccookie = 0x03041965;

//#define DEBUG

#ifdef DEBUG
#define DBG(format, a...) do { dbg(__LINE__, format, ##a); } while(0)

static void dbg(uint32_t line, const char * format, ...) {

  va_list ap;

  va_start(ap, format);
  printf("%4u: ", line);
  vprintf(format, ap);
  va_end(ap);

}

static const char * i4(uint32_t d) {                        // Return indentation string for depth d.

  static const char * sp = "                        ";

  return & sp[strlen(sp) - (d * 2)];

}
#else
#define DBG(format, a...)
#endif // DEBUG

static void ctxmsg(ctx_t ctx, const char * format, ...) {

  va_list  ap;
  uint32_t max = sizeof(ctx->msg) - 1;

  va_start(ap, format);
  vsnprintf(ctx->msg, max, format, ap);
  va_end(ap);

}

static uint32_t isComp(type_t type) {
  return (t2c_Comp == (type->prop & t2c_Comp)) ? 1 : 0;
}

static uint32_t isStruct(type_t type) {
  return (t2c_Struct == (type->prop & t2c_Struct)) ? 1 : 0;
}

static uint32_t isUnion(type_t type) {
  return (t2c_Union == (type->prop & t2c_Union)) ? 1 : 0;
}

uint32_t t2c_isEnum(const t2c_Type_t * type) {              // This one is public.
  return (t2c_Enum == (type->prop & t2c_Enum)) ? 1 : 0;
}

static uint32_t isPrim(type_t type) {
  return (t2c_Prim == (type->prop & t2c_Prim)) ? 1 : 0;
}

static uint32_t isBitset(type_t type) {
  return (t2c_Bitset == (type->prop & t2c_Bitset)) ? 1 : 0;
}

uint32_t t2c_isTypedef(const t2c_Type_t * type) {           // This one is public.
  return (t2c_Typedef == (type->prop & t2c_Typedef)) ? 1 : 0;
}

#define NUM(A) (uint32_t)(sizeof(A) / sizeof(A[0]))

t2c_Type_t t2c_VoidRef = { .name = "void *",   .prop = t2c_Prim, .size = 4, .align = 4, .numslots = 1, .numtags = 1 };

t2c_Type_t t2c_Char    = { .name = "char",     .prop = t2c_Prim,              .size = 1, .align = 1, .numslots = 1, .numtags = 1 };
t2c_Type_t t2c_U64     = { .name = "uint64_t", .prop = t2c_Prim,              .size = 8, .align = 8, .numslots = 1, .numtags = 1 };
t2c_Type_t t2c_U32     = { .name = "uint32_t", .prop = t2c_Prim,              .size = 4, .align = 4, .numslots = 1, .numtags = 1 };
t2c_Type_t t2c_U16     = { .name = "uint16_t", .prop = t2c_Prim,              .size = 2, .align = 2, .numslots = 1, .numtags = 1 };
t2c_Type_t t2c_U08     = { .name = "uint8_t",  .prop = t2c_Prim,              .size = 1, .align = 1, .numslots = 1, .numtags = 1 };
t2c_Type_t t2c_S64     = { .name = "int64_t",  .prop = t2c_Prim | t2c_Signed, .size = 8, .align = 8, .numslots = 1, .numtags = 1 };
t2c_Type_t t2c_S32     = { .name = "int32_t",  .prop = t2c_Prim | t2c_Signed, .size = 4, .align = 4, .numslots = 1, .numtags = 1 };
t2c_Type_t t2c_S16     = { .name = "int16_t",  .prop = t2c_Prim | t2c_Signed, .size = 2, .align = 2, .numslots = 1, .numtags = 1 };
t2c_Type_t t2c_S08     = { .name = "int8_t",   .prop = t2c_Prim | t2c_Signed, .size = 1, .align = 1, .numslots = 1, .numtags = 1 };
t2c_Type_t t2c_F32     = { .name = "float",    .prop = t2c_Prim | t2c_Signed, .size = 4, .align = 4, .numslots = 1, .numtags = 1 };
t2c_Type_t t2c_F64     = { .name = "double",   .prop = t2c_Prim | t2c_Signed, .size = 8, .align = 8, .numslots = 1, .numtags = 1 };

const type_t t2c_prims[11] = {    // Keep the primitive canonicals in this order.
  & t2c_Char, // Corresponds to 'none' in FB2.
  & t2c_S08,
  & t2c_U08,
  & t2c_S16,
  & t2c_U16,
  & t2c_S32,
  & t2c_U32,
  & t2c_S64,
  & t2c_U64,
  & t2c_F32,
  & t2c_F64,
};

static uint32_t roundup(uint32_t value, uint32_t powerOf2) {
  return (value + (powerOf2 - 1)) & ~(powerOf2 - 1);
}

static uint32_t ioMNm(const t2c_Type_t * t, const char * m) { // Return the member index of member with name 'm'.

  uint32_t i;
  
  for (i = 0; i < t->num; i++) {
    if (0 == strcmp(t->Members[i].name, m)) {
      break;
    }
  }
  
  assert(i < t->num);                                       // Should always be found.
  
  return i;

}

typedef struct A4SzCtx_t * szctx_t;

typedef struct A4SzCtx_t {        // Context for type size/offset analysis.
  uint16_t     top;
  uint16_t     overflow;
  uint16_t     numtags;
  uint16_t     numslots;
  struct {
    type_t     type;
    uint16_t   offset;
    uint8_t    pad[6];
  } Stack[8];
} A4SzCtx_t;

static offu_t getoffu(omap_t map, uint32_t numtags) {       // Get a unit to write info to; real or dummy.

  static t2c_MapUnit_t Dummy;

  if (! map || map->overflow) {
    return & Dummy;
  }

  if (numtags < map->cap) {
    map->num++;
    return & map->Unit[numtags];
  }
  else {
    map->overflow = 1;
    return & Dummy;
  }

}

static void a4Sz(ctx_t ctx, szctx_t szctx, omap_t map) {    // Internal function for size and alignment calculation.

  type_t   type = szctx->Stack[szctx->top].type;            // Type to calculate is at top of stack.
  member_t m = type->Members;
  uint32_t i;
  uint16_t align = 0;
  uint16_t offset = 0;                                      // This is also the running size of the type.
  uint32_t size = 0;
  uint16_t padding = 0;
  uint32_t packed = type->prop & t2c_Packed ? 1 : 0;
  offu_t   offu;

  if (szctx->top == NUM(szctx->Stack)) {
    szctx->overflow = 1;
    return;
  }

  for (i = 0; i < szctx->top; i++) {
    if (szctx->Stack[i].type == type) {
      ctxmsg(ctx, "Circularity involving type '%s'.", type->name);
      ctx->error = t2c_Circularity;
    }
  }

  if (ctx->error) { return; }                               // Don't continue in error.

  if (isPrim(type) && isComp(type)) {
    ctxmsg(ctx, "Type '%s' can't be both Comp and Prim.", type->name);
    ctx->error = t2c_BadProperties;
    return;
  }

  if (t2c_isEnum(type) || t2c_isTypedef(type)) {            // These are opaque.
    return;
  }

  DBG("%s type '%s' size %u align %u %s\n", i4(szctx->top), type->name, type->size, type->align, packed ? "packed" : "");

  if (isPrim(type) || isBitset(type)) {                     // Primitives have been done already.
    szctx->numslots++;
    return;
  }

  type->size = 0;
  type->align = 0;

  for (i = 0; ! ctx->error && i < type->num; i++, m++) {
    if (m->type == type && ! m->numind) {
      ctxmsg(ctx, "Type '%s' includes itself without indirection.", type->name);
      ctx->error = t2c_Circularity;
      break;
    }

    if (m->ref2size) {
      if (! m->isVTail && ! m->numind) {
        ctxmsg(ctx, "Member '%s' ref2size '%s' implies either indirection or VTail.", m->name, m->ref2size->name);
        ctx->error = t2c_LoneRef2size;
        break;
      }
      if (m->isVTail && i + 1 < type->num) {
        ctxmsg(ctx, "VTail '%s' can only apply to final member.", m->name);
        ctx->error = t2c_VTailNotFinal;
        break;
      }
    }

    if (m->numind && m->isVTail) {
      ctxmsg(ctx, "Member '%s' can never both be reference and VTail.", m->name);
      ctx->error = t2c_RefAndVTail;
      break;
    }

    offu = getoffu(map, szctx->numtags++);                  // Get unit and increase # fields.
    offu->indent = szctx->top;
    offu->type = type;
    offu->member = m;
    if (isUnion(type)) { szctx->numslots += i ? 0 : 1; }    // Contributes a single memory slot.
    else { szctx->numslots++; }

    DBG("%s - member %u '%s' offset %u, slot %u.\n", i4(szctx->top), i, m->name, offset, szctx->numslots);

    size = 0;
    align = 0;

    if (m->numind) {
      align = ctx->align4ref;
      size  = ctx->size4ref;
    }
    else {
      if (! m->type) {
        ctxmsg(ctx, "Member '%s' has a bad/no type.", m->name);
        ctx->error = t2c_BadMemberType;
        return;
      }
      szctx->numslots--;                                    // To account for this expanded type.
      szctx->Stack[++szctx->top].type = m->type;
      a4Sz(ctx, szctx, map);                                // Call ourselves to analyze the type of the member.
      szctx->top--;                                         // Pop stack.
      align = m->type->align;
      size = m->type->size;
    }

    if (align > type->align) {
      type->align = align;
    }

    if (! packed) {
      padding = roundup(offset, align) - offset;
    }

    if (padding) {
      DBG("%s   %u padding bytes\n", i4(szctx->top), padding);
    }

    if (isStruct(type)) {
      m->offset = (uint32_t)(offset + padding);             // Update member information.
    }
    else {
      assert(isUnion(type));                                // It must be a union in this case.
      m->offset = 0;                                        // Union members are all at offset 0.
    }

    offu->offset  = m->offset;
    offu->size    = m->isVTail ? 0 : size;                  // Vtail elements have no size.
    offu->align   = align;
    offu->padding = padding;

    DBG("%s   '%s' offset %u size %u align %u\n", i4(szctx->top), m->name, m->offset, size, align);

    if (! m->isVTail) {                                     // Members in the VTail[0] don't contribute to size.
      if (isStruct(type)) {
        offset += size + padding;
      }
      else {
        assert(isUnion(type));                              // It must be a union in this case.
        if (size > offset) {                                // For unions, the largest member determines the size.
          offset = size;
        }
      }
    }

  }

  if (! ctx->error) {
    if (! packed) {
      padding = roundup(offset, type->align) - offset;
    }
    assert(! isUnion(type) || 0 == padding);                // A union should not have padding at this point.

    if (padding) {
      DBG("%s needs %u trailing padding bytes.\n", i4(szctx->top), padding);
    }

    type->size = offset + padding;

    DBG("%s Done '%s' size %u align %u, numslots %u numtags %u\n", i4(szctx->top), type->name, type->size, type->align, szctx->numslots, szctx->numtags);
  }

}

void t2c_ana4size(ctx_t ctx, type_t type) {                 // Analyze for size and also alignment.

  A4SzCtx_t SzCtx;

  memset(& SzCtx, 0x00, sizeof(SzCtx));
   
  SzCtx.Stack[0].type = type;

  a4Sz(ctx, & SzCtx, NULL);

  assert(! SzCtx.overflow);

  type->numslots = SzCtx.numslots;                          // Copy back slot information to type.
  type->numtags  = SzCtx.numtags;
  
}

void t2c_ana4off(t2c_ctx_t ctx, t2c_OffMap_t * map) {       // Create an offset map.

  A4SzCtx_t SzCtx;

  memset(& SzCtx, 0x00, sizeof(SzCtx));
   
  SzCtx.Stack[0].type = map->type;

  uint32_t cap = map->cap;                                  // Save capacity, ...
  memset(map, 0x00, sizeof(t2c_OffMap_t));                  // ... clear the head and ...
  map->cap = cap;                                           // ... restore cap and type.
  map->type = SzCtx.Stack[0].type;
  
  a4Sz(ctx, & SzCtx, map);

  assert(! SzCtx.overflow);

  offu_t   u = map->Unit;
  uint32_t n;
  
  for (n = 0; n < map->num; n++, u++) {                     // Go over all map units.
    u->isprim = u->member->type->prop & t2c_Prim ? 1 : 0;   // Set primitive flag.
    SzCtx.Stack[u->indent].offset = u->offset;              // Keep track of current offset for this level.
    for (uint32_t a = 0; a < u->indent; a++) {              // Add running offset to this level.
      u->offset += SzCtx.Stack[a].offset;
    }
  }
  
}

#define getmem(CTX, SIZE) _getmem(__LINE__, CTX, SIZE)

static void * _getmem(uint32_t ln, ctx_t ctx, uint32_t sz) {

  void * mem = ctx->mem(ctx, NULL, sz);

  if (! mem) {
    ctx->error = t2c_AllocFailed;
    ctxmsg(ctx, "Allocation for %u bytes failed.\n", sz);
  }
  else {
    memset(mem, 0x00, sz);
  }

  return mem;
  
}


static void freemem(ctx_t ctx, void * mem) {                // Release memory via our context.
  ctx->mem(ctx, mem, 0);
}

t2c_type_t t2c_name2type(t2c_ctx_t ctx, const char * name) {

  uint32_t   i;
  t2c_type_t found = NULL;

  for (i = 0; i < NUM(t2c_prims) && ! found; i++) {
    if (0 == strcmp(name, t2c_prims[i]->name)) {
      found = t2c_prims[i];
    }
  }

  for (i = 0; ! found && i < ctx->num; i++) {
    if (ctx->types[i]) {
      if (0 == strcmp(ctx->types[i]->name, name)) {
        found = ctx->types[i];
      }
    }
  }

  return found;

}

static void initbitset(ctx_t ctx, type_t bs) {

  type_t   base = bs->boetype;
  uint32_t i;
  member_t m;
  uint32_t remain = (uint32_t)(base->size * 8);             // Number of bits remaining in the base type.
  uint32_t offset = 0;

  if (! isPrim(base)) {
    ctxmsg(ctx, "Bitset '%s' basetype must be primitive.", bs->name);
    ctx->error = t2c_BadBaseType;
    return;
  }

  for (m = bs->Members, i = 0; i < bs->num; i++, m++) {     // Ensure all members have the proper base type.
    m->type = base;
  }

  for (m = bs->Members, i = 0; i < bs->num; i++, m++) {     // Check proper width and assign the offset.
    if (0 == m->width) {
      ctxmsg(ctx, "Can not have 0 width for '%s'.", m->name);
      ctx->error = t2c_ZeroWidth;
    }
    if (remain < m->width) {
      ctxmsg(ctx, "Not enough bits left %u for '%s' width %u.", remain, m->name, m->width);
      ctx->error = t2c_TooSmall;
    }
    if (ctx->error) { return; }
    m->offset = offset;
    remain -= m->width;
    offset += m->width;
  }

  bs->size = base->size;
  bs->align = base->align;

}

static void initenum(ctx_t ctx, type_t et) {

  type_t   base = et->boetype;
  uint32_t i;
  member_t m;
  uint32_t x;

  if (! isPrim(base)) {
    ctxmsg(ctx, "Enum '%s' basetype must be primitive.", et->name);
    ctx->error = t2c_BadBaseType;
    return;
  }

  for (m = et->Members, i = 0; i < et->num; i++, m++) {     // Ensure all members have the proper base type.
    m->type = base;
    for (x = 0; x < et->num; x++) {
      if (x == i) continue;                                 // Don't compare the member with itself.
      if (m->enumval == et->Members[x].enumval) {
        ctxmsg(ctx, "Members '%s' '%s' are duplicates.", m->name, et->Members[x].name), 
        ctx->error = t2c_Duplicate;
      }
      if (ctx->error) { return; }
    }
  }

  et->size = base->size;
  et->align = base->align;

}

void t2c_initype(ctx_t ctx, type_t type) {

  t2c_Member_t Marker = {                                   // Prepare the marker.
    .type = type,
    .name = "<marker>",
  };

  static uint32_t initialized = 0;
  
  if (! initialized) {                                      // Perform some initialization stuff.
    t2c_VoidRef.size  = ctx->size4ref;
    t2c_VoidRef.align = ctx->align4ref;
    memcpy((void *) & ctx->cookie, & t2ccookie, sizeof(t2ccookie));
    if (! ctx->defnamesz) { ctx->defnamesz = 48; }          // Sensible default.
    initialized = 1;
  }

  assert(& type->Marker == (& type->Members[0]) - 1);       // Marker must come just before the members VTail.

  memcpy((void*) & type->Marker, & Marker, sizeof(Marker)); // To overcome the const typeness of the marker (yes, it's a hack).
  
  type->ctx = ctx;
  
  type->prop |= t2c_Comp;

  if (isBitset(type)) {
    if (type->boetype) {
      initbitset(ctx, type);
    }
    else {
      ctxmsg(ctx, "Bitset '%s' has no basetype.\n", type->name);
      ctx->error = t2c_BadBaseType;
    }
  }
  else if (t2c_isEnum(type)) {
    if (type->boetype) {
      initenum(ctx, type);
    }
    else {
      ctxmsg(ctx, "Enum '%s' has no basetype.\n", type->name);
      ctx->error = t2c_BadBaseType;
    }
  }
  else if (t2c_isTypedef(type)) {
    if (1 != type->num) {                                   // Typedefs should have exactly one member.
      if (! type->Members[0].type) {
        ctxmsg(ctx, "Typedef '%s' has no basetype.\n", type->name);
        ctx->error = t2c_BadBaseType;
      }
    }
  }

  t2c_ana4size(ctx, type);
  
}

type_t t2c_mem2cont(const t2c_Member_t * mem, uint32_t mi[1]) {

  const t2c_Member_t * cur;

  for (cur = mem; strcmp(cur->name, "<marker>"); cur--) {   // Always search up for the marker type set in initype.
    assert(mem - cur < 1024);                               // Have some kind of check.
  }

  if (mi) { mi[0] = (uint32_t)(mem - cur - 1); }            // Set member index if not passed NULL.
  
  return cur->type;

}

#define COMPARE(IA, IB)                                              \
do {                                                                 \
  int32_t v = (int32_t)(IA - IB);                                    \
  if (v) {                                                           \
    DBG("%s %s (%u - %s (%u) = %d.\n", i4(r), #IA, IA, #IB, IB, v);  \
    return v;                                                        \
  }                                                                  \
} while (0)

static int32_t _typecmp(const t2c_Type_t *a, const t2c_Type_t *b, uint32_t r);

static int32_t cmpMemb(const t2c_Member_t *a, const t2c_Member_t *b, uint32_t r) { // Compare 2 members if one could replace the other. Does not compare names.

  int32_t d = 0;

  if (isComp(a->type) && isComp(b->type)) {                 // Both composite.
    DBG("%s both [%s:%s] [%s:%s] composite\n", i4(r), a->type->name, a->name, b->type->name, b->name);
    d = _typecmp(a->type, b->type, r + 1);
  }
  else if (isPrim(a->type) && isPrim(b->type)) {            // Both primitive.
    d = a->type - b->type;
    if (d) {
      DBG("%s Both primitives '%s' '%s' differ.\n", i4(r), a->type->name, b->type->name);
    }
  }
  else if (isComp(a->type)) {                               // A is a composite and B isn't.
    d = -1;
    DBG("%s A '%s' is composite, B '%s' primitive.\n", i4(r), a->type->name, b->type->name);
  }
  else {                                                    // B is a composite and A isn't.
    d = +1;
    DBG("%s A '%s' is primitive, B '%s' composite.\n", i4(r), a->type->name, b->type->name);
  }

  if (d) { return d; }

  COMPARE(a->numind,  b->numind);

  COMPARE(a->offset,  b->offset);

  COMPARE(a->fxdsize, b->fxdsize);

  COMPARE(a->isVTail, b->isVTail);

  COMPARE(a->isConst, b->isConst);

  COMPARE(a->width, b->width);                              // _typecmp has checked both are the same type.

  return 0;

}

static int32_t _typecmp(const t2c_Type_t *a, const t2c_Type_t *b, uint32_t r) {  // Compare 2 types. Does not compare the names.

  int32_t  d = a->size - b->size;
  uint32_t i;
  uint32_t ia;
  uint32_t ib;
  uint32_t aIsBitset;
  uint32_t bIsBitset;
  ctx_t    ctx = a->ctx;

  if (a == b) {
    DBG("%s Both a and b same type '%s'.\n", i4(r), a->name);
    return 0;
  }

  if (a->ctx != b->ctx) {
    ctxmsg(a->ctx, "WARNING '%s' %p and '%s' %p have a different context.", a->name, a->ctx, b->name, b->ctx);
  }

  DBG("%s Compare [%s s %u a %u m %u] [%s s %u a %u m %u].\n", i4(r), a->name, a->size, a->align, a->num, b->name, b->size, b->align, b->num);

  COMPARE(a->size, b->size);
 
  COMPARE(a->align, b->align);

  COMPARE(a->num, b->num);

  for (i = 0; i < a->num; i++) {
    DBG("%s member %u A '%s' B '%s'\n", i4(r), i, a->Members[i].name, b->Members[i].name);
    ia = 0xffff;
    if (a->Members[i].ref2size) {
      ia = ioMNm(a, a->Members[i].ref2size->name);
    }
    ib = 0xffff;
    if (b->Members[i].ref2size) {
      ib = ioMNm(b, b->Members[i].ref2size->name);
    }
    d = (int32_t)(ia - ib);                                 // Difference in ref2size, if any.
    if (d) {
      DBG("%s differ in ref2size.\n", i4(r));
      return d;
    }

    if (ctx->cmpNames) {
      d = strcmp(a->Members[i].name, b->Members[i].name);
      if (d) {
        DBG("%s '%s' and '%s' differ.\n", i4(r), a->Members[i].name, b->Members[i].name);
        return d;
      }
    }

    aIsBitset = isBitset(a->Members[i].type);
    bIsBitset = isBitset(b->Members[i].type);

    COMPARE(aIsBitset, bIsBitset);                          // First establish that both are a bitset or both are not.

    d = cmpMemb(& a->Members[i], & b->Members[i], r + 1);   // Now compare further member properties.
    if (d) {
      DBG("%s member %u differs.\n", i4(r), i);
      return d;
    }

  }

  return 0;

}

int32_t t2c_typecmp(const t2c_Type_t *a, const t2c_Type_t *b) {  // Compare 2 types. Does not compare the context.

  return _typecmp(a, b, 0);

}

static uint32_t namesz(ctx_t ctx, const char * name) {      // Return namesize based on current name.

  uint32_t sz = name ? strlen(name) : 0;

  sz += ctx->xtranamesz + 1;                                // +1 for trailing \0.

  DBG("%u %u '%s'\n", sz < ctx->defnamesz ? ctx->defnamesz : sz, sz, name);

  return (sz < ctx->defnamesz) ? ctx->defnamesz : sz;
  
}

static uint32_t indOfMem(const t2c_Type_t * type, member_t mem) {

  uint32_t mi = (uint32_t)(mem - & type->Members[0]);

  assert(mi < type->num);                                   // Must be within same type.
  
  return mi;
  
}

type_t t2c_clone4type(ctx_t ctx, const t2c_Type_t * src) {  // Clone a type (ensures it becomes a context member).

  t2c_Member_t * m;
  uint32_t       i;
  uint32_t       s;
  type_t         clone;
  uint32_t       sz;

  sz  = sizeof(t2c_Type_t);
  sz += (src->num + src->rem) * sizeof(t2c_Member_t);
  sz += namesz(ctx, src->name);

  for (i = 0; i < src->num; i++) {                          // Calculate total member name space needed.
    sz += namesz(ctx, src->Members[i].name);
  }

  sz += src->rem * ctx->defnamesz;                          // The namespace size for the remaining members.

  union {
    t2c_member_t memb;
    t2c_type_t   type;
    char *       name;
  } Cursor;

  DBG("Allocating %u bytes.\n", sz);

  Cursor.type = getmem(ctx, sz);

  if (! Cursor.type) {                                      // Probably t2c_AllocFailed; error set by getmem.
    DBG("Allocation failed '%s'\n", ctx->msg);
    return NULL;
  }

  memset(Cursor.type, 0x00, sz);                            // Clear all the memory.

  clone = Cursor.type;
  
  Cursor.type += 1;                                         // Go to member memory area.
  Cursor.memb += src->rem + src->num;                       // Allocate all the members; now at name buffer space.
  memcpy(clone, src, sizeof(t2c_Type_t));                   // Copy the bulk of the type (overwrites the name reference).
  clone->prop &= (uint32_t) ~t2c_Static;                    // Remove the static property, if any.
  clone->name = Cursor.name;                                // Assign and allocate type name.
  clone->namesz = namesz(ctx, src->name);
  Cursor.name += clone->namesz;
  if (src->name) { strcpy(clone->name, src->name); }

  memcpy(clone->Members, src->Members, src->num * sizeof(t2c_Member_t));

  for (i = 0, m = clone->Members; i < src->num; i++, m++) { // Assign and allocate member names and ref2size, if needed.
    m->name = Cursor.name;
    m->namesz = namesz(ctx, src->Members[i].name);
    Cursor.name += m->namesz;
    if (src->Members[i].name) {
      strcpy(m->name, src->Members[i].name);
    }
    if (src->Members[i].ref2size) {                         // Replace ref2size if any.
      s = indOfMem(src, src->Members[i].ref2size);          // Get the slot number.
      m->ref2size = & clone->Members[s];
    }
    if (src == m->type) {                                   // Replace references to self.
      m->type = clone;
    }
  }

  for (i = 0; i < src->rem; i++, m++) {                     // Now for the remaining members.
    m->name = Cursor.name;
    m->namesz = ctx->defnamesz;
    Cursor.name += m->namesz;
  }

  t2c_initype(ctx, clone);                                  // Ensure the marker is set properly.

  if (ctx->error) {                                         // Something went wrong, remove the bad type.
    DBG("Clone failed '%s'\n", ctx->msg);
    freemem(ctx, clone);
    clone = NULL;
  }
  else {
    ctx->types[ctx->num++] = clone;
  }

  return clone;

}

typedef struct RepCtx_t {         // Replacement context.
  t2c_type_t tdtype;
  uint32_t   count;
  uint8_t    pad[4];
} RepCtx_t;

static void rep4td(t2c_ctx_t ctx, t2c_member_t m, void * arg) {

  RepCtx_t * rep = arg;

  if (t2c_mem2cont(m, NULL) == rep->tdtype) { return; }     // We shouldn't replace ourselves.

  if (m->type == rep->tdtype->Members[0].type && m->numind) {
//    printf("REPLACE [%s*] with '%s' in '%s'\n", m->type->name, rep->tdtype->name, t2c_mem2cont(m, NULL)->name);
    m->type = rep->tdtype;
    m->numind--;
    rep->count++;
  }

}

uint32_t t2c_reptypedefs(t2c_ctx_t ctx) {

  RepCtx_t   Rep;
  uint32_t   i;
  t2c_type_t type;

  memset(& Rep, 0x00, sizeof(Rep));

  for (i = 0; i < ctx->num; i++) {
    type = ctx->types[i];
    if ((type->prop & t2c_Typedef) == t2c_Typedef) {
      Rep.tdtype = type;
      t2c_scan4mem(ctx, rep4td, & Rep);
    }
  }

  return Rep.count;

}

static void clear4mark(ctx_t ctx, type_t t, void * arg) {
  t->weight = 0;
  t->mark4use = 0;
}

typedef struct Stack_t {
  uint16_t       cap;
  uint16_t       top;
  uint8_t        pad[4];
  type_t         types[0];
} Stack_t;

typedef struct Stack_t * stack_t;

uint32_t t2c_mark4use(t2c_ctx_t ctx, const t2c_Type_t * root) {

  uint32_t     i;
  uint32_t     size = sizeof(type_t) * ctx->num;
  stack_t      stack = getmem(ctx, sizeof(Stack_t) + size);
  t2c_type_t   type;
  t2c_member_t m;
  uint32_t     count = 0;
  
  if (! stack) { return 0; }                                // Error set by getmem.

  t2c_scan4type(ctx, clear4mark, NULL);

  stack->cap = ctx->num;
  stack->types[stack->top++] = (t2c_type_t) root;
  
  while (stack->top) {                                      // Start marking; [W] comments only related to weight.
    type = stack->types[--stack->top];                      // Pop the top element.
    type->weight++;
    if (type->mark4use) { continue; }                       // Already marked; go to next, if any.
    type->mark4use = 1; count++;
    m = type->Members;
    for (i = 0; i < type->num; i++, m++) {                  // Now check all its members; push unmarked ones.

      m->type->weight++;                                    // [W] 1 Increment, because it is touched by the containing type, even if already marked.
      if (t2c_Typedef == m->type->prop) {                   // [W]
        m->type->weight += 2;                               // [W] A typedef weighs more (should be declared earlier) ...
        if (root != m->type->Members[0].type) {
          m->type->Members[0].type->weight++;               // [W] ... than the the type it refers to ...
        }                                                   // [W] ... but only if it is not the root type. That should come last, if possible.
      }

      if (! m->type->mark4use) {
        m->type->weight++;                                  // [W] 2 Increment again, because it was not yet marked.
        assert(stack->top < stack->cap);
        stack->types[stack->top++] = m->type;               // Push it on the stack for later processing.
      }
    }
  }

  freemem(ctx, stack);

  return count;

}

t2c_type_t t2c_tdref4type(t2c_ctx_t ctx, t2c_type_t type, const char * name) {

  struct {
    t2c_Type_t   Type;
    t2c_Member_t Member[1];
  } TD;

  memset(& TD, 0x00, sizeof(TD));
  
  TD.Type.prop = t2c_Typedef;
  TD.Type.num = 1;
  TD.Type.name = (char *) name;
  TD.Member[0].type = type;
  TD.Member[0].numind = 1;
  TD.Member[0].name = "";                                   // Can not be NULL; would upset t2c_mem2cont().

  type->ref2type = t2c_clone4type(ctx, & TD.Type);

  assert(type->ref2type);

  return type->ref2type;

}

uint32_t t2c_clearmark4scan(ctx_t ctx) {                    // Set all the type marks to 0.

  uint32_t i;
  uint32_t count;
  
  for (count = 0, i = 0; i < ctx->num; i++) {
    if (ctx->types[i]) {
      ctx->types[i]->mark4scan = 0;
      count++;
    }
  }

  return count;
  
}

void t2c_scan4type(ctx_t ctx, t2c_cb4t_t cb, void * arg) {  // Scan over all types in the context.

  uint32_t i;
  
  for (i = 0; i < ctx->num; i++) {
    if (ctx->types[i]) {
      cb(ctx, ctx->types[i], arg);
    }
  }

}

void t2c_scan4mem(ctx_t ctx, t2c_cb4m_t cb, void * arg) {   // Scan over all members in the context.

  type_t   ct;
  member_t cm;
  uint32_t   x;
  uint32_t   i;

  for (x = 0; x < ctx->num; x++) {
    ct = ctx->types[x]; if (! ct) { continue; }
    for (cm = ct->Members, i = 0; i < ct->num; i++, cm++) {
      cb(ctx, cm, arg);
    }
  }

}

static void check4type(ctx_t ctx, member_t m, void * arg) {

  xref_t xref = arg;
  
  if (m->type == xref->type) {
    if (xref->num < xref->cap) {
      xref->used[xref->num++] = m;
    }
  }

}

void t2c_xref4type(ctx_t ctx, xref_t xref) {

  xref->num = 0;

  t2c_scan4mem(ctx, check4type, xref);

}

void t2c_xref4mem(ctx_t ctx, xref_t xref) {

}

static void freetype(ctx_t ctx, type_t type) {

  uint32_t n2mu;
  
  for (uint32_t i = 0; i < ctx->num; i++) {
    if (ctx->types[i] == type) {
      n2mu = sizeof(type_t) * (ctx->cap - i - 1);
      memmove(& ctx->types[i], & ctx->types[i + 1], n2mu);
      ctx->num--;
      freemem(ctx, type);
    }
  }

}

void t2c_remove4type(ctx_t ctx, xref_t xref) {

  if (xref->type && ! isPrim(xref->type)) {                 // Don't remove primitives; they are not even in the table.
    t2c_xref4type(ctx, xref);                               // See if the type is used somewhere.
    if (0 == xref->num) {
      freetype(ctx, xref->type);
      for (uint32_t i = 0; i < ctx->num; i++) {
        if (ctx->types[i] == xref->type) {
          ctx->types[i] = NULL;
          
        }
      }
    }
    else {
//      assert(0); // for now
    }
  }

}

static int32_t wcmp(const void * r2a, const void * r2b, void * m) {

  uint32_t * max = m;

  const t2c_Type_t * a = * (const t2c_Type_t **) r2a;
  const t2c_Type_t * b = * (const t2c_Type_t **) r2b;

  if (a->weight > max[0]) { max[0] = a->weight; }
  if (b->weight > max[0]) { max[0] = b->weight; }

  return b->weight - a->weight;                             // Highest should float to the top.

}

static uint32_t isRef2Self(type_t t, member_t m) {         // Return non zero when the member is a reference to self.

  if (t2c_isTypedef(m->type)) {                            // For a member, referring to a typedef, use the typedef'ed type.
    m = & m->type->Members[0];
  }

  return ((m->type == t) && m->numind) ? 1 : 0;

}

static void check4Fwds(ctx_t ctx, member_t m, void * arg) { // Check for forwards and references to self.

  type_t cont = t2c_mem2cont(m, NULL);
  type_t t2check = m->type;

  if (isPrim(m->type)) { return; }                          // We never check against primitive type members; declared in <stdint.h>.

  if (isRef2Self(cont, m)) {
    m->isRef2Self = 1;
  }

  if (t2c_isTypedef(m->type)) {                             // If the membertype is a reference to a type ...
    t2check = m->type->Members[0].type;                     // ... we check against the type it refers to.
  }

  if (t2check != m->type) {                                 // A type referring to itself is never a forward.
    if (t2check->weight < cont->weight) {
      m->isForward = 1;
    }
    else if (t2check->weight == cont->weight) {             // Equal weights, the position in the Ctx.types array will determine.
      if (t2check->ti > cont->ti) {                         // The type we refer to comes later, so it's a forward.
        m->isForward = 1;
      }
    }
  }

}

void t2c_prep4gen(ctx_t ctx) {                              // Prepare for generation; try to reduce forward references as much as possible.

  uint32_t M = 0;

  qsort_r(ctx->types, ctx->num, sizeof(type_t), wcmp, & M); // Make the most referred to type come first to reduce forward references.

  printf("// Max weight %u\n", M);

  if (0 == M) {
    ctxmsg(ctx, "No weights set; run mark4use first?\n");
  }

  for (int32_t i = 0; i < ctx->num; i++) {                  // Set type index.
    ctx->types[i]->ti = i;
  }

  t2c_scan4mem(ctx, check4Fwds, NULL);

}

typedef struct Line_t {           // Internal structure to build up a line.
  char *   start;                 // Points into the buffer; start of the line.
  uint16_t off;                   // Current offset in the line.
  uint8_t  pad[6];
} Line_t;

typedef struct Line_t * line_t;


static void out(spec_t spec, line_t line, const char * format, ...) {

  va_list ap;
  int32_t ss;

  if (spec->overflow) { return; }

  va_start(ap, format);
  ss = spec->vsnprintf(line->start + line->off, spec->Buf.rem, format, ap);
  va_end(ap);

  if (ss >= spec->Buf.rem) {                                // Buffer overflown.
    spec->overflow = 1;
    spec->Buf.rem  = 0;                                     // By definition.
  }
  else {
    spec->Buf.rem -= ss;
    line->off     += ss;
  }

}

static void add2spec(spec_t spec, line_t line) {            // Add the line to the type spec.

  if (spec->overflow) { return; }

  if (spec->Lines.num + 1 < spec->Lines.cap) {
    spec->Line[spec->Lines.num++].start = line->start;
    line->start += line->off + 1;                           // Account for the trailing \0.
    line->off = 0;                                          // Reset for next line.
    spec->Buf.rem -= 1;                                     // Account for the \0 at the end, not yet accounted for.
  }
  else {
    spec->overflow = 1;
  }

}

static void pad(spec_t spec, line_t line, uint32_t ts) {    // Fill with space until tab stop or overflow.

  while (line->off + 1u < ts && ! spec->overflow) {
    out(spec, line, " ");
  }

}

static uint32_t prop2ti(uint32_t prop) {                    // Return a type index based upon the property.

  if (t2c_Prim    == (t2c_Prim    & prop)) return 1;
  if (t2c_Bitset  == (t2c_Bitset  & prop)) return 2;
  if (t2c_Struct  == (t2c_Struct  & prop)) return 3;
  if (t2c_Union   == (t2c_Union   & prop)) return 4;
  if (t2c_Enum    == (t2c_Enum    & prop)) return 5;
  if (t2c_Typedef == (t2c_Typedef & prop)) return 6;
  
  return 0;                                                 // Unknown type.
  
}

static const char * ti2name[] = {
  "",
  "", 
  "struct", // Bitsets are rendered as struct.
  "struct",
  "union",
  "enum",
  "typedef",
};

static void fmtmemb(t2c_ctx_t ctx, t2c_type_t type, spec_t spec, line_t line) {

  uint32_t     i;
  t2c_member_t m = type->Members;
  uint32_t     ti = prop2ti(type->prop);
  uint32_t     mti;
  uint32_t     isBitset = (ti == 2);
  uint32_t     isEnum   = (ti == 5);
  char         evfmt[32];
  char         eol[32] = { 0 };

  if (spec->addEOLComments) {
    sprintf(eol, "//");
  }

  if (spec->evfmt) {                                        // Create the format string for the enumeration value.
    sprintf(evfmt, "= %s", spec->evfmt);
  }
  else {
    sprintf(evfmt, "= %%u");                                // Not specified, create a default.
  }

  for (i = 0; i < type->num; i++, m++) {                    // Now do all members.
    mti = prop2ti(m->type->prop);                           // Member type index.

    pad(spec, line, spec->Tab.type);

    if (m->isConst) {
      out(spec, line, "const ");
    }

    if (m->expand || m->anon) {
      out(spec, line, "%s {", ti2name[mti]);
      pad(spec, line, spec->Tab.ends);
      add2spec(spec, line);
      spec->Tab.type += spec->Tab.indent;
      pad(spec, line, spec->Tab.type);
      fmtmemb(ctx, m->type, spec, line);                    // Call ourselves.
      spec->Tab.type -= spec->Tab.indent;
    }

    if (! m->expand) {
      if (! isEnum) {
        if (spec->useFwDec && (3 == mti || 4 == mti)) {     // When a struct or a union and we need to use forwarding declarations.
          if (! t2c_isTypedef(m->type)) {
            out(spec, line, "%s ", ti2name[mti]);
          }
        }
        out(spec, line, "%s ", m->type->name);
      }
    }
    else {
      pad(spec, line, spec->Tab.type);
      out(spec, line, "}");
    }

    for (uint32_t x = 0; x < m->numind; x++) {
      out(spec, line, "*");
    }

    if (! isEnum && ! m->expand) {                          // Enumeration values are filled out later; expanded names are not aligned.
      pad(spec, line, spec->Tab.member);
    }

    if (! m->anon) {
      out(spec, line, " %s", m->name);
    }

    if (isBitset) {
      pad(spec, line, spec->Tab.bits);
      out(spec, line, ": %2u", m->width);
    }
    else if (isEnum) {
      pad(spec, line, spec->Tab.member);                    // Enumeration values start where the member would start.
      out(spec, line, evfmt, m->enumval);
    }
    else if (m->fxdsize) {
      out(spec, line, "[%u]", m->fxdsize);
    }
    else if (m->isVTail) {
      out(spec, line, "[0]");
    }

    out(spec, line, "%c", isEnum ? ',' : ';');
    pad(spec, line, spec->Tab.comment);
    out(spec, line, eol);
    pad(spec, line, spec->Tab.ends);
    if (spec->overflow) { return; }                         // No need to continue
    add2spec(spec, line);

  }

}

void t2c_fmttype(t2c_ctx_t ctx, t2c_type_t type, spec_t spec) {

  Line_t       L;
  uint32_t     i;
  t2c_member_t m = type->Members;
  uint32_t     ti = prop2ti(type->prop);
  char         eol[32] = { 0 };

  spec->Tab.indent = spec->Tab.indent ? spec->Tab.indent : 2;
  spec->Tab.type   = spec->Tab.type   ? spec->Tab.type   : 3;
  spec->vsnprintf  = spec->vsnprintf  ? spec->vsnprintf  : vsnprintf;
  spec->packed     = spec->packed     ? spec->packed     : "__packed__";

  spec->overflow  = 0;
  spec->Lines.num = 0;
  spec->Buf.rem   = spec->Buf.cap;
  
  memset(spec->Buf.buf, 0x00, spec->Buf.cap);

  L.start = spec->Buf.buf;
  L.off   = 0;

  if (1 == ti || 0 == ti) {                                 // We don't format primitive or unknown types.
    return;
  }

  if (spec->addEOLComments) {
    sprintf(eol, "//");
  }

  if (6 == ti) {                                            // A typedef.
    assert(1 == type->num);                                 // Should have exactly 1 member.
    m = & type->Members[0];
    out(spec, & L, "typedef %s ", ti2name[prop2ti(m->type->prop)]);
    pad(spec, & L, spec->Tab.typedef_1);
    out(spec, & L, "%s ", m->type->name);
    for (i = 0; i < m->numind; i++) {
      out(spec, & L, "*");
    }
    pad(spec, & L, spec->Tab.typedef_2);
    out(spec, & L, "%s;", type->name);
    pad(spec, & L, spec->Tab.comment);
    out(spec, & L, eol);
    pad(spec, & L, spec->Tab.ends);
    add2spec(spec, & L);
    assert(1 == spec->Lines.num);
  }
  else {                                                    // A compound type.
    if (spec->useTypedef) {                                 // Create the declaration header.
      out(spec, & L, "typedef ");
    }
    out(spec, & L, "%s ", ti2name[ti]);
    if (spec->useTypedef) {
      out(spec, & L, "%s ", type->name);
    }
    out(spec, & L, "{");
    pad(spec, & L, spec->Tab.comment);
    out(spec, & L, eol);
    pad(spec, & L, spec->Tab.ends);
    add2spec(spec, & L);                                    // Add declaration header line.

    fmtmemb(ctx, type, spec, & L);

    out(spec, & L, "} ");  
    if (type->prop & t2c_Packed) {
      out(spec, & L, "%s ", spec->packed);
    }
    out(spec, & L, "%s;", type->name);  
    add2spec(spec, & L);
  }

}
