# Bluetooth HCI Specifications as source code

The files here are a result of some experimenting with regular expression
parsing the whole of the Bluetooth Core Specification. 

The version that was generated from version 5.3 of the specification, was
called hci-spec2src.h and has now been renamed hci-spec2src-5.3.h.

A new version has been generated from version 5.4 of the specification. The
structure of this new version has been changed, so that any unclarities with
respect to versioned command and event versions should now be gone. Each
command or event as an array of instances; each instance referring to a
version of the command or event, with all parameter and return fields, if
applicable, for that version.

So unversioned commands and events have a single instance, versioned
commands and events have multiple instances. Also in this 5.4 version, the
OGF, if applicable is part of the data.

The data could be useful for generating encoding or decoding functions, or
for the generation of the HCI packets in header files.

The Bluetooth Core Specification, version 5.4, was downloaded from [the
Bluetooth 5.4 website here] 
(https://www.bluetooth.com/specifications/specs/core-specification-5-4/).

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