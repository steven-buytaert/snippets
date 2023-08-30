# Copyright 2023 Steven Buytaert
# Makefile for building the sample code

all: sample

# $@ target
# $< first dependency
# $^ all dependencies
# $? all dependencies more recent than the target

CC      := gcc
CFLAGS  := -ggdb -O0 -Wall -Werror -Wtype-limits -Wsign-compare -Wshadow -Wpointer-arith -Wstrict-prototypes -fstack-protector-all -I.
LDFLAGS := -L .

sample: sample.o sample-print.o ecvt.o uintxx.o libcuxion.a
	$(CC) $(CFLAGS) $(LDFLAGS) sample.o sample-print.o ecvt.o uintxx.o -o $@ -lcuxion

sample.o: sample.c Makefile
	$(CC) $(CFLAGS) -c $<

sample-print.o: sample-print.c Makefile
	$(CC) $(CFLAGS) -c $<

ecvt.o: ../ecvt-strtod/ecvt.c ../ecvt-strtod/ecvt-strtod.h ../ecvt-strtod/internal.h Makefile
	$(CC) $(CFLAGS) -I ../ecvt-strtod -I ../uintxx/ -c $<

uintxx.o: ../uintxx/uintxx.c ../uintxx/uintxx.h Makefile
	$(CC) $(CFLAGS) -I ../uintxx/ -c $<

libcuxion.a: cuxion.o percent_c.o percent_feg.o percent_n.o percent_poxdui.o percent_s.o
	$(AR) rc $@ $?

cuxion.o: cuxion.c cuxion.h Makefile
	$(CC) $(CFLAGS) -c $<

percent_c.o: percent_c.c cuxion.h Makefile
	$(CC) $(CFLAGS) -c $<

percent_feg.o: percent_feg.c cuxion.h Makefile
	$(CC) $(CFLAGS) -I ../ecvt-strtod -DFPSUPPORTED -c $<

percent_n.o: percent_n.c cuxion.h Makefile
	$(CC) $(CFLAGS) -c $<

percent_s.o: percent_s.c cuxion.h Makefile
	$(CC) $(CFLAGS) -c $<

percent_poxdui.o: percent_poxdui.c cuxion.h Makefile
	$(CC) $(CFLAGS) -c $<

clean:
	@rm -f sample libcuxion.a
	@find . -name \*~ -print | xargs rm -rf
	@find . -name \*.o -print | xargs rm -rf
	@find . -name \*.class -print | xargs rm -rf
	@find . -name \*.bak -print | xargs rm -rf

.PHONY: clean
