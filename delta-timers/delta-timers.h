#ifndef DELTA_TIMERS_H
#define DELTA_TIMERS_H

// Copyright (c) 2022 Steven Buytaert

#include <stdint.h>

// Delta timer implementation

typedef struct FTmrCtx_t * ftmrCtx_t;
typedef struct FTmr_t *    ftmr_t;

typedef void (* tmrElapsed_t)(ftmrCtx_t ctx, ftmr_t tmr, uint32_t overshoot);

typedef struct FTmr_t {
  ftmr_t       next;
  tmrElapsed_t elapsed;           // Timeout callback; the second parameter is the overshoot number of timing units.
  union {
    uint32_t   rel;               // Against the previous in the list
    uint32_t   abs;               // Absolute timeout
  } Time;                         // Timing units, can be whatever you want it to be; see advance below.
} FTmr_t;

typedef void   (* const tmrAdvance_t)(ftmrCtx_t ctx, uint32_t delta);
typedef void   (* tmrProtect_t)(ftmrCtx_t ctx, uint32_t lock);
typedef void   (* const tmrInsert_t)(ftmrCtx_t ctx, ftmr_t tmr);
typedef ftmr_t (* const tmrRemove_t)(ftmrCtx_t ctx, ftmr_t tmr);

typedef struct FTmrCtx_t {
  ftmr_t       timers;            // Linked list of timers
  uint32_t     advDelta;          // When inserting during advance processing, add this to the timeout
  tmrAdvance_t advance;           // To advance the list with the given number of timing units
  tmrInsert_t  insert;
  tmrRemove_t  remove;
  tmrProtect_t locker;            // Set to dummy by initFTmrCtx; can be overridden after call.
} FTmrCtx_t;

void initFTmrCtx(ftmrCtx_t ctx);  // Initialize a timer list context for fresh; set defaults.

#endif // DELTA_TIMERS_H
