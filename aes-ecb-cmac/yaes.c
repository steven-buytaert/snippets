
// Copyright 2024 Steven Buytaert

#include <yaes.h>
#include <assert.h>
#include <string.h>

typedef struct ECBCtx_t * ctx_t;  // Shorthand notations.
typedef union ECBCol_t * col_t;

/*

 The following documentation was helpful.
 
 https://www.samiam.org/rijndael.html
 https://medium.com/codex/aes-how-the-most-advanced-encryption-actually-works-b6341c44edb9
 https://en.wikipedia.org/wiki/Advanced_Encryption_Standard

 For the different modes, this site was very good providing an overview:
 https://www.highgo.ca/2019/08/08/the-difference-in-five-modes-in-the-aes-encryption-algorithm/

 And of course the original paper found here: 
 https://csrc.nist.gov/csrc/media/projects/cryptographic-standards-and-guidelines/documents/aes-development/rijndael-ammended.pdf

 The CBC-CMAC notes:
 https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-38B.pdf
 and of course, the spec itself for the AES CMAC.
 https://www.rfc-editor.org/rfc/rfc4493.html

 AES is a specific implementation of the Rijndael algorithm; in the AES case, Nb is always 4 (4 columns).

              0    1   2   3   4   5   6   7   8   9  10  11  12  13  14  15   

             +---+---+---+---+
     Col 0   |   |   |   |   |
             +---+---+---+---+---+---+---+---+
     Col 1                   |   |   |   |   |
                             +---+---+---+---+---+---+---+---+
     Col 2                                   |   |   |   |   |
                                             +---+---+---+---+---+---+---+---+
     Col 3                                                   |   |   |   |   |
                                                             +---+---+---+---+

*/

AESType_t AESTypes[] = {
  { .Size = { 
      .bits     = 128, 
      .bytes    =  16, 
      .expanded = 176,
    }, 
    .Nr         =  10, 
    .Nk         =   4,
  },

  { .Size = { 
      .bits     = 192, 
      .bytes    =  24, 
      .expanded = 208,
    }, 
    .Nr         =  12, 
    .Nk         =   6,
  },

  { .Size = { 
      .bits     = 256, 
      .bytes    =  32, 
      .expanded = 240,
    }, 
    .Nr         =  14, 
    .Nk         =   8,
  },
};

static void shiftRows(ctx_t ctx) {

  uint8_t saved;

  saved              = ctx->Col[0].u08[1];                  // Row 1: rotate 1 to the left.
  ctx->Col[0].u08[1] = ctx->Col[1].u08[1];
  ctx->Col[1].u08[1] = ctx->Col[2].u08[1];
  ctx->Col[2].u08[1] = ctx->Col[3].u08[1];
  ctx->Col[3].u08[1] = saved;

  saved              = ctx->Col[0].u08[2];                  // Row 2: rotate 2 to the left.
  ctx->Col[0].u08[2] = ctx->Col[2].u08[2];
  ctx->Col[2].u08[2] = saved;

  saved              = ctx->Col[1].u08[2];
  ctx->Col[1].u08[2] = ctx->Col[3].u08[2];
  ctx->Col[3].u08[2] = saved;

  saved              = ctx->Col[3].u08[3];                  // Row 3: rotate 3 to the left, aka 1 to the right.
  ctx->Col[3].u08[3] = ctx->Col[2].u08[3];
  ctx->Col[2].u08[3] = ctx->Col[1].u08[3];
  ctx->Col[1].u08[3] = ctx->Col[0].u08[3];
  ctx->Col[0].u08[3] = saved;
  
}

static void undoShiftRows(ctx_t ctx) {                      // Inverse shiftRows operation.

  uint8_t saved;

  saved              = ctx->Col[3].u08[1];                  // Row 1: rotate 1 to the right.
  ctx->Col[3].u08[1] = ctx->Col[2].u08[1];
  ctx->Col[2].u08[1] = ctx->Col[1].u08[1];
  ctx->Col[1].u08[1] = ctx->Col[0].u08[1];
  ctx->Col[0].u08[1] = saved;

  saved              = ctx->Col[0].u08[2];                  // Row 2: rotate 2 to the left == 2 to the right.
  ctx->Col[0].u08[2] = ctx->Col[2].u08[2];
  ctx->Col[2].u08[2] = saved;

  saved              = ctx->Col[1].u08[2];
  ctx->Col[1].u08[2] = ctx->Col[3].u08[2];
  ctx->Col[3].u08[2] = saved;

  saved              = ctx->Col[0].u08[3];                  // Row 3: rotate 3 to the right, aka 1 to the left.
  ctx->Col[0].u08[3] = ctx->Col[1].u08[3];
  ctx->Col[1].u08[3] = ctx->Col[2].u08[3];
  ctx->Col[2].u08[3] = ctx->Col[3].u08[3];
  ctx->Col[3].u08[3] = saved;

}

static const uint8_t sbox[256] = {                          // Substitution box.
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16,
};

static void subBytes(ctx_t ctx) {

  int32_t i = sizeof(ctx->state); while (i--) {
    ctx->state[i] = sbox[ctx->state[i]];
  }

}

static const uint8_t ubox[256] = {                          // Undo substitution box.
  0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
  0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
  0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
  0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
  0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
  0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
  0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
  0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
  0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
  0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
  0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
  0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
  0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
  0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
  0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
  0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d,
};

static void undoSubBytes(ctx_t ctx) {

  int32_t i = sizeof(ctx->state); while (i--) {
    ctx->state[i] = ubox[ctx->state[i]];
  }

}

static const uint8_t logtab[256] = { // Created with a generator of an excellent year.
  0x00, 0xff, 0x7d, 0x6b, 0xfa, 0xd6, 0xe8, 0x15, 0x78, 0x80, 0x54, 0xa3, 0x66, 0xdd, 0x92, 0x42, 
  0xf5, 0xad, 0xfd, 0xdf, 0xd1, 0x2a, 0x21, 0x49, 0xe3, 0x6a, 0x5b, 0xeb, 0x10, 0x0f, 0xbf, 0xfb, 
  0x73, 0x67, 0x2b, 0xf2, 0x7b, 0xa0, 0x5d, 0x7a, 0x4f, 0xab, 0xa7, 0xd5, 0x9e, 0x57, 0xc6, 0x5a, 
  0x61, 0xb8, 0xe7, 0x19, 0xd8, 0x4b, 0x69, 0x1b, 0x8d, 0xb4, 0x8c, 0xf3, 0x3d, 0xae, 0x79, 0x95, 
  0xf0, 0x01, 0xe4, 0x4e, 0xa8, 0x47, 0x70, 0x1a, 0xf8, 0x9a, 0x1e, 0x20, 0xda, 0x5f, 0xf7, 0x7f, 
  0xcc, 0xbb, 0x29, 0x24, 0x25, 0x84, 0x53, 0x1f, 0x1c, 0x86, 0xd4, 0xc7, 0x44, 0x11, 0xd7, 0xb6, 
  0xde, 0x28, 0x36, 0xd2, 0x65, 0x5e, 0x96, 0xb1, 0x56, 0xe5, 0xc8, 0x3f, 0xe6, 0xce, 0x98, 0x0c, 
  0x0b, 0xc5, 0x32, 0xcf, 0x0a, 0xb7, 0x71, 0xc2, 0xba, 0xf4, 0x2c, 0x17, 0xf6, 0x41, 0x13, 0x81, 
  0x6e, 0xec, 0x7e, 0x91, 0x62, 0x30, 0xcb, 0xac, 0x26, 0x03, 0xc4, 0x60, 0xed, 0x82, 0x97, 0x59, 
  0x76, 0x37, 0x18, 0x31, 0x9b, 0x3b, 0x9d, 0xcd, 0x58, 0x2e, 0xdc, 0xd9, 0x75, 0xe9, 0xfc, 0x23, 
  0x4a, 0x09, 0x39, 0x93, 0xa6, 0x3e, 0xa1, 0x07, 0xa2, 0x1d, 0x02, 0xe1, 0xd0, 0x83, 0x9c, 0xc9, 
  0x99, 0x77, 0x04, 0xc3, 0x52, 0x48, 0x45, 0x3a, 0xc1, 0x68, 0x8e, 0x51, 0x55, 0xaa, 0x34, 0xe0, 
  0x5c, 0xb0, 0xa5, 0x6c, 0xb3, 0xb9, 0x50, 0xbd, 0xe2, 0x85, 0xdb, 0x05, 0x14, 0x6f, 0x2f, 0xb2, 
  0xd3, 0xea, 0x63, 0x2d, 0x46, 0xf9, 0xbc, 0xca, 0x64, 0x9f, 0x4c, 0x06, 0x16, 0x8b, 0x89, 0xc0, 
  0x88, 0x22, 0x43, 0x0e, 0xaf, 0x6d, 0x4d, 0x7c, 0x87, 0x08, 0x35, 0xf1, 0xee, 0x33, 0x40, 0x3c, 
  0x38, 0x0d, 0x72, 0x27, 0xa9, 0x8f, 0x94, 0x12, 0x74, 0x8a, 0xbe, 0xb5, 0x90, 0xa4, 0xfe, 0xef, 
};

static const uint8_t exptab[256] = {
  0x01, 0x41, 0xaa, 0x89, 0xb2, 0xcb, 0xdb, 0xa7, 0xe9, 0xa1, 0x74, 0x70, 0x6f, 0xf1, 0xe3, 0x1d, 
  0x1c, 0x5d, 0xf7, 0x7e, 0xcc, 0x07, 0xdc, 0x7b, 0x92, 0x33, 0x47, 0x37, 0x58, 0xa9, 0x4a, 0x57, 
  0x4b, 0x16, 0xe1, 0x9f, 0x53, 0x54, 0x88, 0xf3, 0x61, 0x52, 0x15, 0x22, 0x7a, 0xd3, 0x99, 0xce, 
  0x85, 0x93, 0x72, 0xed, 0xbe, 0xea, 0x62, 0x91, 0xf0, 0xa2, 0xb7, 0x95, 0xef, 0x3c, 0xa5, 0x6b, 
  0xee, 0x7d, 0x0f, 0xe2, 0x5c, 0xb6, 0xd4, 0x45, 0xb5, 0x17, 0xa0, 0x35, 0xda, 0xe6, 0x43, 0x28, 
  0xc6, 0xbb, 0xb4, 0x56, 0x0a, 0xbc, 0x68, 0x2d, 0x98, 0x8f, 0x2f, 0x1a, 0xc0, 0x26, 0x65, 0x4d, 
  0x8b, 0x30, 0x84, 0xd2, 0xd8, 0x64, 0x0c, 0x21, 0xb9, 0x36, 0x19, 0x03, 0xc3, 0xe5, 0x80, 0xcd, 
  0x46, 0x76, 0xf2, 0x20, 0xf8, 0x9c, 0x90, 0xb1, 0x08, 0x3e, 0x27, 0x24, 0xe7, 0x02, 0x82, 0x4f, 
  0x09, 0x7f, 0x8d, 0xad, 0x55, 0xc9, 0x59, 0xe8, 0xe0, 0xde, 0xf9, 0xdd, 0x3a, 0x38, 0xba, 0xf5, 
  0xfc, 0x83, 0x0e, 0xa3, 0xf6, 0x3f, 0x66, 0x8e, 0x6e, 0xb0, 0x49, 0x94, 0xae, 0x96, 0x2c, 0xd9, 
  0x25, 0xa6, 0xa8, 0x0b, 0xfd, 0xc2, 0xa4, 0x2a, 0x44, 0xf4, 0xbd, 0x29, 0x87, 0x11, 0x3d, 0xe4, 
  0xc1, 0x67, 0xcf, 0xc4, 0x39, 0xfb, 0x5f, 0x75, 0x31, 0xc5, 0x78, 0x51, 0xd6, 0xc7, 0xfa, 0x1e, 
  0xdf, 0xb8, 0x77, 0xb3, 0x8a, 0x71, 0x2e, 0x5b, 0x6a, 0xaf, 0xd7, 0x86, 0x50, 0x97, 0x6d, 0x73, 
  0xac, 0x14, 0x63, 0xd0, 0x5a, 0x2b, 0x05, 0x5e, 0x34, 0x9b, 0x4c, 0xca, 0x9a, 0x0d, 0x60, 0x13, 
  0xbf, 0xab, 0xc8, 0x18, 0x42, 0x69, 0x6c, 0x32, 0x06, 0x9d, 0xd1, 0x1b, 0x81, 0x8c, 0xec, 0xff, 
  0x40, 0xeb, 0x23, 0x3b, 0x79, 0x10, 0x7c, 0x4e, 0x48, 0xd5, 0x04, 0x1f, 0x9e, 0x12, 0xfe, 0x01, 
};

static uint32_t mod255(uint32_t i) {                        // Timing invariant modulo 255 operation.

  uint32_t m;

  static const uint8_t mod255mask[32] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
  };

  m = 1 + i + (i >> 8);                                     // Create an initial modulo table index.
  m = (m >> 4) | (m & 0x0f);                                // Compress to a modulo index for our table (strip out middle 4 bits).
  m = m - 1;                                                // Subtract the one we added (to reach either 0xff or 0x1ff from 0xfe or 0x1fe resp.).

  return ((i + (i >> 8)) & mod255mask[m]);                  // Return the modulo after applying the mask of the modulo table

}

static uint8_t gmul(uint8_t a, uint8_t b) {                 // Galois multiply and and b.

  a = logtab[a];
  b = logtab[b];

  uint8_t p = exptab[mod255((uint32_t) a + b)];             // product p = a * b.

  return (a && b && p) ? p : 0;                             // Timing invariant way to handle a = 0 or b = 0 as input.

}

static uint8_t xtime(uint8_t v) {                           // Literally from the spec; same as return gmul(2, v);

  return (uint8_t) ((v << 1) ^ (((v >> 7) & 1) * 0x1b));    // Always EXOR; operation involves no conditional, constant timing.
  
}

static void mixOneColumn(col_t col) { 

  uint8_t * a = col->u08;
  uint8_t Tmp, Tm, a0 = a[0];                               // Make a copy of a[0] as it will be overwritten.

  Tmp = a[0] ^ a[1] ^ a[2] ^ a[3];                          // Nearly copied verbatim from the spec.
   Tm = a[0] ^ a[1]; Tm = xtime(Tm); a[0] ^= Tm ^ Tmp;      // Original a[0] overwritten here, so we made a copy of it.
   Tm = a[1] ^ a[2]; Tm = xtime(Tm); a[1] ^= Tm ^ Tmp;
   Tm = a[2] ^ a[3]; Tm = xtime(Tm); a[2] ^= Tm ^ Tmp;
   Tm = a[3] ^ a0;   Tm = xtime(Tm); a[3] ^= Tm ^ Tmp;      // Only difference with spec is a0 and not a[0], since overwritten.

}

static void mixColumns(ctx_t ctx) {

  mixOneColumn(& ctx->Col[0]);
  mixOneColumn(& ctx->Col[1]);
  mixOneColumn(& ctx->Col[2]);
  mixOneColumn(& ctx->Col[3]);

}

static void undoMixOneColumn(col_t col) {                   // Multiple with inverse matrix.

  uint8_t * a = col->u08;
  uint8_t   a0 = a[0];
  uint8_t   a1 = a[1];
  uint8_t   a2 = a[2];
  uint8_t   a3 = a[3];

  a[0] = gmul(a0, 14) ^ gmul(a3, 9) ^ gmul(a2, 13) ^ gmul(a1, 11);
  a[1] = gmul(a1, 14) ^ gmul(a0, 9) ^ gmul(a3, 13) ^ gmul(a2, 11);
  a[2] = gmul(a2, 14) ^ gmul(a1, 9) ^ gmul(a0, 13) ^ gmul(a3, 11);
  a[3] = gmul(a3, 14) ^ gmul(a2, 9) ^ gmul(a1, 13) ^ gmul(a0, 11);

}

static void undoMixColumns(ctx_t ctx) {

  undoMixOneColumn(& ctx->Col[0]);
  undoMixOneColumn(& ctx->Col[1]);
  undoMixOneColumn(& ctx->Col[2]);
  undoMixOneColumn(& ctx->Col[3]);

}

static void expandKey(ctx_t ctx, AESType_t * type) {

  uint32_t  i;                                              // Round
  uint32_t  w;                                              // Byte offset to the word.
  uint8_t   temp[4];
  uint8_t   saved;
  uint8_t * RKey = ctx->RKey;

  static const uint32_t Nb = 4;                             // By AES Definition; number of columns.

  static const uint8_t Rcon[11] = {
    0x8d, 0x01, 0x02, 0x04, 0x08, 
    0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };

  // Note that the cipher key copy of Nk words has been done already by the init function.

  for (i = type->Nk; i < Nb * (type->Nr + 1); i++) {
    w = (i - 1) * 4;
    temp[0] = RKey[w + 0];                                  // temp = W[i - 1];
    temp[1] = RKey[w + 1];
    temp[2] = RKey[w + 2];
    temp[3] = RKey[w + 3];

    if (i % type->Nk == 0) {
      saved = temp[0];                                      // RotByte implementation; rotate left.
      temp[0] = temp[1];
      temp[1] = temp[2];
      temp[2] = temp[3];
      temp[3] = saved;

      temp[0] = sbox[temp[0]];                              // temp = SubByte(RotByte(temp))
      temp[1] = sbox[temp[1]];
      temp[2] = sbox[temp[2]];
      temp[3] = sbox[temp[3]];

      temp[0] = temp[0] ^ Rcon[i / type->Nk];               // Note that temp[1-3] are not changed.
    }  

    if (type->Nk > 6 && i % type->Nk == 4) {                // For the 256 bit keysize only; temp = SubByte(temp);
      temp[0] = sbox[temp[0]];
      temp[1] = sbox[temp[1]];
      temp[2] = sbox[temp[2]];
      temp[3] = sbox[temp[3]];
    }

    const uint32_t u = i * 4; w = (i - type->Nk) * 4;       // W[i] = W[i - Nk] ^ temp;

    RKey[u + 0] = RKey[w + 0] ^ temp[0];
    RKey[u + 1] = RKey[w + 1] ^ temp[1];
    RKey[u + 2] = RKey[w + 2] ^ temp[2];
    RKey[u + 3] = RKey[w + 3] ^ temp[3];
  }

}

uint32_t yaes_ecb_init(ECBCtx_t * ctx, const uint8_t key[], AESTypeNum_t type) {

  if (! ctx || ! key) { return 0; }                         // Initial check.

  ctx->type = type & 0b1111;

  // 4.3.1. Key Expansion
  // ...The first Nk words contain the Cipher Key. 

  memcpy(ctx->RKey, key, AESTypes[type].Size.bytes);

  expandKey(ctx, & AESTypes[type]);

  return 1;

}

static void addRoundKey(ECBCtx_t * ctx, uint32_t round) {

  const uint32_t off = sizeof(ctx->state) * round;

  for (uint32_t i = 0; i < sizeof(ctx->state); i++) {
    ctx->state[i] ^= ctx->RKey[off + i];
  }

}

void yaes_ecb_encrypt(ECBCtx_t * ctx) {

  uint32_t    round = 1;
  AESType_t * type = & AESTypes[ctx->type];

  assert(ctx->type <= AES_256);

  addRoundKey(ctx, 0);                                      // First round, just add the roundkey.

  do {
    subBytes(ctx);
    shiftRows(ctx);
    if (round < type->Nr) { mixColumns(ctx); }              // Last round isn't mixed.
    addRoundKey(ctx, round);
  } while (round++ < type->Nr);

}

void yaes_ecb_decrypt(ECBCtx_t * ctx) {

  AESType_t * type = & AESTypes[ctx->type];
  uint32_t    round = type->Nr;

  assert(ctx->type <= AES_256);

  addRoundKey(ctx, round--);                                // Undo last round.

  do {
    undoShiftRows(ctx);
    undoSubBytes(ctx);
    addRoundKey(ctx, round);
    if (round) { undoMixColumns(ctx); }                     // Don't unmix round 0.
  } while (round--);

}

// AES CMAC Implementation

static void shiftLeft(uint8_t dst[16], const uint8_t src[16]) {

  uint8_t carry = 0x00;

  for (int32_t i = 15; i >= 0; i--) {
    dst[i] = (uint8_t) (src[i] << 1);
    dst[i] |= carry;
    carry = (src[i] & 0x80) ? 1 : 0;
  }
    
}

static void XORWith(uint8_t dst[16], const uint8_t with[16]) {

  for (uint32_t i = 0; i < 16; i++) {
    dst[i] ^= with[i];
  }

}

static void genSubkey(AES_CMAC_t * ctx, uint8_t K[], uint32_t genK2) {

  uint8_t state[16];                                        // To temporary save the ECB state.
  uint8_t K2[16];

  ECBCtx_t * ecb = & ctx->ECBCtx;

  static const uint8_t Rb[] = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x87
  };

  memcpy(state, ecb->state, sizeof(ecb->state));            // Save current ECB state.

  memset(ecb->state, 0x00, sizeof(ecb->state));             // Step 1.  L := AES-128(K, const_Zero);

  yaes_ecb_encrypt(ecb);                                    // ECB State is now L.
  
  shiftLeft(K, ecb->state);                                 // Step 2. K1 := L << 1;

  if (ecb->state[0] & 0x80) {                               // Step 2. if MSB(L) is NOT equal to 0
    XORWith(K, Rb);                                         //         K1 := (L << 1) XOR const_Rb;
  }

  if (genK2) {
    shiftLeft(K2, K);                                       // Step 3. K2 := (K1 << 1)

    if (K[0] & 0x80) {                                      // Step 3. if MSB(K1) is NOT equal to 0
      XORWith(K2, Rb);                                      //         K2 := (K1 << 1) XOR const_Rb;  
    }
    memcpy(K, K2, sizeof(K2));
  }

  memcpy(ecb->state, state, sizeof(ecb->state));            // Restore ECB state.

}

uint32_t yaes_cmac_init(AES_CMAC_t * ctx, const uint8_t key[], AESTypeNum_t type) {

  if (! ctx || ! key) { return 0; }

  ECBCtx_t * ecb = & ctx->ECBCtx;

  (void) yaes_ecb_init(ecb, key, type);                     // Should return 1.

  ctx->ECBCtx.numun = 0;

  memset(ecb->state, 0x00, sizeof(ecb->state));             // Step 5.  X := const_Zero;

  return 1;
  
}

static const uint8_t blocksize = 16;                        // By AES CMAC definition, 128 bits or 16 bytes.

void yaes_cmac_feed(AES_CMAC_t * ctx, const uint8_t msg[], uint32_t size) {

  ECBCtx_t *      ecb = & ctx->ECBCtx;
  const uint8_t * src = msg;
  uint8_t         todo;
  uint32_t        n;

  assert(ecb->numun <= blocksize);
  
  if (size && ecb->numun < blocksize) {                     // Top up the unprocessed block, if possible.
    todo = blocksize - ecb->numun;                          // Can still store this number of bytes in unprocessed block.
    todo = todo > size ? (uint8_t) size : todo;             // Clip if more than provided.
    memcpy(& ctx->un[ecb->numun], src, todo);
    ecb->numun += todo;
    size       -= todo;
    src        += todo;
  }

  if (size) {                                               // There's a full unprocessed block and enough remainder bytes.
    assert(blocksize == ecb->numun);                        // Must be a full block.
    XORWith(ecb->state, ctx->un);
    yaes_ecb_encrypt(ecb);
    ecb->numun = 0;
  }

  n = size / blocksize;                                     // Number of full blocks remaining.

  while (n) {                                               // Step 6.  for i := 1 to n-1 do
    assert(0 == ecb->numun);                                // (Internal block must be empty.)
    XORWith(ecb->state, src);                               // Step 6.  Y := X XOR M_i;
    yaes_ecb_encrypt(ecb);                                  // Step 6.  X := AES-128(K,Y);
    src  += blocksize;
    size -= blocksize;
    n    -= 1;
    if (size == blocksize) { break; }                       // Always keep an unprocessed block for the final round.
  }

  if (size) {                                               // Save remainder for next time, if any.
    assert(size <= blocksize);
    assert(0 == ecb->numun);
    memcpy(ctx->un, src, size);
    ecb->numun = (uint8_t) size;
  }

}

static void padblock(uint8_t block[16], uint32_t from) {

  assert(from <= 15);

  block[from++] = 0x80;

  for (uint32_t i = from; i < blocksize; i++) {
    block[i] = 0x00;
  }

}

void yaes_cmac_finish(AES_CMAC_t * ctx, const uint8_t msg[], uint32_t size) {

  ECBCtx_t * ecb = & ctx->ECBCtx;
  uint8_t    K[16];

  yaes_cmac_feed(ctx, msg, size);

  if (ecb->numun == blocksize) {                            // Step 4.  if flag is true (no padding)
    genSubkey(ctx, K, 0);                                   //          then M_last := M_n XOR K1;
  }
  else {
    padblock(ctx->un, ecb->numun);                          //          else M_last := padding(M_n) ...
    genSubkey(ctx, K, 1);                                   //                         ... XOR K2;
  }

  XORWith(ctx->un, K);                                      // M_last XOR with K1 or K2.
  XORWith(ecb->state, ctx->un);                             // Y := M_last XOR X;
  yaes_ecb_encrypt(ecb);                                    // T := AES-128(K,Y);

}
