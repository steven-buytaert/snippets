// Copyright 2020 Steven Buytaert

#include <stdarg.h>
#include <unistd.h>

#include <gnumake.h>

#include <umbrella.h>

item_t item(mod_t mod, Type_t type) {                       // Get the first item or NULL if none.

  int32_t from = mod->Index[type].from;

  return (from >= 0) ? & mod->Items[from] : NULL;

}

cchar_t iname(mod_t mod, Type_t type) {                     // Get the name of the item or "" when not set.

  int32_t from = mod->Index[type].from;

  return (from >= 0) ? mod->Items[from].name : "";

}

mod_t ensureCap(mod_t mod, uint32_t req) {                  // Ensure there's req free slots available; return maybe realloced mod.

  size_t grow;
  size_t add;
  size_t size;

  if (mod->avail < req) {
    grow = req > 30 ? req : 30;                             // Grow with minimal 30 slots.
    add = sizeof(Item_t) * grow;                            // Number of extra bytes we need.
    size = sizeof(Mod_t);
    size += sizeof(Item_t) * (mod->num + mod->avail);
    mod = realloc(mod, size + add);
    if (mod) {
      memset(& mod->Items[mod->num + mod->avail], 0, add);  // Clear grown area only.
      mod->avail += grow;
    }
    else {
      mkerror("realloc(%u -> %u) failed", size, size + add);
    }
  }

  return mod;

}

void mkerror(const char * fmt, ...) {                       // Report error and stop Make.

  va_list ap;
  char    buf[256];
  int32_t off;
  char *  expanded;

  off = snprintf(buf, sizeof(buf), "$(error \"");

  va_start(ap, fmt);
  off += vsnprintf(buf + off, sizeof(buf) - (size_t) off, fmt, ap);
  va_end(ap);

  off += snprintf(buf + off, sizeof(buf) - (size_t) off, "\")");

  umblog(1, "%s", buf);
  
  expanded = gmk_expand(buf);
  gmk_free(expanded);

}

void umblog(uint32_t level, const char * fmt, ...) {

  va_list ap;
  char    buf[1024 * 16];
  
  if (Ctx.verbosity >= level) {
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    fprintf(stdout, buf);
    fflush(stdout);
  }

}

void out(Out_t * out, const char * fmt, ...) {              // Safely write to an internal buffer.

  va_list  ap;
  size_t   need;
  size_t   size;
  
  va_start(ap, fmt);
  need = (uint32_t) vsnprintf(NULL, 0, fmt, ap);            // First check how many bytes we need.
  va_end(ap);

  if (need >= out->rem) {                                   // Realloc if not enough left.
    size = out->off + out->rem;
    need = need < 256 ? 256 : need * 2;
    if (! out->fixed) {
      out->buf = realloc(out->buf, size + need);
      if (! out->buf) {
        mkerror("realloc(%u -> %u) failed.", size, size + need);
      }
      out->rem += need;
    }
    else {
      mkerror("bufsize %d insufficient", size);             // Should increase it in the code.
    }
  }

  va_start(ap, fmt);
  need = (uint32_t) vsnprintf(out->buf + out->off, out->rem, fmt, ap);
  va_end(ap);

  out->off += need;
  out->rem -= need;

}

void resetout(Out_t * out) {

  out->rem += out->off;

  out->off = 0;

  if (out->rem) { out->buf[0] = 0; }                        // Zero terminate.

}

void stripspace(Out_t * out) {                              // Strip the final space, if any.

  if (out->off && ' ' == out->buf[out->off - 1]) {
    out->buf[--out->off] = 0;
  }

}

void umbreval(const char * buf, const gmk_floc * floc) {    // Umbrella eval function (with verbosity control).

  umblog(4, "\n# From function %s:%ld\n\n", floc->filenm, floc->lineno);

  if (Ctx.verbosity >= 3) {
    write(1, buf, strlen(buf));                             // To avoid % expansion in printf.
  }

  gmk_eval(buf, floc);

}

void mkdirs(mod_t mod, cchar_t folder) {                    // Create the folder if it doesn't exists yet.

  char    buf[4096];                                        // We use this both as a buffer for realpath and Out.

  Out_t   Out = {                                           // Don't use the global output buffer, as it might be in use.
    .rem   = sizeof(buf),
    .off   = 0,
    .buf   = buf,
    .fixed = 1,
  };

  gmk_floc Floc = {
    .filenm = __FUNCTION__,
    .lineno = __LINE__
  };

  assert(folder);                                           // Can never be NULL.

  if (! realpath(folder, buf)) {
    if (ENOENT == errno) {
      out(& Out, "$(shell mkdir -p %s)\n", folder);         // Via make, we ask mkdir to do the heavy lifting.
    }
    else {
      mkerror("mkdirs %s (%s)", folder, strerror(errno));
    }
  }

  if (Out.off) {
    umbreval(Out.buf, & Floc);
  }

}                                                           // Yes the mod parameter isn't used.
