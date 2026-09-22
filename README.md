# lib50

Utilities for ENGS 50

## Build

```sh
make # build lib50.a
make test # build and run the unit tests
```

## Use

```sh
gcc -std=c11 -Wall -pedantic -I/path/to/lib50/include -o prog prog.c /path/to/lib50/lib50.a
```

```c
#include "lib50/<file>.h"
```
