// Copyright 2020 Steven Buytaert.

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <cuxion.h>

typedef struct SKey_t * skey_t;

typedef int32_t (* cmpfn_t)(skey_t key, fmtspec_t spec);

typedef struct SKey_t {           // A search key
  cmpfn_t      cmp;               // Comparison function
  char         type[35];          // When a character type specifier, only compare type[0]
  char         end;
} SKey_t;

void cux_sprintf(fmtbuf_t buf, const char * fmt, ...) {

  va_list ap;

  va_start(ap, fmt);
  cux_vprintf(buf, fmt, ap);
  va_end(ap);

}

int cux_snprintf(fmtcfg_t cfg, char * str, size_t size, const char * fmt, ...) {

  va_list ap;
  int     result;

  va_start(ap, fmt);
  result = cux_vsnprintf(cfg, str, size, fmt, ap);
  va_end(ap);

  return result;
  
}

int32_t cux_mprintf(fmtcb_t cb, const char * fmt, ...) {    // Perform a measuring format call

  va_list ap;
  int     result;

  va_start(ap, fmt);
  result = cux_vsnprintf(cb->buffer->cfg, NULL, 0, fmt, ap);
  va_end(ap);

  return result;

}

int cux_vsnprintf(fmtcfg_t cfg, char * str, size_t size, const char * fmt, va_list ap) {

  FmtBuf_t Buf;

  cux_initbuf(& Buf, cfg, str, size);
  cux_vprintf(& Buf, fmt, ap);

  return Buf.cursor - Buf.space;

}

#define NE(A) (sizeof(A) / sizeof(A[0]))

static void add_no_char(fmtcb_t cb, uint32_t c) {
  cb->buffer->cursor++;   
}
   
static void add_char(fmtcb_t cb, uint32_t c) {

  if (cb->buffer->remain) {
    *cb->buffer->cursor++ = c;
    cb->buffer->remain--;
  }
  else {
    cb->out = add_no_char;
    cb->buffer->cursor++;
  }
     
}

void cux_fill(fmtcb_t cb, uint32_t required) {              // Fill with the filler character until enough room remains for the field data

  while (required < cb->Field.width--) {
    cb->out(cb, cb->Flags.fill);
  }
  
}

void cux_pad(fmtcb_t cb, uint32_t required) {               // Fill with the filler character until enough room remains for the field data

  if (0 == cb->Flags.left && required < cb->Field.width) {  // We must right align, so we maybe need to put some padding spaces first
    cux_fill(cb, required);
  }

}

typedef struct FmtLSpec_t {
  const char *   spec;            // Length specifier string
  uint8_t        size;            // Size of the length specifier
  uint8_t        numw;            // The size of the argument in number of 32 bit words
  uint8_t        pad[2];
} FmtLSpec_t;

static const FmtLSpec_t LSpec[] = {                         // Sort these from longest size (longest match) to lowest size
  { .spec = "I32", .size = 3, .numw = 1        },
  { .spec = "I64", .size = 3, .numw = 2        },
  { .spec = "hh",  .size = 2, .numw = 1        },
  { .spec = "ll",  .size = 2, .numw = 2        },
  { .spec = "I",   .size = 1, .numw = 1        },
  { .spec = "L",   .size = 1, .numw = 2        },
  { .spec = "h",   .size = 1, .numw = 1        },
  { .spec = "j",   .size = 1, .numw = 1        },
  { .spec = "l",   .size = 1, .numw = 1        },
  { .spec = "q",   .size = 1, .numw = 2        },
  { .spec = "t",   .size = 1, .numw = no32wiap },           // Both t an z determined by 'number of 32 bit words in a pointer'
  { .spec = "z",   .size = 1, .numw = no32wiap },
  {                                            },           // End of list marker
};

static const char * lenspec(fmtcb_t cb, const char * fmt) { // Process a length specification of the given size

  const FmtLSpec_t * cur;

  cb->size = 32;                                            // Default is 32 bits argument

  for (cur = & LSpec[0]; cur->spec; cur++) {
    if (0 == strncmp(fmt, cur->spec, cur->size)) {          // Find first matching, therefore we search from longest to shortest
      fmt += cur->size;
      cb->size = 32 * cur->numw;
      break;
    }
  }
  
  return fmt;
  
}

static fmtspec_t search(fmtcfg_t cfg, skey_t key) {

  int32_t   low = 0;                                        // Must be signed !
  int32_t   mid;                                            // Must be signed !
  int32_t   high = cfg->num - 1;                            // Must be signed !
  int32_t   com;                                            // Must be signed !
  uint32_t  i;
  fmtspec_t spec = NULL;

  while (low <= high && ! spec) {                           // Simple binary search
    mid = (low + high) / 2;                                 // Divide space to be searched in two parts
    com = key->cmp(key, cfg->specs[mid]);                   // Calculate comparison value
    if (0 > com) {
      high = mid - 1;                                       // Search lower half
    }
    else if (0 < com) {
      low = mid + 1;                                        // Search higher half
    }
    else {
      spec = cfg->specs[mid];                               // We've found the spec
    }
  }  

  for (i = 0; ! spec && i < cfg->num; i++) {                // Fall back to linear search if not yet found (fix this)
    if (0 == key->cmp(key, cfg->specs[i])) {
      spec = cfg->specs[i];                                 // Will stop further searching
    }
  }
     
  return spec ? spec : cfg->specs[0];                       // When not found, return default spec

}

static int is_digit(int32_t c) {                            // Return true when the character is a numerical character
  return (c >= '0' && c <= '9');
}

static uint32_t atoi_and_advance(const char **s) {          // Return the integer value in the string, given by reference, advance the string refence

  int32_t i = 0;
   
  while (is_digit(**s)) {
    i = (i * 10) + *((*s)++) - '0';
  }
  
  return i;

}

static void narg(uint32_t s[], fmtcb_t cb, int32_t val) {   // Get the width or precision as the next argument; next argument is passed as 'val'

  if (val < 0) {                                            // When negative ...
    val = - val;                                            // ... make positive ...
    cb->Flags.left = 1;                                     // ... set left alignment
  }

  s[0] = (uint32_t) val;
  
}

static int32_t cmp_type(skey_t key, fmtspec_t spec) {       // Compare based on type character in key->type[0]
  assert(spec);                                             // Can not be called with NULL spec
  return key->type[0] - spec->type;
}

static int32_t cmp_tag(skey_t key, fmtspec_t spec) {        // Compare based on tag
  return strcmp(key->type, spec->tag ? spec->tag : "");     // We can not provide strcmp a NULL tag, so provide the empty string if there's no tag
}

typedef struct FSpec_t {
  char     flag;                  // The flag character
  char     set;                   // What value to write at the following ...
  uint16_t i;                     // ... byte offset in the callback structure
} FSpec_t;

static const FSpec_t FSpecs[] = {
  { .flag = '-', .set =   1, .i = offsetof(FmtCb_t, Flags.left) },
  { .flag = '+', .set =   1, .i = offsetof(FmtCb_t, Flags.sign) },
  { .flag = '#', .set =   1, .i = offsetof(FmtCb_t, Flags.afrm) },
  { .flag = ' ', .set = ' ', .i = offsetof(FmtCb_t, Flags.fill) },
  { .flag = '0', .set = '0', .i = offsetof(FmtCb_t, Flags.fill) },
  {                                                             }, // End of list marker
};

static const char * flags(fmtcb_t cb, const char * c) {     // Process the flags given in the above list, return next character pointer after any flags

  const FSpec_t * cur;
  
  again: for (c++, cur = & FSpecs[0]; cur->flag; cur++) {
    if (c[0] == cur->flag) {
      ((char *) cb)[cur->i] = cur->set;                     // Write the given setting in the callback structure ...
      goto again;                                           // ... and restart flag scanning from scratch
    }
  }

  return c;

}

void cux_vprintf(fmtbuf_t buffer, const char * fmt, va_list args) {

  SKey_t  Key;
  FmtCb_t Cb;
  char *  tag;
  
  Cb.buffer = buffer;                                       // Initialize callback with proper buffer and output driver
  Cb.out = buffer->size ? add_char : add_no_char;

  for (; fmt[0]; fmt++) {                                   // Go over the format string, character by character until terminating 0 reached
    if ('%' != fmt[0]) {
      Cb.out(& Cb, fmt[0]);                                 // When not '%', just copy to output ...
      continue;                                             // ... and do next character
    }

    if ('%' == fmt[1]) {
      Cb.out(& Cb, '%');                                    // Output a percent sign
      fmt++;
      continue;
    }

    memset(& Cb, 0x00, offsetof(FmtCb_t, buffer));          // Set all to default values ...
    Cb.Flags.fill = ' ';                                    // ... and padding filler to space ...
    Cb.Field.precision = 0xffffffff;                        // ... and precision to unspecified.

    fmt = flags(& Cb, fmt);                                 // Parse and process potential flags

    if (is_digit(fmt[0])) {
      Cb.Field.width = atoi_and_advance(& fmt);             // Get the field width as a number and advance the format string
    }
    else if ('*' == fmt[0]) {                               // Field width is given as the next argument
      fmt++;
      narg(& Cb.Field.width, & Cb, va_arg(args, int32_t));
    }

    if ('.' == fmt[0]) {
      fmt++;                                                // Skip past the decimal point and get the precision
      if (is_digit(fmt[0])) {                               // Precision is literal, get and advance format string
        Cb.Field.precision = atoi_and_advance(& fmt);
      }
      else if ('*' == fmt[0]) {                             // Precision is given as the next argument
        fmt++;
        narg(& Cb.Field.precision, & Cb, va_arg(args, int32_t));
      }
    }

    fmt = lenspec(& Cb, fmt);                               // Process length specification, if any

    if ('<' == fmt[0]) {                                    // If we have a tag specifier form ...
      for (tag = Key.type, fmt++; '>' != fmt[0]; fmt++) {   // ... get the specifier tag
        *tag++ = fmt[0];
        if (tag == & Key.end) break;                        // Don't overwrite the key space
      }
      *tag = 0;                                             // 0 terminate the specifier tag, even if we broke above, the 'end' space is available
      Key.cmp = cmp_tag;
    }
    else {                                                  // Single character type specifier
      Key.type[0] = fmt[0];
      Key.type[1] = 0x00;                                   // Zero terminate for callback to dump
      Key.cmp = cmp_type;
    }

    Cb.spec = search(buffer->cfg, & Key);
    Cb.key = Key.type;                                      // Information for the callback, zero terminated specifier string

    if (Cb.spec->args) {                                    // Use specified number of argument words
      Cb.size = (32 * Cb.spec->args);                       // Note that the callback can still examine the length specification
    }

    if (96 == Cb.size) {                                    // Special 'args' length to indicate double format.
      Cb.Arg.f64 = va_arg(args, double);
    }
    else if (64 == Cb.size) {                               // ptrdiff_t or size_t on 64 bit system.
      Cb.Arg.u64 = va_arg(args, uint64_t);
    }
    else {
      Cb.Arg.u32 = va_arg(args, uint32_t);
    }

    Cb.spec->drv(& Cb);                                     // Call the proper driver
  }

  if (buffer->size) {                                       // Only try to add a trailing 0 when there's a buffer
    if (buffer->remain) {
      *buffer->cursor = 0;                                  // Add a trailing 0 but don't change the number of remaining characters
    }
    else {
      buffer->space[buffer->size - 1] = 0;                  // Output was truncated, so add a trailing 0 at the end of the buffer.
    }
  }

}

void cux_initbuf(fmtbuf_t buf, fmtcfg_t cfg, char space[], uint32_t size) {

  buf->space  = space;
  buf->cursor = space;
  buf->size   = size;
  buf->remain = size ? size - 1 : 0;                        // Reserve space for trailing 0 if there is a buffer
  buf->cfg    = cfg;

}
