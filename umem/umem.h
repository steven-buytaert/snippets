// Copyright 2024 Steven Buytaert

#ifndef UMEMMAN_H
#define UMEMMAN_H

#include <stdint.h>
#include <assert.h>

typedef struct Chunk_t *    chunk_t;
typedef struct UMemCtx_t *  umemctx_t;
typedef struct UMemIter_t * umemiter_t;

typedef void     (*ucontcb_t)(umemctx_t umem);
typedef void *   (*umalloc_t)(umemctx_t umem, uint32_t sz, uint8_t tags);
typedef void     (*ufree_t)(umemctx_t umem, void * mem);
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
  ucontcb_t       contcb;         // Contention callback; default does nothing.
  chunk_t         end;            // Virtual end chunk; size 0 and forever with ciu set.
  chunk_t         start;          // Starting chunk; forever with pif clear.
  chunk_t         c2free;         // List of chunks to be freed.
  uint8_t       * space;          // Space provided by user to be managed.
  umalloc_t       malloc;         // Best fit effort allocation.
  umalloc_t       uncmalloc;      // Less contented allocation; NULL return does NOT always means OOM!
  ufree_t         free;           // Release the memory again, slowest, coalescing path.
  ufree_t         uncfree;        // Uncontended free function; real freeing work done later.
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

// Minimum chunk size; must be a binary number aligned for worst case.

static const uint32_t minchunksize = 8;

/*

  Important notes:
  
  The prev field of the Chunk_t structure, is part of the memory of the preceeding
  chunk!

  Despite this fact, the manipulation of chunk->ciu and succ->pif always must be done
  under lock of both chunk and succ; succ->prev *can* be written under lock of chunk alone,
  since the succ->prev fields memory belongs to chunk, but the succ->pif that goes along
  with it, can only be changed under succ lock.

*/

#define iusize(S) ({ 0x001fffffu & (S); })

typedef enum {
  UMemIt_Stop     = 0,            // Stop iteration; *keep* current locks; iterate returns 0!
  UMemIt_Unlock   = 1,            // Keep iterating, unlock current chunk.
  UMemIt_Keep     = 2,            // Keep iterating, keep current chunk locked.
} UMemItStat_t;

typedef UMemItStat_t (*icb_t)(umemiter_t iter, chunk_t c);

typedef struct UMemIter_t {       // UMem Iteration Context.
  umemctx_t       umem;
  chunk_t         start;          // Chunk to start iteration from.
  chunk_t         succ;           // The locked successor of the passed chunk.
  const icb_t     cb;
  chunk_t         found;          // During allocation, chunk that fits the size.
  chunk_t         succ2found;     // Successor of the found chunk, if any.
  uint32_t        size;           // In case of allocation search, required size.
  uint32_t        count;          // Generic counter; available to callback.
} UMemIter_t;

typedef union Mem_t {             // Memory calculation union.
  chunk_t         chunk;
  void *          mem;
  uint8_t *       addr;           // For adding/substracting raw sizes.
  uintptr_t       check;          // For checking alignment.
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
  
  assert(! (num & ~0x001fffffu));
 
  Mem.chunk->size = iusize(num - chunkhdrsz);
 
  Mem.chunk->pif  = 0;
  Mem.chunk->ciu  = 0;
  Mem.chunk->lock = 0;
 
  return Mem.chunk;

}

inline static uint32_t alignedok(void * mem) {              // Return non zero when worst case aligned.

  uintptr_t ptr = (uintptr_t) mem;
  
  return (ptr & (minchunksize - 1)) ? 0 : 1;

}

// Initialize a context.

void initUMemCtx(umemctx_t umem, uint8_t space[], uint32_t sz);

#endif // UMEMMAN_H
