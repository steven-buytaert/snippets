# snippets
Container repo with very simple code snippets.

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