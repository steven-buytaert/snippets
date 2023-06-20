#include <stdlib.h>
#include <assert.h>
#include <string.h>

// Copyright 2023 Steven Buytaert

#include <avalanche.h>

typedef struct AvCount_t * cnt_t; // Internal shorthand.

static uint32_t roundup(uint32_t value, uint32_t powerOfTwo) {
  return (value + (powerOfTwo - 1)) & ~(powerOfTwo - 1);
}

static void countFlipped(avtest_t test, hut_t a, hut_t b) { // Count the bits that where flipped.

  uint32_t i;
  uint8_t  mask;
  cnt_t    cnt;

  assert(a->width == b->width);
  
  for (cnt = test->Counts, i = 0; i < a->width; i++) {      // Go over all bytes and bits to count the flips.
    for (mask = 1; mask; mask <<= 1, cnt++) {
      if ((a->hash[i] & mask) != (b->hash[i] & mask)) {     // This bit was flipped.
        cnt->count++;
      }
    }
  }

}

// See https://en.wikipedia.org/wiki/Methods_of_computing_square_roots

static float sqrtguess(float z) {                           // Initial guess for the qsuare root value.

  union {
    float f;
    uint32_t i;
  } val = { .f = z };

  if (! val.i) { return 0.0; }                              // Not a guess but pretty accurate.

  val.i -= 1 << 23;                                         // Subtract 2^m.
  val.i >>= 1;                                              // Divide by 2.
  val.i += 1 << 29;                                         // Add ((b + 1) / 2) * 2^m.

  return val.f;

}

// See https://blogs.sas.com/content/iml/2016/05/16/babylonian-square-roots.html

static float bsqrt(float z) {                               // Babylonian Square Root

  float x0 = -0.0;
  float x1 = sqrtguess(z);                                  // Initial guess.

  while (x1 != x0) {                                        // Since IEEE754 32 bits is finite, this is guaranteed to happen.
    x0 = x1;                                                // Result of previous loop or the initial guess.
    x1 = (x0 + z / x0) / 2.0;                               // Calculate a better approximation.
    if (x1 != x1) break;                                    // If x1 has degenerated into a NaN.
  }

  return x1;

}

static void calcBadness(avtest_t test) {

  cnt_t    cnt = test->Counts;
  uint32_t i;
  uint32_t bits = test->HUT.width * 8;
  float    tmp;
  float    mean = 0.0;
  float    fifc = (float) test->ifc;
  float    stddev = 0.0;
  float    worst = 0.0;

  test->worstbit = 0xffffffff;
  
  for (cnt = test->Counts, i = 0; i < bits; i++, cnt++) {
    tmp = (float) abs((int)(test->ifc - cnt->count));
    cnt->devperc = (100.0 * tmp) / fifc;
    if (cnt->devperc > worst) {
      worst = cnt->devperc;
      test->worstbit = i;
    }
    mean += tmp;
  }

  mean /= (float) bits;                                     // Absolute average over all bits.
  test->devperc = (100.0 * mean) / fifc;                    // Expressed as a percentage.

  for (cnt = test->Counts, i = 0; i < bits; i++, cnt++) {   // Calculate standard deviation.
    tmp = cnt->devperc;
    tmp -= test->devperc;
    stddev += (tmp * tmp);
  }

  test->stddev = bsqrt(stddev / (float) bits);

}

uint32_t doAvTest(avtest_t test) {                          // Do a test; return non zero when failed.

  uint32_t       i;
  uint8_t        seed[256];
  uint8_t        v2h[256];
  HUT_t          HUT;                                       // A second HUT to alternate between calls so we can compare.
  const uint32_t width = test->HUT.width;
  uint8_t        mask;
  uint8_t        byte = 0;
  uint8_t        bit = 0;
  hut_t          huts[2] = { & test->HUT, & HUT };          // The 2 huts we will alternate between.
  uint32_t       mode = 0;                                  // 0 is set bits one by one, 1 is push a zero through, 2 is push a one through.

  if (! width || ! test->HUT.calc) { return 1; }            // Must have a proper width and a hash function.

  if (! test->count) { test->count = 1000 * width; }

  test->count = roundup(test->count, width * 2);            // You want all bits to be set and cleared evenly, so width x 2.
  
  if (test->count < 640) { test->count *= 640; }

  if (! test->title) { test->title = ""; }

  test->ifc = test->count / 2;

  memset(& HUT, 0x00, sizeof(HUT_t));
  memset(v2h, 0x00, sizeof(v2h));
  memset(seed, 0x00, sizeof(seed));
  memset(& test->HUT.hash, 0x00, sizeof(test->HUT.hash));

  HUT.width = test->HUT.width;
  HUT.calc  = test->HUT.calc;
  
  for (i = 0; i < test->HUT.width * 8; i++) {               // Prepare the counts.
    test->Counts[i].count = 0;
    test->Counts[i].bitnum = i;
  }

  if (! test->keepstate) {
    memset(test->state, 0x00, sizeof(test->state));
  }
  
  test->HUT.calc(huts[1], test->state, v2h, seed);          // Initial hash with cleared seed.
  memcpy(seed, huts[1]->hash, width);                       // Use huts[1] since first in the loop will be huts[0].

  for (i = 0; i < test->count; i++) {
    mask = 1 << bit;
    if (0 == mode) {                                        // Set the next ms bit.
      v2h[byte] ^= mask;
    }
    else if (1 == mode) {                                   // Push a single zero through; all bits are now set.
      memset(v2h, 0xff, width);
      v2h[byte] ^= mask;
    }
    else if (2 == mode) {                                   // Push a single one through.
      memset(v2h, 0x00, width);
      v2h[byte] |= mask;
    }

    if (! test->randomize) {
      if (++bit == 8) {
        bit = 0;
        if (++byte == width) {
          byte = 0;
          if (0 == mode) {                                  // End of the hash width, select next mode.
            mode = 1;
          }
          else if (1 == mode) {
            mode = 2;
          }
          else {
            mode = 0;
            memset(v2h, 0x00, width);                       // Start from fresh; clear again.
          }
        }
      }
    }
    else {
      bit = random() % 8;
      byte = random() % test->HUT.width;
    }

    test->HUT.calc(huts[i & 1], test->state, v2h, seed);    // The huts[i & 1] will alternate between both.
    memcpy(seed, huts[i & 1]->hash, width);
    countFlipped(test, huts[0], huts[1]);                   // Count hash bit flips, after flipping a single v2h bit.
  }

  calcBadness(test);

  return 0;

}

void dumpAvTest(avtest_t test, FILE * out) {

  uint32_t       i;
  const uint32_t width = test->HUT.width;
  cnt_t          cnt = test->Counts;

  if (strlen(test->title)) {
    fprintf(out, "----- %s -------\n", test->title);
  }
  fprintf(out, "      %srandom flips.\n", test->randomize ? "" : "non-");
  fprintf(out, "      std dev %6.3f%%\n", test->stddev);
  fprintf(out, " IFC %8u %6.3f%%\n", test->ifc, test->devperc);
  for (i = 0; i < width * 8; i++, cnt++) {
    fprintf(out, "%2u C %8u %6.3f%%\n", cnt->bitnum, cnt->count, cnt->devperc);
  }

}
