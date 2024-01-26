// Copyright 2024 Steven Buytaert

#ifndef UMEMMAN_H
#define UMEMMAN_H

#include <stdint.h>
#include <assert.h>

typedef struct Chunk_t *    chunk_t;
typedef struct UMemCtx_t *  umemctx_t;
typedef struct UMemIter_t * umemiter_t;

typedef void     (*umemfun_t)(umemctx_t umem);
typedef uint32_t (*uiter_t)(umemiter_t iter);

typedef struct UMemCtx_t {        // Micro Memory Manager Context.
  union {
    uint32_t      count;
    uint16_t      u16[2];
    uint8_t       u08[4];
  };                              // Available to the user; unused by implementation.
  uint8_t         align;          // Number of alignment bytes used.
  uint8_t         breakat;        // Break ties after this many attempts; default 12.
  uint16_t        tiesbroken;     // Number of times the tie was broken; see umalloc.
  uint32_t        numchunks;      // Total number of chunks.
  uint32_t        size;           // Total number of bytes at space.
  umemfun_t       contcb;         // Contention callback; default does nothing.
  umemfun_t       clean;          // Cleanup any chunks on the freelist, if any.
  chunk_t         end;            // Virtual end chunk; size 0 and forever with ciu set.
  chunk_t         start;          // Starting chunk; forever with pif clear.
  chunk_t         c2free;         // List of chunks to be freed.
  uint8_t       * space;          // Space provided by user to be managed.
  uiter_t         iterate;        // Iterate over all chunks.
} UMemCtx_t;

typedef struct Chunk_t {          // ---- Memory of previous chunk -----------------------------
  chunk_t         prev;           // Link to the previous chunk, if pif is set.
  union {                         // ---- Memory of own chunk starts here ----------------------
    volatile struct {
      uint32_t    ciu  :  1;      // Current in use
      uint32_t    pif  :  1;      // Previous is free; prev field is valid.
      uint32_t    lock :  1;
      uint32_t    size : 21;      // Number of bytes available at u08[0].
      uint32_t    tags :  8;      // User defined tags.
    };
    uint32_t      header;
  };
  union {
    uint8_t       u08[4];         // Actually is at least minchunksize bytes.
    uint32_t      u32[1];
  };
} Chunk_t;

static const uint32_t chunkhdrsz = sizeof(uint32_t);

// Minimum chunk size; it depends on the bit width of the system. We want to
// avoid memory overhead for embedded systems as much as possible, but we
// do want all memory allocated blocks to start on a worst case 8 byte aligned
// boundary; also see the alignedok() function below.

static const uint32_t minchunksize = sizeof(void *);

#define iusize(S) ({ 0x001fffffu & ((uint32_t) S); })

typedef enum {
  UMemIt_Stop     = 0,            // Stop iteration; *keep* current locks; iterate returns 0!
  UMemIt_Unlock   = 1,            // Keep iterating, unlock current chunk.
  UMemIt_Keep     = 2,            // Keep iterating, keep current chunk locked.
} UMemItStat_t;

typedef UMemItStat_t (*icb_t)(umemiter_t iter, chunk_t c);

typedef struct UMemIter_t {       // UMem Iteration Context.
  umemctx_t       umem;
  const icb_t     cb;
  chunk_t         start;          // Chunk to start iteration from.
  chunk_t         succ;           // [Set by iterator] The locked successor of the passed chunk.
  chunk_t         found;          // During allocation, chunk that fits the size.
  chunk_t         succ2found;     // Successor of the found chunk, if any.
  uint8_t         pad[4];
  uint32_t        size;           // In search4chunk, required size.
  uint32_t        count;          // Generic counter; available to callback.
  uint32_t        align;          // In search4chunk, alignment required; only used for uamalloc.
} UMemIter_t;

typedef union Mem_t {             // Memory calculation union.
  chunk_t         chunk;
  void *          mem;
  uint8_t *       addr;           // For adding/substracting raw sizes.
  uintptr_t       check;          // For checking alignment.
  uintptr_t       calc;           // For calculation.
} Mem_t;

/*

  Getting pointer arithmetic right, for both 64 and 32 bit systems, is something I struggle with.
  So I use simple drawings to get my head around things. Honi soit qui mal y pense.

               64 bits                              32 bits

        +----+----+----+----+                +----+----+----+----+
        |    prev           | <--- chunk     |                   |
        +----+----+----+----+                +----+----+----+----+
        |    prev           |                |     prev          | <--- chunk
  ------+----+----+----+----+----------------+----+----+----+----+-------------
        |    header         |                |     header        |
        +----+----+----+----+                +----+----+----+----+
        |    mem            |                |     mem           |
        +----+----+----+----+                +----+----+----+----+
        |                   |                |                   |
        +----+----+----+----+                +----+----+----+----+
        :                   :                :                   :

*/

inline static chunk_t mem2chunk(void * mem) {

  Mem_t Mem = { .mem = mem };

  Mem.addr -= sizeof(chunk_t);
  Mem.addr -= chunkhdrsz;

  return Mem.chunk;

}

/*
               64 bits                              32 bits

        +----+----+----+----+                +----+----+----+----+
  1000  |    prev           | <--- chunk     |                   |
        +----+----+----+----+                +----+----+----+----+
  1004  |    prev           |                |     prev          | <--- chunk
  ------+----+----+----+----+----------------+----+----+----+----+-------------
  1008  |    header         |                |     header        |
        +----+----+----+----+                +----+----+----+----+
  1012  |    mem            | size = 16      |     mem           | size = 16
        +----+----+----+----+                +----+----+----+----+
  1016  |                   |                |                   |
        +----+----+----+----+                +----+----+----+----+
  1020  |    prev           | <--- succ      |                   |
        +----+----+----+----+                +----+----+----+----+
  1024  |    prev           |                |    prev           | <--- succ
  ------+----+----+----+----+----------------+----+----+----+----+-------------
  1028  |    header         |                |    header         |

*/

inline static chunk_t chunk2succ(chunk_t chunk) {

  Mem_t Mem = { .chunk = chunk };

  assert(chunk->lock);                                      // Size should not change under our feet.

  Mem.addr += chunk->size + chunkhdrsz;

  return Mem.chunk;

}

/*
               64 bits                               32 bits

        +----+----+----+----+                +----+----+----+----+
   992  |    prev           | <--- chunk     |                   |
        +----+----+----+----+                +----+----+----+----+
   996  |    prev           |                |     prev          | <--- chunk
  ------+----+----+----+----+----------------+----+----+----+----+-------------
  1000  |    header         | <- space[0] -> |     header        |
        +----+----+----+----+                +----+----+----+----+
  1004  |    mem            |                |     mem           |
        +----+----+----+----+                +----+----+----+----+
  1008  |                   |                |                   |
        +----+----+----+----+                +----+----+----+----+
        :                   :                :                   :

*/

inline static chunk_t raw2chunk(uint8_t bytes[], uint32_t num) {

  Mem_t Mem = { .addr = bytes };

  Mem.addr -= sizeof(chunk_t);

  assert(& Mem.chunk->header == (uint32_t *) bytes);

  assert(iusize(num) == num);

  Mem.chunk->size = iusize(num - chunkhdrsz);

  Mem.chunk->pif  = 0;
  Mem.chunk->ciu  = 0;
  Mem.chunk->lock = 0;

  return Mem.chunk;

}

inline static uint32_t alignedok(void * mem) {              // Return non zero when worst case aligned.

  uintptr_t ptr = (uintptr_t) mem;

  return ! (ptr & 0b111);                                   // Return 1 when aligned on an 8 byte boundary.

}

// Initialize a context; returns zero when passed space is too small.

uint32_t initUMemCtx(umemctx_t umem, uint8_t space[], uint32_t sz);

// Basic allocation function; it does NOT clear the allocated memory. It searches
// for a best fitting chunk, unless there is high contention. Under high contention
// not all chunks might be visited for a best fit.

void * umalloc(umemctx_t umem, uint32_t sz, uint8_t tags);

// Basic memory release function; it releases the memory and coalesces it with other
// neighboring chunks if they are free too.

void ufree(umemctx_t umem, void * mem);

#define UMEMFAST

#if defined(UMEMFAST)

// A malloc version that only does a first fit for a matching chunk; faster than umalloc.

void * umalloc_fast(umemctx_t umem, uint32_t sz, uint8_t tags);

// A very fast function to release the memory; the memory is put on a free list that
// is cleaned by regular calls to ufree or umalloc.

void ufree_fast(umemctx_t umem, void * mem);

#endif // UMEMFAST

/*
  The urealloc function will only be build, when the UREALLOC guard
  is defined. It works like the known realloc. The passed tags argument
  is only used when the passed memory pointer is NULL and the function
  should work like malloc. In any other case, the tags of the passed
  memory block will be reused for the reallocation. Note that in the case
  where the block is shrinked, the exact same block address will be returned.
  When a block is to be increased, a new address might be returned. If you pass
  a block that has been allocated with uamalloc and ask it for increasing the
  block, there is NO guarantee that the newly returned block is allocated
  at the proper boundary.
*/

#define UREALLOC 1

#if defined(UREALLOC)

void * urealloc(umemctx_t ctx, void * mem, uint32_t size, uint8_t tags);

#endif // UREALLOC

/*
  The uamalloc function can be used to allocate a block of memory on a given
  alignment boundary. It will only be build when UAMALLOC has been defined.
  Note that the requested alignment of the block, is NOWHERE stored. So when a
  block that has been allocated with uamalloc, is used in urealloc for increasing
  its size, the aligment is NOT guaranteed. Offering suc a block to urealloc for
  shrinking, will retain the alignment, as the same block is returned; see urealloc
  documentation above.
*/

#define UAMALLOC 1

#if defined(UAMALLOC)

void * uamalloc(umemctx_t ctx, uint32_t sz, uint8_t tags, uint32_t align);

#endif // UAMALLOC

#endif // UMEMMAN_H
