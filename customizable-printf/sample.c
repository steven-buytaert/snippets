// Copyright 2020 Steven Buytaert.

#include <stdint.h>
#include <unistd.h>

void myprintf(const char * fmt, ...);                       // In sample-print.c

int main(int argc, char * argv[]) {

  uint32_t keyvalue = (100 << 3) | 3;                       // Table 3 entry 100.

  myprintf("Hello %s. Key %k\n", "world", keyvalue);

  myprintf("Binary %032b\n", keyvalue);
  
  myprintf("Planck length %17e\n", 1.61622837e-35);

  return 0;

}
