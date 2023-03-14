#include <stdio.h>
#include <stddef.h>
#include <stdarg.h>

#include <internal.h>

static const Type_t Builtin[] = {
  { .name = "none",     .canontype =  0 },

  { .name = "uint8",    .canontype =  1 },
  { .name = "int8",     .canontype =  2 },
  { .name = "uint16",   .canontype =  3 },
  { .name = "int16",    .canontype =  4 },
  { .name = "int32",    .canontype =  5 },
  { .name = "uint32",   .canontype =  6 },
  { .name = "int64",    .canontype =  7 },
  { .name = "uint64",   .canontype =  8 },
  { .name = "float",    .canontype =  9 },
  
  { .name = "ubyte",    .canontype =  1 },
  { .name = "byte",     .canontype =  2 },
  { .name = "ushort",   .canontype =  3 },
  { .name = "short",    .canontype =  4 },
  { .name = "uint",     .canontype =  5 },
  { .name = "int",      .canontype =  6 },
  { .name = "ulong",    .canontype =  7 },
  { .name = "long",     .canontype =  8 },
  { .name = "float32",  .canontype =  9 },

  { .name = "bool",     .canontype =  1 },
  { .name = "double",   .canontype =  0 },
  { .name = "float64",  .canontype =  0 },
  { .name = "string",   .canontype =  0 },
};

void error(ctx_t ctx, const char * fmt, ...) {

  va_list ap;

  if (! ctx->error) {                                       // Don't overwrite an existing error.
    va_start(ap, fmt);
    vsnprintf(ctx->msg, sizeof(ctx->msg), fmt, ap);
    va_end(ap);
    ctx->error = 1;
  }

  fprintf(stderr, "ERROR: %s\n", ctx->msg);

  exit(0);

}

static uint32_t roundup(uint32_t value, uint32_t pot) {     // Round up to a power of two.
  return (value + (pot - 1)) & ~(pot - 1);
}

static uint32_t tok2sz(token_t tok) {

  uint32_t size = sizeof(Token_t);

  if (tok->size) { size += tok->size; }                     // If there's text attached.

  return roundup(size, alignof(Token_t));
  
}

token_t tok2next(token_t tok) {                             // To go from one token to the next.

  union {
    token_t   tok;
    uint8_t * u08;
  } A = { tok };

  A.u08 += tok2sz(tok);

  return A.tok;

}

uint32_t find(ctx_t ctx, token_t token, uint8_t tom, tup_t tup) { // See if the token is already associated with a type. Return non zero if found.

  snset_t  set = & ctx->Meta;
  uint32_t i;
  meta_t   meta;
  uint32_t found = 0;

  memset(tup, 0x00, sizeof(Tup_t));

  if (! token) { return 0; }                                // Can pass NULL as token.

  for (i = 0; ! found && i < set->num; i++) {
    meta = set->set[i];
    if (tom == meta->tom) {
//printf("[%s] <> [%s]\n", token->text, meta->id->text);
      if (! strcmp(token->text, meta->id->text)) {
        tup->meta = meta;
        tup->any = ctx->TMA.set[i];
        found = 1;
      }
    }
  }

  return found;

}

void idx2tup(ctx_t ctx, uint32_t idx, tup_t tup) {          // Fill a tuple with the type and meta information of the given index.

  assert(idx < ctx->Meta.num && idx != none);

  tup->any = ctx->TMA.set[idx];
  tup->meta = ctx->Meta.set[idx];

  assert(tup->meta->index == idx);

}

static void refresh(ctx_t ctx, tup_t tup) {                 // Refresh a tuple after a change in the set.
  idx2tup(ctx, tup->meta->index, tup);
}

static void addany(ctx_t ctx, tup_t tup, uint32_t size, uint32_t a4t, uint8_t tom) {

  a4t = roundup(a4t, 8);                                    // Always round up to an 8 byte alignment.

  assert(! ctx->TMA.Grow.locked);
  assert(! ctx->Meta.Grow.locked);

  tup->any  = ctx->TMA.obj(& ctx->TMA, size, a4t);
  tup->meta = ctx->Meta.obj(& ctx->Meta, sizeof(Meta_t), alignof(Meta_t));

  tup->meta->membertype = none;
  tup->meta->container = none;
  tup->meta->tag = none;
  tup->meta->index = ctx->Meta.num - 1;
  tup->meta->tom = tom;

  if (TYPE_ATTR == tom || MEMBER_ATTR == tom || ATTR_DEFINITION == tom) {
    tup->hdr->kind = KoT_ATTR;
  }
  else if (TYPE == tom) {
    tup->hdr->kind = KoT_TYPE;
  }
  else if (MEMBER == tom) {
    tup->hdr->kind = KoT_MEMBER;
  }
  else if (TAG == tom) {
    tup->hdr->kind = KoT_TAG;
  }
  else if (KEYVAL == tom) {
    tup->hdr->kind = KoT_KEYVAL;
  }
  else {
    error(ctx, "Unknown kind %u\n", tom);
  }

  assert(ctx->TMA.num == ctx->Meta.num);                    // Both sets should always contain the same number of elements.

}

static void addmember(ctx_t ctx, tup_t tup, uint32_t sz) {  // Add a member type tuple to the TMA and Meta set.
  addany(ctx, tup, sz, alignof(Member_t), MEMBER);
}

static void addattr(ctx_t ctx, tup_t tup, uint8_t type) {   // Add an attribute type tuple to the TMA and Meta set.
  addany(ctx, tup, sizeof(Attr_t), alignof(Attr_t), type);
}

static void addtype(ctx_t ctx, tup_t tup) {                 // Add a type tuple to the TMA and Meta set.
  addany(ctx, tup, sizeof(Type_t), alignof(Type_t), TYPE);
}

static void addtag(ctx_t ctx, tup_t tup, const char * str) {// Add a tag tuple to the TMA and Meta set.

  uint32_t size = sizeof(Tag_t);

  if (strlen(str) + 1 > sizeof(tup->tag->chars)) {          // See if we need extra space for the string.
    size += strlen(str) - sizeof(tup->tag->chars) + 1;
  }
  
  addany(ctx, tup, size, alignof(Tag_t), TAG);

  tup->tag->size = size;

  memcpy(tup->tag->chars, str, strlen(str));                // Is automatically 0 terminated.

}

static void s4tag(ctx_t ctx, tup_t tup, const char * str) { // Find an existing tag or create a new one.

  uint32_t i;
  tag_t    tag;
  
  for (i = 0; i < ctx->TMA.num; i++) {
    tag = ctx->TMA.set[i];
    if (KoT_TAG == tag->kind) {
      if (0 == strcmp(tag->chars, str)) {
        break;
      }
    }
  }

  if (i == ctx->TMA.num) {                                  // Not found, create one.
    addtag(ctx, tup, str);
    tup->meta->used = 1;
  }
  else {                                                    // Found, load the tuple.
    idx2tup(ctx, i, tup);
    tup->meta->used++;
  }


}

static void addkeyval(ctx_t ctx, const char * key, const char * value) {

  Tup_t Key;
  Tup_t Value;
  Tup_t Attr;

  s4tag(ctx, & Key, key);                                   // Ensure we have the key tag created or found.
  s4tag(ctx, & Value, value);

  addattr(ctx, & Attr, KEYVAL);                             // Add the value attribute.
  
  Attr.meta->container = Key.meta->index;                   // Couple key and value; meta->container = key   (tag)
  Attr.meta->tag       = Value.meta->index;                 //                       meta-tag        = value (tag)

  if (value && 0 == strcmp(key, rootkey)) {                 // The name of the root type.
    ctx->meta4hdr->tag = Value.meta->index;                 // The meta->tag for the header contains the root type name tag.
  }
  
}

type_t anf4type(ctx_t ctx, uint32_t kind, token_t name) {

  Tup_t    T;
  Tup_t    Tag;
  uint32_t i;
  meta_t   meta;
  snset_t  tma = & ctx->TMA;

  s4tag(ctx, & Tag, name->text);                            // Get the name as a tag.

  if (none == ctx->TUC.tidx) {
    ctx->numtypes++;                                        // 1 more type.
    addtype(ctx, & T);
    refresh(ctx, & Tag);                                    // Refresh since addtype might have changed the set.
    T.meta->tag = Tag.meta->index;
    T.meta->id = name;
    T.type->kind = kind;
    ctx->TUC.tidx = T.meta->index;
    printf("\nDefining type [%s] type idx %u", name->text, ctx->TUC.tidx);
    for (i = 0; i < ctx->TUC.tidx; i++) {                   // Assign proper type index to all outstanding non assigned type attributes, if any.
      meta = ctx->Meta.set[i];
      if (TYPE_ATTR == meta->tom) {                         // Only for TYPE_ATTR ...
        if (none == meta->container) {                      // ... that don't have a container yet.
          meta->container = ctx->TUC.tidx;
          tma->stretch(tma, T.meta->index, sizeof(attr_t)); // Make room for an attribute in the type.
          refresh(ctx, & T);
          meta->idx4cont = T.type->numattr++;               // Assign it the proper slot in the type.
          printf(" ATTR{%s}", meta->id->text);
        }
      }
    }
    printf("\n");
  }
  else {
    idx2tup(ctx, ctx->TUC.tidx, & T);
    error(ctx, "Type nesting '%s' in '%s' not allowed.", name->text, T.meta->id->text);
  }
  
  return T.type;

}

void end4type(ctx_t ctx) {

  assert(ctx->TUC.tidx != none);

  Tup_t    T;
  Tup_t    E;
  meta_t   meta;
  uint32_t i;
  Value_t  Val;
  
  idx2tup(ctx, ctx->TUC.tidx, & T);

  if ((T.type->kind & KoT_ENUM) == KoT_ENUM) {              // For an enum, we still need to attach the typetoken.
    assert(ctx->TUC.enumtype);
    memset(& Val, 0x00, sizeof(Val));                       // Enums start at 0 unless overriden below.
    T.meta->typetoken = ctx->TUC.enumtype;
    for (i = ctx->TUC.tidx; i <= ctx->TUC.midx; i++) {      // Now assign proper values to all enum members.
      meta = ctx->Meta.set[i];
      if (MEMBER == meta->tom) {
        idx2tup(ctx, meta->index, & E);
        if (meta->value) {
          token2Value(ctx, meta->value, & Val);
        }
        memcpy(& E.member->Default, & Val, sizeof(Val));
        printf("ENUM MEM [%s] value %ld %s\n", meta->id->text, Val.i64, meta->value ? meta->value->text : "<no val>");
        Val.i64++;                                          // Default value for next member, if any, unless overriden.
      }
    }
  }
  
  printf("TYPE DEFINED [%s] %u members, %u attrs.\n", T.meta->id->text, T.type->nummem, ctx->TUC.numTAttr);
  
  ctx->TUC.tidx = none;
  ctx->TUC.midx = none;
  ctx->TUC.aidx = none;
  ctx->TUC.numTAttr = 0;
  ctx->TUC.numMAttr = 0;
  ctx->TUC.enumtype = NULL;

}

void member(ctx_t ctx, token_t name, token_t typetoken, uint32_t isArray, token_t valOrSize) {

  Tup_t    Con;                                             // Tuple with type containing this member.
  Tup_t    M;                                               // Member tuple.
  Tup_t    Tag;                                             // Tag tuple.
  meta_t   meta;
  uint32_t i;
  snset_t  tma = & ctx->TMA;
  uint32_t size = sizeof(Member_t);

  assert(name);
  assert(ctx->TUC.tidx != none);                            // Must be while parsing a type.

  s4tag(ctx, & Tag, name->text);                            // Find or create the name tag.

  tma->stretch(tma, ctx->TUC.tidx, sizeof(member_t));       // First stretch the enclosing type to contain an extra member ...
  idx2tup(ctx, ctx->TUC.tidx, & Con);                       // ... and get the tuple.
  refresh(ctx, & Tag);                                      // Refresh the tag tuple as its reference might have changed.

  assert(TYPE == Con.meta->tom);

  size += ctx->TUC.numMAttr * sizeof(Attr_t);

  addmember(ctx, & M, size);

  M.meta->idx4cont = Con.type->nummem++;                   // Index for this member in the containing type.
  M.meta->id = name;
  M.meta->isArray = isArray;
  M.meta->typetoken = typetoken;
  M.meta->value = valOrSize;
  M.meta->container = ctx->TUC.tidx;
  M.meta->tag = Tag.meta->index;
  ctx->TUC.midx = M.meta->index;                            // Set the current member under construction index.

  M.member->isArray = isArray;                              // Determine some other properties.

  if (typetoken && 0 == strcmp(typetoken->text, "string")) {
    M.member->isString = 1;
  }

  for (i = ctx->TUC.aidx; i < ctx->TUC.midx; i++) {         // Assign proper member index to all outstanding attributes, if any.
    meta = ctx->Meta.set[i];
    if (MEMBER_ATTR == meta->tom) {                         // Only process member attributes.
      meta->container = ctx->TUC.midx;                      // The attribute is contained by this member.
      meta->idx4cont = M.member->numattr++;                 // The index in Member.attr[] for this attribute.

      tma->stretch(tma, M.meta->index, sizeof(attr_t));     // Stretch the member to include the attribute slot ...
      refresh(ctx, & M);                                    // ... and reload since the reference could have changed.

      printf("ATTR{%s} ", meta->id->text);
    }
  }

  assert(M.member->numattr == ctx->TUC.numMAttr);           // Should give an exact match.

  if (valOrSize) { 
    if (! isArray) {
      printf("MEMBER [%s] = %s", name->text, valOrSize->text);
    }
    else {
      // In structs, the 'array' size must be fixed, since structs are fixed.
      // In tables, the array size can vary and they are called 'vectors'.
      printf("MEMBER [%s:%s]", name->text, valOrSize->text);
    }
  }
  else {
    printf("MEMBER [%s]", name->text);
  }

  printf(" %u attrs ", ctx->TUC.numMAttr);

  if (typetoken) { 
    printf("TYPE [%s%s] ", isArray ? "ARRAY OF " : "", typetoken->text);
  }
  
  printf("\n");

  ctx->TUC.aidx = none;                                     // Reset attribute list after each member defined.
  ctx->TUC.numMAttr = 0;

}

void enumtype(ctx_t ctx, token_t type) {

//  Tup_t E;
  
//  find(ctx, type, TYPE, & E);
  
//  printf("ENUM TYPE [%s] reffed %u\n", type->text, E.meta ? E.meta->used : 0);

  ctx->TUC.enumtype = type;

}

void attr(ctx_t ctx, token_t name, token_t value) {         // Create an attribute for the upcoming member or table/struct.

  Tup_t    A;
  Tup_t    T;
  uint32_t typeattr = (none == ctx->TUC.tidx) ? 1 : 0;      // If the type is not know yet, it will become a type attribute.

  assert(name); 

  printf("[ '%s' is a %s attribute, value %s (%u)] ", name->text, typeattr ? "type/struct" : "member", value ? value->text : "<none>", value ? value->cti : 0);

  addattr(ctx, & A, typeattr ? TYPE_ATTR : MEMBER_ATTR);

  A.meta->id = name;
  A.meta->value = value;

  if (value) {
    token2Value(ctx, value, & A.attr->Value);
  }

  s4tag(ctx, & T, name->text);                              // Find or create the name tag.

  refresh(ctx, & A);                                        // Reload attribute tuple, since it could have been changed by search4tag.

  A.attr->name = name->text;                                // This is a temporary string; the tag->chars will be assigned at the link stage.
  A.meta->tag = T.meta->index;                              // For now record the tag index in the meta info for the attribute. 

  if (none == ctx->TUC.aidx) {                              // First of the set of attributes for the current member?
    ctx->TUC.aidx = A.meta->index;                          // Index of this attribute.
  }

  if (typeattr) {
    ctx->TUC.numTAttr++;
  }
  else {
    ctx->TUC.numMAttr++;
  }

}

void keyval(ctx_t ctx, const char * key, token_t value) {   // Add a key/value pair; internal keys have I: prepended.
  addkeyval(ctx, key, value->text);
}

void nselement(ctx_t ctx, token_t comp) {                   // Process a name space element.

  uint32_t off = strlen(ctx->nmspace);

  snprintf(ctx->nmspace + off, sizeof(ctx->nmspace) - off, "%s_", comp->text);

}

static void builtins(ctx_t ctx) {                           // Add the builtin types to the set of tokens.

  uint32_t       i;
  Type_t const * t = & Builtin[0];
  type_t         type;
  token_t        token;
  uint32_t       size = NUM(Builtin) * 64;                  // Worst case number of bytes we'll add.

  ctx->Tokens.ensure(& ctx->Tokens, NUM(Builtin), size);    // We don't want reallocations on the token set anymore.

  ctx->Tokens.Grow.locked = 1;                              // Lock the set, just in case.
  
  for (i = 0; i < NUM(Builtin); t++, i++) {
    size = sizeof(Token_t) + strlen(t->name);
    token = ctx->Tokens.obj(& ctx->Tokens, size, alignof(Token_t));
    if (token) {
      token->size = strlen(t->name);
      memcpy(token->text, t->name, token->size);
      type = anf4type(ctx, KoT_PRIM, token);
      type->canontype = t->canontype;
      if (ctx->numtypes <= 10) {
        assert(ctx->numtypes - 1 == t->canontype);          // For the first 10 types, the canontype should be the type number in the set.
      }
      end4type(ctx);
      assert(! ctx->error);
    }
    else {
      error(ctx, "Could not add builtin %u to token set.\n", i);
    }
  }

}

void fbpyyparse(ctx_t ctx);                                 // Defined by bison in fb-parser.c.

uint32_t fbpyyerror(ctx_t ctx, const char * msg) {          // Also see yyreport_syntax_error() in bison-fb.y.

  error(ctx, "ERROR: %s", msg);

  return 0;

}

static void * mem4set(snset_t set, void * mem, uint32_t sz) {

  ctx_t ctx = set->custom;

  return ctx->fb2_ctx->mem(ctx->fb2_ctx, mem, sz);

}

extern int fbpyydebug;

void fb2_parse(fb2_ctx_t ctx, const char * schema, uint32_t size) {

  assert(offsetof(Hdr_t, name) == offsetof(Member_t, name));
  assert(offsetof(Hdr_t, kind) == offsetof(Member_t, kind));
  assert(offsetof(Hdr_t,  o2n) == offsetof(Member_t,  o2n));

  assert(offsetof(Hdr_t, name) == offsetof(Type_t,   name));
  assert(offsetof(Hdr_t, kind) == offsetof(Type_t,   kind));
  assert(offsetof(Hdr_t,  o2n) == offsetof(Type_t,    o2n));

  assert(offsetof(Hdr_t, name) == offsetof(Attr_t,   name));
  assert(offsetof(Hdr_t, kind) == offsetof(Attr_t,   kind));
  assert(offsetof(Hdr_t,  o2n) == offsetof(Attr_t,    o2n));

  assert(offsetof(Hdr_t, name) == offsetof(Tag_t,   string));
  assert(offsetof(Hdr_t, kind) == offsetof(Tag_t,     kind));
  assert(offsetof(Hdr_t,  o2n) == offsetof(Tag_t,      o2n));

  assert(offsetof(Hdr_t, name) == offsetof(fb2_Any_t, Type.name));
  assert(offsetof(Hdr_t, kind) == offsetof(fb2_Any_t, Type.kind));
  assert(offsetof(Hdr_t,  o2n) == offsetof(fb2_Any_t, Type.o2n));
  
  assert((offsetof(fb2_BSchema_t, bytes) & 0b111) == 0);    // First element should start on an 8 byte address.

  assert(sizeof(fb2_Type_t)    == 24);                      // Ensure we have fixed width types on all machines.
  assert(sizeof(fb2_Attr_t)    == 24);
  assert(sizeof(fb2_Member_t)  == 32);
  assert(sizeof(fb2_Tag_t)     == 16);
  assert(sizeof(fb2_BSchema_t) == 40);

  Ctx_t   Ctx;                                              // Internal context.
  
  memset(& Ctx, 0x00, sizeof(Ctx));

  Ctx.fb2_ctx = ctx;

  snset_init(& Ctx.Tokens, mem4set);                        // Set with tokens.
  Ctx.Tokens.Grow.bytes = 2048;
  Ctx.Tokens.Grow.slots =   64; 
  Ctx.Tokens.custom = & Ctx;

  snset_init(& Ctx.TMA, mem4set);                           // Type, members and attributes set.
  Ctx.TMA.Grow.bytes = 4096;
  Ctx.TMA.Grow.slots =   64;
  Ctx.TMA.custom = & Ctx;


  snset_init(& Ctx.Meta, mem4set);                          // Meta information set.
  Ctx.Meta.Grow.bytes = 4096;
  Ctx.Meta.Grow.slots =   64;
  Ctx.Meta.custom = & Ctx;

  Ctx.TMA.obj(& Ctx.TMA, sizeof(fb2_BSchema_t), 8);         // Slot 0 is for the header; create it as soon as both sets are initialized.
  Ctx.meta4hdr = Ctx.Meta.obj(& Ctx.Meta, sizeof(Meta_t), 8);
  Ctx.meta4hdr->tom = HEADER;

  Ctx.TUC.tidx = none;                                      // Clear all type under construction data.
  Ctx.TUC.midx = none;
  Ctx.TUC.aidx = none;
  Ctx.TUC.numMAttr = 0;
  Ctx.TUC.numTAttr = 0;
  Ctx.TUC.enumtype = NULL;

  flutscan(& Ctx, schema, size);

//--
  uint32_t i;

  token_t tok;
  token_t prev = NULL;

  tok = Ctx.Tokens.set[0];

  printf("Number of tokens %u, %u reallocations.\n", Ctx.Tokens.num, Ctx.Tokens.reallocs);

  for (i = 0; i < Ctx.Tokens.num; i++, prev = tok, tok = tok2next(tok)) {
    if (prev &&  prev->row < tok->row) printf("\n");
    if (tok->size) printf("%s ", tok->text);
    else printf("[%u:%u] ", tok->type, tok->cti);
//    printf("%p\n", tok);
  }
//--

  builtins(& Ctx);                                          // Add the builtin types.
  
  Ctx.nexttoken = Ctx.Tokens.set[0];                        // Token to start the parsing with, is the first in the set.

//  fbpyydebug = 1; // Enable for debugging

  fbpyyparse(& Ctx);                                        // Now start the parsing.

  while ('_' == Ctx.nmspace[strlen(Ctx.nmspace) - 1]) {     // Strip any trailing underscores.
    Ctx.nmspace[strlen(Ctx.nmspace) - 1] = 0;
  }

  addkeyval(& Ctx, "I:namespace", Ctx.nmspace);             // Add final namespace key/value pair.

  Ctx.TMA.Grow.locked = 1;                                  // This set should now be fixed.

  fb2link(& Ctx);                                           // Create the binary schema by linking all together.

  if (ctx->schema) {
    ctx->schema->Num.canonical = 10;                        // Up to and including float.
  }

  printf("alignof(Token_t) %zu\n", alignof(Token_t));

}
