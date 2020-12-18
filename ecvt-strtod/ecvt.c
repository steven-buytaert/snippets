// Copyright 2020 Steven Buytaert

#include <internal.h>

void ieee2float(ECvt_t * n, Float_t * flt, Bits_t * const b) {

  memset(& flt->I.nsb, 0x00, fltclr);                       // Make sure we don't multiply garbage.

  flt->width = b->width;

  flt->negative = (n->bits & b->signbit) ? 1 : 0;
  flt->Exp.bin  = (n->bits & b->expomask) >> b->mantissa;   // Get the biased exponent.
  flt->mantissa =  n->bits & b->mantmask;                   // Get the fractional part.

  if (flt->Exp.bin) {                                       // Exponent is not 0 ...
    if (flt->Exp.bin != b->saturated) {                     // ... and not saturated, ...
      flt->type      = Finite;                              // ... so a finite number.
      flt->Exp.bin  -= b->bias + b->mantissa;               // Unbias the exponent.
      flt->mantissa |= b->implicitone;                      // Make the implicit 1. now explicit.
      flt->I.nsb     = b->mantissa + 1;                     // By definition of the implicit one position.
    }
    else {                                                  // Exponent saturated, either NaN or Infinite.
      flt->type = flt->mantissa ? NaN : Infinite;
    }
  }
  else {
    if (flt->mantissa) {
      flt->type = Denormal;                                 // Zero exponent, non zero mantissa, so a denormal.
      flt->Exp.bin = - (b->bias + b->mantissa - 1);
      flt->I.nsb = vnsb(& flt->I);
    }
    else {
      flt->type = Zero;
    }
  }

  assert(Zero == flt->type || flt->I.nsb == vnsb(& flt->I));

  n->type = flt->type;

}

static void fmtint(ecvt_t ecvt, uint64_t integral) {        // Format the integral part.

  char * d = & ecvt->digits[sizeof(ecvt->digits)];          // Start at the back; predecremented before write, so start at index == size.

  while (integral && ecvt->cndig < ecvt->ndig) {            // Put characters from least to most significant ...
    *--d = integral % 10;                                   // ... so predecrement and write
    ecvt->cndig++;
    ecvt->decpt++;
    integral /= 10;
  }

  memmove(& ecvt->digits[0], d, ecvt->cndig);               // Move all digits to the front.

}

static void fmt(ecvt_t e, const Float_t * flt) {            // Format a float with mantissa * pow(2, exp) and exp E [-63, 0].

  uint64_t  mask = (1ULL << -flt->Exp.bin) - 1;
  uint64_t  mant = flt->mantissa;

  assert(flt->Exp.bin >= -63 && flt->Exp.bin <= 0);         // Binary exponent must be in the proper range for a 64 bit mantissa.

  assert(vnsb(& flt->I) >= 53);

  fmtint(e, flt->mantissa >> -flt->Exp.bin);                // Format the integral part.

  mant &= mask;                                             // Get the fractional part.

  while (e->cndig < e->ndig && mant) {
    mant *= 10;                                             // Bring next digit before the decimal point.
    e->digits[e->cndig] = (mant >> -flt->Exp.bin);
    if (! e->cndig && ! e->digits[0]) { e->decpt--; }       // No no zero first digit produced yet, decrease decimal point.
    else { e->cndig++; }                                    // There's already a non zero significant digit.
    mant &= mask;
  }

  while (e->cndig < e->ndig) { e->digits[e->cndig++] = 0; } // Fill remainder up with 0; also clears temp space of fmtint.

  e->decpt -= flt->Exp.dec;                                 // Compensate for scaling, if any.

}

typedef const struct Scale_t {    // Power of 10 to scale a mantissa.
  UINTXX_t I;
  uint64_t pow10;
} Scale_t;

static const Scale_t Scales[] = {
  { .I = { .width = 64, .nsb = 10, .exp =  3 }, .pow10 =                                                       0b1111101000 },
  { .I = { .width = 64, .nsb = 30, .exp =  9 }, .pow10 =                                   0b111011100110101100101000000000 },
  { .I = { .width = 64, .nsb = 40, .exp = 12 }, .pow10 =                         0b1110100011010100101001010001000000000000 },
  { .I = { .width = 64, .nsb = 54, .exp = 16 }, .pow10 =           0b100011100001101111001001101111110000010000000000000000 },
  { .I = { .width = 64, .nsb = 64, .exp = 19 }, .pow10 = 0b1000101011000111001000110000010010001001111010000000000000000000 }, 
};

static const uint32_t ns = sizeof(Scales) / sizeof(Scale_t);

static uint32_t clz(const Float_t * flt) {                  // Return the number of leading zeros to see if a multiplication fits.

  return (uint32_t) (flt->I.width - flt->I.nsb);

}

static const int8_t reqnsb =  64;                           // The minimum number of significant bits when upscaling.
static const int8_t minexp = -60;                           // Minimum exponent; don't take exactly -63, but a bit higher.
static const int8_t maxexp = -10;                           // Maximum exponent; we don't take exactly 0 but a bit lower.

static int32_t expo2be(const Float_t * flt) {               // Return the future exponent when all superfluous bits are shifted right.

  return flt->Exp.bin + (flt->I.nsb - reqnsb);

}

static void scaleup(ECvt_t * n, Float_t * flt) {            // Scale a float up to increase the binary exponent.

  Float_t   P = { .I.width = sizeof(P.u32) * 8 };           // The product.
  uint8_t   needed;
  cuintxx_t scaler = NULL;
  uint32_t  i;

  if (expo2be(flt) + Scales[ns - 1].I.nsb < minexp) {       // If the largest doesn't fill the gap yet, use the largest scaler.
    scaler = & Scales[ns - 1].I;
  }
  else {
    for (i = 0; ! scaler && i < ns; i++) {                  // Otherwise, maybe a smaller one will do?
      if (expo2be(flt) + Scales[i].I.nsb >= minexp) {
        scaler = & Scales[i].I;
      }
    }
  }

  assert(scaler); 

  if (clz(flt) < scaler->nsb) {                             // Check if there's enough room for the multiplication.
    needed = scaler->nsb - clz(flt);                        // Number of bits we need for the multiplication.
    vrights(& flt->I, needed, 0);                           // Shift right to create needed space.
    flt->Exp.bin += needed;                                 // Adjust binary exponent; this is what moves us in the [-63, 0] range.
  }

  vmul(& flt->I, scaler, & P.I);

  flt->Exp.dec += scaler->exp;                              // Decimal exponent bumped up by scaler power of ten.

  memcpy(flt->u32, P.u32, sizeof(P.u32));                   // Copy product back to float ...
  
  flt->I.nsb = P.I.nsb;                                     // ... and the nsb from the product.

}

static uint32_t need2scaleUp(const Float_t * flt) {         // Return 1 when the float needs to be scaled up.

  if (flt->I.nsb < reqnsb && flt->Exp.bin < -50) {          // We want enough significant bits, if the exponent isn't too big yet.
    return 1;
  }
  
  if (expo2be(flt) < minexp) {                              // Our binary exponent must be in the range [-64, 0] for fmt.
    return 1;
  }

  return 0;

}

static void digits2ascii(ecvt_t e) {                        // Convert the current digits from numerical to ascii value.

  uint32_t i;
  
  for (i = 0; i < e->ndig; i++) { e->digits[i] += '0'; }

  e->digits[e->ndig] = 0;                                   // Turn it into a C string.

}

static void scaledown(ECvt_t * n, Float_t * flt) {          // Scale down a mantissa to decrease the binary exponent.

  Float_t   Q = { .I.width = sizeof(Q.u32) * 8 };           // The quotient.
  cuintxx_t scaler = NULL;
  uint32_t  i;

  if (flt->Exp.bin - Scales[ns - 1].I.nsb > maxexp) {       // If the largest doesn't fill the gap yet, use the largest scaler.
    scaler = & Scales[ns - 1].I;
  }
  else {
    for (i = 0; ! scaler && i < ns; i++) {                  // Otherwise, maybe a smaller one will do?
      if (flt->Exp.bin - Scales[i].I.nsb <= maxexp) {
        scaler = & Scales[i].I;
      }
    }
  }

  assert(scaler); 

  vlefts(& flt->I, scaler->nsb, 0);

  flt->mantissa |= (1ULL << (scaler->nsb - 1)) - 1;         // Fold in half of the divisor. Improves the rounding (Qnsb = Fnsb - Snsb + B, B = 0 or 1).

  flt->Exp.bin -= scaler->nsb;

  vdiv(& flt->I, scaler, & Q.I, NULL);

  flt->Exp.dec -= scaler->exp;
  
  memcpy(flt->u32, Q.u32, sizeof(Q.u32));                   // Copy quotient back to float ...
  
  flt->I.nsb = Q.I.nsb;                                     // ... and the nsb from the quotient.

}

void sbtecvt(ecvt_t ecvt) {

  Float_t Flt = { .I.width = sizeof(Flt.u32) * 8 };
  int32_t loose;

  memset(& ecvt->negative, 0x00, ecvtclr);

  ieee2float(ecvt, & Flt, ecvt->width == 64 ? & IEEE64c : & IEEE32c);

  if (Finite == Flt.type || Denormal == Flt.type) {

    while (need2scaleUp(& Flt)) {
      scaleup(ecvt, & Flt);
    }

    while (Flt.Exp.bin > maxexp) {                          // Need to scale down because exponent too large?
      scaledown(ecvt, & Flt);
    }

    if ((loose = Flt.I.nsb - reqnsb) > 0) {                 // If we have more bits than fit, we need to loose some.
      vrights(& Flt.I, (uint32_t) loose, 0);                // So shift the excess bits off to the right.
      Flt.Exp.bin += loose;
      Flt.I.nsb -= loose;
    }

    fmt(ecvt, & Flt);

    digits2ascii(ecvt);
  }
  else if (Zero == Flt.type) {
    ecvt->decpt = 1;                                        // The first zero really is the first significant digit.
    digits2ascii(ecvt);
  }
  else {                                                    // Either NaN or Inf
    assert(NaN == Flt.type || Infinite == Flt.type);
  }

}
