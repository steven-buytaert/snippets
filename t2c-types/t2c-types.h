#ifndef T2C_TYPES_H
#define T2C_TYPES_H

// Copyright 2023-2024 Steven Buytaert

#include <stdint.h>

typedef struct t2c_Type_t *   t2c_type_t;
typedef struct t2c_Member_t * t2c_member_t;

typedef enum {
  t2c_Prim     = 0b000000000001,  // Primitive type.
  t2c_Comp     = 0b000000000010,  // Composite type.
  t2c_Bitset   = 0b000000000110,  // A primitive type but chopped up in bitfields.
  t2c_Struct   = 0b000000001010,
  t2c_Union    = 0b000000010010,
  t2c_Enum     = 0b000000100010,
  t2c_Typedef  = 0b000001000010,
  t2c_Signed   = 0b000010000000,  // Implies t2c_Prim, signed type.
  t2c_Static   = 0b000100000000,  // Type is statically allocated; don't free.
  t2c_Packed   = 0b001000000000,  // On a struct, packs the member fields together.
} t2c_Prop_t;

typedef union t2c_Cargo_t {       // User defined cargo. Is copied and moved with other member/type parts.
  uint32_t           cookie;
  uint8_t            u08[8];
  uint16_t           u16[4];
  uint32_t           u32[2];
  uint64_t           u64[1];      // Also to ensure proper alignment.
  void *             refs[2];     // To store references to associated data.
} t2c_Cargo_t;

typedef struct t2c_Ctx_t * t2c_ctx_t;

typedef void * (* t2c_mem_t)(t2c_ctx_t ctx, void * mem, uint32_t size);

typedef enum {                    // Diagnostics that can be assigned to the context error member.
  t2c_OK             =  0,
  t2c_Circularity    =  1,        // A type is being defined including itself as a full member (not indirected).
  t2c_LoneRef2size   =  2,        // A member is marked as having a ref2size member, but is not a VTail or has indirection.
  t2c_VTailNotFinal  =  3,        // A member is marked as VTail but it is not the final member.
  t2c_RefAndVTail    =  4,        // A member is marked both VTail and with indirection; they are mutually exclusive.
  t2c_ZeroWidth      =  5,        // A bitset member has an invalid width of 0.
  t2c_TooSmall       =  6,        // The bitset primitive is not wide enough for the given members.
  t2c_BadBaseType    =  7,        // The bitset/enum has no ccType.boetype or it is not a primitive.
  t2c_NoCapLeft      =  8,        // There's no capacity left.
  t2c_AllocFailed    =  9,        // Allocation failed.
  t2c_BadProperties  = 10,        // Properties that are incompatible, e.g. Comp and Prim bit are set.
  t2c_Duplicate      = 11,        // For an enumeration; at least 2 members have identical values.
  t2c_BadMemberType  = 12,        // Member has a bad type (or no type set).
} t2c_DiagInfo_t;

extern const uint32_t t2ccookie;  // Value for the context cookie.

typedef struct t2c_Ctx_t {
  t2c_Cargo_t        Cargo;       // User defined cargo.
  const uint32_t     cookie;      // Set by t2c_initype; should always have t2ccookie value.
  uint8_t            pad[4];
  const char *       typeExt;     // Extension given to types.
  t2c_mem_t          mem;         // To (re)allocate/free memory; realloc semantics.
  uint8_t            size4ref;    // Size of a reference or pointer.
  uint8_t            align4ref;   // Alignment requirement for a pointer or reference.
  uint8_t            defnamesz;   // Default size for a name, including \0; is the minimum namesz.
  uint8_t            xtranamesz;  // Number of chars to add to strlen(name) of any name; default is 0.
  uint8_t            cmpNames;    // When non zero, also compare member names in t2c_typecmp() for equality.
  uint8_t            error;       // Should remain 0 (t2c_OK).
  uint16_t           cap;         // Capacity at the tail.
  uint16_t           num;         // Number of slots used at the tail, including NULL slots.
  char               msg[134];    // Diagnostic message.
  t2c_type_t         types[0];    // Note that this can contain NULL slots when types are removed.
} t2c_Ctx_t;

typedef struct t2c_Member_t {     // Member of a composite type.
  t2c_Cargo_t        Cargo;       // User defined cargo; is copied when relevant (e.g. moving to cluster).
  char *             name;        // Name of the member; for a bitset member, the name can be "" but never NULL.
  t2c_type_t         type;        // Member type; for a bitset/enum member, all types must refer to the same unsigned primitive.
  t2c_member_t       ref2size;    // Field that determines number of elements (implies isVTail or numind > 0).
  t2c_type_t         parent;      // When NOT NULL, the embedded struct type this member is part of.
  uint32_t           offset;      // Offset byte/bit address in the composite/bitset.
  union {
    uint32_t         fxdsize;     // For struct/union members: when not 0, the fixed array size. fxdsize and numind are mutually exclusive.
    uint32_t         enumval;     // For enumerations, the member value.
  };
  uint8_t            numind;      // Number of indirections, 0 = plain, 1 = *, 2 = **, ...
  uint8_t            isVTail;     // When not zero, variable length array tail.
  uint8_t            isConst;     // Member is constant.
  uint8_t            expand;      // When non zero, expand the compound in the enclosing parent type.
  uint8_t            width;       // Only for a bitset member; width in bits. Offset determines right bit offset.
  uint8_t            isForward;   // This member refers to a composite type, defined later (can't use the typedef'ed type yet).
  uint8_t            isRef2Self;  // This member refers to itself as type, e.g. a linked list; implies isForward.
  uint8_t            anon;        // Member is a composite and has no name.
  uint8_t            namesz;      // When name is a char buf[], the size of the buffer; 0 when unknown.
  uint8_t            pad[7];
} t2c_Member_t;

typedef struct t2c_Type_t {
  t2c_Cargo_t        Cargo;       // User defined cargo.
  t2c_type_t         ref2type;    // Typedef reference to this type, when not NULL. Not copied during a clone. Assigned by t2c_tdref4type.
  char *             name;        // Name of the type; this buffer is ccCtx.defnamesz characters in size.
  t2c_ctx_t          ctx;         // Context that was attached to this type.
  union {
    t2c_type_t       boetype;     // When a bitset or enumeration, the underlying primitive type.
    t2c_type_t       container;   // When a cluster, the type that contains us.
  };
  t2c_Prop_t         prop;        // Type properties.
  uint16_t           size;        // Size in bytes.
  uint16_t           align;       // Alignment requirement.
  uint16_t           num;         // Members in use at tail.
  uint16_t           numslots;    // Number of memory slots.
  uint16_t           numtags;     // Total number of tags/fields.
  uint16_t           rem;         // Free slots remaining at the tail.
  uint16_t           weight;      // Number representing usage; the higher, the earlier the type must be declared.
  uint16_t           ti;          // Type index in Ctx.types; only set after prep4gen.
  uint8_t            namesz;      // Size of the name, when a char buf[]; 0 when not known.
  uint8_t            mark4scan;   // Available for the user; can all be cleared with t2c_clearmark4scan.
  uint8_t            mark4use;    // Will be set by t2c_mark4use() traversal.
  uint8_t            pad[1];
  uint16_t           marks[4];    // Marks the user can use freely.
  const t2c_Member_t Marker;      // Marker to find back the container type for a member (name is NULL, type is container).
  t2c_Member_t       Members[0];  // Type members; capacity = num + rem.
} t2c_Type_t;

typedef struct t2c_MapUnit_t {    // Information for mapping a type.
  t2c_type_t         type;
  t2c_member_t       member;
  uint16_t           size;
  uint16_t           offset;
  uint8_t            isprim;      // Non zero when member is a primitive.
  uint8_t            align;       // Alignment requirement.
  uint8_t            indent;      // Indentation level.
  uint8_t            padding;     // Padding in front of the slot.
} t2c_MapUnit_t;

typedef struct t2c_OffMap_t {     // Offset map information for a type.
  t2c_type_t         type;        // Type to analyze for offsets/alignment/padding/...
  uint8_t            overflow;    // When non zero, the capacity was exhausted.
  uint8_t            mark;        // Available to the user.
  uint16_t           user;        // Available to the user.
  uint16_t           num;         // Number of units used.
  uint16_t           cap;         // Capacity at the tail.
  t2c_MapUnit_t      Unit[0];     // Unit[cap];
} t2c_OffMap_t;

typedef struct t2c_XRef_t {       // Cross reference result (searching for type used or member path).
  t2c_type_t         type;        // Type to start search from/for or to delete.
  const char *       name;        // Member name searched for, if any (otherwise NULL).
  uint16_t           cap;         // Number of members at the tail.
  uint16_t           num;         // Number of slots used at the tail.
  uint8_t            pad[4];
  t2c_member_t       used[0];
} t2c_XRef_t;

extern t2c_Type_t t2c_Char;
extern t2c_Type_t t2c_U64;
extern t2c_Type_t t2c_U32;
extern t2c_Type_t t2c_U16;
extern t2c_Type_t t2c_U08;
extern t2c_Type_t t2c_S64;
extern t2c_Type_t t2c_S32;
extern t2c_Type_t t2c_S16;
extern t2c_Type_t t2c_S08;
extern t2c_Type_t t2c_F32;
extern t2c_Type_t t2c_F64;

extern const t2c_type_t t2c_prims[11];

extern t2c_Type_t t2c_VoidRef;    // A 'void *' type, for e.g. stubbing.

typedef void (* t2c_cb4t_t)(t2c_ctx_t ctx, t2c_type_t   t, void * arg);
typedef void (* t2c_cb4m_t)(t2c_ctx_t ctx, t2c_member_t m, void * arg);

void       t2c_initype(t2c_ctx_t ctx, t2c_type_t type);
void       t2c_ana4size(t2c_ctx_t ctx, t2c_type_t type);                 // Analyze for size and alignment.
void       t2c_ana4off(t2c_ctx_t ctx, t2c_OffMap_t * omap);              // Analyze for size/alignment/offsets.
int32_t    t2c_typecmp(const t2c_Type_t *a, const t2c_Type_t *b);        // Compare 2 types; for sorting.
t2c_type_t t2c_mem2cont(const t2c_Member_t * mem, uint32_t mi[1]);       // From a member, return the container; set mi if not NULL.
t2c_type_t t2c_clone4type(t2c_ctx_t ctx, const t2c_Type_t * type);       // Allocate and clone the given type in the context.
t2c_type_t t2c_tdref4type(t2c_ctx_t ctx, t2c_type_t t, const char *n);   // Create a typedef'ed *reference*; e.g. typedef Foo_t * foo_t;
uint32_t   t2c_clearmark4scan(t2c_ctx_t ctx);                            // Set all type mark4scan to 0, return number cleared.
void       t2c_scan4type(t2c_ctx_t ctx, t2c_cb4t_t cb, void * arg);      // Only call cb on unmarked types.
void       t2c_scan4mem(t2c_ctx_t ctx, t2c_cb4m_t cb, void * arg);
void       t2c_prep4gen(t2c_ctx_t ctx);                                  // Sort the entries for generating the code.
uint32_t   t2c_isTypedef(const t2c_Type_t * type);                       // Return true if the type is a typedef reference type.
uint32_t   t2c_isEnum(const t2c_Type_t * type);
uint32_t   t2c_reptypedefs(t2c_ctx_t ctx);                               // Replace all typedefs; return replacements done.
uint32_t   t2c_mark4use(t2c_ctx_t ctx, const t2c_Type_t * root);         // Mark all types used by this type and its members recursively.
t2c_type_t t2c_name2type(t2c_ctx_t ctx, const char * name);              // Return the type, based upon a name; return NULL when not found.

void       t2c_remove4type(t2c_ctx_t ctx, t2c_XRef_t * xref);            // Remove xref->type; when xref->num != 0, it failed.
void       t2c_xref4type(t2c_ctx_t ctx, t2c_XRef_t * xref);              // Search where type in xref->type is used as member.
void       t2c_xref4mem(t2c_ctx_t ctx, t2c_XRef_t * xref);               // Search where member with xref->name is, starting from xref-type.

typedef int (*vsnprintf_t)(char buf[], size_t size, const char * fmt, va_list ap);

typedef struct t2c_TGSpec_t {     // Type generation specification.
  struct {
    uint16_t   cap;               // Capacity of the buffer.
    uint16_t   rem;               // Remaining capacity.
    uint8_t    pad[4];
    char *     buf;               // Buffer itself.
  } Buf;
  vsnprintf_t  vsnprintf;         // Function pointer to a vsnprintf like function; default is vsnprintf.
  const char * evfmt;             // Enumeration value format.
  const char * packed;            // String to be used for packed structures.
  uint8_t      useFwDec;          // Add 'struct' or 'union' before each referenced type.
  uint8_t      overflow;          // Buffer was too small or line capacity was too small.
  uint8_t      useTypedef;        // When non zero, create a typedef.
  uint8_t      addEOLComments;    // When non zero, add // comments after each member and the type.
  struct {
    uint8_t    type;              // Where the type starts; default col 3.
    uint8_t    member;            // Where the member name starts.
    uint8_t    bits;              // For a bitset, where the #bits starts.
    uint8_t    typedef_1;         // For where the typedef struct/union/enum starts.
    uint8_t    typedef_2;         // For where the typedef struct/union/enum starts.
    uint8_t    comment;           // Where the end of line comment starts.
    uint8_t    ends;              // Where the \0 of the line is; lines are padded with spaces.
    uint8_t    indent;            // Not a column position; number of spaces for indenting; default 2.
  } Tab;                          // Indicates column positions; column position starts at 1.
  struct {
    uint16_t   cap;               // Line capacity at the tail.
    uint16_t   num;               // Number of lines used at the tail.
  } Lines;
  struct {
    char *     start;             // Will always point in the buffer.
  } Line[0];
} t2c_TGSpec_t;

typedef struct t2c_TGSpec_t * t2c_tgspec_t;

void t2c_fmttype(t2c_ctx_t ctx, t2c_type_t type, t2c_tgspec_t spec);

#endif // T2C_TYPES_H
