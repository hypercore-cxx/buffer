#include "../buffer.h"

namespace nodeuv {

  using namespace std;

  string Buffer::toString() {
    string s;
    s.assign(data.base, data.len);
    return s;
  }

  int Buffer::length() {
    return data.len;
  }

  int Buffer::copy(Buffer b) {
    return this->copy(b, 0, 0, b.length());
  }

  int Buffer::copy(Buffer b, int target_start) {
    return this->copy(b, target_start, 0, b.length());
  }

  int Buffer::copy(Buffer b, int target_start, int source_start) {
    return this->copy(b, target_start, source_start, b.length());
  }

  int Buffer::copy(Buffer b, int target_start, int source_start, int source_end) {

    size_t obj_length = b.length();
    size_t target_length = this->length();
    char* target_data = this->data.base;

    if (target_start >= target_length || source_start >= source_end) {
      return 0;
    }

    if (source_start > obj_length) {
      throw runtime_error("out of range index");
    }

    uint32_t to_copy = NODEUV_MIN(NODEUV_MIN(source_end - source_start,
      target_length - target_start),
      obj_length - source_start);

    memmove(this->data.base + target_start, b.data.base + source_start, to_copy);
    return to_copy;
  }

  Buffer::Buffer() {
  }

  Buffer::Buffer(int size) {
    data.base = (char*) malloc(size);
    data.len = size;
  }

  Buffer::Buffer(string str) {
    data = uv_buf_init((char*) str.c_str(), str.length());
  }

  Buffer::Buffer(char* str) {
    data = uv_buf_init(str, strlen(str));
  }

  Buffer::Buffer(const Buffer &buf) {
    data = buf.data;
  }
}

