// Copyright (c) 2023 Steven Buytaert

#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <stdarg.h>

#include <fb2-common.h>

static const fb2s_Type_t Builtin[] = {
  { .name = "none",     .canontype =  0, .ct_type = ct_none               },

  { .name = "int8_t",   .canontype =  1, .ct_type = ct_int8,   .signd = 1 },
  { .name = "uint8_t",  .canontype =  2, .ct_type = ct_uint8,             }, // The first ones refer to themselves as canonical.
  { .name = "int16_t",  .canontype =  3, .ct_type = ct_int16,  .signd = 1 },
  { .name = "uint16_t", .canontype =  4, .ct_type = ct_uint16,            },
  { .name = "int32_t",  .canontype =  5, .ct_type = ct_int32,  .signd = 1 },
  { .name = "uint32_t", .canontype =  6, .ct_type = ct_uint32,            },
  { .name = "int64_t",  .canontype =  7, .ct_type = ct_int64,  .signd = 1 },
  { .name = "uint64_t", .canontype =  8, .ct_type = ct_uint64,            },
  { .name = "float",    .canontype =  9, .ct_type = ct_float,  .signd = 1 },
  { .name = "double",   .canontype = 10, .ct_type = ct_double, .signd = 1 },

  { .name = "int8",     .canontype =  1, .ct_type = ct_int8,   .signd = 1 },
  { .name = "uint8",    .canontype =  2, .ct_type = ct_uint8,             },
  { .name = "int16",    .canontype =  3, .ct_type = ct_int16,  .signd = 1 },
  { .name = "uint16",   .canontype =  4, .ct_type = ct_uint16,            },
  { .name = "int32",    .canontype =  5, .ct_type = ct_int32,  .signd = 1 },
  { .name = "uint32",   .canontype =  6, .ct_type = ct_uint32,            },
  { .name = "int64",    .canontype =  7, .ct_type = ct_int64,  .signd = 1 },
  { .name = "uint64",   .canontype =  8, .ct_type = ct_uint64,            },
  { .name = "float32",  .canontype =  9, .ct_type = ct_float,  .signd = 1 },
  { .name = "float64",  .canontype = 10, .ct_type = ct_double, .signd = 1 },
  
  { .name = "byte",     .canontype =  1, .ct_type = ct_int8,   .signd = 1 },
  { .name = "ubyte",    .canontype =  2, .ct_type = ct_uint8,             },
  { .name = "short",    .canontype =  3, .ct_type = ct_int16,  .signd = 1 },
  { .name = "ushort",   .canontype =  4, .ct_type = ct_uint16,            },
  { .name = "int",      .canontype =  5, .ct_type = ct_int32,  .signd = 1 },
  { .name = "uint",     .canontype =  6, .ct_type = ct_uint32,            },
  { .name = "long",     .canontype =  7, .ct_type = ct_int64,  .signd = 1 },
  { .name = "ulong",    .canontype =  8, .ct_type = ct_uint64, .signd = 1 },

  { .name = "bool",     .canontype =  1, .ct_type = ct_int8               },
  { .name = "string",   .canontype =  0, .ct_type = ct_string             },
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

// find a type/member/attr by its token and type of meta; when found, fill the tuple.

static uint32_t find(ctx_t ctx, token_t token, uint8_t tom, tup_t tup) {

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

static void idx2tup(ctx_t ctx, uint32_t idx, tup_t tup) {   // Fill a tuple with the type and meta information of the given index.

  assert(idx < ctx->Meta.num && idx != none);

  tup->any = ctx->TMA.set[idx];
  tup->meta = ctx->Meta.set[idx];

  assert(tup->meta->index == idx);

}

static void refresh(ctx_t ctx, tup_t tup) {                 // Refresh a tuple after a change in the set.
  idx2tup(ctx, tup->meta->index, tup);
}

static void addany(ctx_t ctx, tup_t tup, uint32_t size, uint32_t a4t, uint8_t tom) {

  a4t = roundup(a4t, 8);                                    // Always round up to an 8 byte alignment for o2n to work later.

  assert(! ctx->TMA.Grow.locked);
  assert(! ctx->Meta.Grow.locked);

  tup->any  = ctx->TMA.obj(& ctx->TMA, size, a4t);
  tup->meta = ctx->Meta.obj(& ctx->Meta, sizeof(Meta_t), alignof(Meta_t));

  tup->meta->membertype = none;
  tup->meta->container = none;
  tup->meta->tag = none;
  tup->meta->index = ctx->Meta.num - 1;
  tup->meta->tom = tom;

  switch(tom) {
    case TYPE_ATTR:
    case MEMBER_ATTR: tup->hdr->fb2ti = fb2e_Attr;    break;
    case TYPE:                                        break;// Don't assign here; proper kind will be assigned in anf4type.
    case MEMBER:      tup->hdr->fb2ti = fb2e_Member;  break;
    case TAG:         tup->hdr->fb2ti = fb2e_Tag;     break;
    case KEYVAL:      tup->hdr->fb2ti = fb2e_KeyVal;  break;
    default: error(ctx, "Unknown type %u\n", tom); break;
  }

  assert(ctx->TMA.num == ctx->Meta.num);                    // Both sets should always contain the same number of elements.

}

static void addmember(ctx_t ctx, tup_t tup, uint32_t sz) {  // Add a member type tuple to the TMA and Meta set.
  addany(ctx, tup, sz, alignof(IMember_t), MEMBER);
}

static void addattr(ctx_t ctx, tup_t tup, uint8_t type) {   // Add an attribute type tuple to the TMA and Meta set.
  addany(ctx, tup, sizeof(KeyVal_t), alignof(KeyVal_t), type);
}

static void addtype(ctx_t ctx, tup_t tup) {                 // Add a type tuple to the TMA and Meta set.
  addany(ctx, tup, sizeof(IType_t), alignof(IType_t), TYPE);
}

static void addtag(ctx_t ctx, tup_t tup, const char * str) {// Add a tag tuple to the TMA and Meta set.

  uint32_t size = sizeof(Tag_t);

  if (strlen(str) + 1 > sizeof(tup->tag->chars)) {          // See if we need extra space for the string.
    size += strlen(str) - sizeof(tup->tag->chars) + 1;
  }
  
  addany(ctx, tup, size, alignof(Tag_t), TAG);

  tup->tag->size = strlen(str) + 1;

  memcpy(tup->tag->chars, str, strlen(str));                // Is automatically 0 terminated.

}

static void s4tag(ctx_t ctx, tup_t tup, const char * str) { // Find an existing tag or create a new one.

  uint32_t i;
  tag_t    tag;
  
  for (i = 0; i < ctx->TMA.num; i++) {
    tag = ctx->TMA.set[i];

    if (fb2e_Tag == tag->fb2ti) {
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

  Tup_t Key_Tag;
  Tup_t Value_Tag;
  Tup_t KeyValue;

  s4tag(ctx, & Key_Tag, key);                               // Ensure we have the key tag created or found.
  s4tag(ctx, & Value_Tag, value);

  addattr(ctx, & KeyValue, KEYVAL);                         // Add the value attribute.
  
  KeyValue.meta->container = Key_Tag.meta->index;           // Couple key and value; meta->container = key   (tag)
  KeyValue.meta->tag       = Value_Tag.meta->index;         //                       meta-tag        = value (tag)

  if (value && 0 == strcmp(key, rootkey)) {                 // The name of the root type.
    ctx->meta4hdr->tag = Value_Tag.meta->index;             // The meta->tag for the header contains the root type name tag.
  }
  
}

type_t anf4type(ctx_t ctx, uint32_t ti, token_t name) {

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
    T.type->fb2ti = ti;

    ctx->TUC.tidx = T.meta->index;
    printf("\nDefining type [%s] type idx %u", name->text, ctx->TUC.tidx);
    for (i = 0; i < ctx->TUC.tidx; i++) {                   // Assign proper type index to all outstanding non assigned type attributes, if any.
      meta = ctx->Meta.set[i];
      if (TYPE_ATTR == meta->tom) {                         // Only for TYPE_ATTR ...
        if (none == meta->container) {                      // ... that don't have a container yet.
          meta->container = ctx->TUC.tidx;
          tma->stretch(tma, T.meta->index, sizeof(keva_t)); // Make room for an attribute in the type.
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

static void token2Value(ctx_t ctx, token_t token, Value_t * val) {
                                                            // Convert a token to a value; note that we only convert here and determine if there
                                                            // is a minus sign in front of the decimal or the float. We don't try to categorize
  const char * number = NULL;                               // here yet. Convert to a 64 bit type. Further refining done in setMemberType(). Also
  int32_t      base = 0;                                    // note that STRING constants are NOT handled here; they yield an error.

  assert(val);
  
  val->minus = ('-' == token->text[0]) ? 1 : 0;             // Only applies to floats and decimals.

  switch (token->cti) {                                     // Find what type of value we need, create the proper search token and search.
    case FLOAT: {                                           // Could be float32 or float64.
      val->f64 = strtod(token->text, NULL);
      if (errno) {
        error(ctx ,"strtod(%s) failed: %s.", token->text, strerror(errno));
      }
      break;
    }
      
    case HEX: {
      base = 16;
      number = & token->text[2];                            // Skip 0x
      break;
    }
      
    case BIN: {
      base = 2;
      number = & token->text[2];                            // Skip 0b
      break;
    }
      
    case DEC: {
      base = 10;
      number = token->text;
      break;
    }
        
    case TRUE:
    case FALSE: {
      val->i64 = token->cti == FALSE ? 0 : 1;
      break;
    }

    default: {
      error(ctx, "Unknown CTI %u.", token->cti);
    }
  }
    
  if (number) {                                             // Do we need to convert a number?
    val->u64 = strtoull(number, NULL, base);
    if (errno) {
      error(ctx, "strtoull(%s) failed: %s.", token->text, strerror(errno));
    }
  }

  assert(! ctx->error); // for now

}

void end4type(ctx_t ctx) {

  assert(ctx->TUC.tidx != none);

  Tup_t    T;
  Tup_t    E;
  meta_t   meta;
  uint32_t i;
  Value_t  Val;
  
  idx2tup(ctx, ctx->TUC.tidx, & T);

  if (fb2e_Enum == T.type->fb2ti) {                         // For an enum, we still need to attach the typetoken.
    assert(ctx->TUC.enumtype);
    memset(& Val, 0x00, sizeof(Val));                       // Enum members start at 0 unless overriden below.
    T.meta->typetoken = ctx->TUC.enumtype;
    for (i = ctx->TUC.tidx; i <= ctx->TUC.midx; i++) {      // Now assign proper values to all enum members.
      meta = ctx->Meta.set[i];
      if (MEMBER == meta->tom) {
        idx2tup(ctx, meta->index, & E);
        if (meta->value) {
          token2Value(ctx, meta->value, & Val);
        }
        memcpy(& E.member->Default, & Val, sizeof(Val));
//        printf("ENUM MEM [%s] value %ld %s\n", meta->id->text, Val.i64, meta->value ? meta->value->text : "<no val>");
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
  uint32_t size = sizeof(IMember_t);

  assert(name);
  assert(ctx->TUC.tidx != none);                            // Must be while parsing a type.

  s4tag(ctx, & Tag, name->text);                            // Find or create the name tag.

  tma->stretch(tma, ctx->TUC.tidx, sizeof(member_t));       // First stretch the enclosing type to contain an extra member ...
  idx2tup(ctx, ctx->TUC.tidx, & Con);                       // ... and get the tuple.
  refresh(ctx, & Tag);                                      // Refresh the tag tuple as its reference might have changed.

  assert(TYPE == Con.meta->tom);

  size += ctx->TUC.numMAttr * sizeof(KeyVal_t);

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

      tma->stretch(tma, M.meta->index, sizeof(keva_t));     // Stretch the member to include the attribute key value slot ...
      refresh(ctx, & M);                                    // ... and reload since the reference could have changed.

      printf("ATTR{%s} ", meta->id->text);
    }
  }

  assert(M.member->numattr == ctx->TUC.numMAttr);           // Should give an exact match.

  if (valOrSize) { 
    if (valOrSize->cti) {
      if (STRING == valOrSize->cti) {                       // A string constant; save it as a tag.
        s4tag(ctx, & Tag, valOrSize->text);                 // Create the tag with the constant.
        refresh(ctx, & M);                                  // Refresh the member tuple as it might have changed.
        M.meta->stringConst = Tag.meta->index;
      }
      else {                                                // A numerical constant; set the member value.
        token2Value(ctx, valOrSize, & M.member->Default);
      }
    }
    if (! isArray) {
      printf("MEMBER [%s] = %s (default)", name->text, valOrSize->text);
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

  Tup_t ET;
  
  if (! find(ctx, type, TYPE, & ET)) {
    error(ctx, "Enum type '%s' not found.\n", type->text);
  }
  
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

  refresh(ctx, & A);                                        // Reload attribute tuple, since it could have been changed by s4tag.

  A.attr->key = name->text;                                 // This is a temporary string; the tag->chars will be assigned at the link stage.
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

  uint32_t        i;
  IType_t const * t = & Builtin[0];
  type_t          type;
  token_t         token;
  uint32_t        size = NUM(Builtin) * 64;                 // Worst case number of bytes we'll add.

  ctx->Tokens.ensure(& ctx->Tokens, NUM(Builtin), size);    // We don't want reallocations on the token set anymore.

  ctx->Tokens.Grow.locked = 1;                              // Lock the set, just in case.
  
  for (i = 0; i < NUM(Builtin); t++, i++) {
    size = sizeof(Token_t) + strlen(t->name);
    token = ctx->Tokens.obj(& ctx->Tokens, size, alignof(Token_t));
    if (token) {
      token->size = strlen(t->name);
      memcpy(token->text, t->name, token->size);
      type = anf4type(ctx, fb2e_Prim, token);
      type->canontype = t->canontype;
      type->signd = t->signd;
      type->ct_type = t->ct_type;
      if (ctx->numtypes <= 11) {
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

void fb2_pyyparse(ctx_t ctx);                               // Defined by bison in fb-parser.c.

uint32_t fb2_pyyerror(ctx_t ctx, const char * msg) {        // Also see yyreport_syntax_error() in bison-fb.y.

  error(ctx, "ERROR: %s", msg);

  return 0;

}

static void * mem4set(snset_t set, void * mem, uint32_t sz) {

  ctx_t ctx = set->custom;

  return ctx->sctx->mem(ctx->sctx, mem, sz);

}

static void attr2member(ctx_t ctx, meta_t meta) {           // Link the attribute into the proper member slot.

  member_t member = ctx->TMA.set[meta->container];
  keva_t   attr   = ctx->TMA.set[meta->index];

  assert(member->fb2ti == fb2e_Member);
  assert(attr->fb2ti == fb2e_Attr);
  assert(meta->idx4cont < member->numattr);

  member->attr[meta->idx4cont] = attr;

}

static uint32_t isaTypeKind(uint8_t ti) {                   // Return 1 when the kind is a type.
  return (ti >= fb2e_Prim && ti <= fb2e_Struct) ? 1 : 0;
}

static void attr2type(ctx_t ctx, meta_t meta) {             // Link the attribute into the proper type slot.

  type_t   type = ctx->TMA.set[meta->container];
  keva_t   attr = ctx->TMA.set[meta->index];
  keva_t * ta = (keva_t *) & type->members[type->nummem];   // Type attribute array just behind the member slots.

  assert(isaTypeKind(type->fb2ti));
  assert(attr->fb2ti == fb2e_Attr);
  assert(meta->idx4cont < type->numattr);

  ta[meta->idx4cont] = attr;

}

static uint32_t isaCompound(uint8_t ti) {                   // Return 1 if a compound type.

  return (ti >= fb2e_Table && ti <= fb2e_Struct) ? 1 : 0;

}

// Return the value associated with the given enum member token.

static fb2_Value_t enumm2val(ctx_t ctx, type_t etype, token_t member) {

  uint32_t    i;
  fb2_Value_t NotFound = { .u64 = 0 };
  
  for (i = 0; i < etype->nummem; i++) {
    if (! strcmp(etype->members[i]->name, member->text)) {
      return etype->members[i]->Default;
    }
  }

  error(ctx, "Member '%s' not found in enum '%s'\n", member->text, etype->name);

  return NotFound;

}

static void setMemberType(ctx_t ctx, meta_t meta) {         // Determine the proper type for the member and also the default value, if any.

  Tup_t    ToM;                                             // Type of the member.
  Tup_t    Con;                                             // Type that contains the member.
  tag_t    tag = ctx->TMA.set[meta->tag];
  member_t member = ctx->TMA.set[meta->index];
  type_t   type;

  assert(memset(& ToM, 0x00, sizeof(ToM)));
  assert(memset(& Con, 0x00, sizeof(Con)));
  assert(none == meta->membertype);                         // Membertype should not be set yet.
  assert(none != meta->container);

  idx2tup(ctx, meta->container, & Con);                     // Get the enclosing/parent type in our tuple.
  assert(Con.meta->tom == TYPE);

  member->name = tag->chars;
  
  if (meta->typetoken) {
    assert(meta->typetoken);
    if (find(ctx, meta->typetoken, TYPE, & ToM)) {
      meta->membertype = ToM.meta->index;
    }
    else {
      error(ctx, "member type '%s' of '%s' not found.", meta->typetoken->text, meta->id->text);
    }
  }
  else {                                                    // No typetoken, see if it's an enum or union member, based on the parent type.
    if (fb2e_Enum == Con.type->fb2ti) {                     // An enum, the type is the type of the enum itself.
      assert(Con.meta->typetoken);                          // So it must have a typetoken.
      if (find(ctx, Con.meta->typetoken, TYPE, & ToM)) {
        assert(fb2e_Prim == ToM.type->fb2ti);               // Must be a primitive. Careful: we are setting the type on the enum *member* itself!
        member->Default.signd = ToM.type->signd;            // NOT on any other member that has this enum as type. That is done below, at the AA note.
        member->Default.type = ToM.type->ct_type;
        meta->membertype = ToM.meta->index;
        ToM.meta->used++;
        Con.type->type4enum = ToM.type;
      }
      else {
        error(ctx, "Type of enum '%s' '%s' not found.", Con.meta->id->text, Con.meta->typetoken->text);
      }
    }
    else if(fb2e_Union == Con.type->fb2ti) {                // A union, the member name itself *is* a type.
      if (find(ctx, meta->id, TYPE, & ToM)) {
        meta->membertype = ToM.meta->index;
        ToM.meta->used++;
      }
      else {
        error(ctx, "Could not find union member type '%s' in union '%s'.", meta->id->text, Con.meta->id->text);
      }
    }
    else {
      // Without a type, can only be a union or an enum.
      error(ctx, "No type on '%s' and not a union/enum.", meta->id->text);
    }
  }

//  printf("MEMBER [%s] ti %x\n", meta->id->text, ToM.type->fb2ti); fflush(stdout);

  assert(isaTypeKind(ToM.type->fb2ti));
  assert(isaCompound(Con.type->fb2ti));                     // Members can only be part of a compound type.
  
  member->type = ToM.type;                                  // We can now give the member the proper type ...
  Con.type->members[meta->idx4cont] = member;               // ... and assign it to the proper slot in the containing type.

  if (fb2e_Prim == member->type->fb2ti) {
    if (meta->stringConst) {
      member->Default.ref = ctx->TMA.set[meta->stringConst];
      member->Default.type = ct_string;
    }
    else {                                                  // A numerical default.
      member->Default.signd = member->type->signd;
      member->Default.type = member->type->ct_type;
      printf("MEMBER [%s] TYPE [%s:%ssigned]\n", member->name, member->type->name, member->type->signd ? "" : "un");
      if (0LL == member->Default.u64) {                     // For -0 or -0.0 strip the minus flag, if any.
        member->Default.minus = 0;
      }
    }
  }
  else if (fb2e_Enum == member->type->fb2ti) {              // See note AA above; here we set the type on a member that is an enum value.
    if (member->type->type4enum) {                          // That type must already have the proper enumtype set.
      printf("MEMBER [%s] is ENUM [%s] default '%s'\n", member->name, meta->typetoken->text, meta->value ? meta->value->text : "(none)");
      if (meta->value) {                                    // This member has a specific default value.
        if (find(ctx, meta->typetoken, TYPE, & ToM)) {
          member->Default.u64 = enumm2val(ctx, ToM.type, meta->value).u64; // There is a val2enumm in the runtime, doing the inverse.
        }
        else {
          error(ctx, "Enum of member '%s' '%s' not found.", member->name, meta->typetoken->text);
        }
      }
      type = member->type->type4enum;
      assert(type->signd);                                  // Enum values are always signed; or make this into an error?
      member->Default.signd = 1;
      member->Default.type = type->ct_type;
    }
    else {
      error(ctx, "Member [%s] refers to enum before it is defined.\n", member->name);
    }
  }

}

fb2s_Any_t * fb2s_go2next(void const * cur) {               // Go to the next element; return NULL when no next element.

  Hdr_t const * here = cur;
  fb2s_Any_t *   next;

  if (! cur) { return NULL; }
  
  assert((here->fb2ti & 0b1111111111110000) == 0);          // Small check.

  if (0 == here->o2n) { return NULL; }                      // No more next element.

  next = (fb2s_Any_t *) ((char *) here + (8 * here->o2n));
  
  assert((next->Type.fb2ti & 0b1111111111110000) == 0);
  
  return next;
  
}

static uint32_t couldBeRoot(ctx_t ctx, uint16_t ti) {     // Return true when the kind could be a root type.

  if (ti == fb2e_Table) return 1;

//  KoT_STRUCT     = 0b00000010, // these could also be root types but are not allowed by the standard
//  KoT_UNION      = 0b00000100,

  return 0;

}
  
void fb2link(ctx_t ctx) {

  snset_t   metaset = & ctx->Meta;
  snset_t   tma  = & ctx->TMA;
  uint32_t  i;
  meta_t    meta;
  tag_t     tag;
  tag_t     key;
  keva_t    attr;
  type_t    type;
  Hdr_t *   cur;
  Hdr_t *   prev = NULL;
  schema_t  schema;
  uint32_t  o2n;
  uint32_t  numtail;

  struct {
    const char * name;
    type_t       type;
  } Root;

  memset(& Root, 0x00, sizeof(Root));

  assert(ctx->TMA.Grow.locked);

  schema = tma->set[0];                                     // Element[0] is header; set is locked so now we can refer to the obj directly.

  ctx->sctx->schema = schema;

  for (i = 0; ! ctx->error && i < metaset->num; i++) {      // Go over all types, members and attributes collected so far.
    meta = metaset->set[i];
    assert(meta->tom == TYPE || meta->tom == TAG || meta->container != none);
    if (meta->tom == TYPE_ATTR) {
      attr2type(ctx, meta);
    }
    else if (meta->tom == MEMBER_ATTR) {
      attr2member(ctx, meta);
    }
    else if (meta->tom == MEMBER) {
      setMemberType(ctx, meta);
    }
    else if (meta->tom == TAG) {                            // Assign the tag string field to the chars array.
      tag = tma->set[i];
      assert(fb2e_Tag == tag->fb2ti);
      tag->string = tag->chars;
      if (i == ctx->meta4hdr->tag) {                        // If this is the tag for the root type, save it.
        Root.name = tag->string;
      }
    }
    else if (meta->tom == TYPE) {                           // Assign the type its proper name tag.
      tag = tma->set[meta->tag];
      assert(fb2e_Tag == tag->fb2ti);
      type = tma->set[meta->index];
      assert(isaTypeKind(type->fb2ti));
      type->name = tag->chars;
      schema->Num.types++;
      if (schema->Num.maxmem < type->nummem) {              // Keep track of the maximums.
        schema->Num.maxmem = type->nummem;
      }
      numtail = (uint32_t)(type->nummem + type->numattr);   // Total number of tail slots for this type.
      if (schema->Num.maxtail < numtail) {
        schema->Num.maxtail = numtail;
      }
    }
    else if (meta->tom == KEYVAL) {
      tag = tma->set[meta->tag];
      key = tma->set[meta->container];
      tag->string = tag->chars;
      key->string = key->chars;
      attr = tma->set[meta->index];
      attr->key = key->chars;
//      attr->Value.tag = tag;
      attr->Value.ref = tag;
//      printf("KEYVAL %s:%s\n", attr->key, attr->Value.tag->chars);
      printf("KEYVAL %s:%s\n", attr->key, tag->chars);
    }
    else if (meta->tom == HEADER) {
      // Do nothing.
    }
    else {
      error(ctx, "Unknown meta type %u\n", meta->tom);
    }
  }


  fflush(stdout); assert(Root.name); // Must have a root here.

  for (i = 1; i < tma->num; i++) {                          // Set the proper offset to next for all elements in the TMA set. Skip the header.
    cur = tma->set[i];
    assert(0 == ((uintptr_t) cur & 0b111));                 // Must be 8 byte aligned since we set wca to 8 in the TMA set at the start.
    if (couldBeRoot(ctx, cur->fb2ti)) {
      if (0 == strcmp(Root.name, cur->name)) {              // Found the root type.
        Root.type = tma->set[i];
        schema->root = (uint32_t)((uint8_t *) Root.type - schema->bytes);
      }
    }
    if (prev) {
      o2n = (uint32_t) ((uint8_t *) cur - (uint8_t *) prev) / 8;       // Since all objects are aligned on an 8 byte address.
      assert(o2n <= 0xffff);
      prev->o2n = o2n;
    }
    prev = cur;
  }

//  for (cur = tma->set[1], i = 1; cur; cur = fb2_go2next(cur), i++) { // Go over all types, members and attributes collected so far.
//    printf("Cur %p type %u\n", cur, cur->fb2ti);
//    printf("%12s '%s' used %u\n", t2a[meta->tom], meta->id ? meta->id->text : "<tag>", meta->used);
//  }

  assert(tma->num == i);

  schema->size = tma->size;
  schema->Num.elements = tma->num - 1;                      // Don't include the header.

  assert(& schema->Elements[0] == tma->set[1]);             // Ensure the first schema element and the first object align properly.

  printf("%u elements, %u object bytes.\n", i, tma->size);

  printf("First   %p\n", tma->set[1]);
  printf("E0 addr %p\n", & schema->Elements[0]);

}

extern int fbpyydebug;

const uint8_t fb2sid[7] = { 0x34, 0x65, 0x14, 0x51, 0x22, 0x51, 0x17 };

void fb2s_parse(fb2s_ctx_t ctx, const char * schema, uint32_t size) {

  assert(offsetof(Hdr_t,  name) == offsetof(IMember_t,   name));
  assert(offsetof(Hdr_t, fb2ti) == offsetof(IMember_t,  fb2ti));
  assert(offsetof(Hdr_t,   o2n) == offsetof(IMember_t,    o2n));

  assert(offsetof(Hdr_t,  name) == offsetof(IType_t,     name));
  assert(offsetof(Hdr_t, fb2ti) == offsetof(IType_t,    fb2ti));
  assert(offsetof(Hdr_t,   o2n) == offsetof(IType_t,      o2n));

  assert(offsetof(Hdr_t,  name) == offsetof(KeyVal_t,     key));
  assert(offsetof(Hdr_t, fb2ti) == offsetof(KeyVal_t,   fb2ti));
  assert(offsetof(Hdr_t,   o2n) == offsetof(KeyVal_t,     o2n));

  assert(offsetof(Hdr_t,  name) == offsetof(Tag_t,     string));
  assert(offsetof(Hdr_t, fb2ti) == offsetof(Tag_t,      fb2ti));
  assert(offsetof(Hdr_t,   o2n) == offsetof(Tag_t,        o2n));

  assert(offsetof(Hdr_t,  name) == offsetof(Any_t,  Type.name));
  assert(offsetof(Hdr_t, fb2ti) == offsetof(Any_t, Type.fb2ti));
  assert(offsetof(Hdr_t,   o2n) == offsetof(Any_t,   Type.o2n));
  
  assert((offsetof(fb2s_Schema_t, bytes) & 0b111) == 0);    // First element should start on an 8 byte address.

  assert(sizeof(fb2s_Type_t)   == 32);                      // Ensure we have fixed width types on all machines.
  assert(sizeof(fb2s_KeyVal_t) == 32);
  assert(sizeof(fb2s_Member_t) == 48);
  assert(sizeof(fb2s_Tag_t)    == 16);
  assert(sizeof(fb2s_Schema_t) == 40);

  Ctx_t Ctx;                                                // Internal context.
  
  memset(& Ctx, 0x00, sizeof(Ctx));

  Ctx.sctx = ctx;

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

  Ctx.TMA.obj(& Ctx.TMA, sizeof(fb2s_Schema_t), 8);         // Slot 0 is for the header; create it as soon as both sets are initialized.
  Ctx.meta4hdr = Ctx.Meta.obj(& Ctx.Meta, sizeof(Meta_t), 8);
  Ctx.meta4hdr->tom = HEADER;

  Ctx.TUC.tidx = none;                                      // Clear all type under construction data.
  Ctx.TUC.midx = none;
  Ctx.TUC.aidx = none;
  Ctx.TUC.numMAttr = 0;
  Ctx.TUC.numTAttr = 0;
  Ctx.TUC.enumtype = NULL;

  fb2scan(& Ctx, schema, size);

//--
/*
  uint32_t i;
  token_t tok;
  token_t prev = NULL;


  tok = Ctx.Tokens.set[0];

  printf("Number of tokens %u, %u reallocations.\n", Ctx.Tokens.num, Ctx.Tokens.reallocs);


  for (i = 0; i < Ctx.Tokens.num; i++, prev = tok, tok = tok2next(tok)) {
    if (prev &&  prev->row < tok->row) printf("\n");
    if (tok->size) printf("%s (%u)", tok->text, tok->type);
    else printf("[%u:%u] ", tok->type, tok->cti);
//    printf("%p\n", tok);
  }
*/
//--

  builtins(& Ctx);                                          // Add the builtin types; implies Builtin[0] has meta index 1, next 2, ...
  
  Ctx.nexttoken = Ctx.Tokens.set[0];                        // Token to start the parsing with, is the first in the set.

//  fbpyydebug = 1; // Enable for debugging

  fb2_pyyparse(& Ctx);                                      // Now start the parsing.

  while ('_' == Ctx.nmspace[strlen(Ctx.nmspace) - 1]) {     // Strip any trailing underscores.
    Ctx.nmspace[strlen(Ctx.nmspace) - 1] = 0;
  }

  addkeyval(& Ctx, "I:namespace", Ctx.nmspace);             // Add final namespace key/value pair.

  Ctx.TMA.Grow.locked = 1;                                  // This set should now be fixed.

  fb2link(& Ctx);                                           // Create the binary schema by linking all together.

  if (ctx->schema) {
    ctx->schema->Num.canonical = 11;                        // Up to and including double.
  }

  memcpy(ctx->schema->magic, fb2sid, sizeof(ctx->schema->magic));

  printf("alignof(Token_t) %zu\n", alignof(Token_t));

}
