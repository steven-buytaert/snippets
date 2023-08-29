# snippets
Container repo with very simple code snippets. All code should compile
cleanly with either clang or gcc. Dependencies have been kept to a minimum;
only standard C library functions are required (except for the gnu modular
make snippet which is kind of obvious).

* circbuffer: circular buffer that allows 1 reader and 1 writer to operate
  on it, lock free.

* uintxx: customizable width unsigned integer operations, multiply, divide,
  shift left and right.

* ecvt-strtod: currently only ecvt implementation. Convert a 64 or 32 bit IEEE754
  double into character digits; rounding to nearest even or not, can be
  selected. For scaling up and down, it makes use of the uintxx code
  mentioned above. There's some sample code.

* customizable-printf: printf where one can add new format characters and
  the code how they should be rendered. Has implementations for most format
  characters of printf. There's a bit of sample code to show its usage. For
  the double formatting (e, g, f) it makes use of the ecvt implementation
  above.

* gnu-4.x-modular-make: implementation of a sample loadable module for GNU
  Make 4.x. It allows subfolders with implementation code for libraries.
  Applications and libraries can express imports they need and they will be
  properly handled by the code. There's not more documentation for the
  moment. Contact me if you are interested.

* snset: a simple implementation of an object allocator; if you need to
  create a set of objects that have a dynamic tail end and you want
  the result to be a single block of memory (e.g. for memory efficiency reasons),
  this will help in creating that block of objects. There is sample
  code showing how to use the set.

* hexdump: create a canonical hexdump in a given character buffer. Always
  terminate the dump, even if the buffer is too small.

* delta-timers: implement a multitude of timers by means of a linked list
  with each timer/element having a delta relative timeout wrt the previous timer
  in the list, so when evaluating the timeout, only the first element/timer needs
  to be checked. There is a small snippet of sample code, using a pthread as
  a timer ticker thread.

  ```C
  clang -Os -Wall -I . *.c -o sample -lpthread
  ./sample # run until ctrl-c
  ```

* txt-tr-utils: a small set of in place text transformation functions. They
  are all very simple. None of them implement any checking for the buffer
  size being large enough.

* t2c-types: a set of utilities to create, manipulate and analyze structure
  types, union types, enums and bitsets. Could prove useful in code generation
  tools. Can cluster fields that are governed by the same member for size,
  together. Can optimize a type to avoid padding as much as possible, by
  reordering members. No sample code.

* fb2: the very early start of a flatbuffer schema parser. It parses a
  flatbuffer schema into an internal format that can be used for further
  processing (tool creation, code generation, ...). Uses the above mentioned
  snset for processing and generating the internal schema. Requires
  flex (>= 2.6.4) and bison (>= 3.6.0) for scanner and parser generation.
  The code generation part will create the control structures necessary for
  walking over and creating a flatbuffer from a graph. No sample code or
  documentation (yet).

* avalanche: a hash avalanche test. The sample code uses the avalanche test
  to compare 3 different hashes; murmur3, lookup3 and a buzhash added for
  comparison. The murmur3 and lookup3 source code files contain the original
  copyrights. Only the lookup3 source code file was slightly modified so it
  could be compiled and linked into the sample. The test can use a
  deterministic mean of flipping the bits or a random mode; ensure the test
  count is high enough for statistical relevance, in the random mode. RNG
  seeding is not done by the test and needs to be added manually, if needed.
  Compile sample with
  ```C
  clang -I . -Wall -Os -o sample sample.c lookup3.c sbtbuz2.c murmurhash.c avalanche.c
  ```
  and run ./sample.

* sbt-utils: implementations for strlcat and strlcpy based upon the
  canonical paper found [here](https://www.millert.dev/papers/strlcpy.html).
  The other function included is resolvepath which is essentially an implementation of
  the Posix realpath function but not relying on the PATH_MAX system limit
  but where the buffersize is passed as an argument.
  [This paper](https://eklitzke.org/path-max-is-tricky) arguments why using
  PATH_MAX is a wrong approach anyway.
  Compile sample with
  ```C
  clang -Wall -Werror -Os -I ./ -o sample sample.c resolvepath.c strlcpy-cat.c
  ```

* bluetooth: a header file that embodies the Bluetooth 5.3 Host Controller
  Interface specification commands and events that can be exchanged. The
  result of some experimenting with regular expressions on the specification
  text. The parser itself is to kludgy to release but the result could be
  useful for people that want to create a code generator for
  encoding/decoding HCI messages or test pattern generation, debugging, etc.

* u-locks: some proof of concept code of micro locking a linked list and
  manipulating it with several mutator threads. Uses optimistic
  synchronization and atomic operations to achieve multi-threaded
  correctness. Single Makefile and single source file; just a proof of
  concept. There's more documention in the u-locks.md file.
