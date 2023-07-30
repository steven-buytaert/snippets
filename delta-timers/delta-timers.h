#ifndef DELTA_TIMERS_H
#define DELTA_TIMERS_H

// Copyright (c) 2022-2023 Steven Buytaert

#include <stdint.h>

// Delta timer implementation

typedef struct FTmrCtx_t * ftmrCtx_t;
typedef struct FTmr_t *    ftmr_t;

typedef void (* tmrElapsed_t)(ftmrCtx_t ctx, ftmr_t tmr, uint32_t overshoot);

typedef struct FTmr_t {
  ftmr_t       next;
  tmrElapsed_t elapsed;           // Timeout callback; the second parameter is the overshoot number of timing units.
  union {
    uint64_t   rel;               // Against the previous in the list.
    uint64_t   abs;               // Absolute timeout; set this when calling FTmrCtx.insert.
  } Time;                         // Timing units, can be whatever you want it to be; see advance below.
} FTmr_t;

typedef void     (* const tmrAdvance_t)(ftmrCtx_t ctx, uint32_t delta);
typedef uint32_t (* tmrProtect_t)(ftmrCtx_t ctx, uint32_t lock);
typedef void     (* const tmrInsert_t)(ftmrCtx_t ctx, ftmr_t tmr);
typedef ftmr_t   (* const tmrRemove_t)(ftmrCtx_t ctx, ftmr_t tmr);

typedef struct FTmrCtx_t {
  ftmr_t       timers;            // Linked list of timers.
  struct {
    ftmr_t     timer;             // Last timer in the list, if any.
    uint64_t   abs;               // The absolute timeout of this timer.
  } Last;
  tmrAdvance_t advance;           // To advance the list with the given number of timing units.
  tmrInsert_t  insert;            // To insert a timer; can be called from timeout callback.
  tmrRemove_t  remove;            // To remove a timer before it elapses. Returns NULL when not found.
  tmrProtect_t locker;            // Set to dummy by initFTmrCtx; can be overridden after call.
  uint32_t     advDelta;          // (internal) when (re)inserting during advance callback, add this to the timeout.
} FTmrCtx_t;

void initFTmrCtx(ftmrCtx_t ctx);  // Initialize a timer list context for fresh; set defaults.

#endif // DELTA_TIMERS_H
