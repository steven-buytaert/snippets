// Copyright 2023 Steven Buytaert

#ifndef FB2_TYPES_H
#define FB2_TYPES_H

#include <stdint.h>

typedef enum {
  ct_none            =  0,
  ct_int8            =  1,
  ct_uint8           =  2,
  ct_int16           =  3,
  ct_uint16          =  4,
  ct_int32           =  5,
  ct_uint32          =  6,
  ct_int64           =  7,
  ct_uint64          =  8,
  ct_float           =  9,
  ct_double          = 10,
  ct_string          = 11,
  
  ct_neg_int8        = 12,        // Following imply the number is negative.
  ct_neg_int16       = 13,
  ct_neg_int32       = 14,
  ct_neg_int64       = 15,
} fb2_ConstType_t;

typedef struct fb2_Tag_t * fb2_tag_t;

typedef struct fb2_Value_t {      // Member or KeyVal value.
  uint8_t            type;        // One of the fb2_ConstType_t values.
  uint8_t            minus;       // When nonzero, the number token was preceded by a minus sign.
  uint8_t            signd;       // Non zero when the type is signed.
  uint8_t            pad[5];
  union {
    int8_t           i08;
    uint8_t          u08;
    int16_t          i16;
    uint16_t         u16;
    int32_t          i32;
    uint32_t         u32;
    int64_t          i64;
    uint64_t         u64;
    float            f32;
    double           f64;
    fb2_tag_t        tag;         // Type only used by schema parser code.
    void const *     ref;
  };
} fb2_Value_t;

typedef enum {                    // Type table type properties.
  fb2_NONE           =    0,      // Lower 4 bits are type.
  fb2_PRIM           =    1,
  fb2_STRUCT         =    2,
  fb2_TABLE          =    3,
  fb2_ENUM           =    4,
  fb2_UNION          =    5,
  fb2_VECTOR         =    6,
  fb2_STRING         =    7,
  fb2_MASK           = 0x0f,      // To mask out the type bits; upper 4 bits are flags.

  fb2_SIGNED         = 0x10,      // Primitive type is signed; implies fb2_PRIM.
  fb2_flag_2         = 0x20,
  fb2_flag_4         = 0x40,
  fb2_flag_8         = 0x80,
} fb2b_Props_t;

typedef const struct Type_t {     // The Types table entry structure; stored in the Ctx.Types set.
  uint16_t           nti;         // Name table index (can be 0).
  uint16_t           size;        // Size in bytes for a full element.
  uint16_t           align;       // Alignment
  uint8_t            props;       // See STProps_t.
  uint8_t            fi;          // Format index.
  uint16_t           vetid;       // When a vector, the index of the vector type element in Types; for an enum the enum type.
  uint16_t           cti;         // When not 0, the component table index.
} Type_t;

typedef struct Member_t {         // A member of a compound type.
  uint16_t           nti;         // Name table index.
  uint16_t           tid;         // Type of the member as an Index in Types[].
  uint32_t           ctoff;       // Constant table offset.
} Member_t;

typedef const struct Comp_t {     // Compound type description.
  uint16_t           tid;         // Type id of this component as index in Types[].
  uint16_t           num;         // Number of members in this component.
  uint16_t           svtid;       // Only non zero for structs; index in svtabs[].
  uint8_t            props;       // See STProps_t.
  uint8_t            pad[1];
  Member_t           Members[0];
} Comp_t;

typedef const struct fb2_VTab_t { // Generic VTable.
  struct {
    uint16_t         vtab;        // Size of this vtable, in bytes.
    uint16_t         table;       // Size of the compound pointing to this vtable, in bytes.
  } Size;
  int16_t            offsets[0];  // Signed offsets where the member data can be found in the compound.
} fb2_VTab_t;

typedef const struct fb2_Vec_t {  // Vector for a string or an array of element offsets.
  uint32_t           num;         // Element count, for string, not including the \0 terminator.
  union {
    char             chars[0];    // The string is 0 terminated.
    uint32_t         offsets[0];  // When a reference to another table element.
  };
} fb2_Vec_t;

typedef union fb2_AU_t {          // Flatbuffer Address Union.
  uint8_t    const * u08r;
  uint16_t   const * u16r;
  uint32_t   const * u32r;
  int32_t    const * i32r;
  uint64_t   const * u64r;
  fb2_VTab_t const * vtab;
  fb2_Vec_t  const * vec;
  void       const * any;
  uint32_t           off;         // To use as offset in the flatbuffer.
  intptr_t           addr;        // To calculate an absolute address.
} fb2_AU_t;

typedef struct fb2_BCtx_t * fb2_bctx_t;

typedef void * (* fb2_alloc_t)(fb2_bctx_t fb2ctx, uint16_t cti, void * mem, uint32_t size);

typedef struct fb2_BCtx_t {       // Binary context; this is the info to read/write flatbuffer binaries.
  const struct {
    uint32_t         constsize;   // Size of the constant table.
    uint16_t         roottid;     // Type index of the root element.
    uint16_t         numtypes;
    uint16_t         numcomps;
    uint16_t         namessize;
    uint16_t         stringtid;   // Type index of the string type.
    char             ID[5];       // Identifier as found in the schema, \0 terminated.
    uint8_t          pad[5];
    Type_t *         Types;       // The table with the types.
    Comp_t **        Comps;       // The table with the compound types.
    const char *     Names;       // Table with type/member names.
    const uint8_t *  Consts;      // Table with constants.
    fb2_VTab_t **    svtabs;      // Structure vtabs.
  } Codec;
  uint8_t *          base;        // Start of the flatbuffer binary data.
  void *             custom;      // Freely available to the caller.
  fb2_alloc_t        alloc;       // Memory allocator; realloc semantics; can pass 0 for the cti; means raw memory.
  uint32_t           root;        // Unsigned offset to the root element.
  uint8_t            pad[4];
} fb2_BCtx_t;

typedef struct fb2_SSE_t {        // Scan Stack Element.
  fb2_AU_t           Addr;        // Absolute memory address in the flatbuffer.
  uint16_t           nti;         // Name table index with name of the element.
  uint8_t            props;       // One of fb2b_Props_t enum.
  uint8_t            shim;        // When non zero, it is an array element or union shim.
  union {
    struct {
      uint16_t       tid;         // Compound type of the element.
      uint16_t       mi;          // Current member index.
    } Comp;                       // Compound.
    uint32_t         index;       // Array index
    uint32_t         utype;       // Union type.
  };
} fb2_SSE_t;

typedef struct fb2_Stack_t * fb2_stack_t;

typedef uint32_t (* fb2_StCb_t)(fb2_stack_t stack, uint32_t check);

typedef struct fb2_Stack_t {      // Reading stack.
  fb2_StCb_t         Cb;          // User callback.
  fb2_bctx_t         ctx;
  uint32_t           top;         // Points at unused top.
  uint32_t           cap;         // Total stack capacity.
  fb2_SSE_t          SSE[0];
} fb2_Stack_t;

#endif // FB2_TYPES_H
