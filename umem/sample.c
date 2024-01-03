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

  This is an extremely simple multithread demonstrator of the micro manager.

  Each thread does nothing else than randomly either allocate or release a
  block of memory. In case of allocation, it clears the received memory.

  The process of allocation/release for each thread runs forever until control-c
  is pressed.
  
*/

typedef struct Block_t {
  void *            mem;
  uint32_t          size;
  uint8_t           fill;
  uint8_t           pad[3];
} Block_t;

typedef struct Mut_t {            // Mutator Thread Context.
  umemctx_t         umem;         // Micro Memory to use.
  pthread_t         thread;
  Block_t           blocks[128];  // Collection of blocks.
  uint64_t          ops;          // Total operation count.
  uint32_t          inuse;        // Number of bytes in use.
  uint32_t          filled;       // Number of slots filled in blocks.
  volatile uint16_t run;
  uint8_t           tid;          // Simple thread id; starts at 0.
  char              name[37];
} Mut_t;

#define NUM(A) (sizeof(A) / sizeof(A[0]))

static uint32_t blockOK(Block_t * block, uint8_t tags) {    // Return non zero when block checks out OK.

  uint32_t  ok = mem2chunk(block->mem)->tags == tags;;
  uint8_t * c = block->mem;
  
  for (uint32_t i = 0; ok && i < block->size; i++, c++) {
    if (*c != block->fill) {
      ok = 0;
    }
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

  while (mut->run) {
    what = rand() & 0b11;

    switch (what) {    
      case 0: case 1: {                                     // Allocate.
        if (mut->filled < NUM(mut->blocks)) {
          do {
            s = (uint32_t) rand() % NUM(mut->blocks);
          } while (mut->blocks[s].mem);                     // Find a vacant slot.
          size = 1 + (uint32_t) (rand() % 256);             // Select a proper size.
          b = & mut->blocks[s];
          if (0 == what) {
            b->mem = umem->malloc(umem, size, mut->tid);    // Slow.
          }
          else {
            b->mem = umem->uncmalloc(umem, size, mut->tid); // Faster.
          }
          if (b->mem) {
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
        if (mut->filled) {
          do {
            s = (uint32_t) rand() % NUM(mut->blocks);
          } while (! mut->blocks[s].mem);                   // Find an occupied slot.
          b = & mut->blocks[s];
          assert(blockOK(b, mut->tid));
          if (2 == what) {
            umem->free(umem, b->mem);                       // Slow release.
          }
          else {
            umem->uncfree(umem, b->mem);                    // Faster release.
          }
          mut->filled--;                                    // Update slot and count.
          b->mem = NULL;
          mut->inuse -= b->size;
          mut->ops++;
        }
        break;
      }
    
    }
  }
  
  return NULL;
  
}

static void contCb(umemctx_t umem) {                        // Contention callback for sample.
  __atomic_add_fetch(& umem->count, 1, __ATOMIC_SEQ_CST);   // Keep a count on the contention.
  usleep(1000 * (1 + (uint32_t)rand() % 0x0f));             // Do some random backoff.
}

static const struct option Options[] = {
  { "numthreads", 1, NULL, 'n' },
  { "space",      1, NULL, 's' },
  { "help",       0, NULL, 'h' },
  { NULL,         0, NULL,  0  },
};

int main(int argc, char * argv[]) {

  uint32_t  numthr = 2;
  uint32_t  spacesz = 1024 * 64;
  uint8_t * space;
  int32_t   o;
  int32_t   ai;                                             // Argument index.
  uint32_t  i;
  Mut_t *   muts;
  UMemCtx_t UMem;
  uint32_t  inuse;
  uint32_t  seconds = 0;

  do {
    o = getopt_long(argc, argv, "hn:s:", Options, & ai);
    switch (o) {
      case 'n': {
        numthr = (uint32_t) atoi(optarg);
        numthr = numthr ? numthr : 1;
        numthr = numthr > 255 ? 255 : numthr;               // Apply some limit.
        break;
      }

      case 's': {
        spacesz = (uint32_t) atoi(optarg);
        spacesz = spacesz > 1024 ? spacesz : 1024;
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
  
  initUMemCtx(& UMem, space, spacesz);
  UMem.contcb = contCb;                                     // Override do nothing contention callback.

  for (i = 0; i < numthr; i++) {
    memset(& muts[i], 0x00, sizeof(Mut_t));
    sprintf(muts[i].name, "mut %u", i);
    muts[i].run = 1;
    muts[i].tid = (uint8_t) i;
    muts[i].umem = & UMem;
    pthread_create(& muts[i].thread, NULL, mutate, & muts[i]);
  }

  while (1) {
    sleep(1);
    uint32_t s = ++seconds;
    uint32_t d = s / (3600 * 24); s -= d * 3600 * 24;
    uint32_t h = s / 3600;        s -= h * 3600;
    uint32_t m = s / 60;          s -= m * 60;
    printf("%u chunks, %u cont/sec, %u day%s %u hour%s, %u minute%s, %u second%s.\n", 
        UMem.numchunks, UMem.count,
        d, d == 1 ? "" : "s",
        h, h == 1 ? "" : "s",
        m, m == 1 ? "" : "s", 
        s, s == 1 ? "" : "s");
    UMem.count = 0;
    inuse = 0;
    for (i = 0; i < numthr; i++) {
      printf("%s %"PRIu64" operations, %u in use.\n", muts[i].name, muts[i].ops, muts[i].inuse);
      inuse += muts[i].inuse;
    }
    printf("%u bytes in use of %u.\n", inuse, spacesz);
  }
   
  return 0;
  
}
