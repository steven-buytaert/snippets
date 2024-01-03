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

typedef struct Mut_t {            // Mutator Thread Context.
  umemctx_t         umem;         // Micro Memory to use.
  pthread_t         thread;
  void *            blocks[64];   // Collection of blocks.
  uint64_t          ops;          // Total operation count.
  uint32_t          filled;       // Number of slots filled in blocks.
  volatile uint16_t run;
  uint16_t          tid;          // Simple thread id; starts at 0.
  char              name[32];
} Mut_t;

#define NUM(A) (sizeof(A) / sizeof(A[0]))

static void * mutate(void * arg) {

  Mut_t *   mut = arg;
  uint32_t  what;
  uint32_t  sz;
  uint32_t  s;
  umemctx_t umem = mut->umem;

  while (mut->run) {
    what = rand() & 0b11;

    switch (what) {    
      case 0: case 1: {                                     // Allocate.
        if (mut->filled < NUM(mut->blocks)) {
          do {
            s = (uint32_t) rand() % NUM(mut->blocks);
          } while (mut->blocks[s]);                         // Find a vacant slot.
          sz = 1 + (uint32_t) (rand() % 256);               // Select a proper size.
          if (0 == what) {
            mut->blocks[s] = umem->malloc(umem, sz, 0);     // Slow.
          }
          else {
            mut->blocks[s] = umem->uncmalloc(umem, sz, 0);  // Faster.
          }
          if (mut->blocks[s]) { mut->filled++; mut->ops++; }
        }
        break;
      }
    
      case 2: case 3: {                                     // Release.
        if (mut->filled) {
          do {
            s = (uint32_t) rand() % NUM(mut->blocks);
          } while (! mut->blocks[s]);                       // Find an occupied slot.
          if (2 == what) {
            umem->free(umem, mut->blocks[s]);               // Slow release.
          }
          else {
            umem->uncfree(umem, mut->blocks[s]);            // Faster release.
          }
          mut->filled--; mut->blocks[s] = NULL; mut->ops++; // Update slot and count.
        }
        break;
      }
    
    }
  }
  
  return NULL;
  
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

  do {
    o = getopt_long(argc, argv, "hn:s:", Options, & ai);
    switch (o) {
      case 'n': {
        numthr = (uint32_t) atoi(optarg);
        numthr = numthr ? numthr : 1;
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

  for (i = 0; i < numthr; i++) {
    memset(& muts[i], 0x00, sizeof(Mut_t));
    sprintf(muts[i].name, "mut %u", i);
    muts[i].run = 1;
    muts[i].tid = (uint16_t) i;
    muts[i].umem = & UMem;
    pthread_create(& muts[i].thread, NULL, mutate, & muts[i]);
  }

  while (1) {
    sleep(1);
    printf("%u chunks.\n", UMem.numchunks);
    for (i = 0; i < numthr; i++) {
      printf("%s %"PRIu64" operations.\n", muts[i].name, muts[i].ops);
    }
  }
   
  return 0;
  
}
