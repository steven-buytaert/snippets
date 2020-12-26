<<<<<<< HEAD
=======
// Copyright 2020 Steven Buytaert.

>>>>>>> 48f6e74 (Implementation of customizable and extendable printf.)
#include <string.h>
#include <assert.h>
#include <cuxion.h>

typedef struct {
  union {
    uint64_t u_64;
    int64_t  s_64;
  } Num;
  char *     result;
  char       buf[67];             // For binary numbers, we can have up to 64 bits and we need some padding
  char       end;
} Ltoa_t;

static void ltoa(Ltoa_t * c, uint32_t base, const char * digits) {

  c->result = & c->end;                                     // We start at the end since we decrement the result pointer

  do {                                                      // Put characters from least to most significant ...
    *--c->result = digits[c->Num.u_64 % base];              // ... so predecrement and write
    c->Num.u_64 /= base;
  } while (c->Num.u_64);

}

static void fmtnumber(fmtcb_t cb, const char * digits) {    // Generic driver to output format a number in a base given by the spec

  char     sign;
  Ltoa_t   Ltoa;
  uint32_t length;

  if (32 == cb->size) {
    if (cb->spec->issigned) {
      Ltoa.Num.s_64 = (int64_t) cb->Arg.s32;
    }
    else {
      Ltoa.Num.u_64 = (uint64_t) cb->Arg.u32;
    }
  }
  else {
    Ltoa.Num.u_64 = cb->Arg.u64;
  }
  
  cb->Flags.sign &= cb->spec->issigned;                     // Any conversion that's unsigned, implies explicit sign flag is cleared
  
  sign = cb->Flags.sign ? '+' : ' ';                        // Explicit sign request so positive numbers must be preceeded with '+'

  if (cb->spec->issigned && Ltoa.Num.s_64 < 0) {            // When we're doing signed numbers, ensure the number we'll convert is positive
    Ltoa.Num.s_64 = - Ltoa.Num.s_64;
    sign = '-';                                             // We must preceed the number with a negative sign
    cb->Flags.sign = 1;                                     // We need to explicitely output the '-' sign
  }

  ltoa(& Ltoa, cb->spec->base, digits);                     // Convert the unsigned value to a string

  length = & Ltoa.end - Ltoa.result;                        // The length of the raw converted number
  length += cb->Flags.sign ? 1 : 0;                         // When explicit sign is requested or when it's a negative number, add 1 for the sign

  if (0 == cb->Flags.left && cb->Field.width > length) {    // We must right align, so we maybe need to put some fillers first
    if ('0' == cb->Flags.fill && cb->Flags.sign) {          // If we need an explicit sign and we're going to pad with 0, put the sign in front of the zeros
      cb->out(cb, sign);
      cb->Flags.sign = 0;                                   // Sign has been done, so clear this flag
    }
    cux_fill(cb, length);
  }  

  if (cb->Flags.sign) {
    cb->out(cb, sign);
  }

  while (Ltoa.result < & Ltoa.end) {                        // Output the converted number
    cb->out(cb, *Ltoa.result++);
  }

  cux_fill(cb, length);

}

static void lower(fmtcb_t cb) {
  fmtnumber(cb, "0123456789abcdef");
}

static void upper(fmtcb_t cb) {
  fmtnumber(cb, "0123456789ABCDEF");
}

FmtSpec_t Fmt_percent_x = {
  .type     = 'x',
  .base     = 16,
  .issigned = 0,
  .drv      = lower,
};

FmtSpec_t Fmt_percent_X = {
  .type     = 'X',
  .base     = 16,
  .issigned = 0,
  .drv      = upper,
};

FmtSpec_t Fmt_percent_d = {
  .type     = 'd',
  .base     = 10,
  .issigned = 1,
  .drv      = lower,
};

FmtSpec_t Fmt_percent_i = {
  .type     = 'i',
  .base     = 10,
  .issigned = 1,
  .drv      = lower,
};

FmtSpec_t Fmt_percent_u = {
  .type     = 'u',
  .base     = 10,
  .issigned = 0,
  .drv      = lower,
};

FmtSpec_t Fmt_percent_o = {
  .type     = 'o',
  .base     = 8,
  .issigned = 0,
  .drv      = lower,
};

FmtSpec_t Fmt_percent_b = {
  .type     = 'b',
  .base     = 2,
  .issigned = 0,
  .drv      = lower,
};

static void drv_p(fmtcb_t cb) {
  cb->out(cb, '0');
  cb->out(cb, 'x');
  lower(cb);
}

FmtSpec_t Fmt_percent_p = {
  .type     = 'p',
  .base     = 16,
  .issigned = 0,
  .args     = no32wiap,           // Note that 'number of 32 bit words in a pointer' give the correct number of argument words on 64 or 32 bit systems
  .drv      = drv_p,
};
