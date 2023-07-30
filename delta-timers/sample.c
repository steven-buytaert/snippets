// Copyright 2023 Steven Buytaert

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <inttypes.h>

#include <delta-timers.h>

void * ticker(void * mutref);     // Defined in pthread-ticker.c.

static FTmrCtx_t Timers;          // Our timer context.

static pthread_mutex_t Mut;       // Mutex used for timer lock.

typedef struct MyTmr_t {
  FTmr_t         Tmr;             // Delta timer header.
  const char *   msg;             // Message to declaim.
  const uint16_t time;            // At what time to tell the message.
  uint8_t        inserted;
  const uint8_t  final;
} MyTmr_t;

static volatile uint32_t done = 0;
static char buffer[256];          // Final buffer to check.
static uint32_t overshot = 0;

static void elapsed(ftmrCtx_t ctx, ftmr_t ftmr, uint32_t overshoot) {

  MyTmr_t * tmr = (MyTmr_t *) ftmr;

  strcat(buffer, tmr->msg);
  
  done = tmr->final;

  overshot += overshoot;

}

/*

  This function emulates the timer tick ISR. It calls the advance function to
  check if any timers have elapsed and returns the number of ticks to arm
  the OS timer with again, for the next call to tickisr().

  You could always return e.g. 1 as timeout as it would not change the outcome,
  but would increase the number of times the tickisr() is being called. Try by
  uncommenting the line where timeout is set to 1.

*/

uint32_t tickisr(uint64_t now) {                            // This function is called by the ticker thread; now in milliseconds.

  static uint64_t previous = 0;

  Timers.advance(& Timers, (uint32_t)(now - previous));     // Advance the timers with delta units between now and previous call.

  previous = now;

  uint32_t to = Timers.timers ? Timers.timers->Time.abs : 1;// Next timeout for the timer interrupt.

  // to = 1;

  printf("%2u ", to);

  return to;

}

static uint32_t tmrlock(ftmrCtx_t ctx, uint32_t lock) {

  if (lock) {
    pthread_mutex_lock(& Mut);
  }
  else {
    pthread_mutex_unlock(& Mut);
  }
  
  return 0;                                                 // We don't take lock acquire time into account here.

}

/*

  The sample code will insert a given set of timers that make up a message, and will insert them in random
  order. We set the timeout of the first message high enough relative to the time unit used in the ticker, so
  that enough messages get inserted for proper order.

*/

int main(int argc, char * argv[]) {

  pthread_t tickerthr;

  MyTmr_t MyTmrs[] = {
    { .msg = "The ",   .time = 20, .final = 0 },
    { .msg = "quick ", .time = 22, .final = 0 },
    { .msg = "brown ", .time = 23, .final = 0 },
    { .msg = "fox ",   .time = 27, .final = 0 },
    { .msg = "jumps ", .time = 30, .final = 0 },
    { .msg = "over ",  .time = 61, .final = 0 },
    { .msg = "the ",   .time = 62, .final = 0 },
    { .msg = "lazy ",  .time = 66, .final = 0 },
    { .msg = "dog.",   .time = 70, .final = 1 },
  };

  const uint32_t nummsg = sizeof(MyTmrs) / sizeof(MyTmr_t);

  pthread_mutex_init(& Mut, NULL);

  initFTmrCtx(& Timers);

  Timers.locker = tmrlock;

  pthread_create(& tickerthr, NULL, ticker, & Mut);

  while (1) {                                               // crtl-c to exit.
    done = 0;
    overshot = 0;
    for (uint32_t i = 0; i < nummsg; i++) {
      MyTmrs[i].inserted = 0;
    }
    memset(buffer, 0x00, sizeof(buffer));
    for (uint32_t i = nummsg; i; ) {
      uint32_t m = rand() % nummsg;                         // Pick any random timer/message and insert it if not yet done.
      if (! MyTmrs[m].inserted) {
        MyTmrs[m].Tmr.Time.abs = MyTmrs[m].time;            // Set the timeout.
        MyTmrs[m].Tmr.elapsed = elapsed;                    // Set the callback.
        Timers.insert(& Timers, & MyTmrs[m].Tmr);           // Insert the timer.
        MyTmrs[m].inserted = 1;
        i--;
      }
    }
    while (! done) { }                                      // Loop until all messages played.
    printf(" '%s'  overshot %u\n", buffer, overshot);
    assert(! strcmp("The quick brown fox jumps over the lazy dog.", buffer));
  }

  return 0;
  
}
