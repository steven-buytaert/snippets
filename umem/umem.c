// Copyright 2024 Steven Buytaert

#include <umem.h>
#include <string.h>

// See https://gcc.gnu.org/wiki/Atomic for extensive information and the GCC manual on
// atomics at https://gcc.gnu.org/onlinedocs/gcc/_005f_005fatomic-Builtins.html

#define CAX(P, X, D) __atomic_compare_exchange(P, X, D, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)

static uint32_t trylock(chunk_t chunk) {

  Chunk_t Exp = { .header = chunk->header };
  Chunk_t Des = { .header = chunk->header };

  Exp.lock = 0;
  Des.lock = 1;

  return CAX(& chunk->header, & Exp.header, & Des.header);

}

static void unlock(chunk_t chunk) {

  Chunk_t Exp = { .header = chunk->header };
  Chunk_t Des = { .header = chunk->header };

  assert(chunk->lock);                                      // Must have been locked before.

  do {
    Exp.lock = 1;
    Des.lock = 0;
  } while (! CAX(& chunk->header, & Exp.header, & Des.header));

}

static void atomic_inc32(uint32_t * where) {
  __atomic_add_fetch(where, 1, __ATOMIC_SEQ_CST);
}

static void atomic_dec32(uint32_t * where) {
  __atomic_sub_fetch(where, 1, __ATOMIC_SEQ_CST);
}

static uint32_t roundup(uint32_t value, uint32_t po2) {     // Round up to a given power of 2.
  return (value + (po2 - 1)) & ~(po2 - 1);
}

static uintptr_t aroundup(uintptr_t value, uintptr_t po2) { // Round up the passed address to a given power of 2.
  return (value + (po2 - 1)) & ~(po2 - 1);
}

static chunk_t split(chunk_t c2s, uint32_t size) {          // Split at given size; return remainder.

  Mem_t    Succ = { .chunk = c2s };
  uint32_t sucsize = c2s->size - size - chunkhdrsz;         // Successor size.

  assert(c2s->lock);                                        // Must be locked.
  assert(size == roundup(size, chunkhdrsz));                // Should have been properly rounded already.
  assert(c2s->size >= size);                                // Chunk should be big enough for requested split.

  assert(sucsize >= minchunksize + 4);                      // +4 amount we might need for alignment.
  Succ.addr += size + chunkhdrsz;

  if (! alignedok(Succ.chunk->u08)) {
    Succ.addr += 4;
    sucsize   -= 4;
    size      += 4;
  }

  Succ.chunk->header = 0;                                   // Clear the whole header size/ciu/lock/pif.
  Succ.chunk->size = iusize(sucsize);
  c2s->size = iusize(size);
  assert(chunk2succ(c2s) == Succ.chunk);                    // Check our size calculations.
  assert(alignedok(Succ.chunk->u08));

  return Succ.chunk;

}

static chunk_t merge(chunk_t c2m, chunk_t suc) {            // Merge chunk with successor; return new successor.

  assert(c2m->lock && suc->lock);                           // Both must be locked.
  assert(chunk2succ(c2m) == suc);                           // Must really be the successor.

  c2m->size = iusize(c2m->size + suc->size + chunkhdrsz);   // First set proper final size.

  return chunk2succ(c2m);                                   // Return new successor.

}

static uint32_t start2end(umemiter_t iter, chunk_t start, chunk_t end) {

  chunk_t  c;
  uint32_t stat = UMemIt_Unlock;                            // If initial successor lock fails.
  uint32_t keepsucc = 0;

  for (c = start; 1; c = iter->succ) {
    assert(c->lock);
    iter->succ = chunk2succ(c);
    if (! trylock(iter->succ)) {
      if (stat == UMemIt_Unlock) { unlock(c); }
      return 0;
    }

    stat = iter->cb(iter, c);

    switch (stat) {
      case UMemIt_Stop: { return 0; }

      case UMemIt_Unlock: {
        if (keepsucc) {
          keepsucc = 0;
        }
        else {
          unlock(c);
        }
        break;
      }

      case UMemIt_Keep: {                                   // In case callback wants to keep the lock ...
        keepsucc = 1;                                       // ... we also keep the lock on the successor.
        break;
      }

      default: {                                            // Bad answer; unlock all and return incomplete.
        unlock(c);
        unlock(iter->succ);
        return 0;
      }
    }

    if (iter->succ == end) {
      if (stat == UMemIt_Unlock) { unlock(iter->succ); }
      return 1;
    }
  }

}

static uint32_t iterate(umemiter_t iter) {                  // Iterator; return non zero when full scan.

  umemctx_t umem = iter->umem;
  uint32_t  full = 1;

  if (iter->start < umem->end) {                            // Forward scan part; starting chunk to end.
    if (! trylock(iter->start)) {
      return 0;
    }
    full = start2end(iter, iter->start, umem->end);
  }

  if (full) {                                               // Only if the first part did a complete iteration.
    if (iter->start > umem->start) {                        // We started in the middle.
      if (! trylock(umem->start)) {
        return 0;
      }
      full = start2end(iter, umem->start, iter->start);
    }
  }

  return full;

}

static void freechunk(umemctx_t umem, chunk_t c2free) {     // Slow path to release a chunk.

  chunk_t  succ, succ2;
  chunk_t  pred;
  uint32_t bothlocked = 0;

  assert(! umem->start->pif && umem->end->ciu);             // Invariant check.
  assert(! umem->end->size && ! umem->end->u32[0]);

  assert(c2free);

  do {
    if (trylock(c2free)) {
      succ = chunk2succ(c2free);
      assert(c2free->ciu);
      assert((c2free->u32[0] = 0xffffffff, 1));             // When debugging, taint first word of user mem.

      if (trylock(succ)) {
        bothlocked = 1;
        while (! succ->ciu) {                               // Forward coalescing; end chunk has ciu set ...
          succ2 = chunk2succ(succ);                         // ... so it will stop there.
          if (! trylock(succ2)) { break; }                  // Try again next time.
          merge(c2free, succ);
          assert((succ->header = 0, 1));                    // Clear when debugging; succ no longer exists.
          atomic_dec32(& umem->numchunks);
          succ = succ2;                                     // Succ2 is now the new successor.
        }

        while (c2free->pif) {                               // Backward coalescing; start chunk has pif ...
          pred = c2free->prev;                              // ... clear, so will stop there.
          if (! trylock(pred)) { break; }                   // Try again next time.
          assert(! pred->ciu);                              // Can not be in use when pif is set in our chunk.
          merge(pred, c2free);                              // Returned succ is invariant! Same as before.
          atomic_dec32(& umem->numchunks);
          assert((c2free->header = 0, 1));                  // Clear when debugging; no longer exists.
          c2free = pred;                                    // c2free has been assimilated...
        }

        succ->prev = c2free;
        succ->pif = 1;
        c2free->ciu = 0;                                    // No longer in use.
        unlock(succ);
      }
      unlock(c2free);                                       // Done or redoing. Release lock in any case.
    }
    if (! bothlocked) { umem->contcb(umem); }               // Maybe have some backoff.
  } while (! bothlocked);

  assert(! umem->start->pif && umem->end->ciu);             // Invariant check.
  assert(! umem->end->size && ! umem->end->u32[0]);

}

typedef struct List_t * list_t;

typedef struct List_t {           // To keep a linked list of to be freed chunks.
  chunk_t next;
} List_t;

static chunk_t chunk2next(chunk_t chunk) {                  // From a chunk, return the next on the list.
  return ((list_t) chunk->u08)->next;
}


static void clean(umemctx_t umem) {                         // Release chunks of to be freed list, if any.

  chunk_t c2free;
  chunk_t next;
  chunk_t des;

  do {                                                      // Atomically unlink the c2free list.
    c2free = umem->c2free;
    des = NULL;
  } while (! CAX(& umem->c2free, & c2free, & des));

  for ( ; c2free; c2free = next) {                          // If there are chunks to be freed, free them.
    next = chunk2next(c2free);                              // Get next *before* freeing the chunk.
    freechunk(umem, c2free);
  }

}

void ufree(umemctx_t umem, void * mem) {                    // Slow path to release memory.

  chunk_t chunk;

  clean(umem);                                              // Clean any from the free list first.

  if (mem) {
    chunk = mem2chunk(mem);
    assert(chunk->ciu);
    freechunk(umem, chunk);
  }

}

static const uint32_t enough2split = minchunksize + 2 * chunkhdrsz;

static void * iter2mem(umemiter_t iter, uint8_t tags) {     // Use found iterator chunk for memory, if any.

  umemctx_t umem = iter->umem;
  chunk_t   rem;
  chunk_t   succ;

  if (iter->found) {
    assert(iter->found->lock);                              // Must have been locked by callback.
    assert(iter->found->ciu);                               // Must have been claimed by iterator callback.
    assert(iter->succ2found);                               // Must have a successor.
    succ = iter->succ2found;
    assert(succ->lock);                                     // Iterator guarantee.
    assert(succ->pif);                                      // Since its predecessor is free.
    if (iter->found->size - iter->size >= enough2split) {   // Big enough remainder to split.
      rem = split(iter->found, iter->size);
      assert((trylock(rem), 1));                            // Only for chunk2succ in next assert.
      assert(succ == chunk2succ(rem));                      // its successor has been locked already, but ...
      assert((unlock(rem), 1));
      succ->prev = rem;                                     // ... it has a new predecessor.
      atomic_inc32(& umem->numchunks);
    }
    else {
      succ->pif = 0;
    }
    iter->found->tags = tags;
    unlock(iter->found);                                    // Unlock in same sequence.
    unlock(succ);
  }

  assert(! umem->start->pif && umem->end->ciu);             // Invariant check.
  assert(! umem->end->size && ! umem->end->u32[0]);

  return iter->found ? iter->found->u08 : NULL;

}

// Perform a search scan for a chunk; if needcomplete is non zero, a full scan
// is required untill the tie breaking point. The needcomplete is *only* relevant
// when a chunk was found already!

static void search4chunk(UMemIter_t *iter, uint32_t sz, uint32_t needcomplete) {

  umemctx_t  umem = iter->umem;
  chunk_t    succ;
  uint32_t   complete;
  uint32_t   count = 0;
  uint32_t   breakat = umem->breakat;                       // Take snapshot.

  clean(umem);                                              // Do any cleanup first.

  if (sz < minchunksize) { sz = minchunksize; }             // Ensure we have a proper size.
  if (sz > iusize(sz)) { iter->found = NULL; return; }      // Too large.
  iter->size = roundup(sz, chunkhdrsz);

  do {
    iter->found = NULL;
    iter->start = umem->start;
    complete = iterate(iter);
    if (! complete) {
      if (iter->found) {
        assert(iter->found->lock);                          // Both found and successor should be locked.
        succ = iter->succ2found;
        assert(succ->lock);
        if (! needcomplete) { break; }                      // Full scan not needed.
        if (++count > breakat) {                            // We tried enough times, this one is it.
          umem->tiesbroken++;
          break;
        }
        iter->found->ciu = 0;                               // Release claim again.
        unlock(iter->found);                                // Let others make progress; unlock in same ...
        unlock(succ);                                       // ... sequence.
      }
      umem->contcb(umem);
    }
  } while (! complete && needcomplete);

}

static UMemItStat_t bestFitCb(umemiter_t iter, chunk_t c) { // Best fit walking over all chunks.

  UMemItStat_t status = UMemIt_Unlock;

  assert(c->lock);
  assert(iter->succ == chunk2succ(c));
  assert(iter->succ->lock);

  if (! c->ciu && c->size >= iter->size) {                  // Free and big enough?
    if (iter->found && c->size < iter->found->size) {       // A tighter fit.
      assert(iter->found->lock);                            // Both found and ...
      assert(iter->succ2found->lock);                       // its successor should have been kept locked.
      iter->found->ciu = 0;                                 // We no longer claim it in use.
      unlock(iter->found);                                  // Unlock the previous ones, first found ...
      if (c != iter->succ2found) {
        unlock(iter->succ2found);                           // ... then successor. Same order as locking.
      }
      iter->found = NULL;                                   // This will ensure c is added below.
    }

    if (! iter->found) {
      iter->found = c;
      iter->succ2found = iter->succ;
      c->ciu = 1;                                           // Claim it in use already.
      status = UMemIt_Keep;                                 // Continue and keep it locked.
    }
  }

  return status;

}

void * umalloc(umemctx_t umem, uint32_t sz, uint8_t tags) { // Slow path umalloc.

  UMemIter_t Iter = {
    .umem = umem,
    .cb   = bestFitCb,
  };

  search4chunk(& Iter, sz, 1);                              // Search for a fitting chunk; complete scan.

  return iter2mem(& Iter, tags);

}

#if defined(UMEMFAST)

static UMemItStat_t firstFitCb(umemiter_t iter, chunk_t c) {// First fit walking over all chunks.

  UMemItStat_t status = UMemIt_Unlock;

  assert(c->lock);
  assert(iter->succ == chunk2succ(c));
  assert(iter->succ->lock);

  if (! c->ciu && c->size >= iter->size) {                  // Free and big enough?
    iter->found = c;
    iter->succ2found = iter->succ;
    c->ciu = 1;                                             // Claim it in use already.
    status = UMemIt_Stop;                                   // Keep the chunk and stop here.
  }

  return status;

}

// Least contented path umalloc. If this returns NULL, it does not necessarily
// means we are out of memory. See the count maximum below.

void * umalloc_fast(umemctx_t umem, uint32_t sz, uint8_t tags) {

  UMemIter_t Iter = {
    .umem = umem,
    .cb   = firstFitCb,
  };

  uint32_t  count = 0;

  if (sz < minchunksize) { sz = minchunksize; }             // Ensure we have a proper size.
  if (sz > iusize(sz)) { return NULL; }                     // Too large.
  Iter.size = roundup(sz, chunkhdrsz);

  Iter.found = NULL;

  do {
    Iter.start = umem->start;
    if (iterate(& Iter)) break;                             // If we had a full scan, don't retry.
  } while (! Iter.found && ++count < 2);                    // Try 2 times only for a complete scan.

  return iter2mem(& Iter, tags);

}

void ufree_fast(umemctx_t umem, void * mem) {               // Uncontended path to release memory.

  chunk_t chunk;
  list_t  list;
  chunk_t exp;

  assert(sizeof(List_t) <= minchunksize);                   // Must fit in the minimum chunk.

  if (mem) {
    list = (list_t) mem;
    chunk = mem2chunk(mem);
    assert(chunk->ciu);
    do {                                                    // Atomically insert in the list.
      list->next = umem->c2free;                            // Take snapshot and reuse ...
      exp = list->next;                                     // ... here; do *NOT* read umem->c2free again!
    } while (! CAX(& umem->c2free, & exp, & chunk));
  }

}

#endif // UMEMFAST

#if defined(UREALLOC)

static void locktuple(umemctx_t ctx, chunk_t chunk, chunk_t succ) {

  uint32_t bothlocked;

#if ! defined(NDEBUG)
  uint32_t maxtries = 100;                                  // This value depends also on the contcb implementation!
#endif

  do {                                                      // Try to get both chunks locked, avoiding deadlock.
    ctx->contcb(ctx);                                       // Let others make progress; both unlocked.
    bothlocked = 0;
    if (trylock(chunk)) {
      if (trylock(succ)) {
        bothlocked = 1;
      }
      else {
        unlock(chunk);
      }
    }
    assert(--maxtries);                                     // Ensure we aren't deadlocked.
  } while (! bothlocked);

}

void * urealloc(umemctx_t ctx, void * mem, uint32_t size, uint8_t tags) {

  chunk_t  chunk;
  chunk_t  succ;
  chunk_t  rem;
  uint32_t bothlocked;
  void *   newmem = NULL;

  clean(ctx);                                               // Clean any from the free list first.

  if (mem) {
    chunk = mem2chunk(mem);
    assert(chunk->ciu);
    if (0 == size) {                                        // Work as free.
      freechunk(ctx, chunk);
      mem = NULL;
    }
    else if (size > iusize(size)) {                         // Too big for this allocator.
      mem = NULL;
    }
    else {                                                  // Grow or shrink.
      if (size < minchunksize) { size = minchunksize; }     // Ensure we have a proper size.
      size = roundup(size, chunkhdrsz);                     // Align size requirement first.
      if (chunk->size >= size) {                            // Shrink the current chunk.
        if (chunk->size - size >= enough2split) {           // Remainder big enough to split off?
          do {
            bothlocked = 0;
            if (trylock(chunk)) {
              succ = chunk2succ(chunk);
              if (trylock(succ)) {
                bothlocked = 1;
                rem = split(chunk, size);
                succ->prev = rem;
                succ->pif = 1;
                atomic_inc32(& ctx->numchunks);
              }
              else {                                        // Could not lock successor.
                unlock(chunk);                              // Release lock for others to make progress.
                ctx->contcb(ctx);
              }
            }
          } while (! bothlocked);                           // Retry locking.
          unlock(succ);
          unlock(chunk);
        }
      }
      else {                                                // Enlarge current chunk or find a new one.
        do {
          bothlocked = 0;
          if (trylock(chunk)) {
            succ = chunk2succ(chunk);
            if (trylock(succ)) {
              bothlocked = 1;
              if (! succ->ciu) {                            // Only if successor isn't being used.
                if (chunk->size + succ->size >= size) {     // We can grow into the successor!
                  newmem = mem;                             // So we don't need a new memory block.
                  succ = merge(chunk, succ);                // Essentially both merge; succ is the new successor!
                  atomic_dec32(& ctx->numchunks);           // Since we merged with our successor.
                  if (! trylock(succ)) {                    // Try to get a lock on the new successor.
                    unlock(chunk);
                    locktuple(ctx, chunk, succ);            // Unlock chunk and get both locks.
                  }
                  succ->pif = 0;                            // Set already in case we don't split next.
                  if (chunk->size - size >= enough2split) { // Can we split of a remainder chunk?
                    rem = split(chunk, size);
                    succ->prev = rem;
                    succ->pif = 1;
                    atomic_inc32(& ctx->numchunks);         // One more chunk again, since we just split.
                  }
                }
              }
            }
            else {                                          // Could not lock successor.
              unlock(chunk);                                // Release lock for others to make progress.
              ctx->contcb(ctx);
            }
          }
        } while (! bothlocked);                             // Retry locking.
        unlock(succ);
        unlock(chunk);
        if (! newmem) {                                     // Need to find a new memory block.
          newmem = umalloc(ctx, size, chunk->tags);
          if (newmem) {                                     // Copy over the old contents.
            memcpy(newmem, mem, chunk->size);
            freechunk(ctx, chunk);                          // Release the old chunk.
            mem = newmem;
          }
          else {
            mem = NULL;                                     // Reallocation failed.
          }
        }
      }
    }
  }
  else {                                                    // Work as malloc.
    mem = umalloc(ctx, size, tags);
  }

  return mem;

}

#endif // UREALLOC

#if defined(UAMALLOC)

/*
   Check if we can split of a chunk, so to create an aligned
   chunk as a result of the split. Example

   chunk_t  req = NULL;                     // Chunk we want to be aligned.
   uint32_t size = 512;                     // We need a 512 byte chunk ...
   if (split4align(chunk, & size, 256)) {   // ... aligned on a 256 byte boundary.
     if (size) {
       req = split(chunk, size);            // Worked, so we can do the split.
     }
     else {
       req = chunk;                         // No need to split; already aligned.
     }
   }
   else {                                   // Chunk hasn't proper characteristics.
     ...
   }

   If the funtion returns non zero, the chunk->size equals size, b->size is at
   least 512 byte and the address of b is aligned to a 256 byte boundary.

   The funtion only checks for alignment and size of the given chunk; not if it
   is free or not, nor lock status (it should be locked of course). Also the
   passed alignment must be a power of 2 already. If no chunk needs to be split
   off the chunk, the returned size is set to 0.
*/

static uint32_t split4align(chunk_t chunk, uint32_t *size, uint32_t align) {

  if (chunk->size < *size) { return 0; }                    // Quick check, not big enough.

  Mem_t Mem = { .addr = & chunk->u08[0] };

  if (! (Mem.check & (align - 1))) { *size = 0; return 1; } // Already aligned, no need to split.

  Mem.addr += enough2split;                                 // We need at least this to split.

  Mem.calc = aroundup(Mem.check, align);

  uint32_t b2r = (uint32_t)(Mem.addr - chunk->u08);         // Bytes required for rounding up to alignment.

  b2r -= chunkhdrsz;                                        // Take into account the header for the new chunk.

  if (chunk->size <= b2r + *size) { return 0; }             // Chunk not big enough.

  *size = b2r; return 1;                                    // A proper split is possible.

}

static uint32_t nextPo2(uint32_t in) {                      // Return the next power of 2 of the given number.

 if (in <= 8) { return 8; }                                 // 8 is the minimum required alignment.

 return (1u << (32 - __builtin_clz((uint32_t) in - 1)));

}

static UMemItStat_t alignCb(umemiter_t iter, chunk_t c) {   // Find a chunk big enough for alignment.

  UMemItStat_t status = UMemIt_Unlock;
  uint32_t     size;

  assert(c->lock);
  assert(iter->succ == chunk2succ(c));
  assert(iter->succ->lock);

  if (! c->ciu && c->size >= iter->size) {                  // Free and big enough for trying?
    size = c->size;                                         // Make temporary copy we can pass as reference.
    if (split4align(c, & size, iter->align)) {
      iter->found = c;
      iter->succ2found = iter->succ;
      c->ciu = 1;                                           // Claim it in use already.
      status = UMemIt_Stop;                                 // Keep the chunk and stop here.
    }
  }

  return status;

}
void * uamalloc(umemctx_t ctx, uint32_t size, uint8_t tags, uint32_t align) {

  UMemIter_t Iter = {
    .umem  = ctx,
    .cb    = alignCb,
    .found = NULL,
    .size  = size,
    .align = nextPo2(align),
  };

  uint32_t splitat = size;

  if (8 == Iter.align) { return umalloc(ctx, size, tags); } // Normal alignment, use normal malloc.

  search4chunk(& Iter, size, 0);                            // Search for a fitting chunk; no full scan needed.

  if (Iter.found) {
    split4align(Iter.found, & splitat, Iter.align);         // Call split4align on it again.
    if (splitat) {                                          // splitat can be 0 if already aligned OK.
      assert(splitat > enough2split);                       // If splitting, broken off front must be large enough.
      chunk_t chunk = split(Iter.found, splitat);
      assert(chunk);
      atomic_inc32(& ctx->numchunks);
      chunk->ciu = 1; chunk->lock = 1;                      // New chunk, lock and claim it manually.
      Iter.found->ciu = 0;                                  // Unclaim split-off part.
      unlock(Iter.found);
      Iter.found = chunk;                                   // So that iter2mem can work on it.
      assert(Iter.succ2found == chunk2succ(chunk));         // Successor must be same as before.
    }
  }

  return iter2mem(& Iter, tags);

}

#endif // UAMALLOC

static void contcb(umemctx_t umem) { (void)umem; }          // Do nothing contention callback.

uint32_t initUMemCtx(umemctx_t ctx, uint8_t space[], uint32_t size) {

  Mem_t    Mem;
  Mem_t    Start;
  chunk_t  start;
  uint32_t stat = 0;

  memset(ctx, 0x00, sizeof(UMemCtx_t));                     // Clear everything.

  if (space && size > 256 && size == iusize(size)) {        // Check if reasonable arguments.

    ctx->space = space;
    ctx->size = size;

    do {                                                    // Ensure we start on a proper boundary.
      Start.chunk = raw2chunk(space, size);
      Mem.addr = & Start.chunk->u08[0];                     // User memory needs to be aligned.
      space++; size--;                                      // 1 byte further/less for next time.
    } while (Mem.check & 0b111);                            // See if user mem is 8 byte aligned.

    size++;                                                 // Size was predecremented.

    ctx->align = (uint8_t)(ctx->size - size);

    Start.chunk->ciu = 0;                                   // Prepare start chunk.
    Start.chunk->pif = 0;
    start = Start.chunk;
    ctx->start = start;
    ctx->numchunks = 1;

    ctx->breakat = 12;
    ctx->iterate = iterate;
    ctx->clean = clean;
    ctx->contcb = contcb;

    trylock(start);                                         // Lock required for chunk2succ; should succeed.
    Mem.chunk = chunk2succ(start);                          // End chunk, but header not part of space.
    Mem.addr -= 2 * chunkhdrsz;                             // End chunk header now full part of space.
    start->size = iusize(start->size - 2 * chunkhdrsz);     // Subtracted from total size.
    Mem.chunk->header = 0;                                  // Clear end chunk header.
    Mem.chunk->pif = 1;                                     // But start chunk starts free.
    Mem.chunk->prev = Start.chunk;
    Mem.chunk->ciu = 1;                                     // End chunk is forever in use.
    ctx->end  = Mem.chunk;
    assert((Mem.chunk->u32[0] = 0, 1));                     // Clear for testing; is checked as invariant.
    unlock(start);
    stat = 1;
  }

  return stat;

}
