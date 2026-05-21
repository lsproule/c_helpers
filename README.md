# helpers

Single-header C helpers. Dynamic arrays, strings, file reading. Just to make life easier.

## Install

```sh
wget https://raw.githubusercontent.com/lsproule/c_helpers/main/helpers.h
```

Drop it next to your code and include it.

## Macros

```c
#define HELPERS_IMPLEMENTATION  // pull in function bodies (one TU)
#define HELPERS_STRIP_PREFIX    // short names: arr_append vs helpers_arr_append
#include "helpers.h"
```

Without `HELPERS_IMPLEMENTATION` the implementation isn't included.

## Docs

Coming soon.
