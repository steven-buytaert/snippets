all: u-locks

# $@ target
# $< first dependency
# $^ all dependencies
# $? all dependencies more recent than the target

CC      := gcc
CFLAGS  := -ggdb -Os -Wall -Werror -Wtype-limits -Wsign-conversion -Wsign-compare -Wshadow -Wpointer-arith -Wstrict-prototypes -fstack-protector-all -I.
LDFLAGS := -lpthread

u-locks: u-locks.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

u-locks.o: u-locks.c Makefile
	$(CC) $(CFLAGS) -c $<

clean:
	@rm -f u-locks u-locks.html
	@find . -name \*~ -print | xargs rm -rf
	@find . -name \*.o -print | xargs rm -rf
	@find . -name \*.class -print | xargs rm -rf
	@find . -name \*.bak -print | xargs rm -rf

.PHONY: clean
