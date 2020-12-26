#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#include <cuxion.h>

static void notfound(fmtcb_t cb) {

  assert(0 == printf("Unknown type/tag '%s'\n", cb->key));

}

static FmtSpec_t Default = {
  .tag = NULL,
  .drv = notfound,
};

static void drv_k(fmtcb_t cb) {                             // Character type %k specifier callback.
 
  uint32_t kvpair = cb->Arg.u32;                            // Upper bits encode to entry number.
  uint32_t entry  = kvpair >> 3;
  uint32_t table  = kvpair & 0x7;                           // Lower 3 bits encode the table index.

  cux_sprintf(cb->buffer, "[%d:%d]", entry, table);
  
}

static FmtSpec_t Custom_k = {     // User defined format character.
  .type = 'k',
  .args = 1,
  .drv  = drv_k,
};

static const struct {
  FmtCfg_t  Cfg;
  fmtspec_t specs[8];
} FmtCfg = {
  .Cfg.num = 8,                   // Format specs; ordered to format character.
  .specs = { 
    & Default,                    // When not found, this one is selected.
    & Fmt_percent_b,              // Binary format.
    & Fmt_percent_c,              // Character format.
    & Fmt_percent_d,              // Signed integers.
    & Fmt_percent_e,              // Floating point, e format (needs 'c', 'd' and 's').
    & Custom_k,                   // Custom specifier.
    & Fmt_percent_s,              // Standard string format.
    & Fmt_percent_x,              // Hexadecimal format.
  },
};

void myprintf(const char * fmt, ...) {                      // The only exported function api.

  va_list  ap;
  FmtBuf_t Fmt;
  char     buf[1024];                                       // Just an example.

  cux_initbuf(& Fmt, & FmtCfg.Cfg, buf, sizeof(buf));
  
  va_start(ap, fmt);
  cux_vprintf(& Fmt, fmt, ap);
  va_end(ap);

  write(1, buf, Fmt.cursor - buf);

}
