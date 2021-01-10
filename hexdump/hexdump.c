// Copyright 2021 Steven Buytaert

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

uint32_t hex2buf(char b[], uint32_t s, const void * d2d, uint32_t n, uint32_t off, uint32_t horcap) {

  const uint8_t * data = d2d;
  uint8_t   hex[64];                                        // The hex dump of the data
  uint32_t  i;
  uint32_t  j;
  
  union {
    uint32_t u;                                             // Offset in output buffer
    int32_t  r;
  } Off = { 0 };

#define out(b, Off, s, f, a...) { if (Off.u < s) { Off.r += snprintf(b + Off.u, s - Off.u, f, ##a); } }

  if (horcap) {
    out(b, Off, s, "                0   1   2   3   4   5   6   7   8   9   a   b   c   d   e   f\n");
  }

  j = 0;
  memset(hex, 0, sizeof(hex));
  out(b, Off, s, " 0x%08x :  ", off);
  hex[0] = '|';

  for (i = 0; i < n; i++) {
    out(b, Off, s, "%02x  ", data[i]);
    hex[j + 1] = isgraph(data[i]) ? data[i] : '.';
    j += 1;
    if (j == 16) {
      j = 0;
      out(b, Off, s, "  %s|\n 0x%08x :  ", hex, off + i + 1);
      memset(hex, 0, sizeof(hex));
      hex[0] = '|';
    }
  }

  if (j != 0) {
    for (i = j; i < 16; i++) { out(b, Off, s, "--  "); }
    out(b, Off, s, "  %s", hex);
    for (i = j; i < 16; i++) { out(b, Off, s, "-");    }
    out(b, Off, s, "|");
  }

  return (int) (s - Off.u) > 0 ? Off.u : s;

}

void hexdump8k(const void * d2d, uint32_t n, const char * msg) {

  char buf[(1024 * 8) + 1];

  n = (n > 512) ? 512 : n;

  hex2buf(buf, sizeof(buf) - 1, d2d, n, 0, 1);

  printf("%s\n%s\n", msg, buf);

}
