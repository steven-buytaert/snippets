// Copyright 2020-2021 Steven Buytaert

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <hexdump.h>

typedef struct Buf_t * buf_t;

typedef struct Buf_t {
  char * space;                   // Points to the space in the buffer
  char * cur;                     // Current position in buffer to add characters
  char * end;                     // Comes just after last character of usable storage; can not be written to
} Buf_t;

typedef struct Itoa_t {           // Buffer large enough to build an 8 byte hexadecimal representation
  char * str;                     // Hexadecimal result string after uitoh call
  char   space[23];
  char   end;
} Itoa_t;

static void addstr(buf_t buf, const char * s, uint32_t n) { // Add the string 's' with the given number of characters 'n' to the buffer
  memcpy(buf->cur, s, n);
  buf->cur += n;
}

static uint32_t remain(buf_t b) {                           // Return remaining buffer space, but ensure we have a spot for the trailing \0
  return (b->cur < b->end) ? (b->end - 1 - b->cur) : 0;
}

static uint32_t used(buf_t b) {                             // Return number of characters used in the buffer
  return (b->cur - b->space);
}

static void add2out(buf_t out, buf_t add) {                 // Add the given buffer to the output buffer, if space permits

  uint32_t u = used(add);
  uint32_t r = remain(out);

  if (r) { memcpy(out->cur, add->space, (r > u) ? u : r); } // Copy to out when there's space remaining

  out->cur += u;                                            // The cursor position is always updated since we need to return # characters that would have been written

}

static void flush2out(buf_t out, buf_t hex, buf_t ascii) {  // Try to copy the hex and ascii representations to the output buffer, if space permits

  addstr(hex, "| ", 2);                                     // Add separator between dump area and ascii area

  add2out(out, hex);
  add2out(out, ascii);

  if (remain(out)) { *out->cur = '\n'; }                    // Add a newline if space permits ...
  out->cur++;                                               // ... but always account for it

}

static void uitoh(Itoa_t * buf, uint32_t w, uint64_t n) {   // Create the hexadecimal representation of a given width 'w' of unsigned number 'n' in the hex buffer

  buf->str = & buf->end;                                    // Build up backwards, so start at the end

  do {                                                      // Put characters from least to most significant ...
    *--buf->str = "0123456789abcdef"[n & 0xf];              // ... so predecrement and write digits[n % 16]
    n >>= 4;                                                // Divide by 16
  } while (n);

  while ((uint32_t) (& buf->end - buf->str) < w) {          // Pad front with zeros to achieve the proper width
    *--buf->str = '0';
  }

}

static void istart(buf_t hex, buf_t ascii, uint32_t addr) { // Reset ascii and hex buffers and start writing a new line with the given address

  Itoa_t Itoa;

  hex->cur = hex->space;                                    // Reset hex buffer
  ascii->cur = ascii->space;                                // Reset ascii buffer

  uitoh(& Itoa, 8, addr);                                   // Convert address to hex and output it nicely
  addstr(hex, "0x", 2);
  addstr(hex, Itoa.str, 8);
  addstr(hex, ": ", 2);

}

static void outhex(buf_t buf, uint32_t byte) {              // Output the given byte as a hexadecimal character in the given buffer

  Itoa_t Itoa;

  uitoh(& Itoa, 2, byte);
  addstr(buf, Itoa.str, 2);
  addstr(buf, " ", 1);                                      // Add a space after each byte

}

uint32_t hex2buf(char b[], uint32_t s, const void * d, uint32_t n) {

  union {
    const uint8_t * dump;
    uint32_t  addr;
  } Data = { .dump = d };

  uint32_t        iaddr = (Data.addr) & (0u - 16u);         // Initial address to dump, aligned on a 16 byte boundary, could be equal to d address argument
  const uint8_t * end = Data.dump + n;
  Buf_t           Out;

  struct {
    Buf_t Buf;
    char  space[32];
  } Ascii;                                                  // ASCII area buffer; note that end is not used

  struct {
    Buf_t Buf;
    char  space[64];
  } Hex;                                                    // Address and hex area buffer; note that end is not used
  
  Hex.Buf.space = Hex.space;                                // Initialize both area buffers
  Ascii.Buf.space = Ascii.space;

  Out.cur = b;                                              // Initialize the output buffer
  Out.space = b;
  s = b ? s : 0;                                            // Set size to 0 when no buffer was given
  Out.end = b + s;

#define ascii(c)  { *Ascii.Buf.cur++ = (c);                   }
#define out(s, n) { addstr(& Hex.Buf, s, n);                  }
#define hex(x)    { outhex(& Hex.Buf, x);                     }
#define flush     { flush2out(& Out, & Hex.Buf, & Ascii.Buf); }
#define start(a)  { istart(& Hex.Buf, & Ascii.Buf, a);        }

  start(iaddr);

  while (iaddr++ < Data.addr) {                             // We start dumping a 16 byte aligned addresses, so output some padding if required
    out("   ", 3); ascii(' ');
  }

  while (Data.dump < end) {
    hex(Data.dump[0]);                                      // Output hexadecimal ...
    ascii(isprint(Data.dump[0])?(char) Data.dump[0] : '.'); // ... and ascii representation, if any, of the current byte
    Data.dump++;                                            // Go to next byte, if any
    if (0 == (0xf & Data.addr)) {                           // Next address is a 16 byte aligned address ...
      flush;                                                // ... flush this line ...
      if (Data.dump < end) {                                // ... and start the next one if there's data to dump left
        start(Data.addr);
      }
    }
  }

  if (0xf & Data.addr) {                                    // Address not 16 byte aligned, we have some remainder ascii representation
    for ( ; 0xf & Data.addr; ++Data.addr) {
      out("   ", 3);                                        // Output some padding to align the ascii dump area ...
    }
    flush;                                                  // Flush the final line with the ascii remainder
  }

  if (s) {                                                  // There's always a spot for a trailing \0, if there was a buffer.
    if (remain(& Out)) { Out.cur[0] = 0; }
    else { Out.end[-1] = 0; }                               // Buffer wasn't large enough, cur progressed beyond end.
  }

  return (uint32_t) (Out.cur - Out.space);

}

void hexdump8k(const void * d2d, uint32_t n, const char * msg) {

  char buf[(1024 * 8) + 1];

  n = (n > 512) ? 512 : n;

  hex2buf(buf, sizeof(buf) - 1, d2d, n);

  printf("%s\n%s\n", msg, buf);

}
