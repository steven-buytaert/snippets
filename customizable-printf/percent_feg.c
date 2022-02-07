// Copyright 2020 Steven Buytaert.

#if defined(FPSUPPORTED)

#include <string.h>
#include <assert.h>
#include <cuxion.h>
#include <ecvt-strtod.h>

typedef void (* fmtgef_t)(fmtcb_t cb, ecvt_t ecvt);

static int32_t min(int32_t a, uint32_t b) {

  return a < (int32_t) b ? a : (int32_t) b;

}

static void fmtf(fmtcb_t cb, ecvt_t ecvt) {

  int dp = ecvt->decpt;

  if (dp > 0) {
    assert(dp <= ecvt->ndig);
    char sd = ecvt->digits[dp]; ecvt->digits[dp] = 0;       // Truncate at decimal point; save the digit at the decimal point first.
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
      int32_t i = min(ecvt->decpt, cb->Field.precision);
      cux_sprintf(cb->buffer, ".%0*u", i, 0);               // Fill with zeros.
      int32_t f = cb->Field.precision - ecvt->decpt;
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
    .ndig     = cb->Field.precision + a2p,                  // a2p == 'Add to the precision this number of digits'.
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

#endif // FPSUPPORTED
