#include <string.h>
#include <assert.h>

/*

  Copyright 2020, Steven Buytaert.

  License information pertaining to the parts of the code following, referenced with
  'Hackers Delight', i.e. functions vmulmnu and vdivmnu, is as follows:

  You are free to use, copy, and distribute any of the code on this web site, whether modified
  by you or not. You need not give attribution. This includes the algorithms (some of which
  appear in Hacker's Delight), the Hacker's Assistant, and any code submitted by readers.
  Submitters implicitly agree to this.

  The textural material and pictures are copyright by the author, and the usual copyright
  rules apply. E.g., you may store the material on your computer and make hard or soft copies
  for your own use. However, you may not incorporate this material into another publication
  without written permission from the author (which the author may give by email).

  The author has taken care in the preparation of this material, but makes no expressed or
  implied warranty of any kind and assumes no responsibility for errors or omissions. No
  liability is assumed for incidental or consequential damages in connection with or arising
  out of the use of the information or programs contained herein. 

  (More info on Hackers Delight: https://en.wikipedia.org/wiki/Hacker%27s_Delight)

*/

#include <uintxx.h>

static uint32_t clz32(uint32_t u32) {

  return u32 ? (uint32_t) __builtin_clz(u32) : 32;          // Builtin result is undefined when argument is 0.

}

uint32_t vnsb(cuintxx_t v) {                                // Return number of significant bits in the uintxx_t.

  int32_t  x = v->width / 32;                               // Number of 32 bit words.
  uint32_t offset = v->width;

  for (x--; x >= 0; x--, offset -= 32) {
    if (v->u32[x]) return offset - clz32(v->u32[x]);
  }

  return 0;

}

static uint32_t roundup(uint32_t value, uint32_t rv) {

  return (value + (rv - 1)) & ~(rv - 1);

}

static uint32_t nsu(cuintxx_t uintxx) {                     // Return the number of significant uint16_t units of the integer.

  assert(vnsb(uintxx) == uintxx->nsb);                      // NSB field must be kept up to date, but we only check when assert enabled!

  return roundup(uintxx->nsb, 16) / 16;

}

// Hackers Delight 2003, Figure 8-1, multiword integer multiplication, signed (upper part).

static void vmulmnu(const uint16_t u[], uint32_t m, const uint16_t v[], uint32_t n, uint16_t w[], uint32_t z) {

  const uint32_t bsize = 16;                                // Size of a single digit, in bits.
  const uint32_t bmask = (1u << bsize) - 1;
  uint32_t       k, t;
  uint32_t       i, j;

  memset(w, 0x00, z * 2);                                   // Clear first.

  for (j = 0; j < n; j++) {
    for (k = 0, i = 0; i < m; i++) {
      t = (uint32_t) (u[i] * v[j]) + w[i + j] + k;
      w[i + j] = t & bmask;
      k = t >> bsize;
    }
    if (j + m < z) { 
      w[j + m] = k;
    } 
    else {
      assert(j + m == z && 0 == k);                         // Cases where nsb(u) + nsb(v) <= w->size * 16 but nsu(u) + nsu(v) > w->size.
    }
  }

}

static void checkwidth(cuintxx_t u) {

  assert(u->width && (u->width & 0x1f) == 0);               // Width can not be zero and must be multiple of 32.

}

void vmul(cuintxx_t u, cuintxx_t v, uintxx_t w) {

  checkwidth(u);
  checkwidth(v);
  checkwidth(w);

  assert(w->width >= vnsb(u) + vnsb(v));

  vmulmnu(u->u16, nsu(u), v->u16, nsu(v), w->u16, w->width / 16);

  w->nsb = vnsb(w);

}

// Hackers Delight 2003, , Figure 9-1, multiword integer division, unsigned.

static uint32_t vdivmnu(const uint16_t u[], uint32_t m, const uint16_t v[], uint32_t n, uint16_t q[], uint16_t w, uint16_t r[], uint32_t z) {

  const uint32_t bsize = 16;                                // Size of a single digit, in bits.
  const uint32_t bmask = (1u << bsize) - 1;
  const uint32_t b = (1u << bsize);                         // Number base or value of a single digit.
  uint16_t       un[m + 1];                                 // Both un and vn are dynamically stack allocated.
  uint16_t       vn[n];
  uint32_t       qh;                                        // Estimated quotient digit.
  uint32_t       rh;                                        // Remainder after division by qh.
  uint32_t       p;
  int32_t        t;                                         // Temporary; must be integer!
  uint32_t       s, i, k, j;

  if (m < n || n <= 0 || v[n - 1] == 0) {                   // Check for invalid parameters. Divisor can not have leading 0.
    assert(0);
    return 1;
  }

  for (j = w - 1u; j > m - n; j--) { q[j] = 0; }            // Clear upper bits we'll not write in the main loop.

  if (n == 1) {                                             // Case of single digit divisor; fast.
    for (k = 0, j = m - 1; (int32_t) j >= 0; j--) {         // K will be used as the remainder for the next division.
      q[j] = (k * b + u[j]) / v[0];                         // Remainder of previous division * base + next digit / v[0].
      k = (k * b + u[j]) - (uint32_t) (q[j] * v[0]);
    }

    if (r) { r[0] = k; memset(& r[1], 0x00, (z * 2) - 2); } // Set remainder if required and clear unused bits.

    return 0;
  }

  // Normalize by shifting v left just enough so that its
  // high-order bit is on and shift u left the same amount.
  
  s = clz32(v[n - 1]) - bsize;                              // 0 <= s <= bsize

  for (i = n - 1; i > 0; i--) {
    vn[i] = (v[i] << s) | (v[i - 1] >> (bsize - s));
  }

  vn[0] = v[0] << s;

  un[m] = u[m - 1] >> (bsize - s);                          // Unconditionally append high-order bit on dividend.

  for (i = m - 1; i > 0; i--) {
    un[i] = (u[i] << s) | (u[i - 1] >> (bsize - s));
  }

  un[0] = u[0] << s;

  for (j = m - n; (int32_t) j >= 0; j--) {                  // Main loop, we have m - n digits to process, MSD to LSD.

    // Estimate the quotient.

    qh = (un[j + n] * b + un[j + n - 1]) /      vn[n - 1];
    rh = (un[j + n] * b + un[j + n - 1]) - qh * vn[n - 1]; 

    again:
    if (qh >= b || qh * vn[n - 2] > b * rh + un[j + n - 2]) {
      qh = qh - 1;
      rh = rh + vn[n - 1];
      if (rh < b) goto again;
    }
    
    // Mutiply with qh and substract.

    for (k = 0, i = 0; i < n; i++) {
      p = qh * vn[i];
      t = (int32_t) (un[i + j] - k - (p & bmask));
      un[i + j] = t;
      k = (p >> bsize) - (uint32_t) (t >> bsize);
    }
    t = (int32_t) (un[j + n] - k);
    un[j + n] = t;
    
    q[j] = qh;                                              // Store quotient digit.

    if (t < 0) {                                            // If we substracted too much, add it back.
      q[j] = q[j] - 1;
      for (k = 0, i = 0; i < n; i++) {
        t = (int32_t) (un[i + j] + (uint32_t) vn[i] + k);
        un[i + j] = t;
        k = (uint32_t) (t >> bsize);
      }
      un[j + n] = un[j + n] + k;
    }

  }                                                         // End of main loop.

  for (i = 0; r && i < n; i++) {                            // Pass back the remainder, if necessary, ...
    r[i] = (un[i] >> s) | (un[i + 1] << (bsize - s));       // ... but unnormalize first.
  }

  assert(r == NULL || z);                                   // If r is NULL, z should also be 0 ...

  for (p = i; p < z; p++) { r[p] = 0; }                     // ... so no check for r being null needed.

  return 0;

}

uint32_t vdiv(cuintxx_t u, cuintxx_t v, uintxx_t q, uintxx_t r) {

  checkwidth(u);
  checkwidth(v);
  checkwidth(q);

  if (r) checkwidth(r);

  uint32_t s = vdivmnu(u->u16, nsu(u), v->u16, nsu(v), q->u16, q->width / 16, r ? r->u16 : NULL, r ? r->width / 16 : 0);

  q->nsb = vnsb(q);

  if (r) { r->nsb = vnsb(r); }

  return s;

}

void vrights(uintxx_t v, uint32_t shift, uint32_t fb) {     // Shift right the given amount; fill vacated bits with 0 or 1.

  uint32_t       i = 0;
  uint32_t       w;                                         // Number of whole words we can skip.
  uint32_t       bits;                                      // Remaining bits we need to shift right.
  const uint32_t n = v->width / 32;

  checkwidth(v);

  shift = (shift > v->width) ? v->width : shift;            // Truncate if too large for the size (not the nsu).

  bits = shift % 32;
  w    = shift / 32;

  if (n - 1 > w) {                                          // We work from lsb to msb - 1
    for (     ; i < n - w - 1; i++) {                       // Note that i has been initialized already.
      v->u32[i]  = v->u32[i + w] >> bits;                   // When now is 0, this is an in cell shift.
      if (bits) {
        v->u32[i] |= v->u32[i + w + 1] << (32 - bits);      // Merge with the source part.
      }
      assert(i + w + 1 < n);
    }
  }

  v->u32[i]  = v->u32[i + w] >> bits;                       // Fix the msb cell; doesn't merge with predecessor ...
  if (bits) {
    v->u32[i] |= fb ? 0xffffffff << (32 - bits) : 0;        // ... just shift and fill.
  }

  if (n == w) { i--; assert(shift == v->width); }           // Shift over whole length; filled or clear all in next loop.

  for (i++; i < n; i++) {                                   // Finish up all higher vacated cells, if any.
    v->u32[i] = fb ? 0xffffffff : 0;                        // Fully clear or fully set.
  }

  v->nsb = vnsb(v);

}

void vlefts(uintxx_t v, uint32_t shift, uint32_t fb) {      // Shift to the left. Fill vacated bits with 0 or 1.

  uint32_t       i;
  uint32_t       w;                                         // Number of whole words we can skip.
  uint32_t       bits;                                      // Remaining bits we need to shift right.
  const uint32_t n = v->width / 32;
  uint32_t       fill = fb ? 0xffffffff : 0;

  checkwidth(v);

  shift = (shift > v->width) ? v->width : shift;            // Truncate if too large for the size (not the nsu).

  bits = shift % 32;
  w    = shift / 32;

  for (i = n - 1; bits && (int32_t) (i - w) >= 1; i--) {    // We work from MSB to LSB word.
    v->u32[i]   = v->u32[i - w];                            // Skip first, if needed.
    v->u32[i] <<= bits;                                     // Move up.
    v->u32[i]  |= v->u32[i - w - 1] >> (32 - bits);         // A: use remainder bits from predecessor.
  }

  fill = bits ? fill >> (32 - bits) : 0;                    // Prepare the fill for the next loop.

  for (         ; (int32_t) (i - w) >= 0; i--) {            // Do the rest
    v->u32[i]   = v->u32[i - w];
    v->u32[i] <<= bits;
    v->u32[i]  |= fill;                                     // B: partial fill.
  }

  for (         ; (int32_t) i >= 0; i--) {
    v->u32[i] = fb ? 0xffffffff : 0;                        // C: fully fill.
  }

  v->nsb = vnsb(v);

}

void vand(uintxx_t v, uintxx_t mask) {                      // Logical and.

  int32_t n = v->width > mask->width ? mask->width : v->width;

  checkwidth(v);

  n /= 16; // Turn width into number of shorts. Can do words??.
  
  while (n-- >= 0) { v->u16[n] &= mask->u16[n]; }

  v->nsb = vnsb(v);

}
