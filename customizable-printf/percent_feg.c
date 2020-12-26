<<<<<<< HEAD
=======
// Copyright 2020 Steven Buytaert.

>>>>>>> 48f6e74 (Implementation of customizable and extendable printf.)
#include <string.h>
#include <assert.h>
#include <cuxion.h>
#include <ecvt-strtod.h>

<<<<<<< HEAD
/* 

  e The double argument is rounded and converted in the style [-]d.ddde±dd where there is one digit (which is nonzero if the argument is nonzero) before
    the decimal-point character and the number of digits after it is equal to the precision; if the precision is missing, it is taken as 6; if the  pre‐
    cision  is zero, no decimal-point character appears.  An E conversion uses the letter E (rather than e) to introduce the exponent.  The exponent al‐
    ways contains at least two digits; if the value is zero, the exponent is 00.

  f The double argument is rounded and converted to decimal notation in the style [-]ddd.ddd, where the number of digits after the decimal-point charac‐
    ter  is  equal to the precision specification.  If the precision is missing, it is taken as 6; if the precision is explicitly zero, no decimal-point
    character appears.  If a decimal point appears, at least one digit appears before it.

  g The double argument is converted in style f or e (or F or E for G conversions).  The precision specifies the number of significant digits.   If  the
    precision is missing, 6 digits are given; if the precision is zero, it is treated as 1.  Style e is used if the exponent from its conversion is less
    than -4 or greater than or equal to the precision.  Trailing zeros are removed from the fractional part of the result; a decimal point appears  only
    if it is followed by at least one digit.

*/

=======
>>>>>>> 48f6e74 (Implementation of customizable and extendable printf.)
typedef void (* fmtgef_t)(fmtcb_t cb, ecvt_t ecvt);

static int32_t min(int32_t a, uint32_t b) {

  return a < (int32_t) b ? a : (int32_t) b;

}

static void fmtf(fmtcb_t cb, ecvt_t ecvt) {

  int32_t i;
  int32_t f;
  char    sd;                                               // Saved digit.
  int     dp = ecvt->decpt;

  if (dp > 0) {
    assert(dp <= ecvt->ndig);
    sd = ecvt->digits[dp]; ecvt->digits[dp] = 0;            // Truncate at decimal point; save the digit at the decimal point first.
    cux_sprintf(cb->buffer, "%s", ecvt->digits);
    ecvt->ndig -= ecvt->decpt;                              // Number of digits we have left to render.
    if (cb->Field.precision) {
      if ((int) (cb->Field.precision - ecvt->ndig) < 0) {   // We have too many digits left, so truncate.
        ecvt->digits[cb->Field.precision + 1] = 0;          // +1 for the saved digit.
      }
      cux_sprintf(cb->buffer, ".%c%s", sd, & ecvt->digits[dp + 1]);
    }
  }
  else {
    cb->out(cb, '0');
    if (cb->Field.precision) {
      ecvt->decpt = -ecvt->decpt;                           // Change sign.
      i = min(ecvt->decpt, cb->Field.precision);
      cux_sprintf(cb->buffer, ".%0*u", i, 0);               // Fill with zeros.
      f = cb->Field.precision - ecvt->decpt;
      if (f > 0 && f < ecvt->ndig) { ecvt->digits[f] = 0; } // More digits than space available, truncate.
      cux_sprintf(cb->buffer, "%s", ecvt->digits);
    }
  }

}

static void fmte(fmtcb_t cb, ECvt_t const * ecvt) {

  char ec = cb->spec->type;
  
  if (ec == 'G') { ec = 'E'; } if (ec == 'g') { ec = 'e'; } // For g/G format, pick the appropriate exponent character.

  const char * dps = cb->Field.precision == 0 ? "" : ".";   // When precision is 0, no decimal point required.

  cux_sprintf(cb->buffer, "%c%s%s%c%+03d", ecvt->digits[0], dps, & ecvt->digits[1], ec, ecvt->decpt - 1);

}

static void fmtg(fmtcb_t cb, ecvt_t ecvt) {

  int32_t exp = ecvt->decpt - 1;                            // The exponent.

  while (ecvt->digits[ecvt->ndig - 1] == '0') {             // Spec: "Trailing zeros are removed from the fractional part of the result;"
    ecvt->digits[--ecvt->ndig] = 0;
  }

  cb->Field.precision--;                                    // For g/G formats, precision applies to all digits, not just fractional.

  if (exp < -4 || exp >= (int) cb->Field.precision + 1) {   // Spec: "Style e is used if the exponent ... < -4 or greater or equal to the precision."
    fmte(cb, ecvt);
  }
  else {
    fmtf(cb, ecvt);
  }

}

static void doany(fmtcb_t cb, uint32_t a2p, fmtgef_t fmt) { // The main driver, calls sbtecvt and handles NaN and Infinite.

  if (0xffffffff == cb->Field.precision) {                  // When not specified, for all 3 formats, 6 is the default.
    cb->Field.precision = 6;
  }

  ECvt_t ECvt = {
    .f64      = cb->Arg.f64,
    .width    = 64,
    .ndig     = cb->Field.precision + a2p,
    .rounding = NearEv,
  };

  sbtecvt(& ECvt);                                          // Dissect the float into its components.

  if (cb->Flags.sign || ECvt.negative) {
    cb->out(cb, ECvt.negative ? '-' : '+');
  }

  if (ECvt.type != NaN && ECvt.type != Infinite) {          // NaN and Infinite are handled here, below.
    fmt(cb, & ECvt);                                        // Call the proper driver for the format.
  }
  else {
    cux_sprintf(cb->buffer, "%s", ECvt.digits);             // Either nan or inf.
  }

}

static void dof(fmtcb_t cb) { doany(cb, 2, fmtf); }

static void doe(fmtcb_t cb) { doany(cb, 1, (fmtgef_t) fmte); }

static void dog(fmtcb_t cb) {

  if (0 == cb->Field.precision) cb->Field.precision = 1;    // When specified as 0, take 1 for g/G formats.

  doany(cb, 0, fmtg);

}

FmtSpec_t Fmt_percent_f = {
  .type     = 'f',
  .base     = 10,
  .issigned = 1,
  .args     = 3,                  // To mark double format.
  .drv      = dof,
};

FmtSpec_t Fmt_percent_e = {
  .type     = 'e',
  .base     = 10,
  .args     = 3,                  // To mark double format.
  .issigned = 1,
  .drv      = doe,
};

FmtSpec_t Fmt_percent_E = {
  .type     = 'E',
  .base     = 10,
  .args     = 3,                  // To mark double format.
  .issigned = 1,
  .drv      = doe,
};

FmtSpec_t Fmt_percent_g = {
  .type     = 'g',
  .base     = 10,
  .args     = 3,                  // To mark double format.
  .issigned = 1,
  .drv      = dog,
};

FmtSpec_t Fmt_percent_G = {
  .type     = 'G',
  .base     = 10,
  .args     = 3,                  // To mark double format.
  .issigned = 1,
  .drv      = dog,
};
