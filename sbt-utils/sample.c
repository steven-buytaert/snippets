// Copyright 2023 Steven Buytaert

// to build: clang -Wall -Werror -I ./ -o sample sample.c resolvepath.c strlcpy-cat.c 

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <sbt-utils.h>

int main(int argc, char * argv[]) {

  char   buf[128];
  char * status;
  
  status = resolvepath(".", buf, sizeof(buf));

  printf("Current working folder: '%s' error %s.\n", status, strerror(errno));

  status = resolvepath("./non-existing.file", buf, sizeof(buf));

  printf("Non existing file: '%s' error %s.\n", buf, strerror(errno));

  status = resolvepath("../../non-existing-dir/foo", buf, sizeof(buf));

  printf("Non existing dir: '%s' error %s.\n", buf, strerror(errno));
  
  return 0;

}
