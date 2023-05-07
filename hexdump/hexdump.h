#ifndef HEXDUMP_H
#define HEXDUMP_H

// Copyright 2020-2023 Steven Buytaert

#include <stdint.h>

// Pass a buffer and size to format into a hexdump of data pointed at by 'd2d' and n
// bytes long. Return the number of bytes that would have been written, if the buffer
// was large enough. Buffer can be NULL. The output will be aligned on a 16 byte boundary.
// If some buffer space was passed, it will always return a properly \0 ended string.

uint32_t hex2buf(char b[], uint32_t s, const void * d2d, uint32_t n);

// Convenience function and example; given buffer is 8k.
// The passed message is printed as title. The 8K buffer is allocated
// on the stack and dumped with printf, preceeded by the message msg.

void hexdump8k(const void * d2d, uint32_t n, const char * msg);

#endif // HEXDUMP_H
