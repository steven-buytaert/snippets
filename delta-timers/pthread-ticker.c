// Copyright 2023 Steven Buytaert

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <inttypes.h>
#include <sys/time.h>

static const int64_t nanos_per_sec  = (1000LL * 1000LL * 1000LL);
static const int64_t nanos_per_msec = (1000LL * 1000LL);
static const int64_t nanos_per_usec = (1000LL);

static void OStimeout(struct timespec * ts, uint32_t ms) {  // Create a timeout spec to expire after a certain number of milliseconds.

  struct timeval Tv;
  int64_t       nanos;

  gettimeofday(& Tv, NULL);

  nanos  = Tv.tv_sec  * nanos_per_sec;
  nanos += Tv.tv_usec * nanos_per_usec;
  nanos += ms * nanos_per_msec;

  ts->tv_nsec = nanos % nanos_per_sec;
  ts->tv_sec  = nanos / nanos_per_sec;

}

static uint64_t ct_millis(void) {                           // Return the current time in milliseconds

  struct timeval Tv;
  uint64_t       ms;

  gettimeofday(& Tv, NULL);

  ms =  ((uint64_t) Tv.tv_sec  * 1000LL);
  ms += ((uint64_t) Tv.tv_usec / 1000LL);

  return ms;

}

uint32_t tickisr(uint64_t currenttime);

void * ticker(void * mutref) {

  pthread_mutex_t * mut = mutref;
  pthread_cond_t    Cond;
  struct timespec   Timeout;
  uint64_t          start = ct_millis();;
  uint32_t          timeout = 1;                            // Timeout in milliseconds.

  pthread_cond_init(& Cond, NULL);

  while (1) {
    OStimeout(& Timeout, timeout);
    pthread_mutex_lock(mut);
    pthread_cond_timedwait(& Cond, mut, & Timeout);         // Waiting for Godot.
    pthread_mutex_unlock(mut);
    timeout = tickisr(ct_millis() - start);                 // Call ISR unlocked; returns next timeout to use.
  }

  return NULL;                                              // Not reached.

}
