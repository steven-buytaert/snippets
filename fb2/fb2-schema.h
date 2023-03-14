// Copyright (c) 2023 Steven Buytaert

#ifndef FB2_SCHEMA_H
#define FB2_SCHEMA_H

#include <stdint.h>

typedef struct fb2_Ctx_t *    fb2_ctx_t;
typedef struct fb2_Member_t * fb2_member_t;
typedef struct fb2_Type_t *   fb2_type_t;
typedef struct fb2_Attr_t *   fb2_attr_t;
typedef struct fb2_Tag_t *    fb2_tag_t;

typedef union Value_t {           // Member or attribute value.
  int8_t         i08;
  int16_t        i16;
  int32_t        i32;
  float          f32;
  int64_t        i64;
  double         f64;
  fb2_tag_t      tag;
} Value_t;

typedef enum KindOfType_t {
  KoT_PRIM       = 0b00000001,
  KoT_STRUCT     = 0b00000010,
  KoT_ENUM       = 0b00000011,
  KoT_UNION      = 0b00000100,
  KoT_TABLE      = 0b00000101,
  KoT_TYPE       = 0b00000111,

  KoT_MEMBER     = 0b00001000,
  KoT_ATTR       = 0b00010000,
  KoT_TAG        = 0b00011000,
  KoT_KEYVAL     = 0b00100000,
} KindOfType_t;

typedef struct fb2_Tag_t {
  union {
    const char * string;          // Points to the chars array.
    int64_t      string_a;
  };
  uint16_t       o2n;
  uint16_t       kind;
  uint16_t       size;            // Size in bytes of this tag, including extra chars.
  char           chars[2];        // A null terminated string.
} fb2_Tag_t;

typedef struct fb2_Attr_t {
  union {
    const char * name;            // Attribute name (refers to a tag->string field).
    int64_t      name_a;
  };
  uint16_t       o2n;             // Offset to next structure, in increments of 8 bytes.
  uint16_t       kind;
  uint8_t        pad[4];
  Value_t        Value;           // Value, if any.
} fb2_Attr_t;

typedef struct fb2_Member_t {
  union {
    const char * name;            // Member name (refers to a tag->string field).
    int64_t      name_a;
  };
  uint16_t       o2n;             // Offset to next structure, in increments of 8 bytes.
  uint16_t       kind;
  uint8_t        numattr;         // Number of attributes.
  uint8_t        pad[1];
  uint8_t        isString;        // Non zero for a string member.
  uint8_t        isArray;         // Non zero when an array.
  union {
    fb2_type_t   type;            // The type of the member; when isArray, the type of the element.
    int64_t      type_a;
  };
  Value_t        Default;         // Default value.
  union {
    fb2_attr_t   attr[0];
    int64_t      attr_a[0];
  };
} fb2_Member_t;

typedef struct fb2_Type_t {
  union {
    const char * name;            // Name of the type (refers to a tag->string field).
    int64_t      name_a;
  };
  uint16_t       o2n;             // Offset to next structure, in increments of 8 bytes.
  uint16_t       kind;            // One of KindOfType_t.
  uint8_t        numattr;         // Number of type attributes in attrs[] array.
  uint8_t        canontype;       // For a primitive type, the type number of the canonical name.
  uint16_t       nummem;          // Number of members in the members[] array, after the attributes.
  uint8_t        pad[8];
  union {
    fb2_member_t members[0];      // members[nummem] that make up this type.
    int64_t      addresses[0];
    fb2_attr_t   attrs[0];        // attrs[numattr] type attributes.
  };
} fb2_Type_t;

typedef union fb2_Any_t {
  fb2_Type_t     Type;
  fb2_Member_t   Member;
  fb2_Attr_t     Attr;
  fb2_Tag_t      Tag;
} fb2_Any_t;

typedef struct fb2_BSchema_t {    // Binary schema format header.
  uint8_t        magic[7];        // File id 0x34651451225117.
  uint8_t        version;         // Major and minor, each 4 bits.
  union {
    uint8_t *    base;            // When NULL, it is freezedried.
    int64_t      base_a;
  };
  uint32_t       size;            // Size of the data + this header.
  struct {
    uint32_t     elements;        // Total number of elements.
    uint32_t     types;           // Number of types.
    uint32_t     maxmem;          // Largest number of members in a single type.
    uint16_t     maxtail;         // Largest number of members + attributes in a single type.
    uint8_t      canonical;       // Number of canonical types. They are stored first.
    uint8_t      pad[1];
  } Num;
  uint32_t       root;            // Root starts at bytes[root]. 0 means not specified.
  union {
    uint64_t     aligner[0];      // For ensuring worst case alignment.
    uint8_t      bytes[0];
    fb2_Any_t    Elements[0];     // Linked list of noe elements; use fb2_go2next() to move to the next.
  };
} fb2_BSchema_t;

typedef void * (* fb2_mem_t)(fb2_ctx_t ctx, void * mem, uint32_t sz);
typedef void   (* fb2_out_t)(fb2_ctx_t ctx, char buf[], uint32_t sz);

typedef struct fb2_Ctx_t {
  fb2_mem_t       mem;            // Memory allocation and release.
  fb2_out_t       out;            // Output a line of code.
  fb2_BSchema_t * schema;         // After parsing, the schema in binary form.
} fb2_Ctx_t;

fb2_Any_t * fb2_go2next(void * cur);
uint32_t    fb2_isPrimitive(void * e);
uint32_t    fb2_isStruct(void * e);
uint32_t    fb2_isEnum(void * e);
uint32_t    fb2_isUnion(void * e);
uint32_t    fb2_isTable(void * e);
void        fb2_parse(fb2_ctx_t ctx, const char * schema, uint32_t size);

#endif // FB2_SCHEMA_H
