// Copyright 2024 Steven Buytaert

#ifndef YAES_H
#define YAES_H

#include <stdint.h>

typedef enum {
  AES_128         = 0,
  AES_192         = 1,
  AES_256         = 2,
} AESTypeNum_t;

typedef const struct AESType_t {
  struct {
    uint16_t      bits;           // Keysize in bits: 128, 192 or 256.
    uint8_t       bytes;          // Keysize in bytes.
    uint8_t       expanded;       // Expanded key size in bytes.
  } Size;
  uint8_t         Nr;             // Number of rounds; same name as in spec.
  uint8_t         Nk;             // Number of 32 bit words in a (single) roundkey.
} AESType_t;

extern AESType_t AESTypes[3];

typedef union ECBCol_t {
  uint8_t         u08[4];
  uint32_t        u32;
} ECBCol_t;

typedef struct ECBCtx_t {
  union {
    uint8_t       state[16];
    ECBCol_t      Col[4];
  };
  uint8_t         type;           // One of the AESTypeNum_t.
  uint8_t         numun;          // Used by AES CMAC; number of unprocessed bytes.
  uint8_t         pad[6];
  uint8_t         RKey[0];        // Roundkey space; size == ECBType.Size.expanded (176 / 208 / 240).
} ECBCtx_t;

typedef struct AES_CMAC_t {       // RFC 4493 Context; note that the tag is ECBCtx.state after finish.
  uint8_t         K1[16];
  uint8_t         K2[16];
  uint8_t         un[16];         // Unprocessed block bytes.
  ECBCtx_t        ECBCtx;
} AES_CMAC_t;

uint32_t yaes_init_ECB(ECBCtx_t * ctx, const uint8_t key[], AESTypeNum_t type);
void     yaes_encrypt_ECB(ECBCtx_t * ctx);
void     yaes_decrypt_ECB(ECBCtx_t * ctx);

uint32_t yaes_init_CMAC(AES_CMAC_t * ctx, const uint8_t key[], AESTypeNum_t type);
void     yaes_cmac_feed(AES_CMAC_t * ctx, const uint8_t msg[], uint32_t size);
void     yaes_cmac_finish(AES_CMAC_t * ctx, const uint8_t msg[], uint32_t size);

#endif // YAES_H
