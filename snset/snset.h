#ifndef SNSET_H
#define SNSET_H

// Copyright 2021 Steven Buytaert

#include <stdint.h>

typedef struct SNSet_t * snset_t;

typedef void * (* snsetmem_t)(snset_t set, void * mem, uint32_t sz);
typedef void * (* snsetapi_t)(snset_t set, uint32_t u1, uint32_t u2);
typedef void * (* ostretch_t)(snset_t set, uint32_t add);
typedef void   (* objditch_t)(snset_t set, uint32_t index);

typedef struct SNSet_t {
  union {
    const char * name;            // Name of the set.
    void *       custom;          // Or a user defined pointer.
  };
  snsetmem_t     mem;             // Function to allocate/release/realloc memory.
  snsetapi_t     obj;             // From set; allocation new = obj(set, size, alignment); obj memory is cleared.
  ostretch_t     stretch;         // From set; stretch the last object with 'add' bytes; returns the (maybe changed) obj reference.
  snsetapi_t     ensure;          // From set; ensure free capacity ensure(set, refs, bytes);
  union {
    void * *     set;             // Set with 'num' references to items and 'unused' unused slots.
    uint8_t *    addr4set;
  };
  union {
    void *       Set;             // Start of the block of Objects.
    uint8_t *    addr4Set;
  };
  union {
    void *       next;            // Next object; first free byte.
    uint8_t *    addr4next;
  };
  uint32_t       num;             // Number of members in the set.
  struct {
    uint16_t     avail;           // Size to grow with when needed.
    uint8_t      num;             // Number of refs slots to grow with.
    uint8_t      locked;          // When not 0, growing is locked; NULL returned when no more space.
  } Grow;
                                  // Rest is internal bookkeeping.
  uint32_t       free;            // Number of free obj reference slots in set.
  uint32_t       avail;           // Number of bytes available at Next.addr.
  uint32_t       size;            // Number of bytes allocated at set; total size.
  uint32_t       reallocs;        // How many reallocations have been done.
} SNSet_t;

void snset_init(snset_t set, snsetmem_t memrealloc);

#endif // SNSET_H
