// Copyright 2024 Steven Buytaert

#include <yaes.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

typedef const struct ECB_TC_t {   // ECB Test Case.
  uint8_t *       key;
  uint8_t         txt[16];        // Plaintext.
  uint8_t         ciphertxt[16];  // Encrypted text.
  uint32_t        keysize;        // Key size in bits.
  uint8_t         pad[4];
} ECB_TC_t;

typedef struct CMAC_TC_t {        // AES CMAC Test Case.
  uint32_t        mlen;           // Plaintext message length, can be 0.
  uint32_t        keysize;        // Key size in bits.
  uint8_t *       key;
  uint8_t *       txt;            // Plaintext.
  uint8_t *       tag;            // Expected AES CMAC value.
} CMAC_TC_t;

// A few AES ECB tests from the Known Answer Test vectors.
// https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Algorithm-Validation-Program/documents/aes/AESAVS.pdf

static const ECB_TC_t ECB_TC[] = {
  // ECBGFSbox128.rsp
  { .keysize = 128,
    .key = (uint8_t[]) { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    .txt =             { 0xf3, 0x44, 0x81, 0xec, 0x3c, 0xc6, 0x27, 0xba, 0xcd, 0x5d, 0xc3, 0xfb, 0x08, 0xf2, 0x73, 0xe6 },
    .ciphertxt =       { 0x03, 0x36, 0x76, 0x3e, 0x96, 0x6d, 0x92, 0x59, 0x5a, 0x56, 0x7c, 0xc9, 0xce, 0x53, 0x7f, 0x5e }},

  // ECBGFSbox192.rsp
  { .keysize = 192,
    .key = (uint8_t[]) { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    .txt =             { 0x1b, 0x07, 0x7a, 0x6a, 0xf4, 0xb7, 0xf9, 0x82, 0x29, 0xde, 0x78, 0x6d, 0x75, 0x16, 0xb6, 0x39 },
    .ciphertxt =       { 0x27, 0x5c, 0xfc, 0x04, 0x13, 0xd8, 0xcc, 0xb7, 0x05, 0x13, 0xc3, 0x85, 0x9b, 0x1d, 0x0f, 0x72 }},

  // ECBGFSbox256.rsp
  { .keysize = 256,
    .key = (uint8_t[]) { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    .txt =             { 0x01, 0x47, 0x30, 0xf8, 0x0a, 0xc6, 0x25, 0xfe, 0x84, 0xf0, 0x26, 0xc6, 0x0b, 0xfd, 0x54, 0x7d },
    .ciphertxt =       { 0x5c, 0x9d, 0x84, 0x4e, 0xd4, 0x6f, 0x98, 0x85, 0x08, 0x5e, 0x5d, 0x6a, 0x4f, 0x94, 0xc7, 0xd7 }},

  // ECBKeySbox128.rsp
  { .keysize = 128,
    .key = (uint8_t[]) { 0x10, 0xa5, 0x88, 0x69, 0xd7, 0x4b, 0xe5, 0xa3, 0x74, 0xcf, 0x86, 0x7c, 0xfb, 0x47, 0x38, 0x59 },
    .txt =             { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    .ciphertxt =       { 0x6d, 0x25, 0x1e, 0x69, 0x44, 0xb0, 0x51, 0xe0, 0x4e, 0xaa, 0x6f, 0xb4, 0xdb, 0xf7, 0x84, 0x65 }},

  // ECBKeySbox192.rsp
  { .keysize = 192,
    .key = (uint8_t[]) { 0xe9, 0xf0, 0x65, 0xd7, 0xc1, 0x35, 0x73, 0x58, 0x7f, 0x78, 0x75, 0x35, 0x7d, 0xfb, 0xb1, 0x6c, 0x53, 0x48, 0x9f, 0x6a, 0x4b, 0xd0, 0xf7, 0xcd },
    .txt =             { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    .ciphertxt =       { 0x09, 0x56, 0x25, 0x9c, 0x9c, 0xd5, 0xcf, 0xd0, 0x18, 0x1c, 0xca, 0x53, 0x38, 0x0c, 0xde, 0x06 }},

  // ECBKeySbox256.rsp
  { .keysize = 256,
    .key = (uint8_t[]) { 0xc4, 0x7b, 0x02, 0x94, 0xdb, 0xbb, 0xee, 0x0f, 0xec, 0x47, 0x57, 0xf2, 0x2f, 0xfe, 0xee, 0x35, 0x87, 0xca, 0x47, 0x30, 0xc3, 0xd3, 0x3b, 0x69, 0x1d, 0xf3, 0x8b, 0xab, 0x07, 0x6b, 0xc5, 0x58 },
    .txt =             { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    .ciphertxt =       { 0x46, 0xf2, 0xfb, 0x34, 0x2d, 0x6f, 0x0a, 0xb4, 0x77, 0x47, 0x6f, 0xc5, 0x01, 0x24, 0x2c, 0x5f }},

  // ECBVarKey128.rsp
  { .keysize = 128,
    .key = (uint8_t[]) { 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    .txt =             { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    .ciphertxt =       { 0x0e, 0xdd, 0x33, 0xd3, 0xc6, 0x21, 0xe5, 0x46, 0x45, 0x5b, 0xd8, 0xba, 0x14, 0x18, 0xbe, 0xc8 }},

  // ECBVarKey192.rsp
  { .keysize = 192,
    .key = (uint8_t[]) { 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    .txt =             { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    .ciphertxt =       { 0xde, 0x88, 0x5d, 0xc8, 0x7f, 0x5a, 0x92, 0x59, 0x40, 0x82, 0xd0, 0x2c, 0xc1, 0xe1, 0xb4, 0x2c }},

  // ECBVarKey256.rsp
  { .keysize = 256,
    .key = (uint8_t[]) { 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    .txt =             { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    .ciphertxt =       { 0xe3, 0x5a, 0x6d, 0xcb, 0x19, 0xb2, 0x01, 0xa0, 0x1e, 0xbc, 0xfa, 0x8a, 0xa2, 0x2b, 0x57, 0x59 }},

  // ECBVarTxt128.rsp
  { .keysize = 128,
    .key = (uint8_t[]) { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    .txt =             { 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    .ciphertxt =       { 0x3a, 0xd7, 0x8e, 0x72, 0x6c, 0x1e, 0xc0, 0x2b, 0x7e, 0xbf, 0xe9, 0x2b, 0x23, 0xd9, 0xec, 0x34 }},

  // ECBVarTxt192.rsp
  { .keysize = 192,
    .key = (uint8_t[]) { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    .txt =             { 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    .ciphertxt =       { 0x6c, 0xd0, 0x25, 0x13, 0xe8, 0xd4, 0xdc, 0x98, 0x6b, 0x4a, 0xfe, 0x08, 0x7a, 0x60, 0xbd, 0x0c }},

  // ECBVarTxt256.rsp
  { .keysize = 256,
    .key = (uint8_t[]) { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    .txt =             { 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    .ciphertxt =       { 0xdd, 0xc6, 0xbf, 0x79, 0x0c, 0x15, 0x76, 0x0d, 0x8d, 0x9a, 0xeb, 0x6f, 0x9a, 0x75, 0xfd, 0x4e }},

  { .key = NULL },

};


// AES CMAC Tests
// https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/AES_CMAC.pdf

static const CMAC_TC_t CMAC_TC[] = {
  { .mlen = 0, .keysize = 128, 
    .key = (uint8_t[]) { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C },
    .tag = (uint8_t[]) { 0xBB, 0x1D, 0x69, 0x29, 0xE9, 0x59, 0x37, 0x28, 0x7F, 0xA3, 0x7D, 0x12, 0x9B, 0x75, 0x67, 0x46 }},
  
  { .mlen = 16, .keysize = 128,
    .key = (uint8_t[]) { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C },
    .txt = (uint8_t[]) { 0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A },
    .tag = (uint8_t[]) { 0x07, 0x0A, 0x16, 0xB4, 0x6B, 0x4D, 0x41, 0x44, 0xF7, 0x9B, 0xDD, 0x9D, 0xD0, 0x4A, 0x28, 0x7C }},

  { .mlen = 20, .keysize = 128,
    .key = (uint8_t[]) { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C },
    .txt = (uint8_t[]) { 0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A,
                         0xAE, 0x2D, 0x8A, 0x57 },
    .tag = (uint8_t[]) { 0x7D, 0x85, 0x44, 0x9E, 0xA6, 0xEA, 0x19, 0xC8, 0x23, 0xA7, 0xBF, 0x78, 0x83, 0x7D, 0xFA, 0xDE }},

  { .mlen = 64, .keysize = 128,
    .key = (uint8_t[]) { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C },
    .txt = (uint8_t[]) { 0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A,
                         0xAE, 0x2D, 0x8A, 0x57, 0x1E, 0x03, 0xAC, 0x9C, 0x9E, 0xB7, 0x6F, 0xAC, 0x45, 0xAF, 0x8E, 0x51,
                         0x30, 0xC8, 0x1C, 0x46, 0xA3, 0x5C, 0xE4, 0x11, 0xE5, 0xFB, 0xC1, 0x19, 0x1A, 0x0A, 0x52, 0xEF,
                         0xF6, 0x9F, 0x24, 0x45, 0xDF, 0x4F, 0x9B, 0x17, 0xAD, 0x2B, 0x41, 0x7B, 0xE6, 0x6C, 0x37, 0x10 },
    .tag = (uint8_t[]) { 0x51, 0xF0, 0xBE, 0xBF, 0x7E, 0x3B, 0x9D, 0x92, 0xFC, 0x49, 0x74, 0x17, 0x79, 0x36, 0x3C, 0xFE }},

  { .mlen = 0, .keysize = 192,
    .key = (uint8_t[]) { 0x8E, 0x73, 0xB0, 0xF7, 0xDA, 0x0E, 0x64, 0x52, 0xC8, 0x10, 0xF3, 0x2B, 0x80, 0x90, 0x79, 0xE5,
                         0x62, 0xF8, 0xEA, 0xD2, 0x52, 0x2C, 0x6B, 0x7B },
    .tag = (uint8_t[]) { 0xD1, 0x7D, 0xDF, 0x46, 0xAD, 0xAA, 0xCD, 0xE5, 0x31, 0xCA, 0xC4, 0x83, 0xDE, 0x7A, 0x93, 0x67 }},

  { .mlen = 16, .keysize = 192,
    .key = (uint8_t[]) { 0x8E, 0x73, 0xB0, 0xF7, 0xDA, 0x0E, 0x64, 0x52, 0xC8, 0x10, 0xF3, 0x2B, 0x80, 0x90, 0x79, 0xE5, 
                         0x62, 0xF8, 0xEA, 0xD2, 0x52, 0x2C, 0x6B, 0x7B },
    .txt = (uint8_t[]) { 0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A },
    .tag = (uint8_t[]) { 0x9E, 0x99, 0xA7, 0xBF, 0x31, 0xE7, 0x10, 0x90, 0x06, 0x62, 0xF6, 0x5E, 0x61, 0x7C, 0x51, 0x84 }},

  { .mlen = 20, .keysize = 192,
    .key = (uint8_t[]) { 0x8E, 0x73, 0xB0, 0xF7, 0xDA, 0x0E, 0x64, 0x52, 0xC8, 0x10, 0xF3, 0x2B, 0x80, 0x90, 0x79, 0xE5, 
                         0x62, 0xF8, 0xEA, 0xD2, 0x52, 0x2C, 0x6B, 0x7B },
    .txt = (uint8_t[]) { 0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A, 
                         0xAE, 0x2D, 0x8A, 0x57 },
    .tag = (uint8_t[]) { 0x3D, 0x75, 0xC1, 0x94, 0xED, 0x96, 0x07, 0x04, 0x44, 0xA9, 0xFA, 0x7E, 0xC7, 0x40, 0xEC, 0xF8 }},

  { .mlen = 64, .keysize = 192,
    .key = (uint8_t[]) { 0x8E, 0x73, 0xB0, 0xF7, 0xDA, 0x0E, 0x64, 0x52, 0xC8, 0x10, 0xF3, 0x2B, 0x80, 0x90, 0x79, 0xE5, 
                         0x62, 0xF8, 0xEA, 0xD2, 0x52, 0x2C, 0x6B, 0x7B },
    .txt = (uint8_t[]) { 0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A,
                         0xAE, 0x2D, 0x8A, 0x57, 0x1E, 0x03, 0xAC, 0x9C, 0x9E, 0xB7, 0x6F, 0xAC, 0x45, 0xAF, 0x8E, 0x51,
                         0x30, 0xC8, 0x1C, 0x46, 0xA3, 0x5C, 0xE4, 0x11, 0xE5, 0xFB, 0xC1, 0x19, 0x1A, 0x0A, 0x52, 0xEF,
                         0xF6, 0x9F, 0x24, 0x45, 0xDF, 0x4F, 0x9B, 0x17, 0xAD, 0x2B, 0x41, 0x7B, 0xE6, 0x6C, 0x37, 0x10 },
    .tag = (uint8_t[]) { 0xA1, 0xD5, 0xDF, 0x0E, 0xED, 0x79, 0x0F, 0x79, 0x4D, 0x77, 0x58, 0x96, 0x59, 0xF3, 0x9A, 0x11 }},

  { .mlen = 0, .keysize = 256,
    .key = (uint8_t[]) { 0x60, 0x3D, 0xEB, 0x10, 0x15, 0xCA, 0x71, 0xBE, 0x2B, 0x73, 0xAE, 0xF0, 0x85, 0x7D, 0x77, 0x81,
                         0x1F, 0x35, 0x2C, 0x07, 0x3B, 0x61, 0x08, 0xD7, 0x2D, 0x98, 0x10, 0xA3, 0x09, 0x14, 0xDF, 0xF4 },
    .tag = (uint8_t[]) { 0x02, 0x89, 0x62, 0xF6, 0x1B, 0x7B, 0xF8, 0x9E, 0xFC, 0x6B, 0x55, 0x1F, 0x46, 0x67, 0xD9, 0x83 }},

  { .mlen = 16, .keysize = 256,
    .key = (uint8_t[]) { 0x60, 0x3D, 0xEB, 0x10, 0x15, 0xCA, 0x71, 0xBE, 0x2B, 0x73, 0xAE, 0xF0, 0x85, 0x7D, 0x77, 0x81,
                         0x1F, 0x35, 0x2C, 0x07, 0x3B, 0x61, 0x08, 0xD7, 0x2D, 0x98, 0x10, 0xA3, 0x09, 0x14, 0xDF, 0xF4 },
    .txt = (uint8_t[]) { 0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A },
    .tag = (uint8_t[]) { 0x28, 0xA7, 0x02, 0x3F, 0x45, 0x2E, 0x8F, 0x82, 0xBD, 0x4B, 0xF2, 0x8D, 0x8C, 0x37, 0xC3, 0x5C }},

  { .mlen = 20, .keysize = 256,
    .key = (uint8_t[]) { 0x60, 0x3D, 0xEB, 0x10, 0x15, 0xCA, 0x71, 0xBE, 0x2B, 0x73, 0xAE, 0xF0, 0x85, 0x7D, 0x77, 0x81,
                         0x1F, 0x35, 0x2C, 0x07, 0x3B, 0x61, 0x08, 0xD7, 0x2D, 0x98, 0x10, 0xA3, 0x09, 0x14, 0xDF, 0xF4 },
    .txt = (uint8_t[]) { 0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A, 
                         0xAE, 0x2D, 0x8A, 0x57 },
    .tag = (uint8_t[]) { 0x15, 0x67, 0x27, 0xDC, 0x08, 0x78, 0x94, 0x4A, 0x02, 0x3C, 0x1F, 0xE0, 0x3B, 0xAD, 0x6D, 0x93 }},

  { .mlen = 64, .keysize = 256,
    .key = (uint8_t[]) { 0x60, 0x3D, 0xEB, 0x10, 0x15, 0xCA, 0x71, 0xBE, 0x2B, 0x73, 0xAE, 0xF0, 0x85, 0x7D, 0x77, 0x81,
                         0x1F, 0x35, 0x2C, 0x07, 0x3B, 0x61, 0x08, 0xD7, 0x2D, 0x98, 0x10, 0xA3, 0x09, 0x14, 0xDF, 0xF4 },
    .txt = (uint8_t[]) { 0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A,
                         0xAE, 0x2D, 0x8A, 0x57, 0x1E, 0x03, 0xAC, 0x9C, 0x9E, 0xB7, 0x6F, 0xAC, 0x45, 0xAF, 0x8E, 0x51,
                         0x30, 0xC8, 0x1C, 0x46, 0xA3, 0x5C, 0xE4, 0x11, 0xE5, 0xFB, 0xC1, 0x19, 0x1A, 0x0A, 0x52, 0xEF,
                         0xF6, 0x9F, 0x24, 0x45, 0xDF, 0x4F, 0x9B, 0x17, 0xAD, 0x2B, 0x41, 0x7B, 0xE6, 0x6C, 0x37, 0x10 },
    .tag = (uint8_t[]) { 0xE1, 0x99, 0x21, 0x90, 0x54, 0x9F, 0x6E, 0xD5, 0x69, 0x6A, 0x2C, 0x05, 0x6C, 0x31, 0x54, 0x10 }},

  { .mlen = 95, .keysize = 128,
    .key = (uint8_t[]) { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C },
    .txt = (uint8_t[]) { "Now is the time, for all good people to come to the aid of humanity. Rage against the nonsense."},
    .tag = (uint8_t[]) { 0x93, 0x63, 0x97, 0x97, 0xa9, 0x6c, 0x36, 0x2e, 0x20, 0x1a, 0xf0, 0x08, 0x34, 0x3b, 0x57, 0xc8 }},
  
  { .key = NULL },

};

int main(int argc, char * argv[]) {

  struct {
    ECBCtx_t  ECB;
    uint8_t   RKey[240];                                    // Large enough for the largest key.
  } ECBTest;

  ECBCtx_t *   ecb = & ECBTest.ECB;                         // Shorthand.
  ECB_TC_t *   etc;
  AESTypeNum_t t = AES_128;                                 // Avoid uninitialized warning when -DNDEBUG defined.
  uint32_t     i;
  
  for (i = 0, etc = ECB_TC; etc->key; etc++, i++) {         // Run ECB encrypt/decrypt testcases.
    switch (etc->keysize) {
      case 128: t = AES_128; break;
      case 192: t = AES_192; break;
      case 256: t = AES_256; break;
      default: assert(0);
    }

    yaes_ecb_init(ecb, etc->key, t);
    memcpy(ecb->state, etc->txt, sizeof(etc->txt));         // Load context.
    
    yaes_ecb_encrypt(ecb);
    assert(! memcmp(ecb->state, etc->ciphertxt, sizeof(etc->txt)));

    yaes_ecb_decrypt(ecb);
    assert(! memcmp(ecb->state, etc->txt, sizeof(etc->txt)));

    memset(ecb->state, 0x00, sizeof(ecb->state));           // Clear for side channel leakage prevention.
  }

  printf("Done %u ECB tests.\n", i);

  struct {
    AES_CMAC_t CMAC;
    uint8_t    RKey[240];                                  // Large enough for the largest key.
  } CMACTest;

  const CMAC_TC_t * ctc;

  ecb = & CMACTest.CMAC.ECBCtx;

  for (i = 0, ctc = CMAC_TC; ctc->key; ctc++, i++) {
    switch (ctc->keysize) {
      case 128: t = AES_128; break;
      case 192: t = AES_192; break;
      case 256: t = AES_256; break;
      default: assert(0);
    }

    yaes_cmac_init(& CMACTest.CMAC, ctc->key, t);

    yaes_cmac_finish(& CMACTest.CMAC, ctc->txt, ctc->mlen);

    assert(! memcmp(ecb->state, ctc->tag, sizeof(ecb->state)));

    memset(ecb->state, 0x00, sizeof(ecb->state));           // Clear for side channel leakage prevention.
  }

  printf("Done %u AES CMAC tests.\n", i);
    
  return 0;
  
}
