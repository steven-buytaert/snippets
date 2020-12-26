// Copyright 2020 Steven Buytaert.

#include <string.h>
#include <cuxion.h>

static void drv(fmtcb_t cb) {

  char *   cursor = cb->Arg.ref ? cb->Arg.ref : "<null>";
  uint32_t length = strlen(cursor);

  cux_pad(cb, length);
     
  while (*cursor) {
    cb->out(cb, *cursor++);
  }

  cux_fill(cb, length);
   
}

FmtSpec_t Fmt_percent_s = {
  .type    = 's',
  .args    = no32wiap,            // Note that 'number of 32 bit words in a pointer' give the correct number of argument words on 64 or 32 bit systems
  .drv     = drv,
};
