// Copyright 2023 Steven Buytaert

#ifndef FB2_TYPES_H
#define FB2_TYPES_H

#include <stdint.h>

typedef enum {
  ct_none      =  0,
  ct_int8      =  1,
  ct_uint8     =  2,
  ct_int16     =  3,
  ct_uint16    =  4,
  ct_int32     =  5,
  ct_uint32    =  6,
  ct_int64     =  7,
  ct_uint64    =  8,
  ct_float     =  9,
  ct_double    = 10,
  ct_string    = 11,
} fb2_ConstType_t;

typedef struct fb2_Tag_t * fb2_tag_t;

typedef struct fb2_Value_t {      // Member or KeyVal value.
  uint8_t      type;              // One of the fb2_ConstType_t values.
  uint8_t      pad[7];
  union {
    int8_t     i08;
    uint8_t    u08;
    int16_t    i16;
    uint16_t   u16;
    int32_t    i32;
    uint32_t   u32;
    int64_t    i64;
    uint64_t   u64;
    float      f32;
    double     f64;
    fb2_tag_t  tag;               // Type only used by schema parser code.
    void *     ref;
  };
} fb2_Value_t;

typedef enum {                    // Type table type properties.
  fb2_NONE     =    0,            // Lower 3 bits are type.
  fb2_PRIM     =    1,
  fb2_STRUCT   =    2,
  fb2_TABLE    =    3,
  fb2_ENUM     =    4,
  fb2_UNION    =    5,
  fb2_VECTOR   =    6,            // Look at subtid for this type.
  unused_1     =    7,

  fb2_SIGNED   = 0x08,            // Upper 5 bits flags.
  unused_5     = 0x10,
  unused_2     = 0x20,
  unused_3     = 0x40,
  unused_4     = 0x80,
} fb2b_Props_t;

typedef struct Type_t {           // The Types table entry structure; stored in the Ctx.Types set.
  uint16_t     nti;               // Name table index (can be 0).
  uint16_t     size;              // Size in bytes for a full element.
  uint16_t     align;             // Alignment
  uint8_t      props;             // See STProps_t.
  uint8_t      fi;                // Format index.
  uint16_t     vetid;             // When a vector, the index of the vector type element in Types; for an enum the enum type.
  uint16_t     cti;               // When not 0, the component table index.
} Type_t;

typedef struct Member_t {         // A member of a compound type.
  uint16_t     nti;               // Name table index.
  uint16_t     tid;               // Type of the member as an Index in Types[].
  uint32_t     devi;              // Default value index.
} Member_t;

typedef struct Comp_t {           // Compound description.
  uint16_t     tid;               // Type id of this component as index in Types[].
  uint16_t     num;               // Number of members in this component.
  uint16_t     svtid;             // Only non zero for structs; index in svtabs[].
  uint8_t      props;             // See STProps_t.
  uint8_t      pad[1];
  Member_t     Members[0];
} Comp_t;

typedef const struct VTab_t {     // Generic VTable.
  struct {
    uint16_t   vtab;              // Size of this vtable, in bytes.
    uint16_t   table;             // Size of the table pointing to this vtable, in bytes.
  } Size;
  int16_t      offs[0];           // Offsets where the member data can be found in the table.
} VTab_t;

#endif // FB2_TYPES_H
