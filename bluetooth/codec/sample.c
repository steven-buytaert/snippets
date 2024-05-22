// Copyright 2024 Steven Buytaert

/*
  To build the sample code from this directory.
 
  $ gcc [-m32] -Os -fstack-protector -Wall -Werror -I . -I .. -o sample sample.c hci-codec.c hci-tables.c
  $ size sample
    text	   data	    bss	    dec	    hex	filename
    9902	    518	      2	  10422	   28b6	sample
    
  Since the tables are generated for use on a 32 bit (embedded) system, make sure the sample
  is compiled in 32 bit mode.
  
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <hci-codec.h>
#include <hci-types-5.4.h>

// Sample of an LE_Advertising_Report event packet.

static uint8_t packet[46] = {
  0x04, 0x3e, 0x2b, 0x02, 0x02, 0x00, 0x00, 0x21,
  0xf9, 0x98, 0x18, 0x6f, 0x3b, 0x15, 0x02, 0x01,
  0x06, 0x03, 0x02, 0x09, 0x18, 0x0d, 0x08, 0x48,
  0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x77, 0x6f, 0x72,
  0x6c, 0x64, 0x21, 0xcb, 0x04, 0x00, 0x21, 0xf9,
  0x98, 0x18, 0x6f, 0x3b, 0x00, 0xcb
};

int main(int argc, char * argv[]) {

  uint8_t space4dec[128] __attribute__((aligned (8)));      // Properly aligned and large enough.
  uint8_t encoded[128];

  // -- Decode the HCI sample packet into a C structure.

  CodecReq_t DReq = {
    .Struct.sz  = sizeof(space4dec),
    .Struct.buf = space4dec,
    .Pkt.buf    = packet,
    .Pkt.sz     = sizeof(packet),
  };

  pkt2struct(& DReq);

  // -- Now perform some operations on the C struct reference.

  HCI_LE_Advertising_Report_Evt_t * rep = (HCI_LE_Advertising_Report_Evt_t *) space4dec;

  assert(type_EVT == rep->Evt.type);
  assert(0x003e == rep->Evt.code);                          // Must be a meta event.
  assert(LE_Advertising_Report_Evt_sub == rep->Subevent_Code);

  printf("LE_Advertising_Report event has %u reports.\n", rep->Num_Reports);
  printf("Report 0 Data_Length %u data %p.\n", rep->Reports[0].Data_Length, rep->Reports[0].Data);
  printf("Report 1 Data_Length %u data %p.\n", rep->Reports[1].Data_Length, rep->Reports[1].Data);

  // ...

  // -- Now encode it back into an HCI packet.

  printf("Report Evt.length %u bytes (from reference packet).\n", rep->Evt.length);

  rep->Evt.length = 0;                                      // No need to provide the packet length.

  CodecReq_t EReq = {
    .Struct.sz  = sizeof(space4dec),
    .Struct.buf = space4dec,
    .Pkt.buf    = encoded,
    .Pkt.sz     = sizeof(encoded),
  };

  assert(sizeof(packet) == struct2pkt(& EReq));             // Re-encoded should match original packet size.

  assert(0 == memcmp(encoded, packet, sizeof(packet)));     // And contents should match original.

  printf("Packet Evt.length %u bytes (calculated by codec).\n", encoded[2]);

  // -- Sizing calculation for decoding (encoding works similar).
  
  DReq.Struct.buf = NULL;                                   // Just set destination buffer to NULL.

  printf("Would need %u bytes for the C Struct,\n", pkt2struct(& DReq));

  return 0;

}
