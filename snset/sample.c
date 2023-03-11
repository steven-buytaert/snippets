// Copyright 2021-2023 Steven Buytaert

#include <snset.h>

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdalign.h>

typedef struct Obj_t * obj_t;

typedef struct Obj_t {            // Demo of a growable object.
  uint16_t    next;               // Offset to next object; in bytes.
  uint16_t    number;             // Consecutive number; starts with 0.
  uint8_t     size;
  char        fill;               // Character we will fill the tail with.
  uint8_t     tail[0];            // Tail element with size fill characters and a \0.
} Obj_t;

static obj_t obj2next(obj_t obj) {                          // Go over the list of objects via the next field.

  union {
    obj_t     obj;
    uint8_t * addr;
  } Obj = { .obj = obj };

  Obj.addr += obj->next;
  
  return obj->next ? Obj.obj : NULL;                        // When next is 0, we return NULL; end of the list.

}

static void setnext(obj_t this, const obj_t obj) {

  this->next = (uint8_t *) obj - (uint8_t *) this;

}

static void * mem(snset_t set, void * mem, uint32_t sz) {

  return realloc(mem, sz);                                  // We use realloc as allocator.

}

/*

  Create a set in which several differently sized objects will be allocated
  and filled. Several reallocations will happen.

  Do NOT store object references when the set can still be reallocated as these
  references will become stale if a new memory block is returned from realloc. Only
  the index to the reference will be constant and after each potential reallocation, the
  reference must be refetched from the set reference array, with the index.

  The stretching of an object is not shown.

*/

int main(int argc, char * argv[]) {

  SNSet_t  Set;
  snset_t  set = & Set;
  uint32_t i;
  uint32_t size;
  int32_t  total;
  obj_t    obj;
  obj_t    prev;
  uint32_t setsize = 99;                                    // Number of elements in the set.
  uint32_t align = alignof(Obj_t);
  char     fill[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G' };

  snset_init(set, mem);

  set->Grow.bytes = 128;                                    // Ensure we have some free object bytes to start with.
  set->Grow.slots =   5;                                    // And some slots; use larger values to reduce reallocations.

  for (i = 0; i < setsize; i++) {
    size = 1 + (random() % 99);                             // Some non zero tail size (must fit in uint8_t of obj->size).
    obj = set->obj(set, sizeof(Obj_t) + size + 1, align);   // Allocate with proper alignment; add one for trailing \0.
    obj->number = i;
    obj->size = size;
    obj->fill = fill[(uint32_t) random() % sizeof(fill)];
    memset(obj->tail, obj->fill, size);                     // Fill it with some readable junk; zero terminated.
                                                            // Do NOT store obj as realloc might invalidate the reference later!
  }

  for (i = 0; i < setsize; i++) {                           // We can now address all objects via the set array.
    obj = set->set[i];
    assert(obj->number == i);                               // Order of allocation is preserved.
    if (i > 0) {
      prev = set->set[i - 1];                               // Previous object in the set.
      assert(prev->number == i - 1);
      setnext(prev, obj);                                   // Set the offset to navigate the list later.
    }
    printf("%2u %2u '%s'\n", obj->number, obj->size, obj->tail);
  }

  total = set->addr4next - set->addr4Set;                   // Number of bytes used by allocated objects.

  printf("Set size %u (%d for obj) reallocs done %u\n", set->size, total, set->reallocs);

  set->seal(set);                                           // Shrink set to used objects only.

  printf("Set size now %u.\n", set->size);

  for (obj = set->Set; obj; obj = obj2next(obj)) {          // Go over list via builtin object to next offset.
    printf("%2u %2u '%s'\n", obj->number, obj->size, obj->tail);
  }

  return 0;
  
}
