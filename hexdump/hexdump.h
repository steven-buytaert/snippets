#ifndef HEXDUMP_H
#define HEXDUMP_H

// Copyright 2021 Steven Buytaert

#include <stdint.h>

// Pass a buffer and size to format into a hexdump of data pointed at by 'd2d' and n
// bytes long; the address shown, starts at 'off'. Return the number of bytes that
// would have been written, if the buffer was large enough. Buffer can be NULL. The
// output will be aligned on a 16 byte boundary.

uint32_t hex2buf(char b[], uint32_t s, const void * d2d, uint32_t n);

// Convenience function and example; given buffer is 8k.
// The passed message is printed as title.

void hexdump8k(const void * d2d, uint32_t n, const char * msg);

#endif // HEXDUMP_H
