#include <string.h>
#include <assert.h>

// Copyright (c) 2022 Steven Buytaert

#include <delta-timers.h>

typedef struct FTmrCtx_t * ctx_t; // Shorthands for context ...
typedef struct FTmr_t *    tmr_t; // ... and a timer reference.

typedef struct Flock_t {          // Structure for aggregating timers that have elapsed
  tmr_t    tmr;                   // Timer to elapse
  uint32_t overshoot;             // Elapse overshoot value
} Flock_t;

#define NUM(A) (sizeof(A) / sizeof(A[0]))

static void insertFTmr(ctx_t ctx, tmr_t tmr) {              // Insert a timer in the linked list with proper delta relative value

  tmr_t    c;                                               // Cursor over the list of timers
  tmr_t *  u = & ctx->timers;                               // Location to update with our new timer
  uint32_t to = tmr->Time.abs + ctx->advDelta;              // Get the absolute timeout; ctx->advDelta set during advance function below.

  struct {
    uint32_t c;                                             // Absolute timout at cursor 'c'
    uint32_t u;                                             // Absolute timeout at update location 'u'
  } Time = { 0, 0 };

  ctx->locker(ctx, 1);                                      // Lock

  for (c = *u; c; u = & c->next, c = c->next) {             // Walk over the list, keeping track of update location
    Time.c += c->Time.rel;
    if (Time.c >= to) {
      tmr->next = u[0]; u[0] = tmr;                         // Link the timer in the list
      tmr->Time.rel = to - Time.u;                          // Write back the relative timeout
      if (tmr->next) {                                      // If there is a next timer, also update its relative timeout
        assert(tmr->next->Time.rel >= tmr->Time.rel);       // If this fires, somethings wrong in the lines above.
        tmr->next->Time.rel -= tmr->Time.rel;
      }
      break;                                                // Timer inserted, we can stop
    }
    Time.u = Time.c;                                        // Absolute timeout for the next update point
  }

  if (! c) {                                                // We've reached the end of the list without find the insertion point ...
    u[0] = tmr; tmr->next = NULL;                           // ... so insert at the end ...
    tmr->Time.rel = to - Time.c;                            // ... with the proper timeout.
  }

  ctx->locker(ctx, 0);                                      // Unlock

}

static tmr_t removeFTmr(ctx_t ctx, tmr_t tmr) {             // Remove a timer from the linked list in the context

  tmr_t      c;                                             // Cursor over the list of timers
  tmr_t *    u;                                             // Location to update with our new timer

  ctx->locker(ctx, 1);                                      // Lock

  u = & ctx->timers;                                        // Set this inside the lock

  for (c = *u; c; u = & c->next, c = c->next) {             // Walk over the list, keeping track of update location
    if (c == tmr) {
      u[0] = tmr->next;
      if (tmr->next) {
        tmr->next->Time.rel += tmr->Time.rel;               // Update the next timer
      }
      break;                                                // Found the timer so quit searching
    }
  }

  ctx->locker(ctx, 0);                                      // Unlock

  return c;

}

static void doFlock(ctx_t ctx, Flock_t F[], uint32_t num) { // Call the elapse functions for timers in a flock.

  uint32_t i;

  for (i = 0; i < num; i++) {
    F[i].tmr->elapsed(ctx, F[i].tmr, F[i].overshoot);
  }

}

static void advanceTmr(ftmrCtx_t ctx, uint32_t delta) {     // Advance the timeout, elapsing timers that timed out, if any.

  uint32_t i = 0;
  Flock_t  Flock[12];                                       // We aggregate timers that elapsed, to avoid frequent unlock/lock

  ctx->locker(ctx, 1);                                      // Lock

  assert(0 == ctx->advDelta);

  while (ctx->timers && delta >= ctx->timers->Time.rel) {
    delta -= ctx->timers->Time.rel;
    Flock[i].tmr = ctx->timers;
    ctx->timers = ctx->timers->next;                        // Unlink from the linked list
    Flock[i].overshoot = delta; i++;
    if (i == NUM(Flock)) {                                  // Flock array full, empty it first
      ctx->advDelta = delta;                                // If the call inserts a timer, ensure it looks like the advance has completed fully
      ctx->locker(ctx, 0);
      doFlock(ctx, Flock, NUM(Flock)); i = 0;               // Run aggregated flock and reset
      ctx->locker(ctx, 1);
    }
  }

  if (ctx->timers) {
    assert(delta < ctx->timers->Time.rel);                  // If not, the above mechanism didn't work properly
    ctx->timers->Time.rel -= delta;
  }

  ctx->advDelta = 0;

  ctx->locker(ctx, 0);                                      // Unlock

  doFlock(ctx, Flock, i);                                   // Run what's left in the flock array, if any

}

static void noTmrLock(ftmrCtx_t ctx, uint32_t lock) {}      // Dummy locker function, as default

static FTmrCtx_t Mother = {
  .timers   = NULL,
  .advDelta = 0,
  .advance  = advanceTmr,
  .insert   = insertFTmr,
  .remove   = removeFTmr,
  .locker   = noTmrLock,
};

void initFTmrCtx(ftmrCtx_t ctx) {

  memcpy(ctx, & Mother, sizeof(Mother));                    // To circumvent the const function pointer issue

}
