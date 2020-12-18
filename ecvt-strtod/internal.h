#ifndef ECVT_INTERNAL
#define ECVT_INTERNAL

// Copyright 2020 Steven Buytaert

#include <uintxx.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>
#include <ecvt-strtod.h>

typedef const struct Bits_t {
  uint8_t      pad[2];
  uint8_t      width;             // Total width in bits.
  uint8_t      mantissa;          // Number of mantissa bits.
  uint16_t     saturated;         // Saturated bias, all bits 1.
  uint16_t     bias;              // Exponent bias.
  uint64_t     mantmask;          // Mantissa mask.
  uint64_t     expomask;          // Exponent mask.
  uint64_t     implicitone;
  uint64_t     signbit;
} Bits_t;

static Bits_t IEEE64c = {
  .width       = 64,
  .mantissa    = 52,
  .saturated   = 0b011111111111,
  .bias        = 0b001111111111,
  .expomask    = 0b0111111111110000000000000000000000000000000000000000000000000000,
  .mantmask    = 0b0000000000001111111111111111111111111111111111111111111111111111,
  .implicitone = 0b0000000000010000000000000000000000000000000000000000000000000000,
  .signbit     = 0b1000000000000000000000000000000000000000000000000000000000000000,
};

static Bits_t IEEE32c = {
  .width       = 32,
  .mantissa    = 23,
  .saturated   = 0b011111111,
  .bias        = 0b001111111,
  .expomask    = 0b01111111100000000000000000000000,
  .mantmask    = 0b00000000011111111111111111111111,
  .implicitone = 0b00000000100000000000000000000000,
  .signbit     = 0b10000000000000000000000000000000,
};

typedef struct IEEE754_t {
  union {
    double     f64;
    float      f32;
    uint64_t   bits;
  };
} IEEE754_t;

typedef struct Float_t * flt_t;

typedef struct Float_t {          // Our own internal floating point format.
  UINTXX_t     I;                 // To facilitate the uintxx vdiv and vmul for scaling.
  union {
    uint64_t   mantissa;
    uint32_t   u32[8];
  };                              // Represents a wide integer.
  struct {
    int16_t    bin;
    int16_t    dec;
  } Exp;                          // Binary and decimal exponent.
  uint8_t      type;
  uint8_t      negative;
  uint8_t      width;             // Width in bits of the IEEE type.
  uint8_t      pad[1];
} Float_t;

static const size_t fltclr  = (sizeof(Float_t) - offsetof(Float_t, I.nsb));
static const size_t ecvtclr = (sizeof(ECvt_t) - offsetof(ECvt_t, negative));

void ieee2float(ECvt_t * n, Float_t * flt, Bits_t * const b);

#endif // ECVT_INTERNAL
