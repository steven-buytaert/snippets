#ifndef CIRCBUFFER_H
#define CIRCBUFFER_H

// Copyright 2020 Steven Buytaert

#include <stdint.h>
#include <assert.h>

typedef struct Bcb_t   * bcb_t;
typedef struct Slice_t * slice_t;

typedef struct Bcb_t {            // Circular byte buffer
  uint32_t           size;
  uint8_t          * wrap;        // Used in bcb_put and bcb_get to check if pointer should wrap around; points to a usable slot
  uint8_t          * end;         // Used for read and write remaining space calculation; points beyond the last usable slot, in other words == wrap + 1
  uint8_t * volatile get;         // A volatile pointer *NOT* a pointer to a volatile
  uint8_t * volatile put;         // A volatile pointer *NOT* a pointer to a volatile
  uint8_t            buffer[4];   // The buffer space; we 'preallocate' some cells already to have at least 1 slot for the empty slot and we're properly aligned
} Bcb_t;

typedef struct Slice_t {          // A slice of a circular byte buffer; used for reading and writing a block at a time
  uint8_t     * data;             // The data pointer to be used after slicing
  uint32_t      num;              // Number of bytes available after slice, number of bytes used when producing or consuming
  uint32_t      max;              // Used internally only when assertions are enabled
  union {
    uint8_t * volatile get;
    uint8_t * volatile put;
  } snap;                         // Used for internal checking when assertions are enabled; snapshot of either get or put
} Slice_t;

void     bcb_readslice(const Bcb_t * cb, slice_t slice);
void     bcb_consumed(bcb_t cb, const Slice_t * slice); 
uint8_t  bcb_get(bcb_t cb);
void     bcb_put(bcb_t cb, uint8_t byte);
int32_t  bcb_isfull(const Bcb_t * cb);   
int32_t  bcb_isempty(const Bcb_t * cb);  
void     bcb_writeslice(const Bcb_t * cb, slice_t slice);
void     bcb_produced(bcb_t cb, const Slice_t * slice);  
uint32_t bcb_canread(const Bcb_t * cb);
uint32_t bcb_canwrite(const Bcb_t * cb);
void     bcb_init(bcb_t cb, uint32_t size);

#endif // CIRCBUFFER_H
