#ifndef FB2_CODE_H
#define FB2_CODE_H

// Copyright 2023 Steven Buytaert

#include <t2c-types.h>
#include <fb2-schema.h>

typedef struct fb2_CodeCtx_t * fb2_codeCtx_t;

typedef enum {
  Alias4Mem   = 1,
  Alias4Type  = 2,
} fb2_Alias_t;

typedef void *       (* fb2c_mem_t)(fb2_codeCtx_t ctx, void * mem, uint32_t sz);
typedef const char * (* fb2c_alias_t)(fb2_codeCtx_t ctx, const char * name, fb2_Alias_t at);
typedef void         (* fb2c_prune_t)(fb2_codeCtx_t ctx);

typedef struct fb2_CodeCtx_t {
  uint32_t          cookie;       // Internal (leave blank).
  char              ID[4];        // File ID of the schema file.
  fb2_schemaCtx_t   schemaCtx;    // FB2 schema parsing context.
  const char *      prefix;       // Prefix used for all types.
  const char *      suffix;       // Suffix used for all types.
  const char *      a4union;      // Add this to union types, just before the suffix.
  const char *      pref4refs;    // Prefix for refs; can be left NULL for first lower case convention.
  fb2c_alias_t      alias;        // Provide a potential alias for a name; if NULL, stub will be used.
  fb2c_prune_t      prune;        // Called just before generating the types, so the type tree can be pruned/customized, if needed.
  fb2c_mem_t        mem;          // Memory allocate, release; realloc semantics.
  t2c_Ctx_t         t2cCtx;       // Type creation context; caution allocate enough tail for types.
} fb2_CodeCtx_t;

void fb2_generate(fb2_codeCtx_t ctx);

#endif // FB2_CODE_H
