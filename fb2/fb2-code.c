// Copyright 2023 Steven Buytaert

#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include <inttypes.h>

#include <snset.h>
#include <fb2-code.h>
#include <fb2-types.h>

typedef struct Ctx_t * ctx_t;

static const uint32_t cookie1 = 0xfafeb0be;
static const uint32_t cookie2 = 0xf0efd0ef;
static const uint32_t cookie3 = 0xf1faf0ef;

typedef enum {
  phase_t2ctypes_generated    = 1,
  phase_consts_generated      = 2,
  phase_names_types_generated = 3, // The nametable and Types[] array has been generated.
  phase_compounds_generated   = 4, // The table with all compound types is generated.
  phase_vtabs_generated       = 5,
} Phase;

typedef struct Ctx_t {            // Internal context.
  uint32_t        cookie1;
  uint16_t        string_tti;     // String type table index.
  uint8_t         phase;
  uint8_t         pad[1];
  fb2_type_t      canon[11];      // The canonical primitive FB2 types.
  fb2_codeCtx_t   fb2code;
  SNSet_t         Const;          // Default and enum values constants.
  SNSet_t         Names;          // Names table.
  SNSet_t         Types;          // Types table; indexed by fb2 type->tti.
  SNSet_t         Compounds;
  SNSet_t         VTabs;          // Structure VTabs.
  t2c_type_t      root;           // The root type as a T2C type.
  t2c_type_t      unresolved;     // Type to resolve later.
} Ctx_t;

typedef struct Type_t *   type_t;
typedef struct Comp_t *   comp_t;
typedef struct Member_t * member_t;
typedef struct VTab_t *   vtab_t;

static const char * ct2a[] = {
    "ct_none,     ", //  0,
    "ct_int8,     ", //  1,
    "ct_uint8,    ", //  2,
    "ct_int16,    ", //  3,
    "ct_uint16,   ", //  4,
    "ct_int32,    ", //  5,
    "ct_uint32,   ", //  6,
    "ct_int64,    ", //  7,
    "ct_uint64,   ", //  8,
    "ct_float,    ", //  9,
    "ct_double,   ", // 10,
    "ct_string,   ", // 11,
    "ct_neg_int8, ", // 12,
    "ct_neg_int16,", // 13,
    "ct_neg_int32,", // 14,
    "ct_neg_int64,", // 15,
};

#define NUM(A) (sizeof(A) / sizeof(A[0]))

typedef enum {
  vector_mark   = 0,  // The T2C type is a vector; used as a bool.
  compound_mark = 1,  // The T2C type as a compound table entry; used as a bool.
  vtab_mark     = 2,  // The T2C type has a vtab at this index.
} Marks;

static uint32_t optimize = 0;

// We juggle 4 different contexts here; so make some accessors and check.

static ctx_t any2ctx(void * any) {

  ctx_t ctx = any;
  
  assert(any && cookie1 == ctx->cookie1);
  
  return ctx;
  
}

static fb2_codeCtx_t ctx2code(void * any) {

  const fb2_codeCtx_t ctx = any2ctx(any)->fb2code;

  assert(cookie2 == ctx->cookie);

  return ctx;

}

static t2c_ctx_t ctx2tc(void * any) {

  t2c_ctx_t ctx = & ctx2code(any)->t2cCtx;

  assert(t2ccookie == ctx->cookie);

  return ctx;

}

static fb2_schemaCtx_t ctx2schema(void * any) {

  fb2_schemaCtx_t ctx = ctx2code(any)->schemaCtx;

  assert(cookie3 == ctx->cookie);

  assert(! memcmp(ctx->schema->magic, fb2sid, sizeof(ctx->schema->magic)));

  return ctx;

}

// --- End of accessor functions.

static void * getmem(ctx_t ctx, uint32_t size) {

  fb2_codeCtx_t code = ctx2code(ctx);

  void * block = code->mem(code, NULL, size);
  
  if (block) { memset(block, 0x00, size); }
  
  return block;

}

static void freemem(ctx_t ctx, void * block) {

  fb2_codeCtx_t code = ctx2code(ctx);

  if (block) { code->mem(code, block, 0); }
  
}

static void genCode(t2c_ctx_t ctx, t2c_type_t type) {       // Generate the code for 1 type.

  struct {
    t2c_TGSpec_t Spec;
    char *       line[256];
    char         buf[8192];
  } GB;
  
  t2c_tgspec_t spec = & GB.Spec;

  GB.Spec.Buf.cap        = NUM(GB.buf);
  GB.Spec.Buf.buf        = GB.buf;
  GB.Spec.Lines.cap      = NUM(GB.line);
  GB.Spec.Tab.type       =  3;
  GB.Spec.Tab.bits       = 48;
  GB.Spec.Tab.typedef_1  = 16;

  GB.Spec.Tab.member     = 22; // 27

  if (0 == strcmp(type->name, "tf_BuiltinOptions_union_t")) {
    GB.Spec.Tab.member     = 35;
  }

  GB.Spec.Tab.typedef_2  = 47;
  GB.Spec.Tab.comment    = 52;
  GB.Spec.Tab.ends       = 55;
  GB.Spec.evfmt          = "0x%04x";
  GB.Spec.useFwDec       =  0;
  GB.Spec.useTypedef     =  1;

  t2c_fmttype(ctx, type, & GB.Spec);

  if (spec->overflow) { printf("// OVERFLOWED\n"); }

  for (uint32_t n = 0; n < spec->Lines.num; n++) {
    printf("%s\n", spec->Line[n].start);
  }

  if (! t2c_isTypedef(type)) {
    printf("\n");
  }
  
}

static uint16_t offsetInSet(snset_t set, uint32_t e) {      // Return the byte offset of element e in the given set.

  intptr_t off = (uint8_t *) set->set[e] - set->addr4Set;   // First calculate the signed offset.

  assert(off < 0xffff);
  
  return (uint16_t) off;

}

static uint32_t findname(ctx_t ctx, const char * n2f) {     // Ensure the string is in the name table; add if not found.

  uint32_t i;
  char *   name;
  uint32_t sz;
  snset_t  set = & ctx->Names;

  for (i = 0; i < set->num; i++) {
    name = set->set[i];
    if (0 == strcmp(name, n2f)) {
      printf("// NAME found '%s' nti %u.\n", n2f, offsetInSet(set, i));
      break;
    }
  }
  if (i == set->num) {                                      // Not found; add.
    sz = strlen(n2f);
    name = set->obj(set, sz + 1, 1);                        // Allocate also for \0.
    memcpy(name, n2f, sz);
    printf("// NAME added '%s' nti %u.\n", n2f, offsetInSet(set, i));
  }

  return offsetInSet(set, i);

}

static type_t findtype(ctx_t ctx, const char * nm, uint16_t * tti) {

  snset_t  set = & ctx->Types;
  uint32_t nti = findname(ctx, nm);
  uint32_t i;
  type_t   type;

  assert(tti);
  
  for (i = 0; i < set->num; i++) {
    type = set->set[i];
    if (type->nti == nti) {
      *tti = i;
      printf("// TYPE '%s' found tti %u\n", nm, *tti);
      break;
    }
  }
  
  if (i == set->num) {                                      // Not found, add it.
    type = set->obj(set, sizeof(Type_t), alignof(Type_t));
    *tti = set->num - 1;
    type->nti = nti;
    printf("// TYPE '%s' added tti %u\n", nm, *tti);
  }
  
  return type;

}

static struct {
  uint8_t    isCompound;
  uint8_t    isIndirect;          // The type is represented by a handle (indirection).
  uint8_t    props4ttt;           // Properties to set on the table type.
  uint8_t    pad[5];
  t2c_Prop_t t2c_type;            // Corresponding t2c type.
} fb2tc[32] = {                   // We have 4 bits for the fb2ti value and a vector bit.
  { 0, 0, .t2c_type = 0,          .props4ttt = fb2_NONE   }, // 0
  { 0, 0, .t2c_type = t2c_Prim,   .props4ttt = fb2_PRIM   }, // 1
  { 1, 1, .t2c_type = t2c_Struct, .props4ttt = fb2_TABLE  }, // 2 table
  { 1, 1, .t2c_type = t2c_Union,  .props4ttt = fb2_UNION  }, // 3 union
  { 1, 0, .t2c_type = t2c_Enum,   .props4ttt = fb2_ENUM   }, // 4 enum
  { 1, 0, .t2c_type = t2c_Struct, .props4ttt = fb2_STRUCT }, // 5 struct
};

static void clearmould(t2c_type_t moat, uint32_t num) {

  t2c_member_t m;
  uint32_t     i;
  
  for (i = 0, m = moat->Members; i < num; i++, m++) {
    memset(m, 0x00, sizeof(t2c_Member_t));
  }

  moat->num = 0;
  moat->boetype = NULL;
  moat->prop = 0;

}

static const char * dummy(fb2_codeCtx_t ctx, const char * name) {   // Dummy alias function.
  return name;
}

static char * mktypename(fb2_CodeCtx_t * ctx, char b[], const char * base, uint32_t fb2ti) {

  char copy[128];

  strcpy(copy, ctx->alias(ctx, base));

  fb2ti = (fb2ti & fb2e_Mask);

  assert(fb2ti > fb2e_Prim);                                // Should never be called on a primitive.

  const char * vta = (fb2ti >= fb2e_Vector) ? "_Vec" : "";
  const char * uta = (fb2ti == fb2e_Union)  ? ctx->a4union : "";

  sprintf(b, "%s%s%s%s%s", ctx->prefix, copy, vta, uta, ctx->suffix);

  printf("// TYPENAME [%s] -> [%s] %s\n", copy, b, fb2ti >= fb2e_Vector ? "VECTOR" : ""); // Use copy as we can be called with b==base.

  return b;

}

static const char * mkrefname(fb2_CodeCtx_t * ctx, char b[], const char * base, uint32_t fb2ti) {

  const char * vta = (fb2ti >= fb2e_Vector) ? "_vec" : "";

  char         copy[128];
  uint32_t     sz;
  const char * prepend = ctx->pref4refs;
  uint32_t     done = 0;

  strcpy(copy, ctx->alias(ctx, base));

  fb2ti = (fb2ti & fb2e_Mask);

  if (! prepend) {
    if (isupper(copy[0])) {                                 // OK we can change the initial case to lower case.
      copy[0] = tolower(copy[0]);
      done = 0;
    }
    else {
      prepend = "ref_";                                     // Backup solution.
    }
  }

  if (! done) {
    sz = strlen(prepend);
    memmove(& copy[sz], & copy[0], strlen(copy) + 1);       // Make room for the insert, also move the \0.
    memcpy(& copy[0], prepend, sz);                         // Insert.
//    printf("NEWREFSCHEME [%s] -> [%s]\n", base, copy);
  }

  sprintf(b, "%s%s%s%s", ctx->prefix, copy, vta, ctx->suffix);

//  printf(" REFNAME [%s] -> [%s] %s\n", base, b, b == base ? "DUPLICATE" : "");

  return base;

}

typedef struct S4Type_t {         // Search for a type.
  t2c_type_t    search;
  t2c_type_t    found;
} S4Type_t;

static void s4type(t2c_ctx_t ctx, t2c_type_t type, void * arg) {

  S4Type_t * s4type = arg;

  if (0 == t2c_typecmp(s4type->search, type)) {
    s4type->found = type;
  }

}

// Try to search for the vector and if not found, create it. The 'base' is the FB2 type name of the
// type to create a vector for, NOT the equivalent T2C type that should already exist.

static t2c_type_t findvector(fb2_CodeCtx_t * app, const char * obase, uint32_t fb2ti) {

  struct {
    t2c_Type_t   Type;
    t2c_Member_t Member[2];
  } Vector;

  t2c_ctx_t      ctx = & app->t2cCtx;
  S4Type_t       S4Type;
  t2c_type_t     basetype;
  char           base[128];
  char           buf[128];
  char *         tex;

  memset(& Vector, 0x00, sizeof(Vector));

  fb2ti = (fb2ti & fb2e_Mask);
  
  assert(0 == (fb2ti & 0b1000));                            // Should never be called with the vector bit flag on.

  if (fb2ti == fb2e_Prim) {                                 // First create the base typename ...
    sprintf(base, "%s", obase);
  }
  else {
    mktypename(app, base, obase, fb2ti);
  }

  basetype = t2c_name2type(ctx, base);                      // ... and search for it.
  assert(basetype);                                         // Should be found.

  if (fb2ti == fb2e_Prim) {                                 // After searching for the basetype, prepare the base for name creation.
    tex = strstr(base, "_t"); if (tex) { tex[0] = 0; }      // Remove any _t extension if present in the base.
  }
  else {
    strcpy(base, obase);                                    // Just restore original basename.
  }

  mktypename(app, buf, base, fb2e_Vector + fb2ti);

//  printf("VECTOR TO BE CREATED FROM [%s] = [%s]\n", obase, buf);

  Vector.Type.name = buf;
  Vector.Type.prop = t2c_Struct;
  Vector.Type.num = 2;
  Vector.Member[0].name = "size";
  Vector.Member[0].type = & t2c_U32;
  Vector.Member[0].isConst = 1;
  Vector.Member[1].name = "elements";
  if (fb2tc[fb2ti].isIndirect) {                            // If the element type is represented by a handle, we need a reference to it as element.
    assert(basetype->ref2type);
    Vector.Member[1].type = basetype->ref2type;
  }
  else {
    Vector.Member[1].type = basetype;
  }
  Vector.Member[1].isVTail = 1;

  t2c_initype(ctx, & Vector.Type);                          // Initialize, don't clone yet.

  S4Type.search = & Vector.Type;
  S4Type.found = NULL;

  t2c_scan4type(ctx, s4type, & S4Type);

  printf("// Vector [%s] %sfound\n", buf, S4Type.found ? "" : "NOT ");

  if (! S4Type.found) {
    S4Type.found = t2c_clone4type(ctx, & Vector.Type);      // Not found so create it by cloning the template Vector.
    S4Type.found->marks[vector_mark] = 1;
    mkrefname(app, buf, base, fb2e_Vector + fb2ti);
    t2c_tdref4type(ctx, S4Type.found, buf);                 // Also create the typedef reference for it.
  }

  return S4Type.found;

}

typedef struct EnumRename_t {
  t2c_type_t   etype;             // The enumtype.
  t2c_member_t m2c;               // Member to check for a clash.
  const char * prefix;            // If a clash occurs, renamed *all* with this prefix ...
  uint32_t     count;             // ... and set this one to the number of replacements done.
  uint8_t      pad[4];
} EnumRename_t;

static void check4clash(t2c_ctx_t ctx, t2c_member_t m, void * arg) {

  EnumRename_t * ren = arg;
  t2c_member_t   m2c = ren->m2c;
  char           buf[128];

  assert(ren->prefix);
  
  if (m2c == m) { return; }                                 // No need to check ourselves.
  
  if (0 == strcmp(m2c->name, m->name)) {
    m2c = & ren->etype->Members[0];
    for (uint32_t i = 0; i < ren->etype->num; i++, m2c++) {
      sprintf(buf, "%s%s", ren->prefix, m2c->name);
      t2c_renam(ctx, m2c, buf);                             // Do the rename.
      ren->count++;
    }
  }

}

static void resolve(t2c_ctx_t t2c_ctx, t2c_member_t m, void * arg) {

  ctx_t           ctx = arg;
  fb2_codeCtx_t   app = ctx2code(ctx);
  fb2_member_t    fb2m;
  char            buf[128];
  const char *    a4union;

  if (m->type == ctx->unresolved) {
    fb2m = m->Cargo.refs[0];                                // The FB member was attached to the cargo of the T2C member.
    a4union = (fb2e_Union == fb2m->type->fb2ti) ? app->a4union : "";
    printf("// Resolve %stype [%s%s_Vec%s]\n", fb2m->isArray ? "vector ": "", app->prefix, fb2m->type->name, app->suffix);
    sprintf(buf, "%s%s%s%s", app->prefix, app->alias(app, fb2m->type->name), a4union, app->suffix);
    m->type = t2c_name2type(t2c_ctx, buf);

if (! m->type) { printf("%s %u [%s] NOT FOUND\n", __FUNCTION__, __LINE__, buf); fflush(stdout); } 

    assert(m->type); // for now otherwise it means a missing type.
    if (fb2m->isArray) {
      printf("// VECTOR [%s] [%s]\n", m->type->name, fb2m->type->name);
      m->type = findvector(app, fb2m->type->name, fb2m->type->fb2ti);
      m->numind++;
    }
    else if (fb2tc[fb2m->type->fb2ti].isIndirect) {         // If an indirect type ...
      m->numind++;                                          // ... we make it a reference.
    }
  }

}

static void removetype(t2c_ctx_t ctx, t2c_type_t type) {

  struct {
    t2c_XRef_t    XRef;
    t2c_member_t  members[4];
  } Delete;

  memset(& Delete, 0x00, sizeof(Delete));
  Delete.XRef.cap = NUM(Delete.members);
  Delete.XRef.type = type;
  t2c_remove4type(ctx, & Delete.XRef);

  fflush(stdout);

  assert(0 == Delete.XRef.num);

}

static const uint32_t ct4cookie = 0x03041965;

typedef struct ct4Comp_t {        // Information to generate a T2C type from an FB2 type.
  fb2_type_t      fb2_type;       // The type to generate the t2c type for.
  fb2_type_t    * canon;          // The canonical FB2 types; pointer to Ctx.canon array.
  ctx_t           ctx;            // The internal context.

  t2c_type_t      t2c_type;       // The generated type.
  t2c_type_t      mould;          // The mould to be used for building up the type.
  uint32_t        maxmem;         // The number of member slots in the mould.
  uint32_t        cookie;
  t2c_type_t      unresolved;     // Placeholder; will be resolved later.
  t2c_type_t      string;         // The string type.
} ct4Comp_t;

/*
static const char * fmts[] = {
  "none",          // 0
  "%u",            // 1
  "%d",            // 2
  "%g",            // 3
  "0x%016"PRIx64,  // 4
  "0x%016"PRIx64,  // 5
};
*/
static const uint8_t type2fmti[] = {
   0, // ct_none      =  0,
   2, // ct_int8      =  1,
   1, // ct_uint8     =  2,
   2, // ct_int16     =  3,
   1, // ct_uint16    =  4,
   2, // ct_int32     =  5,
   1, // ct_uint32    =  6,
   4, // ct_int64     =  7,
   4, // ct_uint64    =  8,
   3, // ct_float     =  9,
   3, // ct_double    = 10,
   0, // ct_string    = 11,
   2, // ct_neg_int8  = 12,
   2, // ct_neg_int16 = 13,
   2, // ct_neg_int32 = 14,
   4, // ct_neg_int64 = 15,
};

static const uint8_t type2neg[] = {
  ct_none,      // ct_none      =  0,
  ct_neg_int8,  // ct_int8      =  1,
  ct_neg_int8,  // ct_uint8     =  2,
  ct_neg_int16, // ct_int16     =  3,
  ct_neg_int16, // ct_uint16    =  4,
  ct_neg_int32, // ct_int32     =  5,
  ct_neg_int32, // ct_uint32    =  6,
  ct_neg_int64, // ct_int64     =  7,
  ct_neg_int64, // ct_uint64    =  8,
  ct_float,     // ct_float     =  9,
  ct_double,    // ct_double    = 10,
  ct_string,    // ct_string    = 11,
  ct_neg_int8,  // ct_neg_int8  = 12,
  ct_neg_int16, // ct_neg_int16 = 13,
  ct_neg_int32, // ct_neg_int32 = 14,
  ct_neg_int64, // ct_neg_int64 = 15,
};
  
static void setPrimType(fb2_member_t fb2m, t2c_member_t t2cm, fb2_type_t cmtype) {

  assert(fb2m->Default.type);
  if (fb2m->Default.minus) {                                // The negative flag was set in token2Value().
  // isn't this already done in fb2-schema.c?
    fb2m->Default.type = type2neg[cmtype->canontype];       // Change to appropriate negative type.
  }

}

static void enum2prim(t2c_ctx_t t2c_ctx, t2c_member_t m, void * arg) {

  ct4Comp_t *  ct4c = arg;
  fb2_member_t fb2m = m->Cargo.refs[0];
  fb2_type_t   mtype;
  t2c_type_t   etype;

  assert(ct4cookie == ct4c->cookie);

  if (fb2m && fb2e_Enum == fb2m->type->fb2ti) {
    mtype = fb2m->type;
    printf("// Replace member [%s] from [%s] ", fb2m->name, mtype->name);
    assert(mtype->type4enum->canontype);                    // Must have a canonical type.
    mtype = ct4c->canon[mtype->type4enum->canontype];       // Switch to this canonical type.
    printf(" [%s]\n", mtype->name);
    etype = t2c_name2type(t2c_ctx, mtype->name);
    assert(etype);                                          // Must be found
    m->type = etype;
    setPrimType(fb2m, m, mtype);
  }

}

static void gen1(fb2_CodeCtx_t * ctx, ct4Comp_t * ct4c) {   // Generate a T2C compound type from a FB2 type.

  t2c_ctx_t       t2cCtx = & ctx->t2cCtx;
  uint32_t        i;
  t2c_member_t    t2cm;
  fb2_member_t    fb2m;
  char            buf[128];
  fb2_type_t      mtype;
  t2c_type_t      mould = ct4c->mould;
  fb2_type_t      src = ct4c->fb2_type;                     // A shorthand for the src FB2 type to generate from.
  uint32_t        isEnum = (fb2e_Enum == src->fb2ti);
  uint32_t        isUnion = (fb2e_Union == src->fb2ti);

  assert(fb2tc[src->fb2ti].isCompound);
  printf("// Compound Type [%s]\n", src->name);
  mould->prop = fb2tc[src->fb2ti].t2c_type;                 // Translate FB2 type to T2C type.
  mould->num = src->nummem;
  t2cm = mould->Members;

  if (isUnion) {                                            // For a union type, manually add a type member.
    t2cm->name = "type";
    t2cm->type = & t2c_U32;
    mould->num++; t2cm++;                                   // One extra member.
    assert(mould->num <= ct4c->maxmem);
  }

  mktypename(ctx, mould->name, src->name, src->fb2ti);

  for (i = 0; i < src->nummem; i++, t2cm++) {               // Do all members.
    fb2m = src->members[i];                                 // Create a shorthand for the rest of this loop.
    t2cm->Cargo.refs[0] = fb2m;                             // Attach the fb2 member to the t2cm member in slot 0.
    t2cm->name = ctx->alias(ctx, fb2m->name);               // Maybe aliased.
    mtype = fb2m->type;
    if (fb2m->isString) {
      t2cm->type = ct4c->string;
      t2cm->numind = 1;                                     // A string is always a reference to the string type.
      fb2m->tti = ct4c->ctx->string_tti;                    // Member will refer to the string type in the types table.
    }
    else if (fb2e_Prim == mtype->fb2ti) {                   // A primitive type.
      assert(mtype->canontype);                             // Must have a canonical type.
      mtype = ct4c->canon[mtype->canontype];                // Switch to canonical type.
      fb2m->tti = mtype->canontype;                         // For canonicals, the canontype it the type table index.
      t2cm->type = t2c_name2type(t2cCtx, mtype->name);
      assert(t2cm->type);//must be found
//--
//void settype(fb2_member_t fb2m, t2c_member_t t2cm, fb2_type_t cmtype) {
      setPrimType(fb2m, t2cm, mtype);
//      fb2m->Default.width = t2cm->type->size;               // All other properties have been set in setMemberType().
//      fb2m->Default.type = mtype->canontype;
//      if (fb2m->Default.negative) {                         // The negative flag was set in token2Value().
//        fb2m->Default.type = type2neg[mtype->canontype];    // Change to appropriate negative type.
//      }
//--

      if (fb2m->isArray) {                                  // The member is a vector of primitives, so find or create it.
        printf("// VECTOR P [%s] [%s]\n", t2cm->type->name, mtype->name);
        t2cm->type = findvector(ctx, mtype->name, fb2e_Prim);
        t2cm->numind = 1;                                   // A vector is always a reference.
      }
      if (isEnum) {                                         // An enum *member* declaration, we should set the enumval; see note AA below.
        printf("// ENUM [%s] type %s val %u.\n", fb2m->name, mtype->name, fb2m->Default.u32);
        t2cm->enumval = fb2m->Default.u32;
        assert(! fb2m->isArray);                            // Can never be an array member.
      }
    }
    else if (fb2e_Enum == mtype->fb2ti) {                   // The member *type* is an enumeration; see note AA above.
      printf("// ENUM [%s] type %s\n", fb2m->name, mtype->name);
      assert(mtype->type4enum);
      assert(! fb2m->isArray);                              // Can never be an array member.
      t2cm->type = ct4c->unresolved;                        // Do NOT convert to primitive yet; done in enum2prim(); we need it marked4use first.
    }
    else {                                                  // Member type is defined in the schema.
      t2cm->type = ct4c->unresolved;                        // Give it a temporary type; we'll attach the proper one later.
    }
  }

  if (isEnum) {
    assert(mould->num);                                     // Must have at least 1 member.
    assert(mould->Members[0].type);
    mould->boetype = mould->Members[0].type;                // We'll use the type of the first member.
    printf("// IS ENUM base type %s\n", mould->Members[0].type->name);
  }

  ct4c->t2c_type = t2c_clone4type(t2cCtx, mould);           // Create the type by cloning the mould.
  ct4c->t2c_type->Cargo.refs[0] = src;                      // Attach the corresponding FB2 type to it.

  mkrefname(ctx, buf, src->name, src->fb2ti);
  t2c_tdref4type(t2cCtx, ct4c->t2c_type, buf);              // Also make the typedef; get's attached to ref2type.

  if (strlen(t2cCtx->msg)) {
    printf("Error: [%u] %s\n", t2cCtx->error, t2cCtx->msg);
  }
  fflush(stdout);      

  assert(! t2cCtx->error); // For now

}

static void genTypes(ctx_t ctx) {

  fb2_codeCtx_t   fb2Code = ctx2code(ctx);
  fb2_schemaCtx_t fb2Ctx = ctx2schema(ctx);
  t2c_ctx_t       t2cCtx = ctx2tc(ctx);
  fb2_BSchema_t * schema = fb2Ctx->schema;
  fb2_Any_t *     e;
  uint32_t        i;
  uint32_t        x;
  type_t          ttt; // Type Table Type.
  t2c_type_t      mould;                                    // The mother of all types.
  t2c_member_t    t2cm;
  uint32_t        msize = sizeof(t2c_Type_t);               // And how large it is.
  char            buf[128];
  t2c_type_t      t2ctype;
  const char *    rootname = NULL;
  ct4Comp_t       CT4;

  if (! fb2Code->prefix) { fb2Code->prefix = ""; }          // Prefix can not be NULL.

  if (! fb2Code->alias) { fb2Code->alias = dummy; }         // Can not be NULL; provide pass through.

  assert(NUM(ctx->canon) >= schema->Num.canonical);         // See that our canonical table is large enough.

  ctx->phase = phase_t2ctypes_generated;                    // Means that generate types has been done.

  memset(& CT4, 0x00, sizeof(CT4));

  CT4.ctx = ctx;
  CT4.canon = ctx->canon;
  CT4.cookie = ct4cookie;
  CT4.maxmem = schema->Num.maxmem + 1;                      // Maximum number of members; +1 for e.g. union type member etc.
  
  msize += sizeof(t2c_Member_t) * CT4.maxmem;

  mould = getmem(ctx, msize);
  assert(mould);
  mould->name = buf;

  CT4.mould = mould;

  mktypename(fb2Code, buf, "String", fb2e_Table);           // Create the string type.
  mould->prop = t2c_Struct;
  mould->num = 2;
  mould->Members[0].name = "size";
  mould->Members[0].type = & t2c_U32;
  mould->Members[0].isConst = 1;
  mould->Members[1].name = "chars";
  mould->Members[1].type = & t2c_Char;
  mould->Members[1].isVTail = 1;
  CT4.string = t2c_clone4type(t2cCtx, mould);
  assert(! t2cCtx->error);
  clearmould(mould, schema->Num.maxmem);                    // Clear the template.

  mkrefname(fb2Code, buf, "String", fb2e_Table);            // Create a typedef for the string.
  t2c_tdref4type(t2cCtx, CT4.string, buf);

  strcpy(buf, "<unresolved>");                              // Type to resolve later.
  mould->prop = t2c_Struct;
  mould->num = 0;
  CT4.unresolved = t2c_clone4type(t2cCtx, mould);
  assert(! t2cCtx->error);
  clearmould(mould, schema->Num.maxmem);                    // Clear the template.


  e = & schema->Elements[0];
  for (i = 0, x = 0; x < schema->Num.canonical; i++, e = fb2_go2next(e)) {
    if (fb2e_Prim == e->Type.fb2ti) {
      t2ctype = t2c_prims[x];                               // Get the corresponding t2c type.
      assert(!x || ! strcmp(t2ctype->name, e->Type.name));  // They must be in the same order, except for the first which is char.
      ctx->canon[x++] = & e->Type;                          // Collect the first schema->Num.canonical primitives.
      ttt = findtype(ctx, e->Type.name, & e->Type.tti);
      ttt->props = fb2_PRIM;
      ttt->fi = type2fmti[e->Type.ct_type];                 // Set the proper formatter index.
      if (t2ctype->prop & t2c_Signed) {
        ttt->props |= fb2_SIGNED;
      }
      ttt->size = t2ctype->size;
      ttt->align = t2ctype->align;
    }
  }

  ttt = findtype(ctx, "string", & ctx->string_tti);
  ttt->props = fb2_PRIM;

  e = & schema->Elements[0];                                // Go over all schema elements.
  for (i = 0; i < schema->Num.elements; i++, e = fb2_go2next(e)) {
    if (fb2e_KeyVal == e->KeyVal.fb2ti) {
      if (! strcmp("I:root-type", e->KeyVal.key)) {
        rootname = e->KeyVal.Value.tag->string;             // Save the root type name for later.
      }
    }

    if (fb2tc[e->Type.fb2ti].isCompound) {                  // Convert all non primitive FB2 types into T2C types.
      CT4.fb2_type = & e->Type;
      gen1(fb2Code, & CT4);
      clearmould(mould, schema->Num.maxmem);                // Clear the template.
    }
  }

  ctx->unresolved = CT4.unresolved;
  t2c_scan4mem(t2cCtx, resolve, ctx);                       // Try to link all 'unresolved' types to the proper type.

  removetype(t2cCtx, CT4.unresolved);                       // This one really must be deleted. It would generate invalid code.

  EnumRename_t Ren;

  for (i = 0; i < t2cCtx->num; i++) {                       // Check for enum value clashes and fix them.
    if (t2c_isEnum(t2cCtx->types[i])) {
      t2ctype = t2cCtx->types[i];
      memset(& Ren, 0x00, sizeof(Ren));
      Ren.etype = t2ctype;
      Ren.prefix = "u_"; // todo find a better prefix
      for (x = 0; x < t2ctype->num; x++) {
        Ren.m2c = & t2ctype->Members[x];
        t2c_scan4mem(t2cCtx, check4clash, & Ren);
        if (Ren.count) {
          printf(" Renamed %u enumeration members.\n", Ren.count);
          break;
        }
      }
    }
  }

  printf("// Have %u types.\n", t2cCtx->num);

  uint32_t tdr = t2c_reptypedefs(t2cCtx);                   // Resolve all typedefs.

  printf("// Replaced %u typedefs.\n", tdr);

  if (optimize) { // -- Cleanup/customize

  sprintf(buf, "%s%s_t", fb2Code->prefix, "Operator");
  t2ctype = t2c_name2type(t2cCtx, buf);
  assert(t2ctype); // for now
  for (i = 0, t2cm = t2ctype->Members; i < t2ctype->num; i++, t2cm++) {
    if (0 == strcmp(t2cm->name, "builtin_options")) {
      printf("RETYPED [%s]\n", t2cm->name);
      t2cm->type = & t2c_VoidRef;
      break;
    }
  }

  sprintf(buf, "%s%s_t", fb2Code->prefix, "OperatorCode");
  t2ctype = t2c_name2type(t2cCtx, buf);
  assert(t2ctype); // for now
  for (i = 0, t2cm = t2ctype->Members; i < t2ctype->num; i++, t2cm++) {
    if (0 == strcmp(t2cm->name, "builtin_code")) {
      printf("RETYPED [%s]\n", t2cm->name);
      t2cm->type = & t2c_U32;
      break;
    }
  }

  } // -- End of Cleanup/customize

  assert(rootname);                                         // Mark from root.
  sprintf(buf, "%s%s_t", fb2Code->prefix, rootname);
  t2ctype = t2c_name2type(t2cCtx, buf);
  assert(t2ctype); // for now
  t2c_mark4use(t2cCtx, t2ctype);                            // Mark all from the root.
  ctx->root = t2ctype;

  t2c_scan4mem(t2cCtx, enum2prim, & CT4);                   // Only *after* mark4use, move enum members types to primitive.

  t2c_prep4gen(t2cCtx);

  for (i = 0; i < t2cCtx->num; i++) {                       // First do the typedefs.
    if (!t2cCtx->types[i]->mark4use) continue;
    t2c_type_t t = t2cCtx->types[i];
    printf("[%s] w %u\n", t->name, t->weight);
  }

  printf("#endif // debug\n#include <fb2-types.h>\n\n");

  for (i = 0; i < t2cCtx->num; i++) {                       // First do the typedefs.
    if (!t2cCtx->types[i]->mark4use) continue;
    if (t2c_isTypedef(t2cCtx->types[i])) {
      genCode(t2cCtx, t2cCtx->types[i]);
    }
  }

  printf("\n");

  for (i = 0; i < t2cCtx->num; i++) {                       // Then the types themselves.
    if (!t2cCtx->types[i]->mark4use) continue;
    if (! t2c_isTypedef(t2cCtx->types[i])) {
      genCode(t2cCtx, t2cCtx->types[i]);
    }
  }

  freemem(ctx, mould);

}

typedef struct Const_t {            // Default or enumeration value.
  uint8_t type;
  uint8_t data[0];
} Const_t;

typedef struct Const_t * const_t;

void dumpval(const char * msg, const fb2_Value_t val) {

  uint8_t isfloat = (val.type == ct_float || val.type == ct_double);
  uint8_t negative = val.minus;

  printf("// %s  %s%"PRId64" %ssigned %s type %s\n", msg, negative ? "-" : "", val.i64, val.signd ? "" : "un", isfloat ? "float" : "decimal", ct2a[val.type]);

}

static void val2const(fb2_Value_t val, const_t dev) {       // Convert a value into something that is saved in the constant table.

  int32_t i;
  uint8_t isfloat = (val.type == ct_float || val.type == ct_double);
  int8_t  mul = (! isfloat && val.minus) ? -1 : +1;

  val.i64 *= mul;                                           // Ensure any non floating point number is positive.

  assert(val.u64);                                          // Zero values should not end up here.

  if (val.type == ct_float) {
    val.f32 = (float) val.f64;                              // Convert the 64 bit IEEE754 to 32 bit IEEE754, if needed.
  }

  memcpy(dev->data, & val.u64, sizeof(val.u64));
  
  for (i = 7; i > 0 && ! isfloat; i--) {                    // We don't minimize floats or doubles.
    if (dev->data[i]) break;                                // Stop at the first non zero.
  }

  static const uint8_t break2type[8] = {
     ct_uint8,
     ct_uint16,
     ct_uint32,
     ct_uint32,
     ct_uint64,
     ct_uint64,
     ct_uint64,
     ct_uint64,
  };

  if (! isfloat) {
    dev->type = break2type[i];
    if (val.signd) { dev->type -= 1; }                      // The signed types are 1 less than the unsigned.
    if (val.minus) { dev->type = type2neg[dev->type]; }     // When negative, turn into the negative countertype.
  }
  else {
    dev->type = val.type;
  }

  if (ct_string == val.type) {                              // Overrides all other stuff.
    dev->type = ct_string;
  }

}

static fb2_Value_t const2val(void * any) {                  // Decode a Const entry into a value.

  fb2_Value_t     CV = { .u64 = 0LL };
  const uint8_t * cc = any;
  const int8_t    mul = (cc[0] >= ct_neg_int8) ? -1 : +1;

  assert(cc[0] >= ct_none && cc[0] <= ct_neg_int64);
  assert(ct_string != cc[0]);

  switch (cc[0]) {
    case ct_double:
    case ct_uint64:
    case ct_int64:
    case ct_neg_int64:
      CV.u64 |= cc[8];
      CV.u64 <<= 8;
      CV.u64 |= cc[7];
      CV.u64 <<= 8;
      CV.u64 |= cc[6];
      CV.u64 <<= 8;
      CV.u64 |= cc[5];                                      // Fall through

    case ct_float:
    case ct_uint32:
    case ct_int32:
    case ct_neg_int32:
      CV.u64 <<= 8;
      CV.u64 |= cc[4];
      CV.u64 <<= 8;
      CV.u64 |= cc[3];                                      // Fall through.

    case ct_uint16:
    case ct_int16:
    case ct_neg_int16:
      CV.u64 <<= 8;
      CV.u64 |= cc[2];                                      // Fall through.

    case ct_none:
    case ct_uint8:
    case ct_int8:
    case ct_neg_int8:
      CV.u64 <<= 8; 
      CV.u64 |= cc[1];                                      // Fall through.
  }

  CV.i64 *= mul;                                            // Add the proper sign.
  CV.type = (cc[0]);

  return CV;

}

static uint32_t dist(uint8_t * start, uint8_t * cur) {      // Return distance in bytes between start and current.
  assert(cur >= start);
  return (uint32_t) (cur - start);
}

//#include <hexdump.h>

static uint32_t encodestr(snset_t set, fb2_tag_t tag) {

  uint8_t * start = set->addr4next;
  uint8_t * cur = start;
  uint32_t  numchars = tag->size;
  uint8_t * dst;
  uint32_t  size;

  printf("// tag [%s] size %u\n", tag->chars, tag->size);

  while (((intptr_t) cur & 0b11) || dist(start, cur) < 2) { // We need an aligned pointer for StrConst_t and room for the header.
    cur++;
  }

  printf("//      cur %p\n", cur);
  printf("//    start %p\n", start);
  printf("//     dist %u\n", dist(start, cur));
  printf("// numchars %u 0x%08x\n", numchars, numchars);

  size = dist(start, cur);                                  // The space for the tag, offset and potential padding bytes. 
  size += sizeof(uint32_t);                                 // The size field.
  size += numchars;                                         // The number of characters.

  dst = set->obj(set, size, 1);                             // Allocate the space.

  assert(dst == start);                                     // If not, our calculcations are off.

  dst[0] = ct_string;
  dst[1] = dist(start, cur);
  memcpy(cur, & numchars, sizeof(uint32_t));
  cur += sizeof(uint32_t);
  memcpy(cur, tag->chars, tag->size);

  printf("/*\n");
  printf("needs %u bytes.\n", size);
  printf("             0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");
//  hexdump8k(dst, size, "STRING");
  printf("*/\n");

  return set->num - 1;

}

static void mem2const(t2c_ctx_t t2cCtx, t2c_member_t m, void * arg) {

  fb2_member_t  fb2m = m->Cargo.refs[0];
  fb2_tag_t     tag;
  uint32_t      i;
  snset_t       set = & any2ctx(arg)->Const;
  const_t       dev;
  fb2_Value_t   V;
  uint32_t      found = 0;

  struct {
    Const_t     Const;
    uint8_t     data[8];
  } M;

  static const uint8_t t2sz[] = {                           // Translate a type to a size in the constant table, including the type byte.
    //                               | == string, todo
    0, 2, 2, 3, 3, 5, 5, 9, 9, 5, 9, 2, 2, 3, 5, 9
  };
  
  if (! t2c_mem2cont(m, NULL)->mark4use) { return; }        // If not marked for use, ditch it.

//if (fb2m) {
//  printf("CONSIDER [%s] default type %u value %u\n", fb2m->name, fb2m->Default.type, (uint32_t) fb2m->Default.u32);
//}

  if (fb2m && fb2m->Default.type && fb2m->Default.u64) {    // Not all fields have a *primitive* fb2 member attached to them (e.g. typedefs, vectors, ...)
    if (ct_string == fb2m->Default.type) {                  // There are never duplicate string tags; so no need to search for a duplicate and ...
      found = 1;                                            // ... we claim it was found and add it here. Below code handles numericals only.
      tag = fb2m->Default.ref;                              // The reference field contains a tag.
      i = encodestr(set, tag);
    }
    else {
//printf("MNAME [%s]\n", m->name);
//      dumpval("  SEARCH4", fb2m->Default); fflush(stdout);
      for (i = 0; i < set->num; i++) {
        dev = set->set[i];
        if (ct_string == dev->type) continue;               // No need to compare strings. here.
        V = const2val(dev);
//      dumpval("  COMPARE", V);
        if (V.u64 == fb2m->Default.u64 && V.type == fb2m->Default.type) {
//          dumpval("CONST FND", V);
//        printf("// FOUND 0x%"PRIx64" at %u\n", fb2m->Default.u64, i);
          found = 1; break;                                 // Must also break, since i is used for offset calulcation.
        }
      }
    }
    if (! found) {                                          // Not found; encode and add.
      memset(& M, 0x00, sizeof(M));
      val2const(fb2m->Default, & M.Const);
      dev = set->obj(set, t2sz[M.Const.type], 1);
      //dumpval("    ADDED", fb2m->Default);
      //printf("// Added 0x%"PRIx64" at %u type %u offset %u\n", fb2m->Default.u64, set->num - 1, M.Const.type, offsetInSet(set, i));
      memcpy(dev, & M, t2sz[M.Const.type]);
    }
    fb2m->ctoff = offsetInSet(set, i);
  }
  
}

static void genConstTable(ctx_t ctx) {

  t2c_ctx_t t2cCtx = ctx2tc(ctx);
  snset_t   set = & ctx->Const;
  uint8_t * preset = set->obj(set, 2, 1);                   // Entry 0 becomes the 0/null entry.

  assert(ctx->phase == phase_t2ctypes_generated);

  preset[0] = ct_none;                                      // Encode it as a none or null type.

  assert(const2val(preset).u32 == 0);

  t2c_scan4mem(t2cCtx, mem2const, ctx);                     // Go over all members to extract the constants.

  ctx->phase = phase_consts_generated;
  
}

// --- Name table

static void m2types(t2c_ctx_t t2cCtx, t2c_member_t m, void * arg) {

  ctx_t         ctx = any2ctx(arg);
  fb2_member_t  fb2m = m->Cargo.refs[0];
  fb2_type_t    fb2t;
  snset_t       typeset = & ctx->Types;
  char          buf[128];
  type_t        ttt;
  type_t        prim;
  uint16_t      vetid;
  uint16_t      tti;

  if (! t2c_mem2cont(m, NULL)->mark4use) { return; }        // If not marked for use, ditch it.

  fb2t = t2c_mem2cont(m, NULL)->Cargo.refs[0];
  
  if (fb2t) {                                               // Ensure the containing type is in the Types table.
    sprintf(buf, "%s", fb2t->name);
    ttt = findtype(ctx, buf, & tti);
    ttt->props = fb2tc[fb2t->fb2ti].props4ttt;
  }

  ttt = NULL;                                               // Clear to trap unsafe access.

  if (fb2m) {
    sprintf(buf, "%s", fb2m->type->name);                   // Find and ensure the base type first; can be an array type.
    ttt = findtype(ctx, buf, & fb2m->tti);
    if (fb2e_Prim == fb2m->type->fb2ti) {                   // If a primitive ...
      prim = typeset->set[fb2m->type->canontype];           // ... ensure it has the proper size, alignment and props.
      assert(prim->props && prim->size && prim->align);
      ttt->props = prim->props;
      ttt->size  = prim->size;
      ttt->align = prim->align;
      ttt->fi = prim->fi;
    }
    else {
      ttt->props = fb2tc[fb2m->type->fb2ti].props4ttt;
    }

    ttt = NULL;                                             // Clear to trap unsafe access.
    prim = NULL;

    if (fb2m->isArray) {                                    // Whatever the member type, a vector has the VECTOR property.
      vetid = fb2m->tti;                                    // Save the base type tid since it will be overwritten.
      sprintf(buf, "[%s]", fb2m->type->name);
      ttt = findtype(ctx, buf, & fb2m->tti);
      ttt->props = fb2_VECTOR;
      ttt->vetid = vetid;
    }
  }

}

static void genNameAndTypeTables(ctx_t ctx) {               // Generate both name and type table.

  t2c_ctx_t t2cCtx = ctx2tc(ctx);

  assert(ctx->phase == phase_consts_generated);

  t2c_scan4mem(t2cCtx, m2types, ctx);                       // Go over all members to create their types and names.
  
  ctx->phase = phase_names_types_generated;
  
}

static void t2compound(t2c_ctx_t t2cCtx, t2c_type_t t, void * arg) {

  fb2_type_t   fb2t = t->Cargo.refs[0];
  fb2_member_t fb2m;
  ctx_t        ctx = any2ctx(arg);
  snset_t      set = & ctx->Compounds;
  comp_t       comp;
  uint32_t     size;
  char         buf[128];
  uint32_t     i = 0;
  member_t     m;
  type_t       ttt;
  const char * fmt;

  if (! t->mark4use || ! fb2t) { return; }                  // If not marked for use or no FB2 type attached, ditch it.

  if (! fb2tc[fb2t->fb2ti].isCompound) { return; }          // Non compounds don't end up in this table.

  assert(! t->marks[compound_mark]);                        // Should not have been marked yet.

  size = sizeof(Comp_t) + t->num * sizeof(Member_t);
  comp = set->obj(set, size, alignof(Comp_t));
  comp->num = t->num;
  comp->svtid = fb2t->vtti;
  sprintf(buf, "%s", fb2t->name);
  ttt = findtype(any2ctx(arg), buf, & comp->tid);
  printf("// COMP added [%s] cti %u tid %u\n", fb2t->name, set->num, comp->tid);
  ttt->cti = set->num;                                      // Compound table entry 0 is NULL; so no need for -1.
  ttt = NULL;                                               // Clear to trap unsafe access.
  comp->props = fb2tc[fb2t->fb2ti].props4ttt;

  for (m = comp->Members; i < fb2t->nummem; i++, m++) {     // Now process the members; use fb2t->nummem and not t->num (unions have an extra member).
    fb2m = fb2t->members[i];
    m->nti = findname(ctx, fb2m->name);                     // Ensure the member name is in the nametable.
    m->ctoff = fb2m->ctoff;
    fmt = fb2m->isArray ? "[%s]" : "%s";                    // Find the member type in the types table.
    sprintf(buf, fmt, fb2m->type->name);
    findtype(any2ctx(arg), buf, & m->tid);
  }

  t->marks[compound_mark] = 1;                              // Markt the type as having a compound.

}

static void genCompoundTable(ctx_t ctx) {

  t2c_ctx_t t2cCtx = ctx2tc(ctx);

  assert(ctx->phase == phase_vtabs_generated);              // We need the fb2t->vtti info prepared.

  t2c_scan4type(t2cCtx, t2compound, ctx);                   // Go over all marked types.
  
  ctx->phase = phase_compounds_generated;
  
}

/*
typedef const struct VTab_t {     // Generic VTable.
  struct {
    uint16_t   vtab;              // Size of this vtable, in bytes.
    uint16_t   table;             // Size of the table pointing to this vtable, in bytes.
  } Size;
  int16_t      offs[0];           // Offsets where the member data can be found in the table.
} VTab_t;
*/
static void struc2vtab(t2c_ctx_t t2cCtx, t2c_type_t t, void * arg) {

  fb2_type_t   fb2t = t->Cargo.refs[0];
  uint32_t     size;
  vtab_t       vtab;
  ctx_t        ctx = any2ctx(arg);
  snset_t      set = & ctx->VTabs;
  uint32_t     i;

  if (fb2t && fb2e_Struct == fb2t->fb2ti) {
    size = sizeof(VTab_t) + sizeof(int16_t) * t->num;
    printf("// VTAB for [%s] sizes %u %u\n", fb2t->name, size, t->size);
    vtab = set->obj(set, size, alignof(VTab_t));
    vtab->Size.vtab = size;
    vtab->Size.table = t->size;
    for (i = 0; i < t->num; i++) {
      vtab->offs[i] = t->Members[i].offset;
    }
    assert(set->num - 1);                                   // There is a dummy already inserted in the table.
    fb2t->vtti = set->num - 1;
    t->marks[vtab_mark] = fb2t->vtti;
  }

}

static void genVTabs(ctx_t ctx) {

  t2c_ctx_t t2cCtx = ctx2tc(ctx);

  assert(ctx->phase == phase_names_types_generated);

  t2c_scan4type(t2cCtx, struc2vtab, ctx);                   // Go over all struct types.
  
  ctx->phase = phase_vtabs_generated;
  
}

static const char * props2char(uint8_t props) {

  uint8_t signflag = (props & fb2_SIGNED);

  props &= ~fb2_SIGNED;

  switch (props) {
    case fb2_VECTOR:    return "fb2_VECTOR";
    case fb2_TABLE:     return "fb2_TABLE ";
    case fb2_UNION:     return "fb2_UNION ";
    case fb2_ENUM:      return "fb2_ENUM  ";
    case fb2_STRUCT:    return "fb2_STRUCT";
    case fb2_PRIM: {
      if (signflag) return "fb2_SIGNED | fb2_PRIM";
      else          return "             fb2_PRIM";
    }
    default: assert(0);
  }

  return NULL;

}

static t2c_type_t vti2type(ctx_t ctx, uint32_t vti) {       // Search a type for the given vtable index.

  t2c_ctx_t  t2ctx = ctx2tc(ctx);
  uint32_t   i;
  t2c_type_t found = NULL;

  for (i = 0; i < t2ctx->num && ! found; i++) {
    if (t2ctx->types[i]->marks[vtab_mark] == vti) {
      found = t2ctx->types[i];
    }
  }

  assert(found);
  
  return found;

}

static void emitTables(ctx_t ctx) {

  snset_t      compset = & ctx->Compounds;
  uint32_t     i;
  uint32_t     x;
  comp_t       comp;
  member_t     m;
  const char * names = (const char *) ctx->Names.addr4Set;
  const char * endofnames = (const char *) ctx->Names.addr4next;
  snset_t      typeset = & ctx->Types;
  snset_t      constset = & ctx->Const;
  snset_t      vtabset = & ctx->VTabs;
  t2c_type_t   t2ctype;
  fb2_type_t   fb2type;
  type_t       type;
  vtab_t       vtab;
  uint8_t *    cc;
  uint32_t     namesz;
  uint32_t     size;
  uint32_t     maxname = 0;
  uint32_t     done;
  char         buf[128];
  char         spaces[128];
  char *       filler;
  const char * cur;
  const char * pre = ctx->fb2code->prefix;

  if (! pre) { pre = ""; }

  memset(spaces, ' ', sizeof(spaces));

  // VTable accessor and table.

  if (vtabset->num > 1) {
    for (i = 1; i < vtabset->num; i++) {                    // Start from entry 1, since 0 is dummy.
      vtab = vtabset->set[i];
      t2ctype = vti2type(ctx, i);
      assert(t2ctype->Cargo.refs[0]);                       // Must have an fb2 type associated with it.
      fb2type = t2ctype->Cargo.refs[0];
      printf("static const struct {\n");
      printf("  VTab_t    VTab;\n");
      printf("  int16_t   offs[%u];\n", t2ctype->num);
      printf("} %s%s_VTab = {\n", pre, fb2type->name);
      printf("  .VTab.Size = { %u, %u },\n", vtab->Size.vtab, vtab->Size.table);
      printf("  .offs = { ");
      for (x = 0; x < t2ctype->num; x++) {
        printf("%d%s ", vtab->offs[x], x + 1 == t2ctype->num ? "" : ",");
      }
      printf("},\n};\n\n");
    }

    printf("static VTab_t * svtabs[] = {\n");
    printf("  (VTab_t *) 0,\n");
    for (i = 1; i < vtabset->num; i++) {
      vtab = vtabset->set[i];
      t2ctype = vti2type(ctx, i);
      assert(t2ctype->Cargo.refs[0]);                       // Must have an fb2 type associated with it.
      fb2type = t2ctype->Cargo.refs[0];
      printf("  & %s%s_VTab.VTab,\n", pre, fb2type->name);
    }
    printf("};\n\n");
  }

  for (i = 0; i < compset->num; i++) {
    comp = compset->set[i];
    assert(comp->tid < typeset->num);
    type = typeset->set[comp->tid];
    //printf("// COMP emit %p\n", comp);
    printf("static struct {\n");
    printf("  Comp_t   Comp;\n");
    if (comp->num) {
      printf("  Member_t Members[%u];\n", comp->num);
    }
    printf("} %s%s = {\n", pre, & names[type->nti]);
    namesz = strlen(& names[type->nti]);
    maxname = (namesz > maxname) ? namesz : maxname;
    printf("  .Comp.tid   = %6u,\n", comp->tid);
    printf("  .Comp.num   = %6u,%s\n", comp->num, (! comp->num) ? " // Has no members." : "");
    printf("  .Comp.props = %s,\n", props2char(comp->props));
    printf("  .Comp.svtid = %6u,\n", comp->svtid);
    if (comp->num) {
      printf("  .Members = {\n");
      if (fb2_UNION == comp->props) {                       // A union has this extra member for none.
        printf("    { .nti =      0, .tid =      0,                 }, // none\n");
      }
      for (m = comp->Members, x = 0; x < comp->num; x++, m++) {
        printf("    { .nti = %6u, ", m->nti);
        printf(".tid = %6u, ", m->tid);
        if (m->ctoff) {
          printf(".ctoff = %6u ", m->ctoff);
        }
        else {
          printf("                ");
        }
        printf("}, // %s\n", & names[m->nti]);
        if (fb2_UNION == comp->props && x + 2 == comp->num) break;
      }
      printf("  },\n");
    }
    printf("};\n\n");
  }

  // Compound table accessor.

  maxname = (maxname < 10) ? 10 : maxname;
  maxname += 8 + strlen(pre);                               // Leave some room for the "& xxx.Comp, ' etc.
  assert(maxname < sizeof(spaces) - 1);
  spaces[maxname] = 0;                                      // Terminate the spaces string.
  printf("static Comp_t * Comps[] = {\n");
  printf("  (Comp_t *) 0, %s// 0 First always NULL.\n", & spaces[13]);
  for (i = 0; i < compset->num; i++) {
    comp = compset->set[i];
    type = typeset->set[comp->tid];
    sprintf(buf, "& %s%s.Comp,", pre, & names[type->nti]);
    filler = & spaces[strlen(buf)];
    printf("  %s %s// %u\n", buf, filler, i + 1);
  }

  printf("};\n\n");

  // Name table accessor.

  printf("static const char names[] = {\n  ");
  for (i = 0, cur = names; cur < endofnames; cur++, i++) {
    if (i && 0 == (i % 25)) printf("  //  %6u\n  ", i);
    if (*cur) { printf("'%c',", *cur); }
    else      { printf(" 0 ,");        }
  }
  printf("\n};\n\n");

  // Types table.

  printf("static Type_t Types[] = {\n");
  for (i = 0; i < typeset->num; i++) {
    type = typeset->set[i];
//    printf("// %s \n", & names[type->nti]); fflush(stdout);
    
    printf("  { .size = %u, .align = %u, .props = %-21s, ", type->size, type->align, props2char(type->props));
    printf(".nti = %6u, ", type->nti);
    if (type->cti) {
      printf(".cti = %3u, ", type->cti);
    }
    else {
      printf("            ");
    }
    if (type->vetid) {
      printf(".vetid = %3u, ", type->vetid);
    }
    else {
      printf("              ");
    }
    if (type->fi) {
      printf(".fi = %u, ", type->fi);
    }
    else {
      printf("         ");
    }
    printf("}, // %3u %s\n", i, & names[type->nti]);
  }
  printf("};\n\n");

  static const uint8_t t2sz[] = {                           // Translate a type to a size, including the type byte.
    2, 2, 2, 3, 3, 5, 5, 9, 9, 5, 9, 2, 2, 3, 5, 9
  };

  printf("static const uint8_t consts[] = {\n");
  for (i = 0; i < constset->num; i++) {
    cc = constset->set[i];
    printf("  %s ", ct2a[cc[0]]);
    if (ct_string == cc[0]) {                               // Encode a string; that's a bit tricky.
      printf("0x%02x, ", cc[1]);                            // Print the offset to the string vector.
      done = 1;                                             // Printed the offset byte already.
      assert(cc[1] >= 2);
      for (x = 0; x < cc[1] - 2u; x++) {                    // Print the padding bytes, if any.
        printf("0x00, "); done++;
      }
      memcpy(& size, & cc[0] + cc[1], sizeof(size));        // Get the size field.
      for (x = 0; x < 4 + size; x++) {                      // The 4 is for the uint32_t size field.
        if (0 == (done % 16)) {
          printf("\n                ");
          done = 0;
        }
        printf("0x%02x, ", cc[cc[1] + x]); done++;
      }
    }
    else {                                                  // Plain old data constant.
      for (x = 1; x < t2sz[cc[0]]; x++) {
        printf("0x%02x, ", cc[x]);
      }
    }
    printf(" // %4u\n", offsetInSet(constset, i));
  }
  printf("};\n");

}

static void * mem4set(snset_t set, void * mem, uint32_t sz) {

  fb2_codeCtx_t code = ctx2code(set->custom);

  return code->mem(code, mem, sz);

}

void fb2_generate(fb2_codeCtx_t ctx) {

  Ctx_t  Ctx;

  memset(& Ctx, 0x00, sizeof(Ctx));

  ctx->cookie = cookie2;                                    // Set up some cookies to check our context getters.
  ctx->schemaCtx->cookie = cookie3;
  Ctx.fb2code = ctx;
  Ctx.cookie1 = cookie1;

  assert(ctx2code(& Ctx));                                  // Check our cookies.
  assert(ctx2tc(& Ctx));
  assert(ctx2schema(& Ctx));

  snset_init(& Ctx.Const, mem4set);                         // Set with default and enumeration values.
  Ctx.Const.Grow.bytes =  128;
  Ctx.Const.Grow.slots =   32; 
  Ctx.Const.custom = & Ctx;

  snset_init(& Ctx.Names, mem4set);                         // Set with names.
  Ctx.Names.Grow.bytes =  128;
  Ctx.Names.Grow.slots =   32; 
  Ctx.Names.custom = & Ctx;

  snset_init(& Ctx.Types, mem4set);                         // Set with types.
  Ctx.Types.Grow.bytes =  128;
  Ctx.Types.Grow.slots =   32; 
  Ctx.Types.custom = & Ctx;

  snset_init(& Ctx.Compounds, mem4set);                     // Set with compounds.
  Ctx.Compounds.Grow.bytes =  128;
  Ctx.Compounds.Grow.slots =   32; 
  Ctx.Compounds.custom = & Ctx;

  snset_init(& Ctx.VTabs, mem4set);                         // Set with struct vtabs.
  Ctx.VTabs.Grow.bytes =  128;
  Ctx.VTabs.Grow.slots =   32; 
  Ctx.VTabs.custom = & Ctx;

  Ctx.VTabs.obj(& Ctx.VTabs, 4, 4);                         // Create empty first vtab slot.

  genTypes(& Ctx);

  genConstTable(& Ctx);

  genNameAndTypeTables(& Ctx);

  genVTabs(& Ctx);
  
  genCompoundTable(& Ctx);

  emitTables(& Ctx);

}
