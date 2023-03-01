// Copyright 2023 Steven Buytaert

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include <cc-types.h>

typedef struct ccCtx_t * ctx_t;   // Internally used shorthand for context.
typedef struct ccXRef_t * xref_t;

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

static const char * i4(uint32_t d) {                        // Return indendation string for depth d.

  static const char * sp = "                        ";

  return & sp[strlen(sp) - (d * 2)];

}
#else
#define DBG(format, a...)
#endif // DEBUG

static void ctxmsg(ctx_t ctx, const char * format, ...) {

  va_list ap;

  va_start(ap, format);
  vprintf(format, ap);
  vsnprintf(ctx->msg, sizeof(ctx->msg), format, ap);
  va_end(ap);

}

static uint32_t isComp(cctype_t type) {
  return (cc_Comp == (type->prop & cc_Comp)) ? 1 : 0;
}

static uint32_t isStruct(cctype_t type) {
  return (cc_Struct == (type->prop & cc_Struct)) ? 1 : 0;
}

static uint32_t isUnion(cctype_t type) {
  return (cc_Union == (type->prop & cc_Union)) ? 1 : 0;
}

static uint32_t isPrim(cctype_t type) {
  return (cc_Prim == (type->prop & cc_Prim)) ? 1 : 0;
}

static uint32_t isBitset(cctype_t type) {
  return (cc_Bitset == (type->prop & cc_Bitset)) ? 1 : 0;
}

typedef struct M2M_t     * m2m_t;
typedef struct ccAlt_t   * alt_t;
typedef struct ClCtx_t   * clctx_t;
typedef struct Clust_t   * clust_t;

typedef struct Clust_t {   // Cluster member
  ccmember_t   ref2size;   // The reference to the size in the original type.
  uint16_t     size;       // How many times it is used as ref2size.
  uint16_t     mi;         // The cluster member is inserted at this member index in the new type.
  uint8_t      move2tail;  // When non zero, move this clustered or single element to the tail.
  uint8_t      done2outer; // When this cluster has been inserted as a member in the new type.
  uint8_t      pad[2];
  cctype_t     type;       // The newly clustered type created from this cluster.
} Clust_t;

typedef struct M2C_t {     // Member to cluster mapping.
  struct {
    uint16_t   orig;       // Member index in original type.
    uint16_t   clui;       // Cluster member index.
  } MI;
  Clust_t *    cluster;    // When not NULL, the member is mapped to this cluster.
} M2C_t;

typedef struct ClCtx_t {   // Cluster Context information
  cctype_t     orig;       // Original type being cluster analyzed.
  uint16_t     created;    // Number of clusters created.
  uint16_t     largest;    // Index of the largest cluster; will be moved to the tail.
  uint16_t     num;        // Number of Clust in use.
  uint16_t     cap;
  M2C_t *      M2C;        // Member map with cap members.
  Clust_t      Clust[0];   // Tail with cap clusters.
} ClCtx_t;

#define NUM(A) (uint32_t)(sizeof(A) / sizeof(A[0]))

const ccType_t cc_Char = { .name = "char",     .prop = cc_Prim, .size = 1, .align = 1 };
const ccType_t cc_U64  = { .name = "uint64_t", .prop = cc_Prim, .size = 8, .align = 8 };
const ccType_t cc_U32  = { .name = "uint32_t", .prop = cc_Prim, .size = 4, .align = 4 };
const ccType_t cc_U16  = { .name = "uint16_t", .prop = cc_Prim, .size = 2, .align = 2 };
const ccType_t cc_U08  = { .name = "uint8_t",  .prop = cc_Prim, .size = 1, .align = 1 };
const ccType_t cc_S64  = { .name = "int64_t",  .prop = cc_Prim, .size = 8, .align = 8 };
const ccType_t cc_S32  = { .name = "int32_t",  .prop = cc_Prim, .size = 4, .align = 4 };
const ccType_t cc_S16  = { .name = "int16_t",  .prop = cc_Prim, .size = 2, .align = 2 };
const ccType_t cc_S08  = { .name = "int8_t",   .prop = cc_Prim, .size = 1, .align = 1 };

static uint32_t roundup(uint32_t value, uint32_t powerOf2) {
  return (value + (powerOf2 - 1)) & ~(powerOf2 - 1);
}

static uint32_t ioMNm(const ccType_t * t, const char * m) { // Return the member index of member with name 'm'.

  uint32_t i;
  
  for (i = 0; i < t->num; i++) {
    if (0 == strcmp(t->Members[i].name, m)) {
      break;
    }
  }
  
  assert(i < t->num);                                       // Should always be found.
  
  return i;

}

static void a4Sz(ctx_t ctx, cctype_t stack[], uint32_t d) { // Internal function for size and alignment calulcation.

  cctype_t   type = stack[d];                               // Type to calculate is at d.
  ccmember_t m = type->Members;
  uint32_t   i;
  uint16_t   align = 0;
  uint16_t   offset = 0;                                    // This is also the running size of the type.
  uint32_t   size = 0;
  uint16_t   padding;

  assert(d < 31);                                           // See in caller.

  for (i = 0; i < d; i++) {
    if (stack[i] == type) {
      ctxmsg(ctx, "Circularity involving [%s].\n", stack[0]->name);
      ctx->error = ccd_Circularity;
    }
  }

  if (ctx->error) { return; }                               // Don't continue in error.

  if (isPrim(type) && isComp(type)) {
    ctxmsg(ctx, "Type %s can't be both Comp and Prim.\n", type->name);
    ctx->error = ccd_BadProperties;
    return;
  }

  if (isPrim(type) || isBitset(type)) { return; }           // Primitives have been done already.

  type->size = 0;
  type->align = 0;

  DBG("%s Type [%s] size %u align %u\n", i4(d), type->name, type->size, type->align);

  for (i = 0; ! ctx->error && i < type->num; i++, m++) {
    if (m->type == type && ! m->numind) {
      ctxmsg(ctx, "Type %s includes itself without indirection.\n", type->name);
      ctx->error = ccd_Circularity;
      break;
    }
    if (m->ref2size) {
      if (! m->isVTail && ! m->numind) {
        ctxmsg(ctx, "[%s] ref2size [%s] implies either indirection or VTail.\n", m->name, m->ref2size->name);
        ctx->error = ccd_LoneRef2size;
        break;
      }
      if (m->isVTail && i + 1 < type->num) {
        ctxmsg(ctx, "VTail [%s] can only apply to final member.\n", m->name);
        ctx->error = ccd_VTailNotFinal;
        break;
      }
    }
    if (m->numind && m->isVTail) {
      ctxmsg(ctx, "[%s] can never both be reference and VTail.\n", m->name);
      ctx->error = ccd_RefAndVTail;
      break;
    }

    DBG("%s member %u [%s] offset %u.\n", i4(d), i, m->name, offset);
    align = 0;
    size = 0;
    if (m->numind) {
      align = ctx->align4ref;
      size  = ctx->size4ref;
    }
    else {
      stack[d + 1] = m->type;
      a4Sz(ctx, stack, d + 1);                              // Call ourselves.
      align = m->type->align;
      size = m->type->size;
    }

    if (align > type->align) {
      type->align = align;
    }

    padding = roundup(offset, align) - offset;

    if (padding) {
      DBG("%s          (%u bytes padding)\n", i4(d), padding);
    }

    if (isStruct(type)) {
      m->offset = offset + padding;                         // Update member information.
    }
    else {
      m->offset = 0;
    }

    DBG("%s          [%s] offset %u size %u align %u\n", i4(d), m->name, m->offset, size, align);

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
    padding = roundup(offset, type->align) - offset;
    assert(! isUnion(type) || 0 == padding);                // A union should not have padding at this point.

    if (padding) {
      DBG("%s needs %u trailing padding bytes.\n", i4(d), padding);
    }

    type->size = offset + padding;

    DBG("%s Done [%s] size %u align %u\n\n", i4(d), type->name, type->size, type->align);
  }

}

void ana4size(ctx_t ctx, cctype_t type) {                   // Analyze for size and also alignment.

  cctype_t stack[32];                                       // Max depth.

  stack[0] = type;

  a4Sz(ctx, stack, 0);

}

#define getmem(CTX, SIZE) _getmem(__LINE__, CTX, SIZE)

static void * _getmem(uint32_t ln, ctx_t ctx, uint32_t sz) {

  void * mem = ctx->mem(ctx, NULL, sz);

  if (! mem) {
    ctx->error = ccd_AllocFailed;
  }

  return mem;
  
}

static char * ccstrdup(ctx_t ctx, const char * str) {       // Return aan allocated duplicate; resembles strdup.

  uint32_t size = strlen(str) + 1;
  char *   dup = getmem(ctx, size);

  if (dup) { strncpy(dup, str, size); }

  return dup;

}

static void freemem(ctx_t ctx, void * mem) {                // Release memory via our context.
  ctx->mem(ctx, mem, 0);
}

static alt_t allocAlt(ctx_t ctx, uint32_t num) {            // Allocate and set up an alternative.

  uint32_t size = sizeof(ccAlt_t);
  alt_t    alt;

  size += num * sizeof(cctype_t);
  alt = getmem(ctx, size);
  if (alt) { memset(alt, 0x00, size); }

  return alt;

}

static void freetype(ctx_t ctx, cctype_t type) {            // Internal function to free a type.

  uint32_t   i;
  uint32_t   x;
  uint32_t   n2mu;
  ccmember_t m;

  for (i = 0; i < ctx->num; i++) {
    if (ctx->types[i] == type) {
      n2mu = sizeof(cctype_t) * (ctx->cap - i - 1);         // Number of bytes to move up.
      memmove(& ctx->types[i], & ctx->types[i + 1], n2mu);
      if (type->alt) {
        freemem(ctx, type->alt);
      }
      if (! (type->prop & cc_Static)) {
        for (x = 0, m = type->Members; x < type->num; x++, m++) {
          if (m->nameisdup) {
            freemem(ctx, m->name);
          }
        }
        freemem(ctx, type);
      }
      ctx->num--;
      break;
    }
  }

  assert(0 == ctx->num || i < ctx->num);                    // Should be found.

}

cctype_t alloctype(ctx_t ctx, uint32_t num) {               // Allocate a type with the given number of members.

  uint32_t   size = sizeof(ccType_t);
  cctype_t   type;
  
  size += sizeof(ccMember_t) * num;
  size += ctx->maxnamesize;

  type = getmem(ctx, size);
  if (type) {
    memset(type, 0x00, size);
    type->name = (char *) & type->Members[num];

    if (ctx->num + 1 < ctx->cap) {                          // Insert it into the table.
      ctx->types[ctx->num++] = type;
    }
    else {
      ctx->error = ccd_NoCapLeft;
    }
  }

  return type;

}

static clust_t add2Cl(clctx_t ci, ccmember_t ref2size) {    // Add member to cluster, if not in there yet; return the cluster member.

  uint32_t x;

  for (x = 0; x < ci->num; x++) {                           // Try to find the ref2size member.
    if (ci->Clust[x].ref2size == ref2size) {
      return & ci->Clust[x];
    }
  }

  assert(ci->num < ci->cap);

  ci->Clust[ci->num++].ref2size = ref2size;                 // Not found, add it.

  return & ci->Clust[ci->num - 1];

}

static ccmember_t finalref(ccmember_t ref2size) {           // Return the most size defining member, if any.

  ccmember_t final = ref2size;
  
  while (final && final->ref2size) {                        // Go to the end of the chain. Careful since final can be NULL.
    final = final->ref2size;
  }
  
  return final;

}

static clctx_t allocluster(ctx_t ctx, uint32_t cap) {       // Allocate a temporary cluster context information structure.

  uint32_t size = sizeof(ClCtx_t);
  clctx_t  cluster;

  size += cap * sizeof(Clust_t);
  size += cap * sizeof(M2C_t);

  cluster = getmem(ctx, size); assert(cluster);
  if (cluster) {
    memset(cluster, 0x00, size);
    cluster->cap = cap;
    cluster->M2C = (M2C_t *) & cluster->Clust[cap];
  }
  
  return cluster;

}

static int32_t mcmp(const void * r2a, const void * r2b, void * r2ctx) {

  ctx_t              ctx = r2ctx;
  const ccMember_t * a = r2a;
  const ccMember_t * b = r2b;
  uint32_t           sz4a;
  uint32_t           sz4b;

  sz4a = a->numind ? ctx->size4ref : a->type->size;
  sz4b = b->numind ? ctx->size4ref : b->type->size;

  return sz4b - sz4a;                                       // We want the largest members end up first.

}

static void mk1Cl(ctx_t ctx, clctx_t clctx, uint32_t ci) {  // Create 1 clustered type from the original type, based upon the cluster info.

  clust_t    clust = & clctx->Clust[ci];
  uint32_t   i;
  cctype_t   type = alloctype(ctx, clust->size);            // New type created from the cluster info.
  ccmember_t nm = type->Members;                            // New member.
  ccmember_t om = clctx->orig->Members;                     // Original member.

  if (! type) { return; }                                   // Error set in context.

  sprintf(type->name, "C%u_%s", ci, clctx->orig->name);

  type->prop = cc_Alt | cc_Struct | cc_Cluster;

  for (i = 0; i < clctx->orig->num; i++, om++) {            // Copy relevant members from original type to new cluster type.
    if (clctx->M2C[i].cluster == clust) {
      *nm = *om;                                            // Bulk copy all member elements first but clear the ...
      nm->ref2size = NULL;                                  // ... ref2size, since that made us cluster in the first place.
      assert(om->numind);                                   // Old member must be a reference.
      if (om->ref2size == clust->ref2size) {
        nm->numind = om->numind - 1;                        // Clustered member will have 1 less indirection, if controlled by this ref2size.
      }
      else {
        nm->numind = om->numind;                            // If not, original number of indirections is preserved.
      }
      nm++; type->num++;
    }
  }

  qsort_r(type->Members, type->num, sizeof(ccMember_t), mcmp, ctx);

  cc_initype(ctx, type);

  clust->type = type;                                       // Save the new type in the cluster info.

  ana4size(ctx, type);
  
}

static uint32_t mkClusters(ctx_t ctx, clctx_t clctx) {      // Create all required clusters; return # members in new base type.

  uint32_t i;
  uint32_t num = clctx->orig->num;                          // Original number of members in the original type.
  uint32_t maxsize = 0;

  for (i = 0; i < clctx->num && ! ctx->error; i++) {
    if (clctx->Clust[i].size > maxsize) {
      maxsize = clctx->Clust[i].size;
      clctx->largest = i;
    }
    if (clctx->Clust[i].size > 1) {
      num -= clctx->Clust[i].size;                          // This number of members moves to ...
      num += 1;                                             // ... this new cluster.
      mk1Cl(ctx, clctx, i);
      clctx->created++;
    }
  }

  if (maxsize) {
    clctx->Clust[clctx->largest].move2tail = 1;             // Mark the largest for moving to tail, if there's one.
  }

  return num;

}

static char * mkMemberName(ctx_t ctx, cctype_t type) {      // Make a member name for the given type; drop the type extension.

  uint32_t size = strlen(type->name);
  char *   temp = alloca(size + 1);
  char *   cut;

  strcpy(temp, type->name);
  cut = strstr(temp, ctx->typeExt);                         // Find the type extension.
  assert(cut);                                              // Should be found.
  cut[0] = 0;                                               // Cut off at the type extension.
  
  return ccstrdup(ctx, temp);                               // Return a duplicate.

}

static void freeCluster(ctx_t ctx, clctx_t cc) {            // Only called to clean up.

  uint32_t i;

  for (i = 0; i < cc->num; i++) {
    if (cc->Clust[0].type) {
      freetype(ctx, cc->Clust[0].type);
    }
  }

}

void cc_ana4cluster(ctx_t ctx, cctype_t type) {

  clctx_t      ci = allocluster(ctx, type->num);
  ccmember_t   m = type->Members;
  uint32_t     i;
  uint32_t     x;
  clust_t      cm;
  cctype_t     ctype;                                       // New cluster type.
  ccmember_t   newm;                                        // New member.
  cctype_t   * fill;
  alt_t        alt;
  ccMember_t   M1;
  ccMember_t   M2;

  if (! ci) { return; }                                     // Error set in context.
  if (! isStruct(type)) { return; }                         // This is only for structure types.

  ci->orig = type;

  for (i = 0; i < type->num; i++, m++) {
    if (m->ref2size) {
      cm = add2Cl(ci, finalref(m->ref2size));               // Assign to the final reference cluster.
      cm->size++;
      ci->M2C[i].cluster = cm;                              // Map the member to this cluster.
      ci->M2C[i].MI.orig = i;                               // Record original member index.
      cm->mi = i;                                           // Already set member index; in case this cluster is only used for marking a new VTail.
    }
  }

  x = mkClusters(ctx, ci);
  if (ctx->error) { return; }

  alt = allocAlt(ctx, 1 + ci->created);                     // Allocate alternative info.
  if (! alt) {                                              // Error set in context.
    freeCluster(ctx, ci);
    freemem(ctx, ci);
    return;
  }

  alt->orig = type; type->alt = alt;                        // Cross link original type and alternative.
  alt->types[0] = alloctype(ctx, x);                        // Create clustered type with remaining number of fields (x).
  if (! alt->types[0]) {                                    // Error set in context.
    freeCluster(ctx, ci);
    freemem(ctx, ci);
    freemem(ctx, alt);
    return;
  }

  alt->types[0]->Cargo = type->Cargo;                       // Copy cargo.
  alt->num = 1;
  sprintf(alt->types[0]->name, "C_%s", type->name);
  alt->types[0]->prop = cc_Alt | cc_Struct;
  fill = & alt->types[1];                                   // Start filling newly created cluster types here, after new base type.
  for (i = 0, cm = ci->Clust; i < ci->num; cm++, i++) {
    if (cm->type) { alt->num++; *fill++ = cm->type; }       // Add the newly created cluster type to the alternative info.
  }

  ctype = alt->types[0];                                    // Build up the new type that will replace the original type.
  for (i = 0, m = type->Members; i < type->num; i++, m++) { // Go over the original class members.
    cm = ci->M2C[i].cluster;
    if (cm && cm->type) {                                   // This member is moved to a cluster.
      if (! cm->done2outer) {                               // Cluster itself is not yet a member.
        cm->type->container = ctype;                        // Set the container type on the cluster.
        cm->mi = ctype->num;                                // Cluster member inserted at this member index in the new type.
        newm = & ctype->Members[ctype->num++];              // The cluster becomes the new member.
        newm->type = cm->type;
        newm->name = mkMemberName(ctx, cm->type);           // Make a new member name by dropping the _t from the type.
        if (newm->name) { newm->nameisdup = 1; }
        cm->done2outer = 1;
      }
    }
    else {
      ctype->Members[ctype->num++] = *m;                    // Copy the unchanged member as is.
    }
    if (ctx->error) { break; }
  }

  if (ctx->error) {                                         // Error set in context.
    freeCluster(ctx, ci);
    freemem(ctx, ci);
    freemem(ctx, alt->types[0]);
    freemem(ctx, alt);
    return;
  }

  assert(x == ctype->num);

  cm = & ci->Clust[ci->largest];

  if (cm->move2tail) {                                      // If needed, swap the last and the cluster member.
    M1 = ctype->Members[ctype->num - 1];
    M2 = ctype->Members[cm->mi];
    M2.isVTail = 1;                                         // This will become the VTail.
    assert(0 == M2.numind || 1 == M2.numind);
    M2.numind = 0;                                          // And since VTail is mutually exclusive with indirection.
    ctype->Members[cm->mi] = M1;
    ctype->Members[ctype->num - 1] = M2;
    cm->mi = ctype->num - 1;                                // Adjust insertion point.
  }

  for (cm = ci->Clust, i = 0; i < ci->num; cm++, i++) {     // Attach the proper ref2size type to newly added clustered members.
    x = ioMNm(ctype, cm->ref2size->name);                   // Do it here since ref2size member could be anywhere.
    ctype->Members[cm->mi].ref2size = & ctype->Members[x];
    if (! cm->move2tail) {                                  // If there's more than 1 cluster and this one didn't ...
      ctype->Members[cm->mi].numind = 1;                    // ... move to the tail, it should become referenced.
    }
  }

  qsort_r(ctype->Members, ctype->num - 1, sizeof(ccMember_t), mcmp, ctx); // Don't sort the last vtail member.

  cc_initype(ctx, ctype);

  ana4size(ctx, ctype);

  uint8_t saved = ctx->cmpMemberNames;
  
  ctx->cmpMemberNames = 1;                                  // The following cc_typecmp we want with name comparison.

  if (0 == cc_typecmp(ctype, type) && 1 == alt->num) {      // Both new and old are the same; so no alternative.
    assert(ci->num == 0);                                   // No clusters should exist.
    assert(ctx->types[ctx->num - 1] == alt->types[0]);      // Should be the last one added.
    freemem(ctx, alt->types[0]);
    freemem(ctx, alt);
    ctx->types[--ctx->num] = NULL;
    type->alt = NULL;
  }

  ctx->cmpMemberNames = saved;                              // Restore used set name comparison.

  freemem(ctx, ci);

}

static void initbitset(ctx_t ctx, cctype_t bs) {

  cctype_t   base = bs->bstype;
  uint32_t   i;
  ccmember_t m;
  uint32_t   remain = base->size * 8;                       // Number of bits remaining in the base type.
  uint32_t   offset = 0;

  if (! isPrim(base)) {
    ctxmsg(ctx, "Bitset [%s] basetype must be primitive.\n", bs->name);
    ctx->error = ccd_BadBaseType;
    return;
  }

  for (m = bs->Members, i = 0; i < bs->num; i++, m++) {     // Ensure all members have the proper base type.
    m->type = base;
  }

  for (m = bs->Members, i = 0; i < bs->num; i++, m++) {     // Check proper width and assign the offset.
    if (0 == m->width) {
      ctxmsg(ctx, "Can not have 0 width for [%s].\n", m->name);
      ctx->error = ccd_ZeroWidth;
    }
    if (remain < m->width) {
      ctxmsg(ctx, "Bits left %u nog enough for [%s] width %u.\n", remain, m->name, m->width);
      ctx->error = ccd_TooSmall;
    }
    if (ctx->error) { break; }
    m->offset = offset;
    remain -= m->width;
    offset += m->width;
  }

  bs->size = base->size;
  bs->align = base->align;

}

void cc_initype(ctx_t ctx, cctype_t type) {

  ccMember_t Marker = {                                     // Prepare the marker.
    .type = type,
    .name = "<marker>",
  };

  assert(& type->Marker == (& type->Members[0]) - 1);       // Marker must come just before the members VTail.

  memcpy((void*) & type->Marker, & Marker, sizeof(Marker)); // To overcome the const typeness of the marker (yes, it's a hack).
  
  type->ctx = ctx;
  
  type->prop |= cc_Comp;

  if (isBitset(type)) {
    if (type->bstype) {
      initbitset(ctx, type);
    }
    else {
      ctx->error = ccd_BadBaseType;
    }
  }

  ana4size(ctx, type);
  
}

cctype_t cc_mem2cont(const ccMember_t * mem, uint32_t mi[1]) {

  const ccMember_t * cur;

  for (cur = mem; strcmp(cur->name, "<marker>"); cur--) {   // Always search up for the marker string set in initype.
    assert(mem - cur < 1024);                               // Have some kind of check.
  }

  if (mi) { mi[0] = mem - cur - 1; }                        // Set member index if not passed NULL.
  
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

static int32_t _typecmp(const ccType_t *a, const ccType_t *b, uint32_t r);

static int32_t cmpMemb(const ccMember_t *a, const ccMember_t *b, uint32_t r) { // Compare 2 members if one could replace the other. Does not compare names.

  int32_t d = 0;

  if (isComp(a->type) && isComp(b->type)) {                 // Both composite.
    DBG("%s both [%s:%s] [%s:%s] composite\n", i4(r), a->type->name, a->name, b->type->name, b->name);
    d = _typecmp(a->type, b->type, r + 1);
  }
  else if (isPrim(a->type) && isPrim(b->type)) {            // Both primitive.
    d = a->type - b->type;
    if (d) {
      DBG("%s Both primitives [%s] [%s] differ.\n", i4(r), a->type->name, b->type->name);
    }
  }
  else if (isComp(a->type)) {                               // A is a composite and B isn't.
    d = -1;
    DBG("%s A [%s] is composite, B [%s] primitive.\n", i4(r), a->type->name, b->type->name);
  }
  else {                                                    // B is a composite and A isn't.
    d = +1;
    DBG("%s A [%s] is primitive, B [%s] composite.\n", i4(r), a->type->name, b->type->name);
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

static int32_t _typecmp(const ccType_t *a, const ccType_t *b, uint32_t r) {  // Compare 2 types. Does not compare the names.

  int32_t  d = a->size - b->size;
  uint32_t i;
  uint32_t ia;
  uint32_t ib;
  uint32_t aIsBitset;
  uint32_t bIsBitset;
  ctx_t    ctx = a->ctx;

  if (a == b) {
    DBG("%s Both a and b same type [%s].\n", i4(r), a->name);
    return 0;
  }

  if (a->ctx != b->ctx) {
    ctxmsg(a->ctx, "WARNING [%s] %p and [%s] %p have a different context.\n", a->name, a->ctx, b->name, b->ctx);
  }

  DBG("%s Compare [%s s %u a %u m %u] [%s s %u a %u m %u].\n", i4(r), a->name, a->size, a->align, a->num, b->name, b->size, b->align, b->num);

  COMPARE(a->size, b->size);
 
  COMPARE(a->align, b->align);

  COMPARE(a->num, b->num);

  for (i = 0; i < a->num; i++) {
    DBG("%s member %u A [%s] B [%s]\n", i4(r), i, a->Members[i].name, b->Members[i].name);
    ia = 0xffff;
    if (a->Members[i].ref2size) {
      ia = ioMNm(a, a->Members[i].ref2size->name);
    }
    ib = 0xffff;
    if (b->Members[i].ref2size) {
      ib = ioMNm(b, b->Members[i].ref2size->name);
    }
    d = ia - ib;                                            // Difference in ref2size, if any.
    if (d) {
      DBG("%s differ in ref2size.\n", i4(r));
      return d;
    }

    if (ctx->cmpMemberNames) {
      d = strcmp(a->Members[i].name, b->Members[i].name);
      if (d) {
        DBG("%s [%s] and [%s] differ.\n", i4(r), a->Members[i].name, b->Members[i].name);
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

int32_t cc_typecmp(const ccType_t *a, const ccType_t *b) {  // Compare 2 types. Does not compare the names.

  return _typecmp(a, b, 0);

}

cctype_t cc_clone4type(ctx_t ctx, const ccType_t * type) {  // Clone a type (ensures it becomes a context member).

  const ccMember_t * m = type->Members;
  uint32_t           i;
  cctype_t           clone = alloctype(ctx, type->num);

  memcpy(clone, type, sizeof(ccType_t));                    // Copy the bulk of the type (overwrites the name reference).
  clone->name = (char *) & clone->Members[type->num];       // Was overwritten by the memcpy; so re-assign it.
  strncpy(clone->name, type->name, ctx->maxnamesize - 1);   // Copy name contents.
  clone->prop &= ~cc_Static;                                // Remove the static property, if any.

  for (i = 0; i < type->num; i++, m++) {
    memcpy(& clone->Members[i], m, sizeof(ccMember_t));
  }

  cc_initype(ctx, clone);                                   // Ensure the marker is set properly.

  return clone;

}

uint32_t cc_clearmarks(ctx_t ctx) {                         // Set all the type marks to 0.

  uint32_t i;
  uint32_t count;
  
  for (count = 0, i = 0; i < ctx->num; i++) {
    if (ctx->types[i]) {
      ctx->types[i]->mark = 0;
      count++;
    }
  }

  return count;
  
}

void cc_scan4type(ctx_t ctx, cc_cb4t_t cb, void * arg) {    // Scan over all types in the context.

  uint32_t i;
  
  for (i = 0; i < ctx->num; i++) {
    if (ctx->types[i]) {
      cb(ctx, ctx->types[i], arg);
    }
  }

}

void cc_scan4mem(ccCtx_t * ctx, cc_cb4m_t cb, void * arg) { // Scan over all members in the context.

  cctype_t   ct;
  ccmember_t cm;
  uint32_t   x;
  uint32_t   i;

  for (x = 0; x < ctx->num; x++) {
    ct = ctx->types[x]; if (! ct) { continue; }
    for (cm = ct->Members, i = 0; i < ct->num; i++, cm++) {
      cb(ctx, cm, arg);
    }
  }

}

static void check4type(ccCtx_t * ctx, ccmember_t m, void * arg) {

  xref_t xref = arg;
  
  if (m->type == xref->type) {
    if (xref->num < xref->cap) {
      xref->used[xref->num++] = m;
    }
  }

}

void cc_xref4type(ccCtx_t * ctx, ccXRef_t * xref) {

  xref->num = 0;

  cc_scan4mem(ctx, check4type, xref);

}

void cc_xref4mem(ccCtx_t * ctx, ccXRef_t * xref) {

}

void cc_remove4type(ccCtx_t * ctx, ccXRef_t * xref) {

  if (xref->type && ! isPrim(xref->type)) {                 // Don't remove primitives; they are not even in the table.
    cc_xref4type(ctx, xref);                                // See if the type is used somewhere.
    if (0 == xref->num) {
      freetype(ctx, xref->type);
    }
    else {
    }
  }

}

typedef struct UDA_t {            // Usage/Dependency Analysis
  uint32_t    iteration;
  uint32_t    changes;
  uint32_t    forwards;           // Non zero when there are forward members.
  uint32_t    pruned;
  uint32_t    refs2self;
} UDA_t;

static uint32_t isRef2Self(cctype_t t, ccmember_t m) {      // Return non zero when the member is a reference to self.

  return ((m->type == t) && m->numind) ? 1 : 0;

}

static void clrweight(ccCtx_t * ctx, cctype_t t, void * arg) {
  t->weight = 0;
}

static void check4forwards(ccCtx_t * ctx, cctype_t t, void * arg) {

  uint32_t   i;
  ccmember_t m;
  
  for (m = t->Members, i = 0; i < t->num; i++, m++) {
    if (m->isForward && t->weight) { t->weight--; }         // Decuct 1 weight for each forward member.
  }

}

static void setweight(ccCtx_t * ctx, ccmember_t m, void * arg) {

  UDA_t  * uda = arg;
  cctype_t cont = cc_mem2cont(m, NULL);                     // Get containing type.

  if (isPrim(m->type)) { return; }                          // We don't weigh primitives.

  if (! isRef2Self(cont, m)) {
    m->type->weight++;
    if (uda->iteration == ctx->num && m->numind) {          // But not until eternity.
      m->isForward = 1;                                     // We mark it as a forward ref.
      uda->forwards++;
    }
    else {
      uda->changes++;
    }
  }

}

static void prunefwds(ccCtx_t * ctx, ccmember_t m, void * arg) {

  UDA_t  * uda = arg;
  cctype_t cont = cc_mem2cont(m, NULL);

  if (isPrim(m->type)) { return; }                          // We don't do primitives.

  if (! isRef2Self(cont, m)) {
    if (m->isForward && m->type->weight > cont->weight) {   // The type the member refers to is defined earlier.
      m->isForward = 0;                                     // We can remove the forward property.
      uda->pruned++;
    }
  }
  else {                                                    // A reference to self is always a forward reference.
    m->isForward = 1;
    m->isRef2Self = 1;
    uda->refs2self++;
  }

}

static int32_t weightcmp(const void * r2a, const void * r2b) {

  const ccType_t * a = * (const ccType_t **) r2a;
  const ccType_t * b = * (const ccType_t **) r2b;

  return b->weight - a->weight;                             // Highest should float to the top.

}

void cc_prep4gen(ccCtx_t * ctx) {

  UDA_t UDA;

  memset(& UDA, 0x00, sizeof(UDA));

  cc_scan4type(ctx, clrweight, NULL);

  do {
    UDA.changes = 0;
    UDA.iteration++;
    cc_scan4mem(ctx, setweight, & UDA);
  } while (UDA.changes && UDA.iteration <= ctx->num);       // Try to order according to usage as much as possible.

  if (UDA.forwards) {                                       // We had types refering to types not yet defined; i.e. forward references.
    cc_scan4type(ctx, check4forwards, NULL);                // This should make the type using less forward members, move higher.
  }

  qsort(ctx->types, ctx->num, sizeof(cctype_t), weightcmp); // Make the most referred to type come first to reduce forward references.

  for (uint32_t i = 0; i < ctx->num; i++) {                 // Set weight from high to low.
    ctx->types[i]->weight = ctx->num - i;
  }

  cc_scan4mem(ctx, prunefwds, & UDA);                       // Get rid of unnecessary forwards and also set the isRef2Self properties.

  DBG("%u types, %u forwards, %u pruned away, %u refs to self.\n", ctx->num, UDA.forwards, UDA.pruned, UDA.refs2self);

}
