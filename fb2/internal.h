// Copyright (c) 2023 Steven Buytaert

#ifndef INTERNAL_H
#define INTERNAL_H

#include <snset.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdalign.h>

#include <fb2-schema.h>

#define NUM(A) (sizeof(A) / sizeof(A[0]))

static const uint32_t none = 0xffffffff;  // Set index equivalent for NULL.

static const char * rootkey = "I:root-type";

typedef struct fb2_Type_t      Type_t;    // Internal shorthands to public types.
typedef struct fb2_Type_t *    type_t;
typedef struct fb2_Member_t    Member_t;
typedef struct fb2_Member_t *  member_t;
typedef struct fb2_KeyVal_t    KeyVal_t;
typedef struct fb2_KeyVal_t *  keva_t;
typedef struct fb2_Tag_t       Tag_t;
typedef struct fb2_Tag_t *     tag_t;
typedef struct fb2_BSchema_t * schema_t;
typedef union  fb2_Value_t     Value_t;
typedef union  fb2_Any_t       Any_t;

typedef struct Ctx_t *         ctx_t;
typedef struct Token_t *       token_t;
typedef void *                 yyscan_t;
typedef struct Tok2Type_t *    tok2type_t;
typedef struct Meta_t *        meta_t;
typedef struct Tup_t *         tup_t;
typedef struct Hdr_t *         hdr_t;

typedef enum Const_t {            // Constant type identification.
  NONE            = 0,
  FLOAT           = 1,
  DEC             = 2,
  HEX             = 3,
  BIN             = 4,   
  FALSE           = 5,
  TRUE            = 6,
} Const_t;

typedef struct Token_t {          // We keep our own token stack
  const char *    line;           // Start of the line on which this toke was found.
  uint16_t        size;           // Size in bytes of the whole token.
  uint16_t        type;           // Type for parser; from tokens.h, generated by bison.
  uint16_t        row;
  uint16_t        col;
  uint8_t         cti;            // One of Const_t; implies CONST type.
  char            text[7];        // Text of the token; \0 terminated.
} Token_t;

typedef enum {
  NOTHING         = 0,
  TYPE            = 1,
  MEMBER          = 2,
  TAG             = 3,
  TYPE_ATTR       = 4,
  MEMBER_ATTR     = 5,
  KEYVAL          = 6,
  HEADER          = 7,
} MetaTag_t;

typedef struct Meta_t {           // Information on an entry in the TMA set. Each index in TMA has a Meta structure at the same index.
  uint8_t         tom;            // Type of meta; one of MetaTag_t, identifying the entry.
  uint8_t         isArray;        // Non zero when this is an array; implies MEMBER type.
  uint16_t        idx4cont;       // Container index, ie. Member.attr[idx4cont] or Type.members[idx4cont].
  uint32_t        attrtid;        // Type of the value of an attribute; implies type == TYPE_ATTR or MEMBER_ATTR.
  uint32_t        used;           // Number of times it is used (for types).
  uint32_t        index;          // Index in both sets.
  token_t         id;             // The identifier for this entry as a token.
  token_t         value;          // The value, if any; when isArray == 1, the fixed size of the array, otherwise the default member value.
  token_t         typetoken;      // The token identifying the type, if not NULL. For an enumeration, the type of the enumeration.
  uint32_t        container;      // For an attribute, the index of the member or type it belongs to; for a member, the enclosing type index.
  uint32_t        membertype;     // For a member, the type of the member.
  uint32_t        tag;            // The index for the string tag.
  uint8_t         pad[4];
} Meta_t;

typedef struct Hdr_t {            // This is the common header for types/members and attributes.
  union {
    const char *  name;
    int64_t       name_a;
  };
  uint16_t        o2n;            // Offset to next structure, in increments of 8 bytes.
  uint16_t        fb2ti;
  uint8_t         pad[4];
} Hdr_t;                          // We check with asserts that all structures conform.

typedef struct Tup_t {            // The combination of meta information and the type, member or attribute.
  meta_t          meta;
  union {
    type_t        type;
    member_t      member;
    keva_t        attr;
    tag_t         tag;
    Hdr_t *       hdr;
    void *        any;
  };
} Tup_t;

typedef struct Ctx_t {            // Context for lexing and parsing.
  fb2_schemaCtx_t fb2_ctx;
  SNSet_t         Tokens;
  meta_t          meta4hdr;       // Meta information for the header.
  token_t         nexttoken;      // Next token to process.
  token_t         token4bison;    // Current token being parsed.
  uint32_t        error;          // Should remain 0.
  uint32_t        tokenum;        // Number of current token; starts at 0.
  uint32_t        numtypes;       // Number of types found so far.
  uint8_t         pad[4];
  struct {
    uint32_t      tidx;           // Type index in TMA.set[] for the type under construction.
    uint32_t      midx;           // Member index in TMA.set[] for the current member being constructed.
    uint32_t      aidx;           // Index of first attribute for a member, if not 'none'.
    uint16_t      numMAttr;       // Number of attributes for the next member defined.
    uint16_t      numTAttr;       // Number of type attributes for the type being defined.
    token_t       enumtype;       // Type of the enumeration under construction, if not NULL.
  } TUC;                          // Type Under Construction.
  yyscan_t        scanner;        // Flex scanner (opaque).
  SNSet_t         TMA;            // Set with types and their corresponding members/attributes.
  SNSet_t         Meta;           // Set with information on TMA entries.
  char            nmspace[128];   // Current namespace.
  char            msg[128];       // In case of an error, contains a message.
} Ctx_t;

token_t  tok2next(token_t tok);
type_t   anf4type(ctx_t ctx, uint32_t ti, token_t name);
void     member(ctx_t ctx, token_t name, token_t type, uint32_t array, token_t val);
void     nselement(ctx_t ctx, token_t comp);
void     attr(ctx_t ctx, token_t name, token_t value);
void     enumtype(ctx_t ctx, token_t type);
void     keyval(ctx_t ctx, const char * key, token_t value);
void     end4type(ctx_t ctx);
void     flutscan(ctx_t ctx, const char * schema, uint32_t size);
int32_t  fb2_pyylex(token_t * tok, ctx_t ctx);
uint32_t fb2_pyyerror(ctx_t ctx, const char * msg);
uint32_t find(ctx_t ctx, token_t token, uint8_t type, tup_t tup); // find a type/member/attr by its token; when found, fill the tup.
void     idx2tup(ctx_t ctx, uint32_t idx, tup_t tup);
void     token2Value(ctx_t ctx, token_t token, Value_t * val);
void     error(ctx_t ctx, const char * fmt, ...);
void     fb2link(ctx_t ctx);

#endif // INTERNAL_H
 