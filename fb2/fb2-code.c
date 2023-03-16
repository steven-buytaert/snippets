// Copyright 2023 Steven Buytaert

#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>

#include <fb2-code.h>

typedef fb2_codeCtx_t ctx_t; // Localo shorthand

#define NUM(A) (sizeof(A) / sizeof(A[0]))

static const uint32_t vector_mark = 0;

static uint32_t optimize = 0;

static void * getmem(ctx_t ctx, uint32_t size) {

  void * block = ctx->mem(ctx, NULL, size);
  
  if (block) { memset(block, 0x00, size); }
  
  return block;

}

static void freemem(ctx_t ctx, void * block) {

  if (block) { ctx->mem(ctx, block, 0); }
  
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

static struct {
  uint8_t    isCompound;
  uint8_t    isIndirect;          // The type is represented by a handle (indirection).
  uint8_t    pad[6];
  t2c_Prop_t t2c_type;            // Corresponding t2c type.
} fb2tc[32] = {                   // We have 4 bits for the fb2ti value and a vector bit.
  { 0, 0, .t2c_type = 0          }, // 0
  { 0, 0, .t2c_type = t2c_Prim   }, // 1
  { 1, 1, .t2c_type = t2c_Struct }, // 2 table
  { 1, 1, .t2c_type = t2c_Union  }, // 3 union
  { 1, 0, .t2c_type = t2c_Enum   }, // 4 enum
  { 1, 0, .t2c_type = t2c_Struct }, // 5 struct
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

static const char * aname(const char * name) {              // Potentially replace a long name with a shorter alias.
  return name;//todo
}

static char * mktypename(fb2_CodeCtx_t * ctx, char b[], const char * base, uint32_t fb2ti) {

  char copy[128];

  strcpy(copy, aname(base));

  fb2ti = (fb2ti & fb2e_Mask);

  assert(fb2ti > fb2e_Prim);                                // Should never be called on a primitive.

  const char * vta = (fb2ti >= fb2e_Vector) ? "_Vec" : "";
  const char * uta = (fb2ti == fb2e_Union)  ? ctx->a4union : "";

  sprintf(b, "%s%s%s%s%s", ctx->prefix, copy, vta, uta, ctx->suffix);

  printf("TYPENAME [%s] -> [%s] %s\n", copy, b, fb2ti >= fb2e_Vector ? "VECTOR" : ""); // Use copy as we can be called with b==base.

  return b;

}

static const char * mkrefname(fb2_CodeCtx_t * ctx, char b[], const char * base, uint32_t fb2ti) {

  const char * vta = (fb2ti >= fb2e_Vector) ? "_vec" : "";

  char         copy[128];
  uint32_t     sz;
  const char * prepend = ctx->pref4refs;
  uint32_t     done = 0;

  strcpy(copy, aname(base));

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

  printf("Vector [%s] %sfound\n", buf, S4Type.found ? "" : "NOT ");

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

static void resolve(t2c_ctx_t ctx, t2c_member_t m, void * arg) {

  fb2_CodeCtx_t * app = arg;
  fb2_member_t    fb2m;
  char            buf[128];
  const char *    a4union;

  if (m->type == app->unresolved) {
    fb2m = m->Cargo.refs[0];                                // The FB member was attached to the cargo of the T2C member.
    a4union = fb2_isUnion(fb2m->type) ? app->a4union : "";
    printf("Resolve %stype [%s%s_Vec%s]\n", fb2m->isArray ? "vector ": "", app->prefix, fb2m->type->name, app->suffix);
    sprintf(buf, "%s%s%s%s", app->prefix, aname(fb2m->type->name), a4union, app->suffix);
    m->type = t2c_name2type(ctx, buf);

if (! m->type) { printf("%s %u [%s] NOT FOUND\n", __FUNCTION__, __LINE__, buf); fflush(stdout); } 

    assert(m->type); // for now otherwise it means a missing type.
    if (fb2m->isArray) {
      printf("VECTOR C [%s] [%s]\n", m->type->name, fb2m->type->name);
      m->type = findvector(app, fb2m->type->name, fb2m->type->fb2ti);
      m->numind++;
    }
    else if (fb2_isTable(fb2m->type) || fb2_isUnion(fb2m->type)) {
      m->numind++;
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
  assert(0 == Delete.XRef.num);

}

typedef struct ct4Comp_t {        // Information to generate a T2C type from an FB2 type.
  fb2_type_t      fb2_type;       // The type to generate the t2c type for.
  fb2_type_t      canon[11];      // The canonical FB2 types.

  t2c_type_t      t2c_type;       // The generated type.
  t2c_type_t      mould;          // The mould to be used for building up the type.
  uint32_t        maxmem;         // The number of member slots in the mould.
  uint8_t         pad[4];
  t2c_type_t      unresolved;     // Placeholder; will be resolved later.
  t2c_type_t      string;         // The string type.
} ct4Comp_t;

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
  printf("Compound Type [%s]\n", src->name);
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
    t2cm->name = aname(fb2m->name);                         // Maybe aliased.
    mtype = fb2m->type;
    if (fb2m->isString) {
      t2cm->type = ct4c->string;
      t2cm->numind = 1;                                     // A string is always a reference to the string type.
    }
    else if (fb2e_Prim == mtype->fb2ti) {                   // A primitive type.
      assert(mtype->canontype);                             // Must have a canonical type.
      mtype = ct4c->canon[mtype->canontype];                // Switch to canonical type.
      t2cm->type = t2c_name2type(t2cCtx, mtype->name);
      assert(t2cm->type);//must be found

      if (fb2m->isArray) {                                  // The member is a vector of primitives, so find or create it.
        printf("VECTOR P [%s] [%s]\n", t2cm->type->name, mtype->name);
        t2cm->type = findvector(ctx, mtype->name, fb2e_Prim);
        t2cm->numind = 1;                                   // A vector is always a reference.
      }
      if (isEnum) {                                         // An enum *member* declaration, we should set the enumval; see note AA below.
        printf("ENUM [%s] type %s setting value\n", fb2m->name, mtype->name);
        t2cm->enumval = fb2m->Default.i64;
        assert(! fb2m->isArray);                            // Can never be an array member.
      }
    }
    else if (fb2e_Enum == mtype->fb2ti) {                   // The member *type* is an enumeration; see note AA above.
      printf("ENUM [%s] type %s\n", fb2m->name, mtype->name);
      assert(mtype->type4enum);
      assert(! fb2m->isArray);                              // Can never be an array member.
      assert(mtype->type4enum->canontype);                  // Must have a canonical type.
      mtype = ct4c->canon[mtype->type4enum->canontype];     // Switch to canonical type.
      t2cm->type = t2c_name2type(t2cCtx, mtype->name);
      assert(t2cm->type);//must be found
    }
    else {                                                  // Member type is defined in the schema.
      t2cm->type = ct4c->unresolved;                        // Give it a temporary type; we'll attach the proper one later.
    }
  }

  if (isEnum) {
    assert(mould->num);                                     // Must have at least 1 member.
    assert(mould->Members[0].type);
    mould->boetype = mould->Members[0].type;                // We'll use the type of the first member.
    printf("IS ENUM base type %s\n", mould->Members[0].type->name);
  }

  ct4c->t2c_type = t2c_clone4type(t2cCtx, mould);           // Create the type by cloning the mould.
  mkrefname(ctx, buf, src->name, src->fb2ti);
  t2c_tdref4type(t2cCtx, ct4c->t2c_type, buf);              // Also make the typedef; get's attached to ref2type.

  if (strlen(t2cCtx->msg)) {
    printf("Error: [%u] %s\n", t2cCtx->error, t2cCtx->msg);
  }
  fflush(stdout);      

  assert(! t2cCtx->error); // For now

}

void xgenTypes(fb2_CodeCtx_t * ctx) {                        // Application context passed.

  fb2_schemaCtx_t fb2Ctx = ctx->fb2schemaCtx;
  t2c_ctx_t       t2cCtx = & ctx->t2cCtx;
  fb2_BSchema_t * schema = fb2Ctx->schema;
  fb2_Any_t *     e;
  uint32_t        i;
  uint32_t        x;
  t2c_type_t      mould;                                    // The mother of all types.
  t2c_member_t    t2cm;
  uint32_t        msize = sizeof(t2c_Type_t);               // And how large it is.
  char            buf[128];
  t2c_type_t      t2ctype;
  const char *    rootname = NULL;
  ct4Comp_t       CT4;

  if (! ctx->prefix) { ctx->prefix = ""; }                  // Prefix can not be NULL.

  assert(NUM(CT4.canon) >= schema->Num.canonical);          // See that our canonical table is large enough.

  memset(& CT4, 0x00, sizeof(CT4));

  CT4.maxmem = schema->Num.maxmem + 1;                      // Maximum number of members; +1 for e.g. union type member etc.
  
  msize += sizeof(t2c_Member_t) * CT4.maxmem;

  mould = getmem(ctx, msize);
  assert(mould);
  mould->name = buf;

  CT4.mould = mould;

  mktypename(ctx, buf, "String", fb2e_Table);               // Create the string type.
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

  mkrefname(ctx, buf, "String", fb2e_Table);                // Create a typedef for the string.
  t2c_tdref4type(t2cCtx, CT4.string, buf);

  strcpy(buf, "<unresolved>");                              // Type to resolve later.
  mould->prop = t2c_Struct;
  mould->num = 0;
  CT4.unresolved = t2c_clone4type(t2cCtx, mould);
  assert(! t2cCtx->error);
  clearmould(mould, schema->Num.maxmem);                    // Clear the template.

  e = & schema->Elements[0];
  for (i = 0, x = 0; x < schema->Num.canonical; i++, e = fb2_go2next(e)) {
    if (fb2_isPrimitive(e)) {
      CT4.canon[x++] = & e->Type;                           // Collect the first schema->Num.canonical primitives.
    }
  }

  e = & schema->Elements[0];                                // Go over all schema elements.
  for (i = 0; i < schema->Num.elements; i++, e = fb2_go2next(e)) {
    if (fb2e_KeyVal == e->KeyVal.fb2ti) {
      if (! strcmp("I:root-type", e->KeyVal.key)) {
        rootname = e->KeyVal.Value.tag->string;             // Save the root type name for later.
      }
    }

    if (fb2tc[fb2_ti(& e->Type)].isCompound) {              // Convert all non primitive FB2 types into T2C types.
      CT4.fb2_type = & e->Type;
      gen1(ctx, & CT4);
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

  printf("Have %u types.\n", t2cCtx->num);

  uint32_t tdr = t2c_reptypedefs(t2cCtx);                   // Resolve all typedefs.

  printf("Replaced %u typedefs.\n", tdr);

  if (optimize) { // -- Cleanup/customize

  sprintf(buf, "%s%s_t", ctx->prefix, "Operator");
  t2ctype = t2c_name2type(t2cCtx, buf);
  assert(t2ctype); // for now
  for (i = 0, t2cm = t2ctype->Members; i < t2ctype->num; i++, t2cm++) {
    if (0 == strcmp(t2cm->name, "builtin_options")) {
      printf("RETYPED [%s]\n", t2cm->name);
      t2cm->type = & t2c_VoidRef;
      break;
    }
  }

  sprintf(buf, "%s%s_t", ctx->prefix, "OperatorCode");
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
  sprintf(buf, "%s%s_t", ctx->prefix, rootname);
  t2ctype = t2c_name2type(t2cCtx, buf);
  assert(t2ctype); // for now
  t2c_mark4use(t2cCtx, t2ctype);                            // Mark all from the root.

  t2c_prep4gen(t2cCtx);

  for (i = 0; i < t2cCtx->num; i++) {                       // First do the typedefs.
    if (!t2cCtx->types[i]->mark4use) continue;
    t2c_type_t t = t2cCtx->types[i];
    printf("[%s] w %u\n", t->name, t->weight);
  }

  printf("#endif // debug\n#include <stdint.h>\n\n");

  for (i = 0; i < t2cCtx->num; i++) {                       // First do the typedefs.
    if (!t2cCtx->types[i]->mark4use) continue;
    if (t2c_isTypedef(t2cCtx->types[i])) {
      genCode(t2cCtx, t2cCtx->types[i]);
    }
  }

  printf("\n");

  for (i = 0; i < t2cCtx->num; i++) {                       // The the types themselves.
    if (!t2cCtx->types[i]->mark4use) continue;
    if (! t2c_isTypedef(t2cCtx->types[i])) {
      genCode(t2cCtx, t2cCtx->types[i]);
    }
  }

  freemem(ctx, mould);

}
