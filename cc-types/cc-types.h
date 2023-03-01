#ifndef CC_TYPES_H
#define CC_TYPES_H

// Copyright 2023 Steven Buytaert

#include <stdint.h>

typedef struct ccType_t *   cctype_t;
typedef struct ccMember_t * ccmember_t;

typedef enum {
  cc_Prim     = 0b00000001,       // Primitive type.
  cc_Comp     = 0b00000010,       // Composite type.
  cc_Bitset   = 0b00000110,       // A primitive type but chopped up in bitfields.
  cc_Struct   = 0b00001010,
  cc_Union    = 0b00010010,
  cc_Static   = 0b00100000,       // Type is statically allocated; don't free.
  cc_Alt      = 0b01000000,       // Type was created as an alternative.
  cc_Cluster  = 0b10000000,       // Type was created as a cluster out an other type.
} ccProp_t;

typedef struct ccAlt_t {          // Alternative for ...
  cctype_t         orig;          // ... this original type.
  uint32_t         num;           // Number of (new) types in the following array.
  cctype_t         types[0];      // Array with new types; types[0] is new type, any other are new clustered types.
} ccAlt_t;

typedef union ccCargo_t {         // User defined cargo. Is copied and moved with other member/type parts.
  void *           ref;           // To store a reference, if the cargo space is too small.
  uint32_t         cookie;
  uint8_t          u08[8];
  uint16_t         u16[4];
  uint32_t         u32[2];
  uint64_t         u64[1];        // To ensure proper alignment.
} ccCargo_t;

typedef struct ccCtx_t * cctctx_t;

typedef void * (*cc_mem_t)(cctctx_t ctx, void * mem, uint32_t size);

typedef enum {                    // Diagnostics that can be assigned to the context error member.
  ccd_OK            =  0,
  ccd_Circularity   =  1,         // A type is being defined including itself as a full member (not indirected).
  ccd_LoneRef2size  =  2,         // A member is marked as having a ref2size member, but is not a VTail or has indirection.
  ccd_VTailNotFinal =  3,         // A member is marked as VTail but it is not the final member.
  ccd_RefAndVTail   =  4,         // A member is marked both VTail and with indirection; they are mutually exclusive.
  ccd_ZeroWidth     =  5,         // A bitset member has an invalid width of 0.
  ccd_TooSmall      =  6,         // The bitset primitive is not wide enough for the given members.
  ccd_BadBaseType   =  7,         // The bitset has no ccType.bstype or it is not a primitive.
  ccd_NoCapLeft     =  8,         // There's no capacity left.
  ccd_AllocFailed   =  9,         // Allocation failed.
  ccd_BadProperties = 10,         // Properties that are incompatible, e.g. Comp and Prim bit are set.
} ccDiagInfo_t;

typedef struct ccCtx_t {
  const char *     typeExt;       // Extension given to types.
  ccCargo_t        Cargo;         // User defined cargo.
  cc_mem_t         mem;           // To (re)allocate/free memory; realloc semantics.
  uint8_t          size4ref;      // Size of a reference or pointer.
  uint8_t          align4ref;     // Alignment requirement for a pointer or reference.
  uint8_t          maxnamesize;   // Maximum size for a name, including \0.
  uint8_t          cmpMemberNames;// When non zero, also compare member names for equalness.
  uint8_t          error;         // Should remain 0 (ccd_OK).
  char             msg[7 + 128];  // Diagnostic message.
  const uint16_t   cap;           // Capacity at the tail; fixed at creation.
  uint16_t         num;           // Number of slots used at the tail, including NULL slots.
  cctype_t         types[0];      // Note that this can contain NULL slots when types are removed.
} ccCtx_t;

typedef struct ccMember_t {       // Member of a composite type.
  char *           name;          // Name of the member; for a bitset member, the name can be "" but never NULL.
  cctype_t         type;          // Member type; for a bitset member, all types must refer to the same unsigned primitive.
  ccmember_t       ref2size;      // Field that determines number of elements (implies isVTail or numind > 0).
  uint32_t         offset;        // Offset byte/bit address in the composite/bitset.
  uint32_t         fxdsize;       // When not 0, the fixed array size. fxdsize and numind are mutually exclusive.
  uint8_t          numind;        // Number of indirections, 0 = plain, 1 = *, 2 = **, ...
  uint8_t          isVTail;       // When not zero, variable length array tail.
  uint8_t          isConst;       // Member is constant.
  uint8_t          width;         // Only for a bitset member; width in bits. Offset determines right bit offset.
  uint8_t          nameisdup;     // Name of the member is a strdup'ed name (internal purposes only).
  uint8_t          isForward;     // This member refers to a composite type, defined later (can't use the typedef'ed type yet).
  uint8_t          isRef2Self;    // This member refers to itself as type, e.g. a linked list; implies isForward.
  uint8_t          pad[1];
  ccCargo_t        Cargo;         // User defined cargo; is copied when relevant (e.g. moving to cluster).
} ccMember_t;

typedef struct ccType_t {
  char *           name;          // Name of the type; this buffer is ccCtx.maxnamesize characters in size.
  ccCtx_t *        ctx;           // Context that was attached to this type.
  ccAlt_t     *    alt;           // When not NULL an alternative for this type.
  ccCargo_t        Cargo;         // User defined cargo; is copied when relevant (e.g. for an alternative).
  union {
    cctype_t       bstype;        // When a bitset, the underlying primitive type.
    cctype_t       container;     // When a cluster, the type that contains us.
  };
  ccProp_t         prop;          // Type properties.
  uint16_t         size;          // Size in bytes.
  uint8_t          align;         // Alignment requirement.
  uint8_t          mark;          // Only types with 0 mark, are passed to the callback in cc_scan.
  uint16_t         num;           // Members in use at tail.
  uint16_t         weight;        // Number representing usage; the higher, the earlier the type must be declared.
  const ccMember_t Marker;        // Marker to find back the container type for a member (name is NULL, type is container).
  ccMember_t       Members[0];    // Type members.
} ccType_t;

typedef struct ccXRef_t {         // Cross reference result (searching for type used or member path).
  cctype_t         type;          // Type to start search from/for or to delete.
  const char *     name;          // Member name searched for, if any (otherwise NULL).
  const uint16_t   cap;           // Number of members at the tail.
  uint16_t         num;           // Number of slots used at the tail.
  ccmember_t       used[0];
} ccXRef_t;

extern const ccType_t cc_Char;
extern const ccType_t cc_U64;
extern const ccType_t cc_U32;
extern const ccType_t cc_U16;
extern const ccType_t cc_U08;
extern const ccType_t cc_S64;
extern const ccType_t cc_S32;
extern const ccType_t cc_S16;
extern const ccType_t cc_S08;

typedef void (* cc_cb4t_t)(ccCtx_t * ctx, cctype_t   t, void * arg);
typedef void (* cc_cb4m_t)(ccCtx_t * ctx, ccmember_t m, void * arg);

void     cc_initype(ccCtx_t * ctx, cctype_t type);
void     cc_ana4cluster(ccCtx_t * ctx, cctype_t type);
void     ana4size(ccCtx_t * ctx, cctype_t type);                  // Analyze for size and alignment.
int32_t  cc_typecmp(const ccType_t *a, const ccType_t *b);
cctype_t cc_mem2cont(const ccMember_t * mem, uint32_t mi[1]);     // From a member, return the container; set mi if not NULL.
cctype_t cc_clone4type(ccCtx_t * ctx, const ccType_t * type);     // Allocate and clone the given type in the context.
void     cc_remove4type(ccCtx_t * ctx, ccXRef_t * xref);          // Remove xref->type; when xref->num != 0, it failed.
void     cc_xref4type(ccCtx_t * ctx, ccXRef_t * xref);            // Search where type in xref->type is used as member.
void     cc_xref4mem(ccCtx_t * ctx, ccXRef_t * xref);             // Search where member with xref->name is, starting from xref-type.
uint32_t cc_clearmarks(ccCtx_t * ctx);                            // Set all type marks to 0, return number cleared.
void     cc_scan4type(ccCtx_t * ctx, cc_cb4t_t cb, void * arg);   // Only call cb on unmarked types.
void     cc_scan4mem(ccCtx_t * ctx, cc_cb4m_t cb, void * arg);
void     cc_prep4gen(ccCtx_t * ctx);                              // Sort the entries for generating the code.

#endif // CC_TYPES_H
