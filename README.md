# SYNOPSIS
A little sugar on top of `uv_buf_t`. This should get moved out to
another module called `nodeuv-buffer`.

# USAGE

## AS A MODULE USING `BALE`.
```cpp
import Buffer "buffer"
Buffer buf(10);
```

## AS A REGLUAR C++ LIBRARY.
Build with `buffer.cc` as a dependency and include the header file.

```cpp
#include "buffer.h"
Buffer buf(10);
```

# API

## CONSTRUCTOR
Various ways to construct a buffer.

### Buffer buf();
### Buffer buf(size);
### Buffer buf(string data);
### Buffer buf(char* data);

## INSTANCE METHODS

### `string` buf.toString();
Convert the buffer to a string.

### `int` buf.length();
The length of the buffer.

### `void` buf.copy(Buffer targetBuf[, int targetStart][, int sourceStart][, int sourceEnd]);
Copy a buffer into another one.

- `targetBuf` a buffer to copy from.
- `targetStart` an optional `int`, defaults to `0`.
- `sourceStart` an optional `int`, defaults to `0`.
- `sourceEnd` an optional `int`, defaults to the srouce buffer's length.

Copies data from a region of this buffer to a region in the target buffer
even if the target memory region overlaps with the source. If undefined
the targetStart and sourceStart parameters default to 0 while sourceEnd 
defaults to the source buffer's `buffer.length`.

## INSTANCE MEMBERS

### buf.data 
The current value of the buffer.

