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

static chunk_t split(chunk_t c2s, uint32_t size) {          // Split at given size; return remainder.

  Mem_t    Succ = { .chunk = c2s };
  uint32_t sucsize = c2s->size - size - chunkhdrsz;         // Successor size.

  assert(c2s->lock);                                        // Must be locked.
  assert(size == roundup(size, sizeof(void *)));            // Should have been properly rounded already.
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
  
  if (full) {
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

static void uncfree(umemctx_t umem, void * mem) {           // Uncontended path to release memory.

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

static void ufree(umemctx_t umem, void * mem) {             // Slow path to release memory.

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

static UMemItStat_t bestFitCb(umemiter_t iter, chunk_t c) { // Best fit walking over all chunks.

  uint32_t status = UMemIt_Unlock;
  
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

static void * umalloc(umemctx_t umem, uint32_t sz, uint8_t tags) { // Slow path umalloc.

  UMemIter_t Iter = { .cb = bestFitCb, .umem = umem };
  chunk_t    succ;
  uint32_t   complete;
  uint32_t   count = 0;
  uint32_t   breakat = umem->breakat;                       // Take snapshot.

  clean(umem);                                              // Do any cleanup first.

  if (sz < minchunksize) { sz = minchunksize; }             // Ensure we have a proper size.
  if (sz > iusize(sz)) { return NULL; }                     // Too large.
  Iter.size = roundup(sz, sizeof(void *));

  Iter.start = umem->start;
  Iter.found = NULL;

  do {
    complete = iterate(& Iter);
    if (! complete) {
      if (Iter.found) {
        assert(Iter.found->lock);                           // Both found and successor should be locked.
        succ = Iter.succ2found;
        assert(succ->lock);
        if (++count > breakat) { 
          umem->tiesbroken++;
          break; 
        }
        Iter.found->ciu = 0;                                // Release claim again.
        unlock(Iter.found);                                 // Let others make progress; unlock in same ...
        unlock(succ);                                       // ... sequence.
        Iter.found = NULL;
      }
      Iter.start = umem->start;
      umem->contcb(umem);
    }
  } while (! complete);

  return iter2mem(& Iter, tags);

}

static UMemItStat_t firstFitCb(umemiter_t iter, chunk_t c) {// First fit walking over all chunks.

  uint32_t status = UMemIt_Unlock;
  
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

static void * uncmalloc(umemctx_t umem, uint32_t sz, uint8_t tags) { // Least contented path umalloc.

  UMemIter_t Iter = { .cb = firstFitCb, .umem = umem };
  uint32_t   count = 0;

  if (sz < minchunksize) { sz = minchunksize; }             // Ensure we have a proper size.
  if (sz > iusize(sz)) { return NULL; }                     // Too large.

  Iter.size = roundup(sz, sizeof(void *));

  Iter.found = NULL;

  do {
    Iter.start = umem->start;
    if (iterate(& Iter)) break;                             // If we had a full scan, don't retry.
  } while (! Iter.found && ++count < 2);                    // Try 2 times only.

  return iter2mem(& Iter, tags);

}

#if defined(UREALLOC)

static void locktuple(umemctx_t ctx, chunk_t chunk, chunk_t succ) {

  uint32_t bothlocked;

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
      size = roundup(size, sizeof(void *));                 // Align size requirement first.
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

static void contcb(umemctx_t umem) { (void)umem; }          // Do nothing contention callback.

void initUMemCtx(umemctx_t ctx, uint8_t space[], uint32_t size) {

  Mem_t   Mem;
  Mem_t   Start;
  chunk_t start;

  assert(roundup(minchunksize, sizeof(uint64_t)) == minchunksize);

  memset(ctx, 0x00, sizeof(UMemCtx_t));                     // Clear everything.

  if (space && size > 256 && size == iusize(size)) {        // Check if reasonable arguments.

    ctx->contcb = contcb;
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
    ctx->malloc = umalloc;
    ctx->iterate = iterate;
    ctx->free = ufree;
    ctx->uncfree = uncfree;
    ctx->uncmalloc = uncmalloc;
  
    trylock(start);                                         // Lock required for chunk2succ; should succeed.
    Mem.chunk = chunk2succ(start);                          // End chunk, but header not part of space.
    Mem.addr -= chunkhdrsz;                                 // End chunk header now part of space.
    start->size = iusize(start->size - chunkhdrsz);         // Subtracted from total size.
    Mem.chunk->header = 0;                                  // Clear end chunk header.
    Mem.chunk->pif = 1;                                     // But start chunk starts free.
    Mem.chunk->prev = Start.chunk;
    Mem.chunk->ciu = 1;                                     // End chunk is forever in use.
    ctx->end  = Mem.chunk;
    unlock(start);
  }

}
