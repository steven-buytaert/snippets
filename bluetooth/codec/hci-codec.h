#ifndef HCI_CODEC_H
#define HCI_CODEC_H

// Copyright 2024 Steven Buytaert

#include <stdint.h>

typedef struct CodecReq_t * codecreq_t;

typedef enum {
  CReq_OK        = 0,
  CReq_OOB       = 1,             // Destination (packet or structure) too small.
  CReq_Not_Found = 2,             // Type (event, command or return) not found.
  CReq_too_Short = 3,             // Source input for decoding or encoding too short.
  CReq_bad_code  = 4,             // An internal coding error; generator issue.
  CReq_is_packed = 5,             // Structure is packed; nothing decoded.
  CReq_null_ptr  = 6,             // Reading from a NULL pointer.
} CodecReqStat_t;

typedef struct CodecReq_t {
  struct {
    void *       buf;             // Can be NULL for decoding to measure required size.
    uint16_t     sz;
    uint16_t     status;          // One of the CodecReqStat_t values.
  } Struct;                       // Structure view.
  struct {
    uint8_t *    buf;             // Can be NULL for encoding to measure the required size.
    uint16_t     sz;
    uint16_t     status;          // One of the CodecReqStat_t values.
  } Pkt;                          // Encoded packet view.
} CodecReq_t;

// Encode a structure into an HCI packet; return packet size if successful, 0 if not.
// Note that the Evt.length or Cmd.length does not have to be preset; it can be left 0.

uint32_t struct2pkt(codecreq_t req);

// Decode a packet into a structure; return structure size of successful, 0 if not.

uint32_t pkt2struct(codecreq_t req);

#endif // HCI_CODEC_H
