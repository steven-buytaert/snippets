// Copyright 2020-2023 Steven Buytaert

#include <dirent.h>
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
  FILE *  stream = stdout;
  
  if (Ctx.verbosity >= level) {
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    fprintf(stream, buf);
    fflush(stream);
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

static void defcb(f2find_t f2f) { f2f->found = 1; }         // Default callback. Stops further search.

typedef int32_t (* strcmp_t)(const char *s1, const char *s2);

static int32_t add2buffer(f2find_t f2f, const char * s) {   // Try to add the string to the buffer of f2f; with error handling.

  uint32_t        rem = sizeof(f2f->buffer) - f2f->off;     // Characters remaining in buffer.
  char *          add =  f2f->buffer + f2f->off;            // Start adding the string here.
  int32_t         nc = snprintf(add, rem, "/%s", s);

  if (nc < 0 || (uint32_t) nc > rem) {                      // Buffer size not enough or strange characters.
    f2f->error = 1;                                         // Will break the loop above.
  }

  return  nc;                                               // Number of characters added.

}

static void f2find_int(f2find_t f2f) {                      // Internal find.

  DIR *           dd;
  struct dirent * de;
  int32_t         nc;
  strcmp_t        cmp = f2f->anycase ? strcasecmp : strcmp;

  dd = opendir(f2f->buffer);
  if (dd) {
    while (! f2f->found && (de = readdir(dd))) {            // Loop over all files in the folder as long as necessary.
      if (de->d_name[0] == '.') continue;                   // Skip current and parent.
      if (DT_DIR == de->d_type) {
        nc = add2buffer(f2f, de->d_name);
        if (f2f->error) { break; }                          // Stop at error.
        f2f->off += nc;
        f2find_int(f2f);                                    // Search recursively.
        if (! f2f->found) {
          f2f->off -= nc;
        }
      }
      else {
//        printf("FOUND '%s'\n", de->d_name);
        if (0 == cmp(de->d_name, f2f->name)) {              // Names must match; note that lengths are also equal in this case.
          nc = add2buffer(f2f, de->d_name);
          if (f2f->error) { break; }                        // Stop at error.
          f2f->off += nc;
          f2f->callback(f2f);                               // Callback decides if we search further or not.
          if (! f2f->found) {
            f2f->off -= nc;                                 // We need to continue; restore our offset.
          }
        }
      }
    }
    closedir(dd);
  }

}

void f2find(f2find_t f2f, const char * roots[]) {

  uint32_t i;
  uint32_t size = sizeof(f2f->buffer);

  assert(f2f->name);

  f2f->found = 0;

  if (! f2f->callback) {
    f2f->callback = defcb;
  }

  for (i = 0; ! f2f->found; i++) {
    if (! roots[i]) break;
    f2f->off = (uint16_t) snprintf(f2f->buffer, size, "%s", roots[i]);
    f2find_int(f2f);
  }

}
