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

// Dump a module to generate test data.

const char * T2A[] = {
  "None",
  "Import",
  "Export",
  "LibName",
  "AppName",
  "Source",
  "CSource",
  "CXXSource",
  "ASource",
  "ASource2pp",
  "Header",
  "DocSource",
  "CFlag",
  "CXXFlag",
  "ASFlag",
  "LDFlag",
  "DocFlag",
  "ModFolder",
  "BuildFolder",
  "LDLib",
  "CXXCompiler",
  "CCompiler",
  "CLinker",
  "CXXLinker",
  "ToolCC",
  "ToolLD",
  "TOOLCXX",
  "UMBFlag",
  "Generated",
  "Path2Obj",
  "Goal",
};

void dumpmod(mod_t mod) {

  uint32_t i;
  uint32_t x;
  item_t   a;

  if (! Ctx.tcdump) return;                                 // Only if requested.

  static uint32_t n = 0;
  
  static FILE * df = NULL;

  if (! df) {
    assert(Ctx.tcdump);
    df = fopen(Ctx.tcdump, "w");
    if (! df) {
      mkerror("Could not open '%s' for tcdump (%s)", Ctx.tcdump, strerror(errno));
    }
  }

  assert(df);

  if (mod) {
    fprintf(df, "struct {\n");
    fprintf(df, "  Mod_t  Mod;\n");
    fprintf(df, "  Item_t Items[%zu];\n", mod->num);
    fprintf(df, "} Mod_%u = {\n", n++);
    fprintf(df, "  .Mod = { .path = { ");

    for (i = 0; i < sizeof(mod->path); i++) {
      if (0 == mod->path[i]) break;
      fprintf(df, "'%c', ", mod->path[i]);
    }

    fprintf(df, " }, .ums = { ");

    for (i = 0; i < sizeof(mod->ums); i++) {
      if (0 == mod->ums[i]) break;
      fprintf(df, "'%c', ", mod->ums[i]);
    }

    fprintf(df, " }, .num = %zu, .name = \"%s\", .Index = { ", mod->num, mod->name);
  
    for (i = 0; i < NUM(mod->Index); i++) {
      fprintf(df, "{ %d, %d }, ", mod->Index[i].from, mod->Index[i].to);
    }
  
    fprintf(df, " } },\n");

    fprintf(df, "  .Items = {\n");
  
    for(a = & mod->Items[0], i = 0; i < mod->num; i++, a++) {
      fprintf(df, "    { .type = %11s, .sub = %11s, .len = %2zu, .name = { ", T2A[a->type], T2A[a->sub], a->len);
      for (x = 0; x < sizeof(a->name); x++) {
        if (0 == a->name[x]) break;
        fprintf(df, "'%c', ", a->name[x]);
      }
      fprintf(df, " } },\n");
    }

    fprintf(df, "  },\n");
    fprintf(df, "};\n\n");

  }
  else {

    fprintf(df, "void initTC(void) {\n\n");

    fprintf(df, "  Ctx.start = & Mod_0.Mod;\n");
    
    for (i = 1; i < n; i++) {
      fprintf(df, "  Mod_%d.Mod.next = & Mod_%d.Mod;\n", i - 1, i);
    }

    fprintf(df, "  Ctx.last = & Mod_%d.Mod;\n\n", i - 1);

    fprintf(df, "}\n");

    fclose(df);
  
  }

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
