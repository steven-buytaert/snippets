// Copyright 2024 Steven Buytaert

#include <umem.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <inttypes.h>

/*

  This is a simple multithread demonstrator of the micro memory manager.

  Each thread does nothing else than randomly either allocate or release a
  block of memory. In case of allocation, it clears the received memory. In
  case of release, the contents of the block and the given tag, are checked.

  The process of allocation/release for each thread runs forever until control-c
  is pressed.
  
  The monitor thread, will dump some statistics, each second;
    - the number of current chunks in the micro manager
    - the contention rate per second (see the contention callback).
    - running time
    - for each thread, the number of operations done and the number of bytes
      currently allocated and that mutator thread.
    - at the end, the total number of bytes in use; since it is calculated without
      any locking, this is just a snapshot.
  
*/

typedef struct Block_t {
  void *            mem;          // Points to the allocated memory.
  uint32_t          size;         // Size of the block in bytes.
  uint8_t           fill;         // Block has been filled with this byte.
  uint8_t           pad[3];
} Block_t;

typedef struct Mut_t {            // Mutator Thread Context.
  umemctx_t         umem;         // Micro Memory to use.
  pthread_t         thread;
  Block_t           blocks[512];  // Collection of blocks.
  uint64_t          ops;          // Total operation count.
  uint32_t          inuse;        // Number of bytes in use.
  uint32_t          filled;       // Number of slots filled in blocks.
  volatile uint16_t run;
  uint8_t           tid;          // Simple thread id; starts at 0.
  volatile uint8_t  releaseonly;  // When non zero, only fast release allowed.
  char              name[36];
} Mut_t;

#define NUM(A) (sizeof(A) / sizeof(A[0]))

static uint32_t blockOK(Block_t * block, uint8_t tags) {    // Return non zero when block checks out OK.

  uint32_t  ok = mem2chunk(block->mem)->tags == tags;;
  uint8_t * c = block->mem;
  
  for (uint32_t i = 0; ok && i < block->size; i++, c++) {
    ok = (*c == block->fill);
  }
  
  return ok;

}

static void * mutate(void * arg) {

  Mut_t *   mut = arg;
  uint32_t  what;
  uint32_t  size;
  uint32_t  s;
  umemctx_t umem = mut->umem;
  Block_t * b;
  void *    mem;

  while (mut->run) {
    what = rand() & 0b111;
    if (mut->releaseonly) { what = 2; }                     // 2 is slower coalescing release.

    switch (what) {    
      case 0: case 1: case 4: case 5: {                     // Allocate; most prefered command.
        if (mut->filled < NUM(mut->blocks)) {               // Only allocate if slots left.
          do {
            s = (uint32_t) rand() % NUM(mut->blocks);
          } while (mut->blocks[s].mem);                     // Find a vacant slot.
          size = 1 + (uint32_t) (rand() % 256);             // Select a proper size.
          b = & mut->blocks[s];
          b->mem = NULL;
          if (0 == what) {
            b->mem = umalloc(umem, size, mut->tid);         // Slow.
          }
#if defined(UMEMFAST)
          else {
            b->mem = umalloc_fast(umem, size, mut->tid);    // Faster.
          }
#endif
          if (b->mem) {
            assert(0 == ((uintptr_t) b->mem & 0b111));      // Assert proper worst case alignment.
            b->fill = (uint8_t) rand() & 0xff;
            memset(b->mem, b->fill, size);                  // Fill the block.
            b->size = size;
            mut->filled++;
            mut->ops++;
            mut->inuse += size;
          }
        }
        break;
      }
    
      case 2: case 3: {                                     // Release.
        if (mut->filled) {                                  // Only release if blocks left.
          do {
            s = (uint32_t) rand() % NUM(mut->blocks);
          } while (! mut->blocks[s].mem);                   // Find an occupied slot.
          b = & mut->blocks[s];
          assert(blockOK(b, mut->tid));
#if defined(UMEMFAST)
          if (2 == what) {
            ufree(umem, b->mem);                            // Slow release.
          }
          else {
            ufree_fast(umem, b->mem);                       // Faster release.
          }
#else
          ufree(umem, b->mem);                              // Only slow release.
#endif
          mut->filled--;                                    // Update slot and count.
          b->mem = NULL;
          mut->inuse -= b->size;
          mut->ops++;
        }
        break;
      }
      
      case 6: case 7: {                                     // Reallocate existing block.
#if defined(UREALLOC)
        if (mut->filled) {                                  // Only reallocate if blocks left.
          do {
            s = (uint32_t) rand() % NUM(mut->blocks);
          } while (! mut->blocks[s].mem);                   // Find an occupied slot.
          b = & mut->blocks[s];
          assert(blockOK(b, mut->tid));
          size = 1 + (uint32_t) (rand() % 256);             // Select a new size.
          mem = urealloc(umem, b->mem, size, 0x00);         // Tags of current block will be used.
          if (mem) {                                        // Realloc worked.
            mut->ops++;
            mut->inuse += (size - b->size);                 // Update number of bytes in use up or down.
            if (size > b->size) {                           // Block has grown.
              memset(mem, b->fill, size);                   // Refill whole block.
            }
            b->mem = mem;
            b->size = size;
          }
        }
#endif
        break;
      }
      
    }
  }
  
  return NULL;
  
}

static void contCb(umemctx_t umem) {                        // Contention callback for sample.
  __atomic_add_fetch(& umem->count, 1, __ATOMIC_SEQ_CST);   // Keep a count on the contention.
  usleep(2000 * (1 + ((uint32_t) rand() % 0x0f)));          // Do some random backoff.
}

static const struct option Options[] = {
  { "numthreads", 1, NULL, 'n' },
  { "space",      1, NULL, 's' },
  { "help",       0, NULL, 'h' },
  { NULL,         0, NULL,  0  },
};

int main(int argc, char * argv[]) {

  uint32_t  numthr = 2;
  uint32_t  spacesz = 1024 * 128;
  uint8_t * space;
  int32_t   o;
  int32_t   ai;                                             // Argument index.
  uint32_t  i;
  Mut_t *   muts;
  UMemCtx_t UMem;
  uint32_t  inuse;
  uint32_t  seconds = 0;
  uint32_t  releasing = 0;
  
  static const uint32_t mbyte = 1024 * 1024;

  do {
    o = getopt_long(argc, argv, "hn:s:", Options, & ai);
    switch (o) {
      case 'n': {
        numthr = (uint32_t) atoi(optarg);
        numthr = numthr ? numthr : 1;
        numthr = numthr > 99 ? 99 : numthr;                 // Apply some limit.
        break;
      }

      case 's': {
        spacesz = (uint32_t) atoi(optarg);
        if (spacesz > (2 * mbyte) - 1) {                    // Limit to what we can represent.
          spacesz = (2 * mbyte) - 1;
        }
        break;
      }

      case 'h': case '?': {
        printf("%s [options]\n", argv[0]);
        printf("--numthreads -n : number of mutator threads; default %u threads.\n", numthr);
        printf("--space      -s : size of the memory block to manage; default %u bytes.\n", spacesz);
        printf("Runs until ctrl-c.\n");
        return 1;
        break;
      }
    }
  } while (o != -1);                                        // Process as long as there are options.

  muts = malloc(sizeof(Mut_t) * numthr);
  assert(muts);
  space = malloc(spacesz);
  assert(space);
  
  uint32_t stat = initUMemCtx(& UMem, space, spacesz);
  assert(stat);                                             // Check it was properly initialized.
  UMem.contcb = contCb;                                     // Override do nothing contention callback.

  for (i = 0; i < numthr; i++) {
    memset(& muts[i], 0x00, sizeof(Mut_t));
    sprintf(muts[i].name, "mut %2u:", i);
    muts[i].run = 1;
    muts[i].tid = (uint8_t) i;
    muts[i].umem = & UMem;
    pthread_create(& muts[i].thread, NULL, mutate, & muts[i]);
  }

  while (1) {
    sleep(1);
    if (releasing) { releasing--; }                         // Decrement release counter.
    if (seconds && 0 == (seconds & 0xf)) {
      releasing = 1;
    }
    uint32_t s = ++seconds;
    uint32_t d = s / (3600 * 24); s -= d * 3600 * 24;
    uint32_t h = s / 3600;        s -= h * 3600;
    uint32_t m = s / 60;          s -= m * 60;
    printf("%u chunk%s, %u cont/sec, %u day%s, %u hour%s, %u minute%s, %u second%s%s.\n", 
        UMem.numchunks,  UMem.numchunks == 1 ? "" : "s",
        UMem.count,
        d, d == 1 ? "" : "s",
        h, h == 1 ? "" : "s",
        m, m == 1 ? "" : "s", 
        s, s == 1 ? "" : "s",
        releasing ? ", only release" : "");
    UMem.count = 0;
    inuse = 0;
    for (i = 0; i < numthr; i++) {
      printf("%s %"PRIu64" operations, %u in use.\n", muts[i].name, muts[i].ops, muts[i].inuse);
      inuse += muts[i].inuse;
      muts[i].releaseonly = releasing ? 1 : 0;
    }
    printf("%u bytes in use of %u.\n", inuse, spacesz);
  }
   
  return 0;
  
}
