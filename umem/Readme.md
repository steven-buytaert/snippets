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

TODO: small pthread sample application.

TODO: realloc like implementation

TODO: reduce contention by not always starting with the same chunk.
