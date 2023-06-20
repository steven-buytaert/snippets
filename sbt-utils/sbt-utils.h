#ifndef SBT_UTILS_H
#define SBT_UTILS_H

// Copyright 2023 Steven Buytaert

#include <stdint.h>

// strlcpy and strlcat implementations.

size_t strlcpy(char * restrict dst, const char * restrict src, size_t size);
size_t strlcat(char * restrict dst, const char * restrict src, size_t size);

// strlcpy and strlcat implementations with an explicit overflow passed. The passed
// overflow reference can not be NULL and 1 will be added to it when overflow happens.

size_t strcpy4o(char * restrict dst, const char * restrict src, size_t size, uint32_t * overflow);
size_t strcat4o(char * restrict dst, const char * restrict src, size_t size, uint32_t * overflow);

// A clone of realpath but with an explicit buffer size passed.

char * resolvepath(const char * restrict path, char * restrict respath, size_t rpsize);

#endif // SBT_UTILS_H
