// Copyright (c) 2023 Steven Buytaert

#ifndef FB2_SCHEMA_H
#define FB2_SCHEMA_H

#include <fb2-types.h>

typedef struct fb2_SchemaCtx_t * fb2_schemaCtx_t;
typedef struct fb2_Member_t *    fb2_member_t;
typedef struct fb2_Type_t *      fb2_type_t;
typedef struct fb2_KeyVal_t *    fb2_keyval_t;
typedef struct fb2_Tag_t *       fb2_tag_t;

typedef enum {
  fb2e_None      =       0,       // Should never occur.
  fb2e_Prim      =       1,
  fb2e_Table     =       2,
  fb2e_Union     =       3,
  fb2e_Enum      =       4,
  fb2e_Struct    =       5,

  fb2e_Member    =       6,
  fb2e_KeyVal    =       7,
  fb2e_Attr      =       8,       // Is also a KeyVal_t type; attribute name is the key.
  fb2e_Tag       =       9,
  fb2e_Vector    =  0x0010,       // Vector bit (not used in the parser code).
  fb2e_Mask      =  0x001f,       // To mask out the type and vector bits from the signature.
  fb2e_Signature =  0xa520,       // Signature to check.
} fb2e_TypeIndex_t;

typedef struct fb2_Tag_t {
  union {
    const char * string;          // Points to the chars array.
    int64_t      string_a;
  };
  uint16_t       o2n;
  uint16_t       fb2ti;           // fb2e_Tag.

  uint16_t       size;            // Size in bytes of this tag, including extra chars.
  char           chars[2];        // A null terminated string.
} fb2_Tag_t;

typedef struct fb2_KeyVal_t {
  union {
    const char * key;             // Key name (refers to a tag->string field).
    int64_t      key_a;
  };
  uint16_t       o2n;             // Offset to next structure, in increments of 8 bytes.
  uint16_t       fb2ti;            // fb2e_KeyVal or fb2e_Attr.

  uint8_t        pad[4];
  fb2_Value_t    Value;           // Value, if any.
} fb2_KeyVal_t;

typedef struct fb2_Member_t {
  union {
    const char * name;            // Member name (refers to a tag->string field).
    int64_t      name_a;
  };
  uint16_t       o2n;             // Offset to next structure, in increments of 8 bytes.
  uint16_t       fb2ti;           // fb2e_Member.
  uint16_t       nti;             // Name table index.
  uint16_t       tti;             // Type table index.
  uint16_t       ctoff;           // Constant table offset (if not zero); for enum value or default value.

  uint8_t        numattr;         // Number of attributes.
  uint8_t        isString;        // Non zero for a string member.
  uint8_t        isArray;         // Non zero when an array.
  uint8_t        pad[3];
  union {
    fb2_type_t   type;            // The type of the member; when isArray, the type of the element.
    int64_t      type_a;
  };
  fb2_Value_t    Default;         // Default value.
  union {
    fb2_keyval_t attr[0];
    int64_t      attr_a[0];
  };
} fb2_Member_t;

typedef struct fb2_Type_t {
  union {
    const char * name;            // Name of the type (refers to a tag->string field).
    int64_t      name_a;
  };
  uint16_t       o2n;             // Offset to next structure, in increments of 8 bytes.
  uint16_t       fb2ti;           // fb2e_Table, fb2e_Union, fb2e_Enum or fb2e_Struct.
  uint16_t       nti;             // Name table index (set and used by code generator only).
  uint16_t       tti;             // Type table index (set and used by code generator only).
  uint16_t       vtti;            // VTab table index (set and used by code generator only).

  uint8_t        numattr;         // Number of type attributes in attrs[] array.
  uint8_t        canontype;       // For a primitive type, the type number of the canonical name.
  uint16_t       nummem;          // Number of members in the members[] array, after the attributes.
  uint8_t        fbtype;          // For the primitives, the index to the flatbuffer name, e.g. byte, ubyte, short, ...
  uint8_t        pad[1];
  union{ 
    fb2_type_t   type4enum;       // When an enum, the primitive type of the enum.
    int64_t      type4enum_a;
  };
  union {
    fb2_member_t members[0];      // members[nummem] that make up this type.
    int64_t      addresses[0];
    fb2_keyval_t attrs[0];        // attrs[numattr] type attributes.
  };
} fb2_Type_t;

typedef union fb2_Any_t {
  fb2_Type_t     Type;
  fb2_Member_t   Member;
  fb2_KeyVal_t   KeyVal;
  fb2_Tag_t      Tag;
} fb2_Any_t;

extern const uint8_t fb2sid[7];   // The schema id.

typedef struct fb2_BSchema_t {    // Binary schema format header.
  uint8_t        magic[7];        // File id 0x34651451225117 (see fb2sid).
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

typedef void * (* fb2_mem_t)(fb2_schemaCtx_t ctx, void * mem, uint32_t sz);

typedef struct fb2_SchemaCtx_t {
  fb2_mem_t       mem;            // Memory allocation and release.
  fb2_BSchema_t * schema;         // After parsing, the schema in binary form.
  uint32_t        cookie;         // Internal use (leave blank).
  uint8_t         pad[4];
} fb2_SchemaCtx_t;

fb2_Any_t * fb2_go2next(void const * cur);
uint32_t    fb2_ti(void const * e);
uint32_t    fb2_isPrimitive(void const * e);
uint32_t    fb2_isStruct(void const * e);
uint32_t    fb2_isEnum(void const * e);
uint32_t    fb2_isUnion(void const * e);
uint32_t    fb2_isTable(void const * e);
void        fb2_parse(fb2_schemaCtx_t ctx, const char * schema, uint32_t size);

#endif // FB2_SCHEMA_H
