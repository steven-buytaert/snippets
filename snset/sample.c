// Copyright 2021 Steven Buytaert

#include <snset.h>

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdalign.h>

typedef struct Obj_t * obj_t;

typedef struct Obj_t {            // Demo of a growable object.
  uint8_t     size;
  char        fill;               // Character we will fill the tail with.
  uint8_t     tail[0];            // Tail element with size fill characters and a \0.
} Obj_t;

static void * mem(snset_t set, void * mem, uint32_t sz) {

  return realloc(mem, sz);                                  // We use realloc as allocator.

}

/*

  Create a set in which several differently sized objects will be allocated
  and filled. Several reallocations will happen.

  Do NOT store object references when the set can still be reallocated as these
  references will become stale as a new memory block is returned from realloc. Only
  the index to the reference will be constant.

  The stretching of the last object is not shown as it is trivial.

*/

int main(int argc, char * argv[]) {

  SNSet_t  Set;
  snset_t  set = & Set;
  uint32_t i;
  uint32_t size;
  obj_t    obj;
  uint32_t align = alignof(Obj_t);
  char     fill[] = { 'A', 'B', 'C', 'D' };

  snset_init(set, mem);

  set->Grow.avail = 128;                                    // Ensure we have some object bytes to start with.
  set->Grow.num =     5;                                    // And some slots; use larger values to reduce reallocations.

  for (i = 0; i < 36; i++) {
    size = 1 + (random() % 99);                             // Some non zero tail size (must fit in uint8_t of obj->size).
    obj = set->obj(set, sizeof(Obj_t) + size + 1, align);   // Allocate with proper alignment; add one for trailing \0.
    obj->size = size;
    obj->fill = fill[random() % sizeof(fill)];
    memset(obj->tail, obj->fill, size);                     // Fill it with some readable junk; zero terminated.
                                                            // Do NOT store obj as realloc might invalidate the reference later!
  }

  for (i = 0; i < 36; i++) {
    obj = set->set[i];
    printf("%2u %2u '%s'\n", i, obj->size, obj->tail);
  }

  printf("Set size %u reallocs done %u\n", set->size, set->reallocs);

  return 0;
  
}
