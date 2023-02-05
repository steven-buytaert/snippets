// From Makefile level
// gcc -shared -I ../make-4.3/src/ -Wall -Wsign-compare -fpic -o ./tools/my-make.so ./tools/my-make.c

// gcc -shared -I ../../make-4.3/src/ -Wall -Wsign-compare -fpic -o my-make.so my-make.c

// Copyright 2020-2023 Steven Buytaert

#include <umbrella.h>

#define SETFLOC(FLOC) gmk_floc FLOC = { .filenm = __FUNCTION__, .lineno = __LINE__ }

static mod_t findmod(mod_t req, const char * imported) {    // Find module 'imported' for requester 'req'.

  mod_t        mod;
  const char * lib;
  
  for (mod = Ctx.start; mod; mod = mod->next) {
    lib = iname(mod, LibName);
    if (lib[0]) {                                           // If not the empty string.
      if (0 == strcmp(lib, imported)) break;
    }
  }

  if (! mod) {
    mkerror("Could not resolve import '%s' for '%s'", imported, req->name);
  }

  return mod;
  
}

static const uint32_t bufsize = 1024;

static char * getbuf(void) {

  static char     buffers[8][bufsize];                      // A set of 8 buffers.
  static uint32_t next = 0;

  char * buf = buffers[next++];

  if (next == 8) { next = 0; }                              // Flip around.
  
  return buf;

}

static const char * bad(mod_t mod, Type_t type) {           // Return the build artefact folder for a certain item type.

  Out_t Out = {
    .rem   = bufsize,
    .off   = 0,
    .buf   = getbuf(),
    .fixed = 1,
  };

  if (Source == type) {
    out(& Out, "%s", mod->Gen.folder);
  }
  else {
    out(& Out, "%s/", Ctx.bad);

    switch (type) {
      case DocSource: out(& Out, "doc");            break;
      case Export:    out(& Out, "include");        break;
      case Import:    out(& Out, "lib");            break;
      case AppName:   out(& Out, "bin");            break;
      case ToolName:  out(& Out, "bin");            break;
      case LibName:   out(& Out, "lib");            break;
      default: mkerror("No bad for type %d", type); break;
    }
  }

  return Out.buf;

}

static const char * rep(item_t item, cchar_t suf) {         // Replace the suffix with the given one.

  const char * ext = & item->name[item->len - 1];
  size_t       rem = item->len;
  int32_t      i;

  Out_t Out = {
    .rem   = bufsize,
    .off   = 0,
    .buf   = getbuf(),
    .fixed = 1,
  };

  while (rem && ext[0] != '.') { ext--; rem--; }            // Find the dot or not.

  if (! rem) { 
    mkerror("No suffix found for '%s'", item->name);        // Note that $(error ...) adds a trailing dot.
  }
  else {
    for (i = 0; i < (ext - & item->name[0]); i++) {
      out(& Out, "%c", item->name[i]);
    }
    out(& Out, "%s", suf);
  }

  return Out.buf;

}

static const char * here(mod_t mod) {                       // Create a path the points to $(HERE) for this module.

  Out_t Out = {
    .rem   = bufsize,
    .off   = 0,
    .buf   = getbuf(),
    .fixed = 1,
  };

  if (mod->pathlen) {
    out(& Out, "%s", mod->path);
  }

  return Out.buf;

}

static void gendoc(void) {

  item_t   a;
  mod_t    mod;
  Out_t *  o;
  uint32_t num = 0;

  SETFLOC(Floc);
    
  for (mod = Ctx.start; mod; mod = mod->next) {
    o = & mod->Out;
    
    mkdirs(mod, bad(mod, DocSource));
    
    for (eachitem(a, DocSource, mod)) {
      out(o, "HTML_TARGETS += %s/%s\n", bad(mod, DocSource), rep(a, ".html"));
      out(o, "PDF_TARGETS += %s/%s\n", bad(mod, DocSource), rep(a, ".pdf"));

      out(o, "%s/%s: ", bad(mod, DocSource), rep(a, ".html"));
      out(o, "%s/%s %s/Rules.mk Makefile\n", here(mod), a->name, mod->path);
      out(o, "\t$(ASCIIDOC) $(DOCFLAGS) $(DOCFLAGS-%s) --out-file=$@ $<\n\n", mod->ums);
    }
    if (o->off) {
      umbreval(o->buf, & Floc);
      num++;
    }
    resetout(o);
  }

  if (num) {
    o = & Ctx.start->Out;                                   // We'll use the global module's output buffer.
    resetout(o);
    out(o, "%%.pdf: %%.html\n");
    out(o, "\twkhtmltopdf -q --page-size A4 $< $@\n\n");
    umbreval(o->buf, & Floc);
    resetout(o);
  }

}


static void genexported(void) {                             // Generate the exported header file rules, from src to build/include.

  item_t   x;
  mod_t    mod;
  Out_t *  o;
  cchar_t  newline = "\n";

  SETFLOC(Floc);

  for (mod = Ctx.start; mod; mod = mod->next) {
    o = & mod->Out;

    mkdirs(mod, bad(mod, Export));

    for (eachitem(x, Export, mod)) {
      out(o, "%s/", bad(mod, Export));
      out(o, "%s: ", x->name);
      out(o, "%s/%s\n", here(mod), x->name);
      out(o, "\tcp --force --target-directory=%s $<\n\n", bad(mod, Export));
    }
    if (o->off) {
      umbreval(o->buf, & Floc);
      resetout(o);
      newline = "";
    }
  }

}

static void importpreqs4mod(mod_t mod) {                    // Recursively generate import prerequisites in the global out buffer.

  item_t  imported;
  item_t  exported;
  Out_t * o = & Ctx.start->Out;

  if (mod->Flags.preqsImported) return;                     // Don't go into an infinite loop.

  mod->Flags.preqsImported = 1;

  for (eachitem(imported, Import, mod)) {                   // Walk over all imported modules.
    assert(imported->mod);                                  // Must have been resolved already.
    for (eachitem(exported, Export, imported->mod)) {       // For each imported module, write out its exports.
      if (! imported->mod->Flags.preqsImported) {
        out(o, "%s/%s ", bad(mod, Export), exported->name);
      }
    }
    if (item(imported->mod, Import)) {                      // If the imported module also imports another module, call recursively.
      importpreqs4mod(imported->mod);
    }
  }

}

static void genimportpreqs(mod_t mod) {                     // Recursively generate import prerequisites in the global out buffer.

  Out_t * o = & Ctx.start->Out;                             // We use the global buffer to build this prerequsite list.

  resetout(o);

  importpreqs4mod(mod);

  stripspace(o);

}

static void genAllObj(mod_t mod, Type_t SrcType) {          // Generate a string of object files for the module, separated by spaces.

  item_t  src;
  Out_t * o = & mod->Out;

  assert(Source == SrcType || Sample == SrcType);           // Can only be Source or Sample code.

  for (eachitem(src, SrcType, mod)) {                       // List all the object files for either application, sample app or library.
    if (! src->drop) {
      out(o, "%s/%s ", bad(mod, Source), rep(src, ".o"));
    }
  }

  stripspace(o);
  
}

static void genCCcmd(mod_t mod, item_t src) {               // Generate the compile or assembly command for the given source.

  Out_t *      o = & mod->Out;
  cchar_t      bad = iname(Ctx.start, BuildFolder);         // Built Artifacts Directory in globals module.
  const char * tool = mod->isTool ? "TOOL" : "";

  if (CSource == src->sub) {
    out(o, "$(%sCC) $(CFLAGS) $(CFLAGS-%s) -I %s/include -I %s -c $< -o $@", tool, mod->ums, bad, mod->path);
  }
  else if (ASource == src->sub) {
    out(o, "$(%sAS) $(ASFLAGS) $(ASFLAGS-%s) -I %s/include -I %s $< -o $@ ", tool, mod->ums, bad, mod->path);
  }
  else if (CXXSource == src->sub) {
    out(o, "$(%sCXX) $(CXXFLAGS) $(CXXFLAGS-%s) -I %s/include -I %s -c $< -o $@", tool, mod->ums, bad, mod->path);
  }
  else {
    mkerror("Subtype %d not yet", src->sub);
  }

}

static void genlibpreqs(Out_t * o, mod_t mod) {             // Generate the library prerequisites on the given output.

  item_t imported;

  if (mod->Flags.libsImported) return;
  
  mod->Flags.libsImported = 1;

  for (eachitem(imported, Import, mod)) {                   // Walk over all imported modules.
    if (! imported->mod->Flags.libsImported) {
      out(o, " %s/", bad(mod, Import));
      out(o, "lib%s.a", imported->name);
    }
    if (item(imported->mod, Import)) {
      genlibpreqs(o, imported->mod);                        // Call recursively if imported module also imports modules.
    }
  }

}

static void genlib4LD(Out_t * o, mod_t mod) {               // Generate the libraries to link with for the linker.

  item_t imported;

  if (mod->Flags.modsImported) return ;
  
  mod->Flags.modsImported = 1;

  for (eachitem(imported, Import, mod)) {                   // Walk over all imported modules.
    if (! imported->mod->Flags.modsImported) {
      out(o, "-l%s ", imported->name);
    }
    if (item(imported->mod, Import)) {
      genlib4LD(o, imported->mod);                          // Call recursively if imported module also imports modules.
    }
  }

}

static void genLDcmd(mod_t mod, item_t lib) {               // Generate the link command.

  Out_t *      o = & mod->Out;
  cchar_t      bad = iname(Ctx.start, BuildFolder);         // Built Artifacts Directory in globals module.
  const char * tool = mod->isTool ? "TOOL" : "";

  out(o, "$(%sLD) ", tool);
  if (item(Ctx.start, LDFlag)) { out(o, "$(LDFLAGS) "); }   // Only if there is a not empty global LDFLAGS.
  out(o, "$(filter %%.o,$^) -L %s/lib ", bad);
  if (lib) { out(o, "-l%s ", lib->name); }                  // Add an extra passed library, if not NULL.
  if (item(mod, LDFlag)) {
    out(o, "$(LDFLAGS-%s) ", mod->ums);                     // Only if not empty.
  }
  genlib4LD(o, mod);
  stripspace(o);
  out(o, " -o $@\n");

}

typedef const struct Type2Flag_t {
  Type_t       Type;
  const char * flags;
} Type2Flag_t;

static Type2Flag_t T2F[] = {
  { CFlag,   "CFLAGS"   },
  { CXXFlag, "CXXFLAGS" },
  { ASFlag,  "ASFLAGS"  },
  { LDFlag,  "LDFLAGS"  },
  { DocFlag, "DOCFLAGS" },
  { None,    NULL       },
};

static void genflags(mod_t mod) {

  Out_t *       o = & mod->Out;
  item_t        flag;
  Type2Flag_t * t2f;

  for (t2f = & T2F[0]; t2f->flags; t2f++) {
    flag = item(mod, t2f->Type);
    if (flag) {                                             // If there's at least one flag.
      if (mod == Ctx.start) {                               // Global flags don't take an ums extension.
        out(o, "%s := ", t2f->flags);
      }
      else {
        out(o, "%s-%s := ", t2f->flags, mod->ums);
      }
      for ( ; flag && flag->type == t2f->Type; flag++) {
        out(o, "%s ", flag->name);
      }
      stripspace(o);
      out(o, "\n\n");
    }
  }

}

static void genCompileRules(mod_t mod, Type_t SrcType) {

  item_t  src;  
  Out_t * o = & mod->Out;
  item_t  export;

  assert(Source == SrcType || Sample == SrcType);           // Can only be Source or Sample code.

  for (eachitem(src, SrcType, mod)) {                       // Generate the compile rules.
    if (! src->drop) {
      out(o, "%s/%s: ", bad(mod, Source), rep(src, ".o"));  // We always pass 'Source' as type.
      out(o, "%s/%s", here(mod), src->name);
      if (Ctx.start->Out.off) {                             // Add the imported prerequisites, if any (from genimportpreqs).
        out(o, " %s", Ctx.start->Out.buf);
      }
      if (Sample == SrcType) {                              // For sample code, we must also add the exports of the library as prerequisites.
        for (eachitem(export, Export, mod)) {
          out(o, " %s/%s", bad(mod, Export), export->name);
        }
      }
      out(o, " %s/Rules.mk Makefile\n\t", mod->path);
      genCCcmd(mod, src);                                   // Generate the specific compile/assemble command, based upon the type.
      out(o, "\n\n");
    }
  }

}

static void filterout(mod_t mod) {                          // Filter out the sample code source from the normal source; if e.g. wildcard has been used.

  item_t src;
  item_t sample;

  for (eachitem(src, Source, mod)) {
    for (eachitem(sample, Sample, mod)) {
      if (0 == strcmp(src->name, sample->name)) {
        umblog(1, "# Dropping '%s', is sample source.\n", src->name);
        src->drop = 1;
      }
    }
  }

}

static void gen1mod(mod_t mod) {

  item_t  src;
  item_t  lib;
  item_t  app;
  item_t  tool;
  item_t  sample;
  Out_t * o = & mod->Out;

  SETFLOC(Floc);

  mod->Gen.folder = mod->Items[mod->Gen.index].name;        // We can now fix the gen folder.

  lib = item(mod, LibName);
  app = item(mod, AppName);
  tool = item(mod, ToolName);

  if ((lib && app) || (lib && tool) || (tool && app)) {
    mkerror("Either LIBNAME, APPNAME or TOOLNAME in %s/Rules.mk", mod->path);
  }

  if (tool) {                                               // OK if we're compiling a tool, as of now, we treat it like an application.
    app = tool;
    mod->isTool = 1;
  }

  sample = item(mod, Sample);

  if (sample) {
    filterout(mod);
  }

  if (sample && ! lib) {
    mkerror("Sample only when LIBNAME is present; %s/Rules.mk", mod->path);
  }

  if (! lib && ! app && ! tool) { return; }                 // Can happen for documentation only modules.

  out(o, "# Module: %s:%s\n\n", mod->path, mod->name);

  genflags(mod);                                            // Generate all the flags for this module.

  genimportpreqs(mod);                                      // Import prerequisites are generated in the global buffer.

  out(o, "DEPS += ");                                       // Generate the DEPS additions.

  for (eachitem(src, Source, mod)) {                        // Add library or application/tool sources to the DEPS.
    if (src->sub == CSource || src->sub == CXXSource) {
      if (! src->drop) {
        out(o, "%s/%s ", bad(mod, Source), rep(src, ".d"));
      }
    }
  }

  if (! mod->isRemote) {
    for (eachitem(src, Sample, mod)) {                      // Also add sample code source(s) to the DEPS.
      if (src->sub == CSource || src->sub == CXXSource) {
        if (! src->drop) {
          out(o, "%s/%s ", bad(mod, Source), rep(src, ".d"));
        }
      }
    }
  }

  stripspace(o); 
  
  out(o, "\n\n");

  mkdirs(mod, mod->Gen.folder);                             // Check gen folder and create if it doesn't exist yet.
  
  genCompileRules(mod, Source);                             // Generate the compile rules for the normal lib or app/tool sources.

  if (! mod->isRemote) {
    genCompileRules(mod, Sample);                           // Generate the compile rules for sample code, if any and if not a remote folder.
  }

  if (lib) {
    out(o, "LIB_TARGETS += %s/", bad(mod, LibName));
    out(o, "lib%s.a\n\n%s/", lib->name, bad(mod, LibName));
    out(o, "lib%s.a: ", lib->name);
    genAllObj(mod, Source);
    out(o, "\n\t$(AR) cr $@ $?\n\n");

    if (mod->isFPICmod) {                                   // See if we also need to create a shared library; in the same spot as the archive.
      out(o, "LIB_TARGETS += %s/", bad(mod, LibName));
      out(o, "lib%s.so\n\n%s/", lib->name, bad(mod, LibName));
      out(o, "lib%s.so: ", lib->name);
      genAllObj(mod, Source);
      out(o, "\n\t$(LD) -shared -o $@ $^\n\n");
    }

  }

  if (app) {
    out(o, "APP_TARGETS += %s/", bad(mod, AppName));
    out(o, "%s\n\n%s/", app->name, bad(mod, AppName));
    out(o, "%s: ", app->name);
    genAllObj(mod, Source);
    genlibpreqs(o, mod);
    out(o, "\n\t");
    genLDcmd(mod, NULL);
    out(o, "\n\n");
  }

  if (sample && ! mod->isRemote) {                          // We only compile sample code of non remote folders.
    out(o, "SAMPLE_TARGETS += %s/", bad(mod, AppName));     // A sample is an application, so use AppName.
    out(o, "%s\n\n", rep(sample, ""));
    out(o, "%s/%s: ", bad(mod, AppName), rep(sample, ""));
    genAllObj(mod, Sample);
    genlibpreqs(o, mod);
    out(o, " %s/lib%s.a", bad(mod, Import), mod->name);     // For samples, we also include the libname as prerequisite.
    out(o, "\n\t");
    genLDcmd(mod, lib);
    out(o, "\n\n");
  }

  umbreval(o->buf, & Floc);
  resetout(o);

}

static void resolve(void) {                                 // Resolve all imports.

  mod_t  mod;
  item_t imported;

  for (mod = Ctx.start; mod; mod = mod->next) {
    for (eachitem(imported, Import, mod)) {
      imported->mod = findmod(mod, imported->name);         // Will generate an error if not found.
    }
  }
  
}

static void genGenerated(void) {                            // Make the GENERATED flags for cleaning.

  item_t  gen;
  mod_t   mod;
  Out_t * o = & Ctx.start->Out;

  SETFLOC(Floc);

  resetout(o);

  out(o, "GENERATED := ");

  for (mod = Ctx.start; mod; mod = mod->next) {
    for (eachitem(gen, Generated, mod)) {
      out(o, "%s ", gen->name);
    }
  }

  out(o, "\n\n");

  umbreval(o->buf, & Floc);

  resetout(o);

}

static void resetFlags(void) {                              // Reset all transient flags.

  mod_t mod;
  
  for (mod = Ctx.start; mod; mod = mod->next) {
    memset(& mod->Flags, 0x00, sizeof(mod->Flags));
  }

}

char * genmod(const char *nm, uint32_t argc, char * argv[]) {

  mod_t   mod;
  Out_t * o = & Ctx.start->Out;;

  SETFLOC(Floc);

  genGenerated();

  if (! Ctx.cleaning) {

    genexported();

    resolve();

    resetout(o);
    genflags(Ctx.start);                                    // Regenerate the global flags (reset by our module reading).
    umbreval(o->buf, & Floc);
    resetout(o);
  
    for (mod = Ctx.start; mod; mod = mod->next) {
      if (mod == Ctx.start) continue;                       // Skip global module
      resetFlags();
      gen1mod(mod);
    }

    resetout(o);

    gendoc();
    
  }

  return NULL;

}
