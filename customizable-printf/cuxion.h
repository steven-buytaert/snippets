#ifndef CUXION_H
#define CUXION_H

// Copyright 2020 Steven Buytaert.

#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>

// For an introduction to the format placeholder syntax, see http://en.wikipedia.org/wiki/Printf

typedef struct FmtSpec_t *        fmtspec_t;
typedef const struct FmtCfg_t *   fmtcfg_t;
typedef struct FmtBuf_t *         fmtbuf_t;
typedef struct FmtCb_t *          fmtcb_t;

#define no32wiap (sizeof(void *) / sizeof(uint32_t))

typedef struct FmtSpec_t {
  char           type;            // If not 0, a conversion specifier like in '%s', where the type == 's'
  uint8_t        args;            // 0 = set by length specifier, 2 or 1 number of words for pointer/size_t, 3 means double floating point.
  uint8_t        base;            // For numbers only, the number base 10, 16 or 8 (or 2 for %b specifier)
  uint8_t        issigned;        // For numbers only, when non 0, it's a signed number type specifier
  char *         tag;             // When not null, a conversion string like in '%<con>' where the tag == "con"
  void        (* drv)(fmtcb_t c); // Function that will be called when format specifier is encountered
} FmtSpec_t;

typedef const struct FmtCfg_t {
  uint32_t       num;
  fmtspec_t      specs[0];
} FmtCfg_t;

typedef struct FmtBuf_t {
  char *         space;           // Start of the buffer space we will write into
  char *         cursor;          // The current position in this output space
  uint32_t       size;            // Total size of space in bytes
  uint32_t       remain;
  fmtcfg_t       cfg;
} FmtBuf_t;

typedef void (* volatile fmtout_t)(fmtcb_t cb, uint32_t c);

typedef struct FmtCb_t {
  union {
    uint64_t     u64;             // This union *must* be aligned to 8 bytes
    int64_t      s64;
    uint32_t     u32;
    int32_t      s32;
    double       f64;             // All float arguments to functions are promoted to double.
    void *       ref;
  } Arg;
  struct {
    uint8_t      sign;            // 1 == always output a sign '+' or '-'
    uint8_t      left;            // 1 == left align the output, default is right align
    uint8_t      afrm;            // 1 == use the alternate form; meaning depends on the type
    char         fill;            // The character to use for filling or padding, default is space
  } Flags;
  struct {
    uint32_t     width;           // When 0, unspecified (field width is stretchable)
    uint32_t     precision;       // When 0xffffffff, unspecified, otherwise the precision.
  } Field;
  uint32_t       size;            // Size in bits of the Arg data; either 32 or 64
  const char *   key;             // Search key used to find the specifier
  fmtbuf_t       buffer;          // Buffer that was passed by the caller
  fmtspec_t      spec;            // The format specifier or type that caused this callback
  fmtout_t       out;             // Write a single character to the buffer
} FmtCb_t;

int     cux_snprintf(fmtcfg_t cfg, char * str, size_t size, const char * fmt, ...);
int     cux_vsnprintf(fmtcfg_t cfg, char * str, size_t size, const char * fmt, va_list ap);

void    cux_initbuf(fmtbuf_t buf, fmtcfg_t cfg, char space[], uint32_t size);
void    cux_sprintf(fmtbuf_t buf, const char * fmt, ...);
void    cux_vprintf(fmtbuf_t buf, const char * fmt, va_list ap);

int32_t cux_mprintf(fmtcb_t cb, const char * fmt, ...);
void    cux_pad(fmtcb_t cb, uint32_t required);
void    cux_fill(fmtcb_t cb, uint32_t required);

extern FmtSpec_t Fmt_percent_X;   // Standard %X processing
extern FmtSpec_t Fmt_percent_b;   // Output a binary representation
extern FmtSpec_t Fmt_percent_c;   // Standard %c processing
extern FmtSpec_t Fmt_percent_d;   // Standard %d processing
extern FmtSpec_t Fmt_percent_i;   // Standard %i processing
extern FmtSpec_t Fmt_percent_n;   // Standard %n processing
extern FmtSpec_t Fmt_percent_o;   // Standard %o processing
extern FmtSpec_t Fmt_percent_p;   // Standard %p processing
extern FmtSpec_t Fmt_percent_s;   // Standard %s processing
extern FmtSpec_t Fmt_percent_u;   // Standard %u processing
extern FmtSpec_t Fmt_percent_x;   // Standard %x processing
extern FmtSpec_t Fmt_percent_e;   // Standard %e processing
extern FmtSpec_t Fmt_percent_E;   // Standard %E processing
extern FmtSpec_t Fmt_percent_f;   // Standard %f processing
extern FmtSpec_t Fmt_percent_g;   // Standard %g processing
extern FmtSpec_t Fmt_percent_G;   // Standard %G processing

#endif // CUXION_H
