// Copyright 2021 Steven Buytaert

#include <snset.h>
#include <assert.h>
#include <string.h>

#include <stdio.h>

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

  a4o = roundup(a4o, 2);                                    // Ensure it is really a power of two.

  if (addr) {  
    mask.align = a4o - 1;                                   // Prepare the mask.
    oadd.align &= mask.align;
    padding = (a4o - oadd.align) & mask.align;
  }

  assert(padding == 0 || padding <= a4o - 1);               // By definition.

  return padding;

}

static void * ensure(snset_t set, uint32_t noe, uint32_t nob) {

  Ref_t       Item;
  intptr_t    off;
  uint32_t    b2m;
  uint32_t    add;                                          // Growth in bytes.
  uint32_t    i;
  uint8_t *   old = set->addr4Set;                          // The old memory block.

  nob = roundup(nob, sizeof(void *));                       // We want the set pointer to stay properly aligned.

  if (set->free < noe || set->avail < nob) {
    noe = set->free < noe ? noe : 0;
    nob = set->avail  < nob ? nob : 0;

    add = nob + noe * sizeof(void *);                       // Growth required for slots and object bytes.
    if (add < set->Grow.avail) { add = set->Grow.avail; }   // Ensure proper growth.
    add = roundup(add, 8);                                  // Make sure the size is properly aligned.
    set->Set = set->mem(set, old, set->size + add);
    if (set->Set) {
      off = set->addr4Set - old;                            // Offset to add to any reference.

      set->addr4next += off;
      set->addr4set  += off + nob;                          // Set has been pushed down nob bytes wrt. Set.

      if (old && nob) {                                     // Not when set is being created or only reference slots added.
        b2m = set->num * sizeof(void *);                    // Number of reference bytes we need to move.
        memmove(set->addr4set, set->addr4set - nob, b2m);
      }
      for (i = 0; off && i < set->num; i++) {
        Item.any    = set->set[i];
        Item.addr  += off;
        set->set[i] = Item.any;
      }

      set->free  += noe;
      set->avail += nob;
      set->size  += add;

      set->reallocs++;
    }
  }

  assert(0 == padding(set->addr4set, sizeof(void *)));      // This address should always remain pointer aligned.

  return set->Set;

}

static void * stretch(snset_t set, uint32_t add) {          // Stretch the last object.

  void * obj = NULL;

  if (set->avail <= add) {
    if (! set->Grow.locked) {
      ensure(set, set->Grow.num, 16 * add);                 // If we need to grow, grow some extra.
    }
  }

  if (set->avail > add) {
    memset(set->addr4next, 0x00, add);                      // Clear the area.
    set->addr4next += add;
    set->avail     -= add;
    obj = set->set[set->num - 1];
  }
  
  return obj;

}

static void * obj(snset_t set, uint32_t sze, uint32_t a4o) {

  void *   obj = NULL;
  uint32_t pad = padding(set->addr4next, a4o);              // Calculate amount of padding required.

  assert(set->addr4next + set->avail == set->addr4set);

  sze = roundup(sze, a4o); assert(a4o);                     // Round up to proper alignment.

  if (0 == set->free || set->avail <= sze + pad) {          // Grow the set if not large enough.
    if (! set->Grow.locked) {
      ensure(set, set->Grow.num, 16 * (pad + sze));         // If we need to grow, grow some extra.
      pad = padding(set->addr4next, a4o);                   // Recalculate padding; addr4next has changed!
    }
  }

  if (set->free && set->avail > sze + pad) {
    assert(set->addr4next + pad + sze < set->addr4set);     // Should never run into the reference array.
    set->addr4next += pad;
    obj = set->next;
    memset(obj, 0x00, sze);
    set->set[set->num++] = obj;
    set->addr4next += sze;
    set->free--;
    set->avail -= (pad + sze);
  }

  return obj;
  
}

void snset_init(snset_t set, snsetmem_t mem) {

  memset(set, 0x00, sizeof(SNSet_t));

  set->mem = mem;
  set->obj = obj;
  set->ensure = ensure;
  set->stretch = stretch;

}
