// Copyright 2023 Steven Buytaert

#define  _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>

#include <t2c-types.h>

typedef struct t2c_Ctx_t *    ctx_t;    // Internally used shorthands.
typedef struct t2c_XRef_t *   xref_t;
typedef struct t2c_Type_t *   type_t;
typedef struct t2c_Member_t * member_t;
typedef struct t2c_Alt_t *    alt_t;
typedef struct t2c_TGSpec_t * spec_t;

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

typedef struct M2M_t     * m2m_t;
typedef struct ClCtx_t   * clctx_t;
typedef struct Clust_t   * clust_t;

typedef struct Clust_t {   // Cluster member
  member_t   ref2size;     // The reference to the size in the original type.
  uint16_t   size;         // How many times it is used as ref2size.
  uint16_t   mi;           // The cluster member is inserted at this member index in the new type.
  uint8_t    move2tail;    // When non zero, move this clustered or single element to the tail.
  uint8_t    done2outer;   // When this cluster has been inserted as a member in the new type.
  uint8_t    pad[2];
  type_t     type;         // The newly clustered type created from this cluster.
} Clust_t;

typedef struct M2C_t {     // Member to cluster mapping.
  struct {
    uint16_t orig;         // Member index in original type.
    uint16_t clui;         // Cluster member index.
  } MI;
  Clust_t *  cluster;      // When not NULL, the member is mapped to this cluster.
} M2C_t;

typedef struct ClCtx_t {   // Cluster Context information
  type_t     orig;         // Original type being cluster analyzed.
  uint16_t   created;      // Number of clusters created.
  uint16_t   largest;      // Index of the largest cluster; will be moved to the tail.
  uint16_t   num;          // Number of Clust in use.
  uint16_t   cap;
  M2C_t *    M2C;          // Member map with cap members.
  Clust_t    Clust[0];     // Tail with cap clusters.
} ClCtx_t;

#define NUM(A) (uint32_t)(sizeof(A) / sizeof(A[0]))

t2c_Type_t t2c_VoidRef = { .name = "void *",   .prop = t2c_Prim, .size = 4, .align = 4 };

t2c_Type_t t2c_Char    = { .name = "char",     .prop = t2c_Prim,              .size = 1, .align = 1 };
t2c_Type_t t2c_U64     = { .name = "uint64_t", .prop = t2c_Prim,              .size = 8, .align = 8 };
t2c_Type_t t2c_U32     = { .name = "uint32_t", .prop = t2c_Prim,              .size = 4, .align = 4 };
t2c_Type_t t2c_U16     = { .name = "uint16_t", .prop = t2c_Prim,              .size = 2, .align = 2 };
t2c_Type_t t2c_U08     = { .name = "uint8_t",  .prop = t2c_Prim,              .size = 1, .align = 1 };
t2c_Type_t t2c_S64     = { .name = "int64_t",  .prop = t2c_Prim | t2c_Signed, .size = 8, .align = 8 };
t2c_Type_t t2c_S32     = { .name = "int32_t",  .prop = t2c_Prim | t2c_Signed, .size = 4, .align = 4 };
t2c_Type_t t2c_S16     = { .name = "int16_t",  .prop = t2c_Prim | t2c_Signed, .size = 2, .align = 2 };
t2c_Type_t t2c_S08     = { .name = "int8_t",   .prop = t2c_Prim | t2c_Signed, .size = 1, .align = 1 };
t2c_Type_t t2c_F32     = { .name = "float",    .prop = t2c_Prim | t2c_Signed, .size = 4, .align = 4 };
t2c_Type_t t2c_F64     = { .name = "double",   .prop = t2c_Prim | t2c_Signed, .size = 8, .align = 8 };

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

static void a4Sz(ctx_t ctx, type_t stack[], uint32_t d) {   // Internal function for size and alignment calulcation.

  type_t   type = stack[d];                                 // Type to calculate is at d.
  member_t m = type->Members;
  uint32_t i;
  uint16_t align = 0;
  uint16_t offset = 0;                                      // This is also the running size of the type.
  uint32_t size = 0;
  uint16_t padding;

  assert(d < 31);                                           // See in caller.

  for (i = 0; i < d; i++) {
    if (stack[i] == type) {
      ctxmsg(ctx, "Circularity involving type [%s].", stack[0]->name);
      ctx->error = t2c_Circularity;
    }
  }

  if (ctx->error) { return; }                               // Don't continue in error.

  if (isPrim(type) && isComp(type)) {
    ctxmsg(ctx, "Type [%s] can't be both Comp and Prim.", type->name);
    ctx->error = t2c_BadProperties;
    return;
  }

  if (isPrim(type) || isBitset(type) || t2c_isEnum(type) || t2c_isTypedef(type)) {     // Primitives have been done already.
    return;
  }

  type->size = 0;
  type->align = 0;

  DBG("%s Type [%s] size %u align %u\n", i4(d), type->name, type->size, type->align);

  for (i = 0; ! ctx->error && i < type->num; i++, m++) {
    if (m->type == type && ! m->numind) {
      ctxmsg(ctx, "Type [%s] includes itself without indirection.", type->name);
      ctx->error = t2c_Circularity;
      break;
    }
    if (m->ref2size) {
      if (! m->isVTail && ! m->numind) {
        ctxmsg(ctx, "Member [%s] ref2size [%s] implies either indirection or VTail.", m->name, m->ref2size->name);
        ctx->error = t2c_LoneRef2size;
        break;
      }
      if (m->isVTail && i + 1 < type->num) {
        ctxmsg(ctx, "VTail [%s] can only apply to final member.", m->name);
        ctx->error = t2c_VTailNotFinal;
        break;
      }
    }
    if (m->numind && m->isVTail) {
      ctxmsg(ctx, "Member [%s] can never both be reference and VTail.", m->name);
      ctx->error = t2c_RefAndVTail;
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
      if (! m->type) {
        ctxmsg(ctx, "Member [%s] has a bad/no type.", m->name);
        ctx->error = t2c_BadMemberType;
        return;
      }
      stack[d + 1] = m->type;
      a4Sz(ctx, stack, d + 1);                              // Call ourselves to analyze the type of the member.
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
      m->offset = (uint32_t)(offset + padding);             // Update member information.
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

void t2c_ana4size(ctx_t ctx, type_t type) {                 // Analyze for size and also alignment.

  type_t stack[32];                                         // Max depth.

  stack[0] = type;

  a4Sz(ctx, stack, 0);

}

#define getmem(CTX, SIZE) _getmem(__LINE__, CTX, SIZE)

static void * _getmem(uint32_t ln, ctx_t ctx, uint32_t sz) {

  void * mem = ctx->mem(ctx, NULL, sz);

  if (! mem) {
    ctx->error = t2c_AllocFailed;
  }
  else {
    memset(mem, 0x00, sz);
  }

  return mem;
  
}

static char * ccstrdup(ctx_t ctx, const char * str) {       // Return an allocated duplicate; resembles strdup.

  uint32_t size = strlen(str) + 1;
  char *   dup = getmem(ctx, size);

  if (dup) { strncpy(dup, str, size); }

  return dup;

}

static void freemem(ctx_t ctx, void * mem) {                // Release memory via our context.
  ctx->mem(ctx, mem, 0);
}

void t2c_renam(t2c_ctx_t ctx, t2c_member_t m, const char * name) {

  char * newname = ccstrdup(ctx, name);

  if (newname) {
    if (m->nameisdup) {
      freemem(ctx, (void *) m->name);
    }
    m->name = newname;
    m->nameisdup = 1;                                       // We allocated the name here.
  }

}

static alt_t allocAlt(ctx_t ctx, uint32_t num) {            // Allocate and set up an alternative.

  uint32_t size = sizeof(t2c_Alt_t);
  alt_t    alt;

  size += num * sizeof(type_t);
  alt = getmem(ctx, size);

  return alt;

}

static void freetype(ctx_t ctx, type_t type) {              // Internal function to free a type.

  uint32_t i;
  uint32_t x;
  uint32_t n2mu;
  member_t m;

  for (i = 0; i < ctx->num; i++) {
    if (ctx->types[i] == type) {
      n2mu = sizeof(type_t) * (ctx->cap - i - 1);           // Number of bytes to move up.
      memmove(& ctx->types[i], & ctx->types[i + 1], n2mu);
      ctx->num--;                                           // One less member.
      if (type->alt) {
        freemem(ctx, type->alt);
      }
      if (! (type->prop & t2c_Static)) {
        for (x = 0, m = type->Members; x < type->num; x++, m++) {
          if (m->nameisdup) {
            freemem(ctx, (char *) m->name);
          }
        }
        freemem(ctx, type);                                 // Only now release the memory.
      }
      return;
    }
  }

  assert(0);                                                // Should have been found.

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

static type_t alloctype(ctx_t ctx, uint32_t num) {          // Allocate a type with the given number of members.

  uint32_t size = sizeof(t2c_Type_t);
  type_t   type;
  
  size += sizeof(t2c_Member_t) * num;
  size += ctx->maxnamesize;

  type = getmem(ctx, size);
  if (type) {
    type->name = (char *) & type->Members[num];

    if (ctx->num + 1 < ctx->cap) {                          // Insert it into the table.
      ctx->types[ctx->num++] = type;
    }
    else {
      ctxmsg(ctx, "No slots left for storing types; cap %u.", ctx->cap);
      ctx->error = t2c_NoCapLeft;
      freemem(ctx, type);
    }
  }

  return type;

}

static clust_t add2Cl(clctx_t ci, member_t ref2size) {      // Add member to cluster, if not in there yet; return the cluster member.

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

static member_t finalref(member_t ref2size) {               // Return the most size defining member, if any.

  member_t final = ref2size;
  
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
    cluster->cap = cap;
    cluster->M2C = (M2C_t *) & cluster->Clust[cap];
  }
  
  return cluster;

}

static int32_t mcmp(const void * r2a, const void * r2b, void * r2ctx) { // Member size compare for qsort.

  ctx_t              ctx = r2ctx;
  const t2c_Member_t * a = r2a;
  const t2c_Member_t * b = r2b;
  uint32_t           sz4a;
  uint32_t           sz4b;

  sz4a = a->numind ? ctx->size4ref : a->type->size;
  sz4b = b->numind ? ctx->size4ref : b->type->size;

  return (int32_t) (sz4b - sz4a);                           // We want the largest members end up first.

}

static void mk1Cl(ctx_t ctx, clctx_t clctx, uint32_t ci) {  // Create 1 clustered type from the original type, based upon the cluster info.

  clust_t  clust = & clctx->Clust[ci];
  uint32_t i;
  type_t   type = alloctype(ctx, clust->size);              // New type created from the cluster info.
  member_t nm = type->Members;                              // New member.
  member_t om = clctx->orig->Members;                       // Original member.

  if (! type) { return; }                                   // Error set in context.

  sprintf(type->name, "C%u_%s", ci, clctx->orig->name);

  type->prop = t2c_Alt | t2c_Struct | t2c_Cluster;

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

  qsort_r(type->Members, type->num, sizeof(t2c_Member_t), mcmp, ctx);

  t2c_initype(ctx, type);

  clust->type = type;                                       // Save the new type in the cluster info.

  t2c_ana4size(ctx, type);
  
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

static char * mkMemberName(ctx_t ctx, type_t type) {        // Make a member name for the given type; drop the type extension.

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

void t2c_ana4cluster(ctx_t ctx, type_t type) {

  clctx_t      ci = allocluster(ctx, type->num);
  member_t     m = type->Members;
  uint32_t     i;
  uint32_t     x;
  clust_t      cm;
  type_t       ctype;                                       // New cluster type.
  member_t     newm;                                        // New member.
  type_t *     fill;
  alt_t        alt;
  t2c_Member_t M1;
  t2c_Member_t M2;

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

  alt = allocAlt(ctx, 1u + ci->created);                    // Allocate alternative info.
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
  alt->types[0]->prop = t2c_Alt | t2c_Struct;
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

  qsort_r(ctype->Members, ctype->num - 1u, sizeof(t2c_Member_t), mcmp, ctx); // Don't sort the last vtail member.

  t2c_initype(ctx, ctype);

  t2c_ana4size(ctx, ctype);

  uint8_t saved = ctx->cmpNames;
  
  ctx->cmpNames = 1;                                        // The following t2c_typecmp we want with name comparison.

  if (0 == t2c_typecmp(ctype, type) && 1 == alt->num) {     // Both new and old are the same; so no alternative.
    assert(ci->num == 0);                                   // No clusters should exist.
    assert(ctx->types[ctx->num - 1] == alt->types[0]);      // Should be the last one added.
    freemem(ctx, alt->types[0]);
    freemem(ctx, alt);
    ctx->types[--ctx->num] = NULL;
    type->alt = NULL;
  }

  ctx->cmpNames = saved;                                    // Restore used set name comparison.

  freemem(ctx, ci);

}

static void initbitset(ctx_t ctx, type_t bs) {

  type_t   base = bs->boetype;
  uint32_t i;
  member_t m;
  uint32_t remain = (uint32_t)(base->size * 8);             // Number of bits remaining in the base type.
  uint32_t offset = 0;

  if (! isPrim(base)) {
    ctxmsg(ctx, "Bitset [%s] basetype must be primitive.", bs->name);
    ctx->error = t2c_BadBaseType;
    return;
  }

  for (m = bs->Members, i = 0; i < bs->num; i++, m++) {     // Ensure all members have the proper base type.
    m->type = base;
  }

  for (m = bs->Members, i = 0; i < bs->num; i++, m++) {     // Check proper width and assign the offset.
    if (0 == m->width) {
      ctxmsg(ctx, "Can not have 0 width for [%s].", m->name);
      ctx->error = t2c_ZeroWidth;
    }
    if (remain < m->width) {
      ctxmsg(ctx, "Not enough bits left %u for [%s] width %u.", remain, m->name, m->width);
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
    ctxmsg(ctx, "Enum [%s] basetype must be primitive.", et->name);
    ctx->error = t2c_BadBaseType;
    return;
  }

  for (m = et->Members, i = 0; i < et->num; i++, m++) {     // Ensure all members have the proper base type.
    m->type = base;
    for (x = 0; x < et->num; x++) {
      if (x == i) continue;                                 // Don't the member with itself.
      if (m->enumval == et->Members[x].enumval) {
        ctxmsg(ctx, "Members [%s] [%s] are duplicates.", m->name, et->Members[x].name), 
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
      ctxmsg(ctx, "Bitset [%s] has no basetype.\n", type->name);
      ctx->error = t2c_BadBaseType;
    }
  }
  else if (t2c_isEnum(type)) {
    if (type->boetype) {
      initenum(ctx, type);
    }
    else {
      ctxmsg(ctx, "Enum [%s] has no basetype.\n", type->name);
      ctx->error = t2c_BadBaseType;
    }
  }
  else if (t2c_isTypedef(type)) {
    if (1 != type->num) {                         // Typedefs should have exactly one member.
      if (! type->Members[0].type) {
        ctxmsg(ctx, "Typedef [%s] has no basetype.\n", type->name);
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

static int32_t _typecmp(const t2c_Type_t *a, const t2c_Type_t *b, uint32_t r) {  // Compare 2 types. Does not compare the names.

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
    ctxmsg(a->ctx, "WARNING [%s] %p and [%s] %p have a different context.", a->name, a->ctx, b->name, b->ctx);
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
    d = (int32_t)(ia - ib);                                 // Difference in ref2size, if any.
    if (d) {
      DBG("%s differ in ref2size.\n", i4(r));
      return d;
    }

    if (ctx->cmpNames) {
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

int32_t t2c_typecmp(const t2c_Type_t *a, const t2c_Type_t *b) {  // Compare 2 types. Does not compare the context.

  return _typecmp(a, b, 0);

}

type_t t2c_clone4type(ctx_t ctx, const t2c_Type_t * type) { // Clone a type (ensures it becomes a context member).

  const t2c_Member_t * m = type->Members;
  uint32_t             i;
  type_t               clone = alloctype(ctx, type->num);

  if (! clone) { return NULL; }                             // Probably 'no capacity left'; error set by alloctype.

  memcpy(clone, type, sizeof(t2c_Type_t));                  // Copy the bulk of the type (overwrites the name reference).
  clone->name = (char *) & clone->Members[type->num];       // Was overwritten by the memcpy; so re-assign it.
  strncpy(clone->name, type->name, ctx->maxnamesize - 1u);  // Copy name contents.
  clone->prop &= (uint8_t) ~t2c_Static;                     // Remove the static property, if any.
  clone->ref2type = NULL;                                   // This field is never cloned.

  for (i = 0; i < type->num; i++, m++) {
    memcpy(& clone->Members[i], m, sizeof(t2c_Member_t));
  }

  t2c_initype(ctx, clone);                                  // Ensure the marker is set properly.

  if (ctx->error) {                                         // Something went wrong, remove the bad type.
    freetype(ctx, clone);
    clone = NULL;
  }

  return clone;

}

typedef struct RepCtx_t {         // Replacement context.
  t2c_type_t tdtype;
  uint32_t   count;
} RepCtx_t;

static void rep4td(t2c_ctx_t ctx, t2c_member_t m, void * arg) {

  RepCtx_t * rep = arg;

  if (t2c_mem2cont(m, NULL) == rep->tdtype) { return; }     // We shouldn't replace ourselves.

  if (m->type == rep->tdtype->Members[0].type && m->numind) {
//    printf("REPLACE [%s*] with [%s] in [%s]\n", m->type->name, rep->tdtype->name, t2c_mem2cont(m, NULL)->name);
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

void t2c_remove4type(ctx_t ctx, xref_t xref) {

  if (xref->type && ! isPrim(xref->type)) {                 // Don't remove primitives; they are not even in the table.
    t2c_xref4type(ctx, xref);                               // See if the type is used somewhere.
    if (0 == xref->num) {
      freetype(ctx, xref->type);
    }
    else {
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
  char *   start;
  uint16_t off;
} Line_t;

typedef struct Line_t * line_t;

static void out(spec_t spec, line_t line, const char * format, ...) {

  va_list ap;
  int32_t ss;

  if (spec->overflow) { return; }

  va_start(ap, format);
  ss = vsnprintf(line->start + line->off, spec->Buf.rem, format, ap);
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

  if (spec->Lines.num + 1 < spec->Lines.cap && ! spec->overflow) {
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

void t2c_fmttype(t2c_ctx_t ctx, t2c_type_t type, spec_t spec) {

  Line_t       L;
  uint32_t     i;
  t2c_member_t m = type->Members;
  uint32_t     ti = prop2ti(type->prop);
  uint32_t     mti;
  uint32_t     isBitset = (ti == 2);
  uint32_t     isEnum   = (ti == 5);
  char         evfmt[32];
  char         eol[32];
  uint32_t     tab4type = spec->Tab.type;
  uint32_t     anonunion = 0;

  static const char * ti2name[] = {
    "",
    "", 
    "struct", // Bitsets are rendered as struct.
    "struct",
    "union",
    "enum",
    "typedef",
  };

  spec->overflow  = 0;
  spec->Lines.num = 0;
  spec->Buf.rem   = spec->Buf.cap;
  
  memset(spec->Buf.buf, 0x00, spec->Buf.cap);

  L.start = spec->Buf.buf;
  L.off   = 0;

  if (1 == ti || 0 == ti) {                                 // We don't format primitive or unknown types.
    return;
  }

  if (spec->evfmt) {                                        // Create the format string for the enumeration value.
    sprintf(evfmt, "= %s", spec->evfmt);
  }
  else {
    sprintf(evfmt, "= %%u");                                // Not specified, create a default.
  }

  if (spec->addEOLComments) {
    sprintf(eol, "//");
  }
  else {
    //sprintf(eol, "");
    eol[0] = 0;
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

    for (i = 0; i < type->num; i++, m++) {                  // Now do all members.

      pad(spec, & L, spec->Tab.type);

      if (anonunion != m->anonunion) {                      // Start the anonymous union.
        anonunion = 1;                                      // Active.
        out(spec, & L, "union {");
        pad(spec, & L, spec->Tab.ends);
        add2spec(spec, & L);
        spec->Tab.type += 2;                                // Indent 2 more.
        pad(spec, & L, spec->Tab.type);
      }

      if (m->isConst) {
        out(spec, & L, "const ");
      }

      if (! isEnum) {
        mti = prop2ti(m->type->prop);                       // Member type index.
        if (spec->useFwDec && (3 == mti || 4 == mti)) {     // When a struct or a union and we need to use forwarding declarations.
          out(spec, & L, "%s ", ti2name[mti]);
        }
        out(spec, & L, "%s ", m->type->name);
      }

      for (uint32_t x = 0; x < m->numind; x++) {
        out(spec, & L, "*");
      }

      if (! isEnum) {                                       // Enumeration values are filled out later.
        pad(spec, & L, spec->Tab.member);
      }

      out(spec, & L, "%s", m->name);

      if (isBitset) {
        pad(spec, & L, spec->Tab.bits);
        out(spec, & L, ": %2u", m->width);
      }
      else if (isEnum) {
        pad(spec, & L, spec->Tab.member);                   // Enumeration values start where the member would start.
        out(spec, & L, evfmt, m->enumval);
      }
      else if (m->fxdsize) {
        out(spec, & L, "[%u]", m->fxdsize);
      }
      else if (m->isVTail) {
        out(spec, & L, "[0]");
      }

      out(spec, & L, "%c", isEnum ? ',' : ';');
      pad(spec, & L, spec->Tab.comment);
      out(spec, & L, eol);
      pad(spec, & L, spec->Tab.ends);
      if (spec->overflow) { return; }                       // No need to continue
      add2spec(spec, & L);

      if (i + 1 == type->num || (m + 1)->anonunion != m->anonunion) {
        if (anonunion) {
          anonunion = 0;                                    // No longer active.
          spec->Tab.type = tab4type;                        // Restore previous indent.
          pad(spec, & L, spec->Tab.type);
          out(spec, & L, "};");
          pad(spec, & L, spec->Tab.ends);
          add2spec(spec, & L);
        }
      }
    }

    out(spec, & L, "} %s;", type->name);  
    add2spec(spec, & L);
  }

}
