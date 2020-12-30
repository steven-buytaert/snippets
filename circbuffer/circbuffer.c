// Copyright 2020 Steven Buytaert

#include <circbuffer.h>

void bcb_readslice(const Bcb_t * cb, slice_t slice) {

  uint8_t * put = cb->put;                                  // Take snapshot of put pointer first, it can be manipulated under our feet by another thread
  uint8_t * get = cb->get;

  if (get <= put) {                                         // Also handles the case where get == put == empty buffer
    slice->num = (uint32_t) (put - get);
  }
  else {
    slice->num = (uint32_t) (cb->end - get);                // Take what remains between current get and end of the buffer
  }
   
  slice->data = get;                                        // Data to read always starts at get

  assert((slice->snap.put = put, 1));                       // (Y) Save put when assertions enabled to check in bcb_consumed
  assert((slice->max = slice->num, 1));                     // (X) Save num when assertions are enabled to check in bcb_consumed
  assert(slice->num <= cb->size);

}
 
void bcb_consumed(bcb_t cb, const Slice_t * slice) {

  uint8_t * get = cb->get;

  assert(slice->max >= slice->num);                         // (X) Check that we've never commit more then what was available in bcb_readslice
  assert(get + slice->num <= cb->end);                      // Should never go beyond end

  if (get < slice->snap.put) {                              // (Y) If get is less then the saved put pointer (note this 'if' is optimized away) ...
    assert(get + slice->num <= slice->snap.put);            // ... it should never go beyond the saved put; when they become equal, the buffer is empty
  }
   
  if ((get + slice->num) == cb->end) {                      // Note that num is guaranteed smaller or equal to num set in readslice
    get = cb->buffer;                                       // Never wrap beyond put !!
  }
  else {
    get += slice->num;                                      // Note that num is guaranteed smaller or equal to num set in readslice
  }
   
  cb->get = get;                                            // Assign atomically

}

uint8_t bcb_get(bcb_t cb) {

  uint8_t * get = cb->get;
  uint8_t   byte = get[0];

  assert(! bcb_isempty(cb));                                // We shouldn't be empty

  if (get == cb->wrap) {
    get = cb->buffer;   
  }
  else {
    get += 1;
  }
   
  cb->get = get;                                            // Assign atomically

  return byte;

}
 
void bcb_put(bcb_t cb, uint8_t byte) {

  uint8_t * put = cb->put;                                  // We take a snapshot of the put pointer

  put[0] = byte;

  assert(! bcb_isfull(cb));                                 // We shouldn't be full

  if (put == cb->wrap) {
    put = cb->buffer;                                       // Wrap around to start
  }
  else {
    put += 1;                                               // Just advance next put position
  }
   
  cb->put = put;                                            // Assign new put atomically

}

int32_t bcb_isfull(const Bcb_t * cb) {                      // Return true when no more circular buffer space is available in a thread safe way

  uint8_t * get = cb->get;                                  // Take get pointer first, it can be manipulated under our feet by read
  uint8_t * put = cb->put;
  int32_t   isfull = ((put + 1) == get);                    // If there is only 1 free slot left between put and get, we're full

  if ((put == cb->wrap) && (get == cb->buffer)) {           // If put would wrap around and get is at the start ...
    isfull = 1;                                             // ... we're full too
  }
  
  return isfull;
 
}

int32_t bcb_isempty(const Bcb_t * cb) {                     // Return true when the circular buffer is empty
  return (cb->put == cb->get);
}

void bcb_writeslice(const Bcb_t * cb, slice_t slice) {

  uint8_t * get = cb->get;                                  // Snapshot of get pointer first, it can be manipulated under our feet by another thread
  uint8_t * put = cb->put;

  if (put >= get) {
    slice->num = (uint32_t) (cb->end - put);
    if (get == cb->buffer) {
      slice->num -= 1u;                                     // Adjust available space, put must not run into get
    }
  }
  else {
    slice->num = (uint32_t) (get - put) - 1;                // -1 since put must always point to a free slot (put should not run into get)
  }

  slice->data = put;                                        // Set address user can write to

  assert((slice->snap.get = get, 1));                       // Assign check value only when assertions enabled
  assert((slice->max = slice->num, 1));                     // Assign check value only when assertions enabled
  assert(slice->num <= cb->size);
 
}

void bcb_produced(bcb_t cb, const Slice_t * slice) {

  uint8_t * put = cb->put;

  assert(slice->max >= slice->num);                         // Check that we've never commit more then what was available in bcb_writeslice
  assert(put + slice->num <= cb->end);                      // Should never go beyond end

  if (put < slice->snap.get) {                              // If put less then saved get pointer ...
    assert(put + slice->num < slice->snap.get);             // ... put should not overtake get
  }
   
  if ((put + slice->num) == cb->end) {
    put = cb->buffer;                                       // Wrap around to start
  }
  else {
    put += slice->num;                                      // Just advance next put position
  }
   
  assert(0 == slice->num || put != slice->snap.get);        // When we produced data, put should never become get because it would mean we're empty

  cb->put = put;                                            // Assign new put atomically

}
 
uint32_t bcb_canread(const Bcb_t * cb) {

  uint32_t  num;
  uint8_t * put = cb->put;                                  // Read, so snapshot of put first ...
  uint8_t * get = cb->get;                                  // ... then get
  
  if (get <= put) {
    num = (uint32_t) (put - get);
  }
  else {
    num = cb->size - (uint32_t) (get - put);
  }
   
  return num;

}

uint32_t bcb_canwrite(const Bcb_t * cb) {

  uint32_t  num;
  uint8_t * get = cb->get;                                  // Write, take snapshot of get first ...
  uint8_t * put = cb->put;                                  // ... then put

  if (get == put) {
    num = cb->size;
  }
  else if (get < put) {
    num = (cb->size - (uint32_t) (put - get)) - 1;
  }
  else {
    assert(get > put);
    num = (uint32_t) (get - put - 1);
  }
   
  assert(num <= cb->size);

  return num;

}
 
void bcb_init(bcb_t cb, uint32_t size) {

  cb->put  = cb->buffer;
  cb->get  = cb->buffer;
  cb->size = size;
  cb->wrap = cb->buffer + cb->size;                         // Note that we have 4 extra cells in the header
  cb->end  = cb->wrap + 1;                                  // We precalculate end so that we don't have to do additions to get for overflow

}
