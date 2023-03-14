#include <stdio.h>//MAGWEG
#include <errno.h>

#include <internal.h>

void token2Value(ctx_t ctx, token_t token, Value_t * val) { // Convert a token to a value.

  const char * number = NULL;
  int32_t      base = 0;

printf("CTI %u\n", token->cti);

  switch (token->cti) {                                     // Find what type of value we need, create the proper search token and search.
    case FLOAT: {
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
printf("VALUE NUMBER [%s]\n", number);
      break;
    }
        
    case TRUE:
    case FALSE: {
      val->i08 = token->cti == FALSE ? 0 : 1;
      break;
    }
      
    default: {
      error(ctx, "Unknown CTI %u.", token->cti);
    }
  }
    
  if (number) {                                             // Do we need to convert a number?
    val->i64 = strtol(number, NULL, base);
    if (errno) {
      error(ctx, "strtol(%s) failed: %s.", token->text, strerror(errno));
    }
  }

}

static void attr2member(ctx_t ctx, meta_t meta) {           // Link the attribute into the proper member slot.

  member_t member = ctx->TMA.set[meta->container];
  attr_t   attr   = ctx->TMA.set[meta->index];

  assert(member->kind == KoT_MEMBER);
  assert(attr->kind == KoT_ATTR);
  assert(meta->idx4cont < member->numattr);

  member->attr[meta->idx4cont] = attr;

}

static uint32_t isaTypeKind(uint8_t kind) {                 // Return 1 when the kind is a type.
  return (kind & KoT_TYPE) ? 1 : 0;
}

static void attr2type(ctx_t ctx, meta_t meta) {             // Link the attribute into the proper type slot.

  type_t   type = ctx->TMA.set[meta->container];
  attr_t   attr = ctx->TMA.set[meta->index];
  attr_t * ta = (attr_t *) & type->members[type->nummem];   // Type attribute array just behind the member slots.

  assert(isaTypeKind(type->kind));
  assert(attr->kind == KoT_ATTR);
  assert(meta->idx4cont < type->numattr);

  ta[meta->idx4cont] = attr;

}

static uint32_t isaCompound(uint8_t kind) {                 // Return 1 if a compound type.

  if ((kind & KoT_STRUCT) == KoT_STRUCT) return 1;
  if ((kind & KoT_ENUM)   == KoT_ENUM)   return 1;
  if ((kind & KoT_UNION)  == KoT_UNION)  return 1;
  if ((kind & KoT_TABLE)  == KoT_TABLE)  return 1;

  return 0;

}

static void member2type(ctx_t ctx, meta_t meta) {           // Link the member with the proper type.

  Tup_t    ToM;                                             // Type of the member.
  Tup_t    Con;                                             // Type that contains the member.
  tag_t    tag = ctx->TMA.set[meta->tag];
  member_t member = ctx->TMA.set[meta->index];

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
    if (Con.type->kind == KoT_ENUM) {                       // An enum, the type is the type of the enum itself.
      assert(Con.meta->typetoken);                          // So it must have a typetoken.
      if (find(ctx, Con.meta->typetoken, TYPE, & ToM)) {
        meta->membertype = ToM.meta->index;
        ToM.meta->used++;
      }
      else {
        error(ctx, "Type of enum '%s' '%s' not found.", Con.meta->id->text, Con.meta->typetoken->text);
      }
    }
    else if(Con.type->kind == KoT_UNION) {                  // A union, the member name itself *is* a type.
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

  // printf("MEMBER [%s] kind %x\n", meta->id->text, ToM.type->kind); fflush(stdout);

  assert(isaTypeKind(ToM.type->kind));
  assert(isaCompound(Con.type->kind));                      // Members can only be part of a compound type.
  
  member->type = ToM.type;                                  // We can now give the member the proper type ...
  Con.type->members[meta->idx4cont] = member;               // ... and assign it to the proper slot in the containing type.
  
}

uint32_t fb2_isPrimitive(void * any) {

  type_t e = any;

  return (e && KoT_PRIM == (e->kind & KoT_TYPE));

}

uint32_t fb2_isStruct(void * any) {

  type_t e = any;

  return (e && KoT_STRUCT == (e->kind & KoT_TYPE));

}

uint32_t fb2_isEnum(void * any) {

  type_t e = any;

  return (e && KoT_ENUM == (e->kind & KoT_TYPE));

}

uint32_t fb2_isUnion(void * any) {

  type_t e = any;

  return (e && KoT_UNION == (e->kind & KoT_TYPE));

}

uint32_t fb2_isTable(void * any) {

  type_t e = any;

  return (e && KoT_TABLE == (e->kind & KoT_TYPE));

}

fb2_Any_t * fb2_go2next(void * cur) {                       // Go to the next element; return NULL when no next element.

  Hdr_t *     here = cur;
  fb2_Any_t * next;
  
  assert((here->kind & 0b0000000011000000) == 0);           // Small check.

  if (0 == here->o2n) { return NULL; }                      // No more next element.

  next = (fb2_Any_t *) ((char *) here + (8 * here->o2n));
  
  assert((next->Type.kind & 0b0000000011000000) == 0);
  
  return next;
  
}

static uint32_t couldBeRoot(ctx_t ctx, uint16_t kind) {     // Return true when the kind could be a root type.

  if ((kind & KoT_TABLE) == KoT_TABLE) return 1;

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
  attr_t    attr;
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

  ctx->fb2_ctx->schema = schema;

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
      member2type(ctx, meta);
    }
    else if (meta->tom == TAG) {                            // Assign the tag string field to the chars array.
      tag = tma->set[i];
      assert(KoT_TAG == tag->kind);
      tag->string = tag->chars;
      if (i == ctx->meta4hdr->tag) {                        // If this is the tag for the root type, save it.
        Root.name = tag->string;
      }
    }
    else if (meta->tom == TYPE) {                           // Assign the type its proper name tag.
      tag = tma->set[meta->tag];
      assert(KoT_TAG == tag->kind);
      type = tma->set[meta->index];
      assert(isaTypeKind(type->kind));
      type->name = tag->chars;
      schema->Num.types++;
      if (schema->Num.maxmem < type->nummem) {              // Keep track of the maximums.
        schema->Num.maxmem = type->nummem;
      }
      numtail = type->nummem + type->numattr;               // Total number of tail slots for this type.
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
      attr->name = key->chars;
      attr->Value.tag = tag;
      printf("KEYVAL %s:%s\n", attr->name, attr->Value.tag->chars);
    }
    else if (meta->tom == HEADER) {
      // Do nothing.
    }
    else {
      error(ctx, "Unknown meta type %u\n", meta->tom);
    }
  }

  for (i = 1; i < tma->num; i++) {                          // Set the proper offset to next for all elements in the TMA set. Skip the header.
    cur = tma->set[i];
    assert(0 == ((uintptr_t) cur & 0b111));                 // Must be 8 byte aligned since we set wca to 8 in the TMA set at the start.
    if (couldBeRoot(ctx, cur->kind)) {
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
//    printf("Cur %p type %u\n", cur, cur->kind);
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

