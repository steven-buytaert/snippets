// Copyright 2021-2023 Steven Buytaert

#include <snset.h>
#include <assert.h>
#include <string.h>

/*

  Memory layout of the SNSet structure.
  
     -------------------+------------------+------- Single block of memory allocated at Set.
     Set -------------->|                  |<---+
     of objects         |                  |    |
                        +------------------+    |
                        |                  |<---|----+
                        +------------------+    |    |
                        |                  |    |    |
                        |                  |<---|----|----+
                        |                  |    |    |    |
 ------------------+----+------------------+--- | -- | -- | --------- can be snipped here later.
     next ---------|--->|                  |    |    |    |
                   |    :                  :    |    |    |
              avail|    :                  :    |    |    |
     --------------+----+------------------+--  |    |    |
     set ----------|--->| 0 |              |----+    |    |
     of references |    +------------------+         |    |
                   |    | 1 |              |---------+    |
                   |    +------------------+              |
                num|    | 2 |              |--------------+
                  -+----+------------------+--    
                   |    | 3 |              |
                   |    +------------------+
                   |    :   :              :
                   |    :   :              :
                   |    +------------------+
          freeslots|    | x :              |
     --------------+----+------------------+--    

  The array of references to refer to the objects, during construction, is at
  the end of the whole block of memory that starts at Set. So at the end of the
  construction, if the objects have an internal mechanism to navigate from one object
  to the other, the array of references can be realloced away, as well as the free
  object space at next.

*/

typedef union {
  void *    any;
  uint8_t * addr;
  intptr_t  align;
} Ref_t;

static uint32_t roundup(uint32_t value, uint32_t pot) {     // Round up to a power of two.
  return (value + (pot - 1)) & ~(pot - 1);
}

static uint32_t padding(uint8_t * addr, uint32_t a4o) {     // Calculate number of padding bytes required; a4o must be a power of 2!

  Ref_t    oadd = { .addr = addr };                         // Original address.
  Ref_t    mask = { .addr = NULL };                         // Alignment requirement as a mask.
  uint32_t padding = 0;                                     // Padding bytes; preset with value when addr == NULL.

  assert(a4o == roundup(a4o, 2));                           // It must be a power of two.

  if (addr) {  
    mask.align = a4o - 1;                                   // Prepare the mask.
    oadd.align &= mask.align;
    padding = (a4o - oadd.align) & mask.align;
  }

  assert(padding == 0 || padding <= a4o - 1);               // By definition.

  return padding;

}

static uint32_t abso(int32_t diff) {                        // Return the difference as an absolute value (unsigned).
  return diff > 0 ? (uint32_t) diff : (uint32_t) -diff;
}

static void seal(snset_t set) {                             // Seal off a set.

  uint8_t * block = set->Grow.block;
  uint32_t  size = set->addr4next - set->Grow.block;        // Number of bytes used by allocated objects and alignment.
  Ref_t     NewSet;
  Ref_t     OldSet = { .any = set->Set };
  uint32_t  i = 0;
  intptr_t  off = OldSet.addr - set->Grow.block;            // Offset between old block and old set; Previously added for alignment.
  intptr_t  diff;
  intptr_t  off2first = set->addr4ref[0] - OldSet.addr;     // Difference between first object and current start of the set.

  assert(off >= 0 && off < set->wca);                       // Must be a positive value and smaller than the worst case alignment. 
  assert(size >= off);

  block = set->mem(set, block, size + set->wca);

  assert(block);                                            // Shrink should always work.

  NewSet.addr = block;

  diff = NewSet.addr - OldSet.addr;
  
  i = abso((int32_t)(roundup(diff, set->wca) - diff));      // Number of bytes to add to reach proper worst case alignment.

  NewSet.addr += i;                                         // Align the address to the worst case alignment.

  assert(i < set->wca);

  if (i || off) {
    memmove(NewSet.addr, block + off, (size_t)(size - off));// Move the actual data into the proper position.
  }

  set->Grow.block = block;
  set->addr4Set = NewSet.addr + off2first;                  // First object should be at addr4Set, therefore + off2first.
  set->size = size;
  set->Grow.size = size + set->wca;
  set->Grow.locked = 1;
  set->avail = 0;
  set->freeslots = 0;
  set->addr4set = NULL;                                     // Doesn't exist no longer.

}

static void * resize(snset_t set, uint32_t extra) {         // Grow the whole block with extra bytes; return the new aligned Set pointer.

  uint8_t * old = set->Grow.block;
  Ref_t     NewSet;
  Ref_t     OldSet = { .any = set->Set };
  uint32_t  i = 0;
  intptr_t  off = OldSet.addr - set->Grow.block;            // Offset between old block and old set; Previously added for alignment.
  intptr_t  diff;

  assert(off >= 0 && off < set->wca);                       // Must be a positive value and smaller than the worst case alignment. 

  if (extra + set->wca <= set->Grow.size - set->size) {     // If the growth required is smaller than the accumulated alignment extras ...
    set->padgrowth++;
    return OldSet.any;                                      // ... we don't need to grow the block.
  }

  set->Grow.block = set->mem(set, old, set->Grow.size + extra + set->wca);

  if (! set->Grow.block) {                                  // Allocation failed.
    set->Grow.block = old;
    return NULL;
  }

  NewSet.addr = set->Grow.block;

  diff = NewSet.addr - OldSet.addr;
  
  i = abso((int32_t)(roundup(diff, set->wca) - diff));      // Number of bytes to add to reach proper worst case alignment.

  NewSet.addr += i;                                         // Align the address to the worst case alignment.

  assert(i < set->wca);

  if (i || off) {
    memmove(NewSet.addr, set->Grow.block + off, set->size); // Move the actual data into the proper position.
  }

  set->reallocs++;

  set->Grow.size += extra + set->wca;

  return NewSet.any;

}

static void * ensure(snset_t set, uint32_t nos, uint32_t nob) {

  Ref_t       Item;
  intptr_t    off;
  uint32_t    b2m;
  uint32_t    add;                                          // Growth in bytes.
  uint32_t    i;
  uint8_t *   old = set->addr4Set;                          // The old Set block.

  if (set->freeslots < nos || set->avail < nob) {
    nos = set->freeslots < nos ? nos : 0;
    nob = set->avail < nob ? nob : 0;

    if (nob && nob < set->Grow.bytes) { nob = set->Grow.bytes; }
    if (nos && nos < set->Grow.slots) { nos = set->Grow.slots; }

    nob = roundup(nob, alignof(void *));                    // We want the set pointer to stay properly aligned.
    add = nob + nos * sizeof(void *);                       // Growth required for slots and object bytes.
    add = roundup(add, set->wca);                           // Make sure the size is properly aligned.
    set->Set = resize(set, add);
    if (set->Set) {
      off = set->addr4Set - old;                            // Offset to add to any reference.

      set->addr4next += off;
      set->addr4set  += off + nob;                          // Reference set has been pushed down nob bytes wrt. Set.

      if (old && nob) {                                     // Not when set is being created or only reference slots added.
        b2m = set->num * sizeof(void *);                    // Number of reference slot bytes we need to move.
        memmove(set->addr4set, set->addr4set - nob, b2m);
      }

      for (i = 0; off && i < set->num; i++) {               // Adjust all references, if needed (offset not 0).
        Item.any    = set->set[i];
        Item.addr  += off;
        set->set[i] = Item.any;
      }

      set->freeslots += nos;
      set->avail     += nob;
      set->size      += add;

    }
  }

  assert(0 == padding(set->addr4set, sizeof(void *)));      // This address should always remain pointer aligned.

  return set->Set;

}

static void * obj(snset_t set, uint32_t sze, uint32_t a4o) {

  void *   obj = NULL;
  uint32_t pad = padding(set->addr4next, a4o);              // Calculate amount of padding required.

  assert(set->addr4next + set->avail == set->addr4set);     // At the end of the free object space, the set array starts.

  a4o = roundup(a4o, 2);                                    // Ensure alignment it is really a power of two.

  assert(set->wca);                                         // Worst case alignment can never be 0.
  if (set->wca < a4o) { set->wca = a4o; }                   // Track worst case alignment.

  sze = roundup(sze, a4o); assert(a4o);                     // Round up to proper alignment.

  if (0 == set->freeslots || set->avail <= sze + pad) {     // Grow the set if not large enough.
    if (! set->Grow.locked) {
      ensure(set, set->Grow.slots, 16 * (pad + sze));       // If we need to grow, grow some extra.
      pad = padding(set->addr4next, a4o);                   // Recalculate padding; addr4next has changed!
    }
  }

  if (set->freeslots && set->avail > sze + pad) {
    assert(set->addr4next + pad + sze < set->addr4set);     // Should never run into the reference array.
    memset(set->addr4next, 0x00, pad);                      // Clear the padding area, might be used by grow later.
    set->addr4next += pad;                                  // Apply proper padding before we ...
    obj = set->next;                                        // ... allocate the object.
    memset(obj, 0x00, sze);
    set->set[set->num++] = obj;
    set->addr4next += sze;
    set->freeslots--;
    set->avail -= (pad + sze);
  }

  return obj;
  
}

static void * grow(snset_t set, uint32_t x, uint32_t add) { // Grow set->set[x] object, possibly in the middle of the set, with 'add' bytes.

  void *    obj = NULL;
  Ref_t     Item;
  uint32_t  i;
  uint32_t  last = (x + 1 == set->num) ? 1 : 0;
  uint8_t * next;                                           // First byte after the grown object.
  uint32_t  bytes2move;

  assert(x < set->num);                                     // Must be inside the set.

  add = roundup(add, set->wca);                             // Roundup to worst case element alignment for this set.

  if (set->avail < add) {                                   // Grow the set if not large enough.
    if (! set->Grow.locked) {
      ensure(set, 0, add);                                  // We don't need extra reference slots.
    }
  }

  if (set->avail >= add) {                                  // Stretch when there's room.
    obj = set->set[x];
    set->avail -= add;

    if (last) {                                             // We need to grow the last object in the set; nothing needs moving.
      next = set->addr4next;
    }
    else {
      next = set->set[x + 1];
      bytes2move = set->addr4next - next;                   // Bytes of following objects we need to move down to grow this object.
      memmove(add + next, next, bytes2move);                // Make room for the grown object by moving bytes down.
    }

    memset(next, 0x00, add);                                // Clear the grown area, after the potential memmove.

    for (i = x + 1; i < set->num; i++) {                    // Now adjust all lower reference slots, if any.
      Item.any    = set->set[i];
      Item.addr  += add;
      set->set[i] = Item.any;
    }

    set->addr4next += add;
      
  }

  return obj;
  
}

void snset_init(snset_t set, snsetmem_t mem) {

  memset(set, 0x00, sizeof(SNSet_t));

  set->mem = mem;
  set->obj = obj;
  set->ensure = ensure;
  set->stretch = grow;
  set->seal = seal;
  set->wca = 1;                                             // Smallest alignment possible.

}
