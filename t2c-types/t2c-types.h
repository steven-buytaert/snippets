#ifndef T2C_TYPES_H
#define T2C_TYPES_H

// Copyright 2023 Steven Buytaert

#include <stdint.h>

typedef struct t2c_Type_t *   t2c_type_t;
typedef struct t2c_Member_t * t2c_member_t;

typedef enum {
  t2c_Prim     = 0b00000001,      // Primitive type.
  t2c_Comp     = 0b00000010,      // Composite type.
  t2c_Bitset   = 0b00000110,      // A primitive type but chopped up in bitfields.
  t2c_Struct   = 0b00001010,
  t2c_Union    = 0b00010010,
  t2c_Static   = 0b00100000,      // Type is statically allocated; don't free.
  t2c_Alt      = 0b01000000,      // Type was created as an alternative.
  t2c_Cluster  = 0b10000000,      // Type was created as a cluster out an other type.
} t2c_Prop_t;

typedef struct t2c_Alt_t {        // Alternative for ...
  t2c_type_t         orig;        // ... this original type.
  uint32_t           num;         // Number of (new) types in the following array.
  t2c_type_t         types[0];    // Array with new types; types[0] is new type, any other are new clustered types.
} t2c_Alt_t;

typedef union t2c_Cargo_t {       // User defined cargo. Is copied and moved with other member/type parts.
  void *             ref;         // To store a reference, if the cargo space is too small.
  uint32_t           cookie;
  uint8_t            u08[8];
  uint16_t           u16[4];
  uint32_t           u32[2];
  uint64_t           u64[1];      // Also to ensure proper alignment.
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
  t2c_BadBaseType    =  7,        // The bitset has no ccType.bstype or it is not a primitive.
  t2c_NoCapLeft      =  8,        // There's no capacity left.
  t2c_AllocFailed    =  9,        // Allocation failed.
  t2c_BadProperties  = 10,        // Properties that are incompatible, e.g. Comp and Prim bit are set.
} t2c_DiagInfo_t;

typedef struct t2c_Ctx_t {
  const char *       typeExt;     // Extension given to types.
  t2c_Cargo_t        Cargo;       // User defined cargo.
  t2c_mem_t          mem;         // To (re)allocate/free memory; realloc semantics.
  uint8_t            size4ref;    // Size of a reference or pointer.
  uint8_t            align4ref;   // Alignment requirement for a pointer or reference.
  uint8_t            maxnamesize; // Maximum size for a name, including \0.
  uint8_t            cmpNames;    // When non zero, also compare member names in t2c_typecmp() for equalness.
  uint8_t            error;       // Should remain 0 (t2c_OK).
  char               msg[135];    // Diagnostic message.
  const uint16_t     cap;         // Capacity at the tail; fixed at creation.
  uint16_t           num;         // Number of slots used at the tail, including NULL slots.
  t2c_type_t         types[0];    // Note that this can contain NULL slots when types are removed.
} t2c_Ctx_t;

typedef struct t2c_Member_t {     // Member of a composite type.
  char *             name;        // Name of the member; for a bitset member, the name can be "" but never NULL.
  t2c_type_t         type;        // Member type; for a bitset member, all types must refer to the same unsigned primitive.
  t2c_member_t       ref2size;    // Field that determines number of elements (implies isVTail or numind > 0).
  uint32_t           offset;      // Offset byte/bit address in the composite/bitset.
  uint32_t           fxdsize;     // When not 0, the fixed array size. fxdsize and numind are mutually exclusive.
  uint8_t            numind;      // Number of indirections, 0 = plain, 1 = *, 2 = **, ...
  uint8_t            isVTail;     // When not zero, variable length array tail.
  uint8_t            isConst;     // Member is constant.
  uint8_t            width;       // Only for a bitset member; width in bits. Offset determines right bit offset.
  uint8_t            nameisdup;   // Name of the member is a strdup'ed name (internal purposes only).
  uint8_t            isForward;   // This member refers to a composite type, defined later (can't use the typedef'ed type yet).
  uint8_t            isRef2Self;  // This member refers to itself as type, e.g. a linked list; implies isForward.
  uint8_t            pad[1];
  t2c_Cargo_t        Cargo;       // User defined cargo; is copied when relevant (e.g. moving to cluster).
} t2c_Member_t;

typedef struct t2c_Type_t {
  char *             name;        // Name of the type; this buffer is ccCtx.maxnamesize characters in size.
  t2c_ctx_t          ctx;         // Context that was attached to this type.
  t2c_Alt_t *        alt;         // When not NULL an alternative for this type.
  t2c_Cargo_t        Cargo;       // User defined cargo; is copied when relevant (e.g. for an alternative).
  union {
    t2c_type_t       bstype;      // When a bitset, the underlying primitive type.
    t2c_type_t       container;   // When a cluster, the type that contains us.
  };
  t2c_Prop_t         prop;        // Type properties.
  uint16_t           size;        // Size in bytes.
  uint8_t            align;       // Alignment requirement.
  uint8_t            mark;        // Only types with 0 mark, are passed to the callback in t2c_scan.
  uint16_t           num;         // Members in use at tail.
  uint16_t           weight;      // Number representing usage; the higher, the earlier the type must be declared.
  const t2c_Member_t Marker;      // Marker to find back the container type for a member (name is NULL, type is container).
  t2c_Member_t       Members[0];  // Type members.
} t2c_Type_t;

typedef struct t2c_XRef_t {       // Cross reference result (searching for type used or member path).
  t2c_type_t         type;        // Type to start search from/for or to delete.
  const char *       name;        // Member name searched for, if any (otherwise NULL).
  const uint16_t     cap;         // Number of members at the tail.
  uint16_t           num;         // Number of slots used at the tail.
  t2c_member_t       used[0];
} t2c_XRef_t;

extern const t2c_Type_t t2c_Char;
extern const t2c_Type_t t2c_U64;
extern const t2c_Type_t t2c_U32;
extern const t2c_Type_t t2c_U16;
extern const t2c_Type_t t2c_U08;
extern const t2c_Type_t t2c_S64;
extern const t2c_Type_t t2c_S32;
extern const t2c_Type_t t2c_S16;
extern const t2c_Type_t t2c_S08;

typedef void (* t2c_cb4t_t)(t2c_ctx_t ctx, t2c_type_t   t, void * arg);
typedef void (* t2c_cb4m_t)(t2c_ctx_t ctx, t2c_member_t m, void * arg);

void       t2c_initype(t2c_ctx_t ctx, t2c_type_t type);
void       t2c_ana4cluster(t2c_ctx_t ctx, t2c_type_t type);
void       t2c_ana4size(t2c_ctx_t ctx, t2c_type_t type);             // Analyze for size and alignment.
int32_t    t2c_typecmp(const t2c_Type_t *a, const t2c_Type_t *b);
t2c_type_t t2c_mem2cont(const t2c_Member_t * mem, uint32_t mi[1]);   // From a member, return the container; set mi if not NULL.
t2c_type_t t2c_clone4type(t2c_ctx_t ctx, const t2c_Type_t * type);   // Allocate and clone the given type in the context.
void       t2c_remove4type(t2c_ctx_t ctx, t2c_XRef_t * xref);        // Remove xref->type; when xref->num != 0, it failed.
void       t2c_xref4type(t2c_ctx_t ctx, t2c_XRef_t * xref);          // Search where type in xref->type is used as member.
void       t2c_xref4mem(t2c_ctx_t ctx, t2c_XRef_t * xref);           // Search where member with xref->name is, starting from xref-type.
uint32_t   t2c_clearmarks(t2c_ctx_t ctx);                            // Set all type marks to 0, return number cleared.
void       t2c_scan4type(t2c_ctx_t ctx, t2c_cb4t_t cb, void * arg);  // Only call cb on unmarked types.
void       t2c_scan4mem(t2c_ctx_t ctx, t2c_cb4m_t cb, void * arg);
void       t2c_prep4gen(t2c_ctx_t ctx);                              // Sort the entries for generating the code.

#endif // T2C_TYPES_H
