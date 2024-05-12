# Bluetooth HCI Specifications as source code

The files here are a result of some experimenting with regular expression
parsing the whole of the Bluetooth Core Specification. 

The version that was generated from version 5.3 of the specification, was
called hci-spec2src.h and has now been renamed hci-spec2src-5.3.h.

A new version has been generated from version 5.4 of the specification. The
structure of this new version has been changed, so that any unclarities with
respect to versioned command and event versions should now be gone. Each
command or event has an array of instances; each instance referring to a
version of the command or event, with all parameter and return fields, if
applicable, for that version.

So unversioned commands and events have a single instance, versioned
commands and events have multiple instances. Also in this 5.4 version, the
OGF, if applicable is part of the data.

These files could be useful for generating encoding or decoding functions, or
for the generation of the HCI packets as structs in header files. The only
limit is your imagination.

The Bluetooth Core Specification, version 5.4, was downloaded from [the
Bluetooth 5.4 website here](https://www.bluetooth.com/specifications/specs/core-specification-5-4/).

The text that was parsed, is generated from the downloaded core specification as
follows.

```bash
$ pdftotext -layout -enc ASCII7 -colspacing 0.6 -nopgbrk Core_v5.4.pdf
```

The generated Core_v5.4.txt file was then modified slightly, to account for
some errors in it. E.g. some commas were forgotten in the parameter field
descriptions and some fields changed name between the preamble and the
description section; e.g 'Tx_Power' in the preamble was referred to as
'TX_Power' in the description or vice versa. The parser would flag these as
warnings and they were manually fixed in the text, ensuring that the meaning of
the text was not changed.

Even then, this code is provided 'as is'; there is no guarantee that the
parsing did not introduce any errors.

As example how a generated header file, generated from the specification
source could look like, the file hci-types-5.4.h is included. It was
generated from the hci-spec2src-5.4.h file, with some help of the t2c-types
types and functions found elsewhere in this repo. The code for this
generator itself, is not in the repo (yet).

## HCI Packet Encoder and Decoder

In the folder codec, there is a packet encoder and decoder implementation,
still under construction (e.g. the count modifier is not yet implemented).
The encoder/decoder uses tables that are generated from the types in
hci-types.h that are themselves generated from the file hci-spec2src.h that
is generated from the Bluetooth specification text itself.

So the steps taken are

1. Generate hci-spec2src-5.4.h from Bluetooth Core Specification 5.4.
2. Generate hci-types-5.4.h from hci-spec2src-5.4.h.
3. Generate codec/hci-tables.c from hci-spec2src-5.4.h.

The file hci-tables.c contains the simple instructions to encode
and decode the packets from/to the C structures that are defined in the
hci-types-5.4.h header file.

The encoder/decoder (currently) is only for the 5.4 Bluetooth specification.

There is no sample code (yet) to demonstrate the use of the encoder/decoder
but the public API that is in hci-codec.h is pretty straightforward; one
structure and 2 API functions.

All the other
files contain internals only and some stuff used during the generation of
the tables themselves.
