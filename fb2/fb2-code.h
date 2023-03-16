#ifndef FB2_CODE_H
#define FB2_CODE_H

// Copyright 2023 Steven Buytaert

#include <t2c-types.h>
#include <fb2-schema.h>

typedef struct fb2_CodeCtx_t * fb2_codeCtx_t;

typedef void * (* fb2c_mem_t)(fb2_codeCtx_t ctx, void * mem, uint32_t sz);

typedef struct fb2_CodeCtx_t {
  fb2_schemaCtx_t   fb2schemaCtx; // FB2 schema parsing context.
  t2c_type_t        unresolved;   // Type to resolve later.
  const char *      prefix;       // Prefix used for all types.
  const char *      suffix;       // Suffix used for all types.
  const char *      a4union;      // Add this to union types, just before the suffix.
  const char *      pref4refs;    // Prefix for refs; can be left NULL for first lower case convention.
  fb2c_mem_t        mem;          // Memory allocate, release; realloc semantics.
  t2c_Ctx_t         t2cCtx;       // Type creation context; caution allocate enough tail for types.
} fb2_CodeCtx_t;

#endif // FB2_CODE_H
