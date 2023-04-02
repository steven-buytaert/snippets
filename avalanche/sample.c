// Copyright 2023 Steven Buytaert

// Compile e.g. with clang -I . -Wall -Os -o sample sample.c lookup3.c sbtbuz1.c murmurhash.c avalanche.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <avalanche.h>

static uint32_t b2w(const uint8_t b[]) {

  uint32_t w;
  
  w  = b[0];
  w |= b[1] <<  8;
  w |= b[2] << 16;
  w |= b[3] << 24;

  return w;

}

static void w2b(uint32_t w, uint8_t b[]) {

  b[0] = w;
  b[1] = w >>  8;
  b[2] = w >> 16;
  b[3] = w >> 24;

}

uint32_t sbtbuz1(const void * d, uint32_t s, uint32_t seed);

static void hash4sbt1(hut_t hut, uint32_t args[16], const uint8_t v2h[], const uint8_t sb[]) {

  uint32_t seed = b2w(sb);
  uint32_t hash = sbtbuz1(v2h, hut->width, seed);

  w2b(hash, hut->hash);

}

uint32_t murmurhash(const char *key, uint32_t len, uint32_t seed);

static void murmur3(hut_t hut, uint32_t args[16], const uint8_t v2h[], const uint8_t sb[]) {

  uint32_t seed = b2w(sb);
  uint32_t hash = murmurhash((const char *) v2h, hut->width, seed);

  w2b(hash, hut->hash);

}

uint32_t hashlittle(const void *key, size_t length, uint32_t initval); // lookup3

static void lookup3(hut_t hut, uint32_t args[16], const uint8_t v2h[], const uint8_t sb[]) {

  uint32_t seed = b2w(sb);
  uint32_t hash = hashlittle(v2h, hut->width, seed);

  w2b(hash, hut->hash);

}

static avtest_t avtest32(void) {

  uint32_t sz = sizeof(AvTest_t) + 32 * sizeof(AvCount_t);  // They are all 32 bit hashes.
  avtest_t test = malloc(sz);
  
  assert(test);
  
  memset(test, 0x00, sz);
  
  test->HUT.width = sizeof(uint32_t);                       // Hash width in bytes.
  test->count = 64 * 100000;                                // More than enough for statistical purposes.
  test->randomize = 1;                                      // Because we use the randomize bit flip mode.

  return test;
  
}

int main(int argc, char * argv[]) {

  avtest_t avt = avtest32();

  avt->title = "Murmur3";
  avt->HUT.calc = murmur3;

  doAvTest(avt); dumpAvTest(avt, stdout);

  avt->title = "Lookup3";
  avt->HUT.calc = lookup3;

  doAvTest(avt); dumpAvTest(avt, stdout);

  avt->title = "sbtbuz1";                                   // Hash to check against; not as good as M3 and LU3 but fast.
  avt->HUT.calc = hash4sbt1;

  doAvTest(avt); dumpAvTest(avt, stdout);

}
