#ifndef NODEUV_BUFFER_H
#define NODEUV_BUFFER_H

#define NODEUV_MIN(a, b) ((a) < (b) ? (a) : (b))

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <functional>

extern "C" {
#include "uv.h"
}

namespace nodeuv {

  using namespace std;

  class Buffer {

    public:

      uv_buf_t data;

      string toString();
      int length();
      int copy(Buffer b);
      int copy(Buffer b, int target_start);
      int copy(Buffer b, int target_start, int source_start);
      int copy(Buffer b, int target_start, int source_start, int source_end);

      Buffer();
      Buffer(int size);
      Buffer(char* str);
      Buffer(string str);
      Buffer(const Buffer &buf);
      Buffer& operator= (const Buffer &buf);
  };
}

#endif

