#ifndef UMBRELLA
#define UMBRELLA

// Copyright 2020 Steven Buytaert

#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <gnumake.h>

typedef const char *          cchar_t;
typedef const struct Var_t *  var_t;
typedef struct Mod_t *        mod_t;
typedef struct Item_t *       item_t;
typedef struct Out_t *        out_t;

typedef struct Out_t {
  size_t        rem;
  size_t        off;
  uint32_t      fixed;            // When not 0, not reallocatable.
  char *        buf;
} Out_t;

typedef enum {
  None          =  0,
  Import        =  1,             // An imported module.
  Export        =  2,             // An exported header file.
  LibName       =  3,
  AppName       =  4,
  Source        =  5,             // Generic source file tag.
  CSource       =  6,
  CXXSource     =  7,
  ASource       =  8,             // Raw assembly.
  ASource2pp    =  9,             // Assembly going via preprocessor first.
  Header        = 10,
  DocSource     = 11,
  CFlag         = 12,             // One single element of the CFlags.
  CXXFlag       = 13,
  ASFlag        = 14,
  LDFlag        = 15,
  DocFlag       = 16,
  ModFolder     = 17,
  BuildFolder   = 18,
  LDLib         = 19,
  CXXCompiler   = 20,
  CCompiler     = 21,
  CLinker       = 22,
  CXXLinker     = 23,
  ToolCC        = 24,
  ToolLD        = 25,
  TOOLCXX       = 26,
  UMBFlag       = 27,
  Generated     = 28,
  Path2Obj      = 29,
  Goal          = 30,
  Sample        = 31,
  NUMTYPES      = 32,
} Type_t;

typedef mod_t (* ahndlr_t)(mod_t mod, var_t var, char * val);

typedef const struct Var_t {
  const char *  name;
  const uint8_t len;
  const uint8_t expand;
  const uint8_t reset;            // Reset variable at the end of module call or not.
  const uint8_t itemize;
  Type_t        Type;
  ahndlr_t      hndlr;
} Var_t;

typedef struct Item_t {           // An item in a module; a file or a dependency.
  uint8_t       type;
  uint8_t       sub;              // Subtype; e.g. type == Source and sub == CSource.
  uint8_t       drop;             // When not zero, drop this as source (sample source handling).
  const char *  folder;           // When Type == ModFolder, the name of the folder as seen here.
  const char *  remote;           // The remote folder name, if applicable.
  mod_t         mod;              // When resolved, the link to module that satisfies the dependency.
  size_t        len;              // strlen of the name, not including trailing \0.
  char          name[64];
} Item_t;

typedef struct Index_t {
  int16_t       from;
  int16_t       to;
} Index_t;

typedef struct Flags_t {
  uint32_t      preqsImported;    // When not 0, prerequisites for this module are already imported or being imported.
  uint32_t      libsImported;     // When not 0, the required imported libraries already have been imported.
  uint32_t      modsImported;
} Flags_t;

typedef struct Mod_t {            // Single Module context.
  mod_t         next;             // All modules are kept on a linked list.
  Out_t         Out;              // Output buffer for a module.
  const char *  name;             // Either the APPNAME or LIBNAME of this module.
  struct {
    size_t      index;            // The item index that points to the GEN folder.
    char *      folder;           // Only when the module size has been fixed.
  } Gen;
  char          path[256];
  size_t        pathlen;          // Length of the path.
  char          ums[256];         // Unique module suffix.
  uint32_t      isRemote;         // Will be non zero, when this is a remote folder/symbolic link.
  uint32_t      isFPICmod;        // The module CFLAGS or CXXFLAGS contains fpic.
  Flags_t       Flags;            // These are transient flags; reset after each module generated.
  Index_t       Index[NUMTYPES];
  size_t        avail;            // How many item slots are available (free).
  size_t        num;
  Item_t        Items[0];
} Mod_t;

typedef struct Ctx_t {
  mod_t         start;            // The first module is the global one; it always exists.
  mod_t         last;
  uint32_t      num;
  uint8_t       verbosity;
  uint8_t       cleaning;         // Will be non zero when cleaning.
  const char *  tcdump;
  const char *  bad;
  const char *  base;
} Ctx_t;

extern Ctx_t Ctx;

mod_t   ensureCap(mod_t mod, uint32_t req);                 // Ensure there's req free item slots available.
void    mkerror(const char * fmt, ...);                     // Report an error and stops make.
char *  genmod(const char *nm, uint32_t n, char * argv[]);
item_t  item(mod_t mod, Type_t type);                       // Get the first item or NULL if none.
cchar_t iname(mod_t mod, Type_t type);                      // Get the name of the item or "" when not set, *NEVER* null.
void    out(Out_t * out, const char * fmt, ...);            // Safely write to an internal buffer.
void    resetout(Out_t * out);                              // Reset output buffer.
void    stripspace(Out_t * out);                            // Strip the final space, if any.
void    umbreval(const char *buffer, const gmk_floc *floc); // Umbrella eval function (with verbosity control).
void    umblog(uint32_t level, const char * fmt, ...);      // Log something; level 0 is always printed, any other if verbosity set.
void    mkdirs(mod_t mod, cchar_t folder);                  // Make the given folder path if not existing, like mdkir -p.

#define NUM(X) (sizeof(X) / sizeof(X[0]))

#define eachitem(I, T, M) (I) = item(M, T); I && (I)->type == T; (I)++

#endif // UMBRELLA
