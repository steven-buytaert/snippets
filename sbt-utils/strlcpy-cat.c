// Copyright 2023 Steven Buytaert

#include <string.h>

#include <sbt-utils.h>

// A straightforward implementation of strlcat and strlcpy as
// described in https://www.millert.dev/papers/strlcpy.html 

size_t strlcpy(char * restrict dst, const char * restrict src, size_t size) {

  size_t srclen = strlen(src);
  size_t n2c;

  if (size && --size) {                                     // Can we write a terminating NUL?
    n2c = size > srclen ? srclen : size;                    // Number of bytes to copy: smallest of remain and src length.
    memcpy(dst, src, n2c);                                  // Can use memcpy since pointers are restrict, i.e. src/dst can't overlap.
    dst[n2c] = 0;                                           // Always terminate properly.
  }

  return srclen;

}

size_t strlcat(char * restrict dst, const char * restrict src, size_t size) {

  char * ins;                                               // Insertion point.

  for (ins = dst; *ins && size; ins++, size--) { }          // Find insertion point in dst or until we run out of space.

  return strlcpy(ins, src, size) + (ins - dst);             // Use remaining size and insertion point as input to strlcpy.

}

// Versions of strlcat and strlcpy with a passed overflow reference; overflow can not be NULL.

size_t strcpy4o(char * restrict dst, const char * restrict src, size_t size, uint32_t * overflow) {

  size_t result = strlcpy(dst, src, size);

  if (result >= size) { (*overflow)++; }                    // Don't set, just add to the existing overflow count.

  return result;

}

size_t strcat4o(char * restrict dst, const char * restrict src, size_t size, uint32_t * overflow) {

  size_t result = strlcat(dst, src, size);

  if (result >= size) { (*overflow)++; }                    // Don't set, just add to the existing overflow count.

  return result;

}
