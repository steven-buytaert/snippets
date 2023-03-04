#ifndef SNSET_H
#define SNSET_H

// Copyright 2021-2023 Steven Buytaert

#include <stdint.h>
#include <stdalign.h>

typedef struct SNSet_t * snset_t;

typedef void * (* snsetmem_t)(snset_t set, void * mem, uint32_t sz);
typedef void * (* snsetapi_t)(snset_t set, uint32_t u1, uint32_t u2);
typedef void   (* snstseal_t)(snset_t set);
typedef void * (* snsetadd_t)(snset_t set, uint32_t index, uint32_t add);

typedef struct SNSet_t {
  union {
    const char * name;            // Name of the set.
    void *       custom;          // Or a user defined pointer.
  };
  snsetmem_t     mem;             // From user; function to allocate/release/realloc memory; realloc semantics.
  snsetapi_t     obj;             // From set; allocation new = obj(set, size, alignment); returned obj memory is cleared.
  snsetapi_t     ensure;          // From set; ensure free capacity ensure(set, refs, bytes);
  snsetadd_t     stretch;         // From set; stretch the object at 'index' with 'add' bytes; returns the (maybe changed) obj reference.
  snstseal_t     seal;            // From set; seal off a set or trim a set to object bytes only.
  
  union {
    void *       Set;             // Start of the block of Objects; aligned to Grow.wca.
    uint8_t *    addr4Set;
  };                              // This is the start of the whole memory block. Valid after sealing.
  union {
    void *       next;            // Next object; first free byte.
    uint8_t *    addr4next;
  };
  union {
    void * *     set;             // Set with 'num' references to items and 'freeslots' unused slots.
    uint8_t *    addr4set;        // Address where the set array starts; invalid after sealing.
    uint8_t **   addr4ref;        // Reference to an object for address calculation.
  };

  struct {
    uint8_t *    block;           // Original block allocated from Set.mem(...). Is valid after sealing.
    uint32_t     size;            // Original size allocated at block. Is valid after sealing.
    uint32_t     bytes;           // Number of object bytes to grow with when needed.
    uint32_t     slots;           // Number of refs slots to grow with.
    uint8_t      locked;          // When not 0, growing is locked; NULL returned when no more space.
    uint8_t      pad[3];
  } Grow;

  uint32_t       num;             // Number of members in the set.
  uint32_t       freeslots;       // Number of free obj reference slots in 'set'.
  uint32_t       avail;           // Number of bytes available at 'next'.
  uint32_t       size;            // Number of bytes allocated at 'Set'; total size.
  uint16_t       wca;             // Current worst case alignment encounter.
  uint16_t       padgrowth;       // How many times the padding area was used to grow.
  uint32_t       reallocs;        // How many reallocations have been done.
} SNSet_t;

void snset_init(snset_t set, snsetmem_t memrealloc);

#endif // SNSET_H
