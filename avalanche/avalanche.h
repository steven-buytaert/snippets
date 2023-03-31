#ifndef AVALANCHE_H
#define AVALANCHE_H

// Copyright 2023 Steven Buytaert

#include <stdint.h>

typedef struct HUT_t *     hut_t;
typedef struct AvTest_t *  avtest_t;

typedef struct AvCount_t {
  uint32_t     bitnum;            // Bitnumber.
  uint32_t     count;             // Actual flip count.
  float        devperc;           // How many percent deviation from the ifc.
} AvCount_t;

typedef void (* huthash_t)(hut_t, uint32_t state[16], const uint8_t v2h[], const uint8_t seed[]);

typedef struct HUT_t {            // Hash under test.
  huthash_t    calc;              // Calculate a single hash; store result in hash[256] in proper endianess.
  uint32_t     width;             // The byte width of the hash.
  uint8_t      hash[256];         // Result of hashing v2h[width].
} HUT_t;

typedef struct AvTest_t {         // Avalanche test
  const char * title;             // Included in the report.
  HUT_t        HUT;               // Hash Under Test.
  uint32_t     count;             // Total number of hashes to be done; keep 0 for default.
  uint32_t     ifc;               // The ideal flip count = count / 2.
  float        devperc;           // Average deviation over all bits as a percentage.
  float        stddev;            // Standard deviation.
  uint32_t     worstbit;          // Bit with the worst deviation.
  uint32_t     keepstate;         // Set to non zero to avoid state being cleared.
  union {
    void *     ref[8];            // For worst case alignment.
    uint64_t   u64[8];            // For worst case alingment.
    uint32_t   state[16];         // Freely useable for calc(); cleared at start when keepstate is zero.
  };
  AvCount_t    Counts[0];         // HUT.width * 8 number of counting slots.
} AvTest_t;

uint32_t doAvTest(avtest_t test); // Perform an avalanche test.
void     dumpAvTest(avtest_t test);

#endif // AVALANCHE_H
