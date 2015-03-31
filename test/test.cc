#include "../buffer.h"
#include <istream>

using namespace nodeuv;

#define ASSERT(message, ...) do { \
    if(!(__VA_ARGS__)) { \
          std::cerr << "FAIL: " << message << std::endl; \
        } \
    else { \
          std::cout << "OK: " << message << std::endl; \
        } \
} while(0);

int main() {

  //
  // sanity test.
  //
  ASSERT("sanity: true is false", true == false);
  ASSERT("sanity: true is true", true == true);



}

