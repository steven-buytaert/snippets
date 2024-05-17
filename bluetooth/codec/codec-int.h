#ifndef HCI_CODEC_INT_H
#define HCI_CODEC_INT_H

// Copyright 2024 Steven Buytaert

// This file contains internal codec type declarations; it is a separate
// header file since it is also included during code generation. The code
// generator build step will define the GENERATED preprocessor macro.

#if defined(GENERATE)
#define CONST
#else
#define CONST const               // At runtime most are read only.
#endif

#include <hci-codec.h>

typedef const union CoI_t * codec_t;

typedef union CoI_t {             // Copy or Instruction.
  uint8_t           code;
  uint8_t           MSB;          // The MSB for a largecopy instruction.
  uint8_t           LSB;          // The LSB for a largecopy instruction.
  struct {
    uint8_t         num    : 4;   // Number of bytes to copy.
    uint8_t         skip   : 3;   // Number of bytes to skip dst for alignment *after* the copy.
    uint8_t         nainst : 1;   // 0 for copy operations.
  };
  struct {
    uint8_t         action : 3;   // See below for CoI_Action enumeration.
    uint8_t         arg    : 4;   // Argument if relevant.
    uint8_t         inst   : 1;   // 1 for actions to take; see CoI_Action_t.
  };
} CoI_t;

typedef enum {                    // See the switch/case in de encoder and decoder.
  inlinefix         = 0b000,      // arg = byte width.
  laterfix          = 0b001,      // arg = byte width.
  loop              = 0b010,      // arg = byte width.
  endloop           = 0b011,
  modif             = 0b100,      // modifier; argument specifies the modification.
  copyws            = 0b101,      // arg now is skip count; read next 2 bytes for the size.
  RFU               = 0b111,      // Reserved as escape; for future use.
} CoI_Action_t;

typedef CONST struct CodecTab_t * ctab_t;

typedef struct TE_t {             // Table Element.
  union {
    uint8_t         OCF;          // For a command or return structure.
    struct {
      uint8_t       code   : 7;   // issub == 0, event code, otherwise subevent code.
      uint8_t       issub  : 1;   // 1 for a subevent; implies 0x3e for event code.
    };
  };
  uint8_t           coistart;     // Offset in the respective CoI table.
  uint8_t           numcoi : 4;   // Number of table entries for encoding/decoding this type.
  uint8_t           isret  : 1;   // 1 for a return type, 0 for a command.
  uint8_t           rfu    : 3;   // Reserved for future use.
#if defined(GENERATE)
  const char *      section;
  const char *      name;
  ctab_t            ctab;         // The table the entry was added to.
#endif
} TE_t;

typedef CONST TE_t * te_t;

typedef CONST struct CodecTab_t {
  uint16_t          numtab;       // Number of table entries.
  uint16_t          numcoi;       // Number of CoI entries.
  CONST TE_t *      table;        // Table with sorted entries.
  CONST CoI_t *     CoI;          // Table with codec instructions.
#if defined(GENERATE)
  const char *      name;
#endif
} CodecTab_t;

extern const ctab_t ctabs[9];     // Defined in generated hci-tables.c file.

typedef int32_t (* cmpte_t)(te_t a, te_t b);

inline static int32_t cmpevt(te_t a, te_t b) {

  int32_t cm = a->code - b->code;
  
  if (0 == cm) { cm = a->issub - b->issub; }
  
  return cm;

}

inline static int32_t cmpcmd(te_t a, te_t b) {

  int32_t cm = a->OCF - b->OCF;

  if (0 == cm) { cm = a->isret - b->isret; }                // Same OCF, compare isret flags.
  
  return cm;

}

#endif // HCI_CODEC_INT_H
