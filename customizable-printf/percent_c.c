<<<<<<< HEAD
=======
// Copyright 2020 Steven Buytaert.

>>>>>>> 48f6e74 (Implementation of customizable and extendable printf.)
#include <string.h>
#include <assert.h>
#include <cuxion.h>

static void drv(fmtcb_t cb) {

  cux_pad(cb, 1);
  cb->out(cb, (char) cb->Arg.u32);
  cux_fill(cb, 1);
   
}

FmtSpec_t Fmt_percent_c = {
  .type    = 'c',
  .drv     = drv,
};
