// gcc -shared -I ../make-4.3/src/ -Wall -Wsign-compare -fpic -o ./tools/my-make.so ./tools/my-make.c

// gcc -shared -I ../../make-4.3/src/ -Wall -Wsign-compare -fpic -o my-make.so my-make.c

// Copyright 2020-2023 Steven Buytaert

#include <unistd.h>
#include <sys/stat.h>

#include <umbrella.h>

int plugin_is_GPL_compatible = 1; // MIT License is GP Compatible.

Ctx_t Ctx;                        // The global context.

static char * resetvars = NULL;   // To issue a reset of variables at each module read.
static uint32_t numpaths = 0;
static const char * umbpaths[256];// NULL terminated array of paths to search for modules (see UMBPATH environment variable).

static mod_t nop(mod_t mod, var_t var, char * val) {

  return mod;

}

static mod_t gen(mod_t mod, var_t var, char * val) {

  mod->Gen.index = mod->num - 1;                            // By definition, the last item added.

  return mod;

}

static mod_t name(mod_t mod, var_t var, char * val) {

  if (strlen(val)) {
    mod->name = strdup(val);
    umblog(1, "# -------- Reading module %s:%s\n", mod->path, mod->name);
  }

  return mod;
  
}

static mod_t base(mod_t mod, var_t var, char * val) {

  Ctx.base = strdup(val);

  return mod;
  
}

static mod_t path(mod_t mod, var_t var, char * val) {

  uint32_t i;

  strcpy(mod->path, val);

  strcpy(mod->ums, mod->path);

  mod->pathlen = strlen(mod->path);
  
  assert(mod->path[mod->pathlen - 1] != '/');               // Paths should never end with a slash.

  for (i = 0; i < sizeof(mod->ums) && mod->ums[i]; i++) {   // Create the unique module suffix from the path.
    if (mod->ums[i] == '/') mod->ums[i] = '_';
  }

  return mod;
  
}

static uint32_t isPICFlag(const char * fn) {

  if (0 == strcasecmp(fn, "-fpic")) return 1;
  if (0 == strcasecmp(fn, "-fpie")) return 1;

  return 0;

}

static mod_t fpic(mod_t mod, var_t var, char * val) {       // Called upon CFLAGS and CXXFLAGS to check for fpic option.

  item_t flag;

  for (eachitem(flag, CFlag, mod)) {
    if (isPICFlag(flag->name)) mod->isFPICmod = 1;
  }

  for (eachitem(flag, CXXFlag, mod)) {
    if (isPICFlag(flag->name)) mod->isFPICmod = 1;
  }

  return mod;
  
}

static Var_t Args4Mod[] = {       // In the order as passed to the module function.
  //                 L   X   R   I
  { "HERE",          4,  0,  0,  0, None,        path, },
  { "GEN",           3,  1,  1,  1, Path2Obj,    gen,  },
  { "LIBNAME",       7,  1,  1,  1, LibName,     name, },
  { "EXPORTS",       7,  1,  1,  1, Export,      nop,  },
  { "APPNAME",       7,  1,  1,  1, AppName,     name, },
  { "IMPORTS",       7,  1,  1,  1, Import,      nop,  },
  { "SAMPLE",        6,  1,  1,  1, Sample,      nop,  },
  { "SOURCES",       7,  1,  1,  1, Source,      nop,  },
  { "SRC",           3,  1,  1,  1, Source,      nop,  },   // Also regulary used to gather source files.
  { "CFLAGS",        6,  1,  1,  1, CFlag,       fpic, },
  { "CXXFLAGS",      8,  1,  1,  1, CXXFlag,     fpic, },
  { "LDFLAGS",       7,  1,  1,  1, LDFlag,      fpic, },
  { "DOCTEXT",       7,  1,  1,  1, DocSource,   nop,  },
  { "DOCFLAGS",      8,  1,  1,  1, DocFlag,     nop,  },
  { "ASFLAGS",       7,  1,  1,  1, ASFlag,      nop,  }, 
  { "GENERATED",     9,  1,  1,  1, Generated,   nop,  }, 
  { "TOOLNAME",      8,  1,  1,  1, ToolName,    name  },   // The name of a tool built with the tool compiler.
  { NULL }
};

static Var_t Args4Globals[] = {
  { "MAKECMDGOALS", 12,  1,  0,  1, Goal,        nop,  },
  { "DIRS",          4,  1,  0,  1, ModFolder,   nop,  },
  { "BAD",           3,  1,  0,  1, BuildFolder, nop,  },
  { "BASE",          4,  0,  0,  0, None,        base, },
  { "CFLAGS",        6,  1,  0,  1, CFlag,       fpic, },
  { "CXXLAGS",       7,  1,  0,  1, CXXFlag,     fpic, },
  { "LDFLAGS",       7,  1,  0,  1, LDFlag,      fpic, },
  { "LDLIBS",        6,  1,  0,  1, LDLib,       nop,  },
  { "DOCFLAGS",      8,  1,  0,  1, DocFlag,     nop,  },
  { "CXX",           3,  1,  0,  1, CXXCompiler, nop,  },
  { "CC",            2,  1,  0,  1, CCompiler,   nop,  },
  { "LD",            2,  1,  0,  1, CLinker,     nop,  },
  { "CXXLD",         5,  1,  0,  1, CXXLinker,   nop,  },
  { "TOOLCC",        6,  1,  0,  1, ToolCC,      nop,  },
  { "TOOLLD",        6,  1,  0,  1, ToolLD,      nop,  },
  { "TOOLCXX",       7,  1,  0,  1, TOOLCXX,     nop,  },
  { "UMBFLAGS",      8,  1,  0,  1, UMBFlag,     nop,  },
  { NULL },
};

static struct {
  const char * ext;
  Type_t       type;
} src2type[] = {
  { "c",   CSource       },
  { "cc",  CXXSource     },
  { "cpp", CXXSource     },
  { "c++", CXXSource     },
  { "s",   ASource    },
  { "S",   ASource2pp },
};

static uint8_t ext2type(const char * file, size_t sz) {

  const char * ext = & file[sz - 1];
  size_t       rem = sz;
  Type_t       type = None;
  uint32_t     i;
  
  while (rem && ext[0] != '.') { ext--; rem--; }            // Find the dot or not.

  for (i = 0; rem && i < NUM(src2type) && ! type; i++) {    // Only if there is an extension, search for the type.
    if (0 == strcmp(src2type[i].ext, ext + 1)) {
      type = src2type[i].type;
    }
  }

  if (None == type) { 
    mkerror("File '%s' not recognized as source", file);    // Note that $(error ...) adds a trailing dot.
  }

  return (uint8_t) type;

}

static void setindex(Type_t type, mod_t mod) {              // Set from and to of the index.

  int16_t index = (int16_t) (mod->num - 1);                 // mod->num already incremented.

  if (mod->Index[type].from == -1) {
    mod->Index[type].from = index;
  }

  mod->Index[type].to = index;

} 

static uint16_t isRemote(mod_t mod) {

  item_t folder;

  for (eachitem(folder, ModFolder, Ctx.start)) {            // Go over all ModFolders in the globals module.
    if (folder->remote) {                                   // If they have a remote part ...
      if (0 == strcmp(folder->folder, mod->path)) {         // ... see if there's a match.
        return 1;
      }
    }
  }

  return 0;

}

static const mode_t fmode = S_IRWXU |                       // User   rwx
                            S_IRWXG |                       // Group  rwx
                            S_IROTH | S_IXOTH;              // Others r-x
                            
static uint32_t check4intermediate(const char * path) {     // Check if *intermediate* folders need to be created.

  char         buf[4096];
  uint32_t     nic = 0;                                     // Number of intermediate components.
  uint32_t     i;
  size_t       length = strlen(path);
  const char * cur = path;
  struct stat  Stat;

  for (i = 0; i < length; i++, cur++) {                     // Find each component ended by a '/'.
    buf[i] = *cur;
    if (buf[i] == '/') {
      nic++;
      buf[i] = 0;                                           // Properly end for stat and mkdir call.
      if (stat(buf, & Stat)) {
        if (errno == ENOENT) {
          if (mkdir(buf, fmode)) {
            mkerror("Could not create '%s' %s\n", buf, strerror(errno));
          }
        }
      }
      buf[i] = '/';                                         // Insert again for next component, if any.
    }
  }

  return nic;
  
}

static const char ** mkpaths(const char * folder) {         // Create a set of search paths for the folder, based upon UMBPATH.

  const char ** roots;
  uint32_t      i;
  uint32_t      size = sizeof(char *);                      // Space for the final NULL slot.
  char *        dst;

  for (i = 0; i < numpaths; i++) {                          // We will create numpaths roots.
    size += strlen(folder) + strlen(umbpaths[i]) + 2;       // Add 1 for the '/' and 1 for the trailing \0.
    size += sizeof(char *);                                 // Add space for the slot itself.
  }

  roots = malloc(size);
  assert(roots);
  memset(roots, 0x00, size);
  dst = (char *) & roots[numpaths + 1];                     // Don't forget the NULL slot.
  
  for (i = 0; i < numpaths; i++) {
    roots[i] = dst;
    dst += sprintf(dst, "%s/%s", umbpaths[i], folder);
    dst += 1;                                               // 0\ terminate each path.
    umblog(3, ".../%s search path[%u] = '%s'\n", folder, i, roots[i]);
  }

  return roots;

}

static uint32_t folder(mod_t mod, item_t spec) {   

  char          rem[4096];                                  // Remote resolved path.
  char          buf[4096];
  const char *  dc = strstr(spec->name, "::");              // See if the folder specification contains a remote part.
  struct stat   Stat;
  uint32_t      nic;
  uint32_t      i;
  int32_t       off = 0;
  F2Find_t      F2Find;
  const char ** roots;
  uint32_t      absolute = 0;

  spec->folder = spec->name;                                // Default case when no remote part.

  if (dc) {
    if ((size_t) ((dc + 2) - spec->name) == spec->len) {    // Nothing specified behind the ::
      mkerror("Malformed remote module '%s'", spec->name);
    }
    if (dc == spec->name) {
      mkerror("Malformed remote module '%s'", spec->name);  // Nothing specified in front of the ::
    }
    spec->folder = dc + 2;
    spec->name[dc - spec->name] = 0;
    while ('/' == spec->name[strlen(spec->name) - 1]) {     // Strip away trailing slashes, if any.
      spec->name[strlen(spec->name) - 1] = 0;
    }
    spec->remote = spec->name;
  }

  if (dc && ! Ctx.cleaning) {                               // Remote spec; create link if not yet done and not cleaning.
    if (0 == strncmp(spec->remote, ".../", 4)) {
      if (numpaths) {                                       // Only search if there are paths to search in.
        F2Find.name = "Rules.mk";
        roots = mkpaths(spec->remote + 4);                  // Strip off the .../ before making the roots.
        f2find(& F2Find, roots);
        free(roots);
        if (F2Find.found) {
          size_t size = strlen(F2Find.buffer);
          F2Find.buffer[size - 9] = 0;                      // Strip of the Rules.mk from the buffer.
          spec->remote = F2Find.buffer;                     // Remote is now this one.
          absolute = ('/' == F2Find.buffer[0]) ? 1 : 0;
        }
      }
      else {
        mkerror("Using %s but no UMBPATH set.", spec->remote);
      }
    }

    if (lstat(spec->folder, & Stat)) {                      // Use lstat not stat.
      if (ENOENT == errno) {                                // Symbolic link doesn't exist yet; create it.
        if (! realpath(spec->remote, rem)) {                // Check if the remote path is OK.
          mkerror("No such remote '%s'", spec->remote);
        }
        if (stat(rem, & Stat)) {                            // Check if it is a folder we can link to.
          mkerror("Stat '%s'; %s", rem, strerror(errno));
        }
        if (! S_ISDIR(Stat.st_mode)) {                      // The remote must point to a folder that can contain a Rules.mk.
          mkerror("Remote '%s' not a dir", spec->remote);
        }
        sprintf(buf, "%s/Rules.mk", spec->remote);          // Check there is indeed a Rules.mk file present.
        if (0 == lstat(buf, & Stat)) {                      // Now stat should NOT return an error.
          nic = check4intermediate(spec->folder);
          for (i = 0; i < nic; i++) {                       // Add intermediate references if any.
            if ('/' == spec->remote[0]) break;              // No need to add ../ when the path is absolute.
            off += sprintf(rem + off, "../");
          }
          sprintf(rem + off, "%s", spec->remote);
          if (symlink(rem, spec->folder)) {
            mkerror("Symlink failed '%s'; %s", spec->folder, strerror(errno));
          }
        }
        else {
          mkerror("%s: %s", buf, strerror(errno));
        }
      }
    }
  }

  return (Ctx.cleaning && dc) ? 0 : 1;                      // Return 0 when spec is a remote and we're cleaning; leave out of DIRS since it's gone.

}

static void stripfolder(mod_t mod, item_t item) {           // Strip the module folder part from a source name, if any.

  size_t move;

  if (item->len > mod->pathlen) {
    if (! strncmp(mod->path, item->name, mod->pathlen)) {
      if (item->name[mod->pathlen] == '/') {
        move = item->len - mod->pathlen;
        memmove(& item->name[0], & item->name[mod->pathlen + 1], move);
        item->len = move - 1;
      }
    }
  }

}

static mod_t itemize(mod_t mod, Var_t const * var, char * lst, Type_t type) {

  size_t s = strlen(lst);
  char * cur;
  char * s4s;
  item_t item;
  
  while (s && lst[s - 1] == ' ') { lst[s - 1] = 0; s--; }   // Strip strailing spaces.

  while (s && lst[0] == ' ') { lst++; s--; }                // Strip leading spaces.

  for (s4s = lst, cur = lst; s && cur; cur = s4s) {
    while (s && *s4s != ' ') { s--; s4s++; }                // Search for space or end of list.
    *s4s++ = 0; if (s) { s--; }                             // Marks the end of the item. When s == 0, final item.
    mod = ensureCap(mod, 1);
    item = & mod->Items[mod->num++]; mod->avail--;          // Get the item.
    item->type = (uint8_t) type;
    setindex(item->type, mod);

    if (s4s - cur > (int32_t) sizeof(item->name)) {         // If the length including \0 is too big to fit.
      mkerror("Item '%s' too big", cur);
    }
    
    strcpy(item->name, cur);
    item->len = (size_t)(s4s - cur - 1);

    if (Source == type || Sample == type) {                 // Source items and sample code items have a subtype.
      item->sub = ext2type(cur, item->len);                 // Deduce type from extension.
      setindex(item->sub, mod);
      stripfolder(mod, item);                               // Strip folder spec from a source, if any.
    }

    while (s && *s4s == ' ') { s--; s4s++; }
  }

  return mod;                                               // Could have been realloced.

}

static mod_t parse(char * argv[], Var_t Var[], uint32_t num) {

  uint32_t i;
  char *   expanded = NULL;
  var_t    var;
  mod_t    mod = malloc(sizeof(Mod_t));


  memset(mod, 0x00, sizeof(Mod_t));

  for (i = 0; i < NUMTYPES; i++) {
    mod->Index[i].from = -1;
    mod->Index[i].to   = -1;
  }

  for (var = & Var[0], i = 0; var->name; i++, var++) {
    if (var->expand) {
      assert(! strncmp(var->name, & argv[i][2], var->len));
      assert(argv[i][var->len + 2] == ')');
    }

    if (var->expand) {
      expanded = gmk_expand(argv[i]);
    }

    if (var->itemize) {
      assert(var->expand);
      mod = itemize(mod, var, expanded, var->Type);
    }

    mod = var->hndlr(mod, var, expanded ? expanded : argv[i]);

    if (expanded) {
      gmk_free(expanded); expanded = NULL;
    }

  }

  mod = ensureCap(mod, 1); mod->num++; mod->avail--;        // We have a final empty item; type == None

  return mod;

}

static char * module(const char *nm, uint32_t argc, char * argv[]) {

  mod_t mod = parse(argv, Args4Mod, NUM(Args4Mod));         // Read and parse the module variables into items/

  const gmk_floc Floc = {
    .filenm = __FUNCTION__,
    .lineno = __LINE__
  };

  umbreval(resetvars, & Floc);                              // Reset all variables from the module.

  assert(Ctx.start);                                        // Set by global parse.

  Ctx.last->next = mod;                                     // Link module in the list.
  Ctx.last = mod;
  Ctx.num++;

  mod->isRemote = isRemote(mod);                            // Check if it is a remote module.

  return NULL;

}

static char * setvars(const char *nm, uint32_t argc, char * argv[]) {

  Out_t * o = & Ctx.start->Out;                             // Use the globals output buffer.

  const gmk_floc Floc = {
    .filenm = __FUNCTION__,
    .lineno = __LINE__
  };

  resetout(o);

  out(o, "BAD  := %s\n", Ctx.bad);  
  out(o, "GEN  := %s/gen/%s\n", Ctx.bad, argv[0]);
  out(o, "HERE := %s\n", argv[0]);
  out(o, "BASE := %s\n\n", Ctx.base);

  umbreval(o->buf, & Floc);

  resetout(o);

  return NULL;

}

static void umbclean(void) {                                // Remove the symbolic links, if any.

  item_t       fspec;
  struct stat  Stat;

  for (eachitem(fspec, ModFolder, Ctx.start)) {
    if (fspec->remote) {                                    // We remove the symbolic links when we clean.
      if (0 == lstat(fspec->folder, & Stat)) {              // Careful, use lstat here!
        if (S_ISLNK(Stat.st_mode)) {
          unlink(fspec->folder);
        }
      }
    }
  }

}

static char * globals(const char *nm, uint32_t argc, char * argv[]) {

  item_t   arg;
  mod_t    mod = parse(argv, Args4Globals, NUM(Args4Globals));
  Out_t *  o = & mod->Out;

  if (Ctx.start) {
    mkerror("$(globals ...) called more than once");
  }
  else {
    Ctx.start = mod;
    Ctx.last = mod;
    Ctx.num = 1;
  }

  assert(mod->name == NULL); mod->name = "(globals)";       // Give the module a name.

  Ctx.verbosity = 0;
  Ctx.tcdump = NULL;

  for (eachitem(arg, UMBFlag, mod)) {
    if (0 == strcmp(arg->name, "debug")) {
      Ctx.verbosity = 10;
    }
    else if (0 == strncmp(arg->name, "tcdump=", 7)) {
      Ctx.tcdump = arg->name + 7;
      umblog(1, "Will dump testcase data to %s.\n", Ctx.tcdump);
    }
    else if (0 == strncmp(arg->name, "verbose=", 8)) {
      if (! Ctx.verbosity) {
        Ctx.verbosity = (0xff & atoi(arg->name + 8));
        umblog(1, "Verbosity now %d\n", Ctx.verbosity);
      }
    }
  }

  Ctx.bad = iname(mod, BuildFolder);                        // Get the build artefact folder.
  
  Ctx.bad = Ctx.bad[0] ? Ctx.bad : "./";                    // When not set, build in current folder.

  const gmk_floc Floc = {
    .filenm = __FUNCTION__,
    .lineno = __LINE__
  };

  for (eachitem(arg, Goal, mod)) {                          // See if one of the goals is to clean the place.
    if (0 == strcmp(arg->name, "clean")) {
      Ctx.cleaning = 1; break;                              // Do NOT call umbclean yet; we need to call folder() on each first.
    }
  }

  resetout(o);
  out(o, "DIRS := ");
  for (eachitem(arg, ModFolder, mod)) {                     // Make a cleaned (no remote parts) version of the DIRS ...
    if (folder(mod, arg)) {                                 // Create remote symbolic links only when not cleaning.
      out(o, "%s ", arg->folder);
    }
  }
  out(o, "\n");
  stripspace(o);
  umbreval(o->buf, & Floc);                                 // ... and make it overrride the current DIRS.
  resetout(o);

  if (Ctx.cleaning) {                                       // Now we can call clean, if needed.
    umbclean();
  }

  return NULL;

}

static char * ums(const char *nm, uint32_t argc, char * argv[]) {

  char *   addend;
  char *   expanded = NULL;
  char     buf[1024];
  size_t   sz;
  int32_t  i;
  int32_t  suffix = 0;;

  if ('$' == argv[0][0]) {
    expanded = gmk_expand(argv[0]);
    strncpy(buf, expanded, sizeof(buf) - 1);
    gmk_free(expanded);
  }
  else {
    strncpy(buf, argv[0], sizeof(buf) - 1);
  }

  sz = strlen(buf);

  umblog(3, "%s:%3d [%s] = [%s] %zu\n", __FILE__, __LINE__, argv[0], buf, sz);

  while ('/' == buf[sz - 1] || ' ' == buf[sz - 1]) {        // Strip any trailing slashes or spaces.
    buf[--sz] = 0;
  }

  umblog(3, "%s:%3d [%s] = [%s] %zu\n", __FILE__, __LINE__, argv[0], buf, sz);

  for (i = (int32_t) sz - 1; i >= 0 && ! suffix; i--) {
    if ('.' == buf[i]) {
      suffix = i;
      buf[i] = 0;
      sz = (size_t) i;
    }
  }

  umblog(3, "%s:%3d [%s] = [%s] %zu\n", __FILE__, __LINE__, argv[0], buf, sz);

  for (i = (int32_t) sz - 1; suffix && i >= 0; i--) {       // If we had a suffix, find the final slash.
    if ('/' == buf[i]) {
      buf[i] = 0;
      sz = (size_t) i;
      break;
    }
  }

  umblog(3, "%s:%3d [%s] = [%s] %zu\n", __FILE__, __LINE__, argv[0], buf, sz);

  for (i = 0; i < (int32_t) sz; i++) {                      // Replace spaces and slashes with dashes.
    if (' ' == buf[i]) { buf[i] = '-'; }
    if ('/' == buf[i]) { buf[i] = '-'; }
  }

  umblog(3, "%s:%3d [%s] = [%s] %zu\n", __FILE__, __LINE__, argv[0], buf, sz);

  addend = gmk_alloc((uint32_t) sz + 2);                    // For the - and the trailing \0.
  
  addend[0] = '-';
  
  strcpy(& addend[1], buf);

  umblog(3, "\n");

  return addend;

}

int umbrella_gmk_setup(void) {                              // Module name is 'umbrella.so' so umbrella_gmk_setup() is called at load time.

  uint32_t      bufsz = 1024;
  int32_t       off = 0;
  char *        cur = malloc(bufsz);
  char *        end;
  uint32_t      i = 0;
  char *        colon;
  Var_t const * var;

  assert(cur);
  
  resetvars = cur;
  
  for (var = & Args4Mod[0]; var->name; var++) {             // Create the variable reset buffer.
    if (var->reset) {
      off += sprintf(cur + off, "%-9s :=\n", var->name);
      assert((int) bufsz - off > 0);                        // Or we need to increase the buffer.
    }
  }

  cur = getenv("UMBPATH");                                  // See if there is a module search path set.
  if (cur) {
    end = cur + strlen(cur);
    while ((colon = index(cur, ':'))) {
      assert(i + 1 < NUM(umbpaths));
      umbpaths[i++] = strndup(cur, (uint32_t)(colon - cur));
      cur = colon + 1;
    }

    if (cur < end) {                                        // Add the final path, if any.
      assert(i + 1 < NUM(umbpaths));
      umbpaths[i++] = strndup(cur, (uint32_t)(end - cur));
    }
  }

  numpaths = i;

  gmk_add_function("module", module, NUM(Args4Mod) - 1, NUM(Args4Mod) - 1, GMK_FUNC_NOEXPAND);

  gmk_add_function("globals", globals, NUM(Args4Globals) - 1, NUM(Args4Globals) - 1, GMK_FUNC_NOEXPAND);

  gmk_add_function("genmod", genmod, 0, 0, GMK_FUNC_NOEXPAND);

  gmk_add_function("setvars", setvars, 1, 1, GMK_FUNC_NOEXPAND);

  gmk_add_function("ums", ums, 1, 1, GMK_FUNC_NOEXPAND);

//  char * list = strdup("  foo.c     bar.cc  ass.s ASS.S dada.c  blabla.cc noextensionfilenamethatisrathrelong ");

//  itemize(NULL, & Args4Mod[6], list, Source);

//  printf("$(globals ");
//  for (var = & Args4Globals[0], i = 0; i < NUM(Args4Globals); i++, var++) { // Create the variable reset buffer.
//    printf("$(%s)%s", var->name, (i + 1 == NUM(Args4Globals)) ? "" : ",");
//  }
//  printf(")\n");

  return 1;

}
