#ifndef UINTXX_H
#define UINTXX_H

//  Copyright (c) 2020 Steven Buytaert

#include <stdint.h>

// Variable length Little Endian encoded Unsigned Integer.

typedef struct UINTXX_t *        uintxx_t;
typedef struct UINTXX_t const * cuintxx_t;

typedef struct UINTXX_t {
  const uint16_t width;           // Width in bits of the tail; must be a multiple of 32!
  uint16_t       nsb;             // Number of significant bits. Initially set by caller with vnsb.

  int16_t        n;               // Not used by the implementation. Available to user.
  int16_t        exp;             // Not used by the implementation. Available to user.

  union {
    uint64_t     u64[0];          // Ensures this tail is worst case aligned.
    uint32_t     u32[0];
    uint16_t     u16[0];          // Space must be provided by the caller.
  };                              // Tail contains the value, *little endian encoded*; also on BE machines.
} UINTXX_t;

// Operations; vnsb returns the number of significant bits.

void     vmul(cuintxx_t u, cuintxx_t v, uintxx_t w);
uint32_t vdiv(cuintxx_t u, cuintxx_t v, uintxx_t q, uintxx_t r);
uint32_t vnsb(cuintxx_t u);
void     vand(uintxx_t v, uintxx_t mask);
void     vrights(uintxx_t v, uint32_t shift, uint32_t fb);
void     vlefts(uintxx_t v, uint32_t shift, uint32_t fb);

#endif // UINTXX_H
