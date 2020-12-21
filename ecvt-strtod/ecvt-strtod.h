#ifndef ECVT_STRTOD_H
#define ECVT_STRTOD_H

// Copyright 2020 Steven Buytaert

#include <stdint.h>

typedef enum {
  None         = 0,               // Not characterized yet, counts as error.
  Denormal     = 1,
  Normal       = 2, 
  Zero         = 3,
  Infinite     = 4,
  NaN          = 5,
} ECVTTypes_t;

typedef enum {
  NoRoun       = 0,
  NearEv       = 1,
} ECVTRounding_t;

typedef struct ECvt_t * ecvt_t;

typedef struct ECvt_t {           // ECVT Arguments and result.
  union {
    double     f64;
    float      f32;
    uint64_t   bits;
  };
  uint8_t      width;             // Width in bits of the double type.
  uint8_t      ndig;              // Number of digits requested.
  uint8_t      rounding;          // Rounding mode.

  uint8_t      negative;          // If not zero, number is negative.
  uint8_t      type;              // One of the range [Denormal, Nan].
  uint8_t      cndig;             // Actual or current number of digits.
  int16_t      decpt;             // Digit that also contains decimal point; -1 means only fractional.
  uint16_t     scaled;            // Internal, when debugging, scaling operations done.
  char         digits[22];        // ndig digits; most significant in digits[0].
} ECvt_t;

void sbtecvt(ecvt_t ecvt);

#endif // ECVT_STRTOD_H
