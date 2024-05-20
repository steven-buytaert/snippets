// Copyright 2024 Steven Buytaert

// To build 'gcc -o spec2yaml -Wall spec2yaml.c -I .'

/*

  This is a small sample on how to convert the generated specification source header
  into a YAML file. Not all information is dumped into the YAML file, but the code is
  straightforward to add whatever information you would need in the YAML file.

*/

#include <stdio.h>
#include <string.h>

#define INCLUDE_SPECS

#include <spec2src.h>

#define NUM(A) (sizeof(A) / sizeof(A[0]))

typedef HCI_t *       hci_t;
typedef HCI_Inst_t *  inst_t;
typedef HCI_Field_t * fld_t;
typedef HCI_Event_t * evt_t;

static void clean(char buf[]) {

  for (char * c = buf; *c; c++) {
    if (*c == ' ') { *c = '_'; }
    if (*c == '[') { *c = '_'; }
    if (*c == ']') { *c = '_'; }
    if (*c == '-') { *c = '_'; }
  }

}

int main(int argc, char * argv[]) {

  uint32_t i;
  uint32_t x;
  uint32_t z;
  hci_t    hci;
  inst_t   inst;
  fld_t    fld;
  evt_t    evt;
  char     name[128];

  printf("---\n");

  for (i = 0, hci = HCITable; i < NUM(HCITable); i++, hci++) {
    printf("- section: \"%s\"\n", hci->section);
    printf("  title: \"%s\"\n", hci->title);
    printf("  iscmd: %s\n", hci->iscmd ? "true" : "false");
    if (hci->iscmd) {
      printf("  OGF: %u\n", hci->OGF);
    }
    for (x = 0, inst = hci->Inst; x < hci->numinst; x++, inst++) {
      memset(name, 0x00, sizeof(name));
      strcpy(name, inst->name);
      clean(name);
      printf("  %s:\n", name);
      if (hci->iscmd) {
        printf("    OCF: %u\n", inst->OCF);
      }
      else {
        printf("    code: %u\n", inst->code);
      }

      if (inst->CoE) {
        printf("    %s_%s:\n", name, hci->iscmd ? "Cmd" : "Evt");
        for (z = 0, fld = inst->CoE; z < inst->numcoe; z++, fld++) {
          printf("      - %s: %u\n", fld->name, fld->octets ? fld->octets : 1);
          if (fld->ref2size) {
            printf("        ref2size: %s\n", fld->ref2size);
          }
          if (! strcmp("Subevent_Code", fld->name)) {
            printf("        value: %d\n", inst->subevent);
          }
        }
      }

      if (inst->Ret) {
        printf("    %s_Ret:\n", name);
        for (z = 0, fld = inst->Ret; z < inst->numret; z++, fld++) {
          printf("      - %s: %u\n", fld->name, fld->octets ? fld->octets : 1);
          if (fld->ref2size) {
            printf("        ref2size: %s\n", fld->ref2size);
          }
        }
      }

      if (hci->Events) {
        printf("    Events:\n");
        for (z = 0, evt = hci->Events; z < hci->numevts; z++, evt++) {
          printf("      - %s:\n", evt->name);
        }
      }

    }
    if (i + 1 < NUM(HCITable)) { printf("\n"); }           // No newline after last item.
  }
  
  return 0;
  
}
