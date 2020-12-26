#include <string.h>
#include <assert.h>
#include <cuxion.h>

static void drv(fmtcb_t cb) {

  uint32_t * done = cb->Arg.ref;                            // Argument is a reference to a value where we need to write ...

  if (cb->Arg.ref) {
    done[0] = cb->buffer->cursor - cb->buffer->space;       // ... the number of characters done already
  }
   
}

FmtSpec_t Fmt_percent_n = {
  .type    = 'n',
  .args    = no32wiap,                                      // Note that 'number of 32 bit words in a pointer' give the correct number of argument words on 64 or 32 bit systems
  .drv     = drv,
};
