# Micro Memory Manager

This micro manager implements a system to manage a block of given
memory so that one can allocate blocks of memory from it and release the
blocks back later, like regular malloc and free.

There are some restrictions and other differences versus other memory
manager systems like malloc/free.

* It allows multi threaded operation by implementing micro locks in each
  chunk separately. I.e. it does not require an overall mutex and is
  therefore not depending on any OS support. It does require a compiler that
  implements the GCC atomic operation primitives.

* It provides 2 different flavors for allocation and releasing of memory;
  an uncontended path, for calling e.g. from an IRQ where you never want a
  deadlock, and a slower path, that will lock subsequent chunks for merging
  and/or splitting.

* It implements a chunk iterator system, where one can walk over all the
  chunks and inspect the contents and the tag of each block.

* During allocation, it allows the block of memory to be given a set of tag
  bits, that can be used to identify the block of memory. In combination
  with an iteration mechanism, this allows for e.g. a garbage collection
  system.

* Since some bits are being used for tags and other household information,
  only 21 bits are available for the size of a chunk. Currently, the size is
  encoded as number of bytes of the chunk, but could easily be changed into
  number of 8 byte units.

There are a lot of assert statements in the code to check that the required
invariant conditions remain honoured. Some assert statements also contain a
comma operator, with the last operand being a 1, to satisfy the assert
statement and where the previous comma operands perform an operation that
enhances the checking for errors; e.g.

```C
assert((succ->header = 0, 1));  // Clear when debugging; succ no longer exists.
```

To build and run the extremely simple multithreaded sample application:

```
$ gcc -Wall -Wconversion -Wextra -Wpadded -Werror -I . -o sample sample.c umem.c -lpthread
$ ./sample -n 10 # Run with 10 mutator threads.
```

NOTE: one can also use clang for building the sample or add any
optimization flags to the compiler command line.

The sample application will just run the given number of threads (default is
2) in parallel, allocating and releasing memory, randomly, until control-c
is pressed. Every second the number of bytes in use per thread, and in
total, is shown, as well as the number of chunks currently in the memory
manager and the contention rate per second, on the memory manager.

The sample application will perform some simple checks to see if the block
contents have been tampered with, before releasing the block. The simple
thread id, allocated at the start, is used as the tags content for each
block and is also checked upon.

Every 16 seconds of operation, the monitor thread will force all mutator
threads to only use the slow release; that means that all chunks should
again coalesce into the single starting chunk.

TODO: realloc like implementation

TODO: reduce contention by not always starting with the same chunk.
