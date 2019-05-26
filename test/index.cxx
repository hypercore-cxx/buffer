#include "../index.hxx"
#include "../deps/heapwolf/cxx-tap/index.hxx"

#include <iostream>
#include <sstream>

int main() {
  using namespace Hyper::Util;

  TAP::Test t;

  t.test("sanity", [](auto t) {
    t->ok(true, "true is true");
    t->end();
  });

  t.test("constructor Buffer(n)", [](auto t) {
    Buffer<uint8_t> buf(3);
    t->equal(buf.length(), 3, "constructed with size");
    t->end();
  });

  t.test("constructor Buffer({ el, ... })", [](auto t) {
    Buffer<uint8_t> buf({ 0xFF, 0xF1 });
    t->equal(buf.length(), 2, "constructed with literal");
    t->end();
  });

  t.test("constructor Buffer(string)", [](auto t) {
    Buffer<uint8_t> buf("sup");
    t->equal(buf.length(), 3, "constructed with string");
    t->end();
  });

  t.test("construct from another instance", [](auto t) {
    Buffer<uint8_t> a("sup");
    Buffer<uint8_t> b(a);
    t->equal(b.length(), 3, "constructed from another instance");
    t->end();
  });

  t.test("element access by index", [](auto t) {
    Buffer<uint8_t> buf({ 0xFF, 0xF1 });
    t->equal(buf[0], 0xFF, "index[0]");
    t->equal(buf[1], 0xF1, "index[1]");
    t->end();
  });

  t.test("toString() method", [](auto t) {
    Buffer<uint8_t> buf("sup");
    t->equal(buf.toString(), "sup", "expected stringified value");
    t->end();
  });

  t.test("concat() method", [](auto t) {
    Buffer<uint8_t> a({ 0x01, 0x02 });
    Buffer<uint8_t> b({ 0x03, 0x04 });
    Buffer<uint8_t> c({ 0x05, 0x06 });

    auto d = a.concat({ b, c });

    t->equal(d.length(), 6, "length is equal");
    t->equal(d[2], 0x03, "values are in order");
    t->equal(d[5], 0x06, "values are in order");

    std::cout << std::endl << "# " << d;

    t->end();
  });

  t.test("ostream overload", [](auto t) {
    Buffer<uint8_t> a({ 0xf1, 0x02 });

    std::stringstream ss;
    ss << a;

    t->equal(ss.str(), "<Buffer f1 02>", "ostream output correct");
    t->end();
  });

  t.test("ostream empty buffer", [](auto t) {
    Buffer<uint8_t> a({});

    std::stringstream ss;
    ss << a;

    t->equal(ss.str(), "", "string is empty");
    t->end();
  });

  t.test("a.copy(b) where b is zero bytes", [](auto t) {
    Buffer<uint8_t> a({ 0x00, 0x01, 0x02, 0xff });
    Buffer<uint8_t> b;
    a.copy(b);
    t->equal(b.length(), 0, "target is zero bytes");
    t->end();
  });

  t.test("compare buffers", [](auto t) {
    Buffer<uint8_t> a({ 0x00, 0x01, 0xff, 0x00 });
    Buffer<uint8_t> b({ 0x00, 0x01, 0xff, 0x00 });
    Buffer<uint8_t> c({ 0x02, 0x00, 0xff, 0x00 });
    t->ok(a == b, "buffers are the same");
    t->ok(a != c, "buffers are not the same");
    t->end();
  });

  t.test("a.copy(b)", [](auto t) {
    Buffer<uint8_t> a({ 0x00, 0x01, 0x02, 0xff });
    Buffer<uint8_t> b(4);
    a.copy(b);
    t->equal(a[0], b[0], "values at index[0] are equal");
    t->equal(a[1], b[1], "values at index[1] are equal");
    t->equal(a[2], b[2], "values at index[2] are equal");
    t->equal(a[3], b[3], "values at index[3] are equal");
    t->equal(a.length(), b.length(), "values are the same size");
    t->end();
  });

  t.test("copy from A to B start at target offset", [](auto t) {
    Buffer<uint8_t> a({ 0x01, 0x02, 0x03, 0x04, 0x05 });
    Buffer<uint8_t> b(6);
    a.copy(b, 1);

    std::cout << std::endl << "# " << a;
    std::cout << std::endl << "# " << b;

    t->equal(b[0], 0, "values at index[0] are equal");
    t->equal(b[1], 1, "values at index[1] are equal");
    t->equal(b.length(), 6, "values are the same size");
    t->end();
  });

  t.test("copy from a to b, target offset and source offset", [](auto t) {
    Buffer<uint8_t> a({ 01, 02, 03, 04, 05, 06, 07, 0xFF });
    Buffer<uint8_t> b({ 00, 00, 00, 00, 00, 00 });
    t->equal(a.copy(b, 3, 2), 3, "correct number of bytes written");

    std::cout << std::endl << "# " << a;
    std::cout << std::endl << "# " << b;

    t->equal(b[0], 0, "value at index[0] is correct");
    t->equal(b[1], 0, "value at index[1] is correct");
    t->equal(b[2], 0, "value at index[2] is correct");
    t->equal(b[3], 3, "value at index[3] is correct");
    t->equal(b[4], 4, "value at index[4] is correct");
    t->equal(b[5], 5, "value at index[5] is correct");
    t->equal(b.length(), 6, "length is correct");
    t->end();
  });

  t.test("slice", [](auto t) {
    Buffer<uint8_t> a({ 0x01, 0x02, 0x03, 0x04 });
    auto sliceA = a.slice(0, 2);
    t->equal(sliceA[0], 1, "value at index[0] is correct");
    t->equal(sliceA[1], 2, "value at index[1] is correct");
    t->equal(sliceA.length(), 2, "new buffer is correct length");

    std::cout << std::endl << "# " << sliceA;

    auto sliceB = a.slice(2, 4);
    t->equal(sliceB[0], 3, "value at index[0] is correct");
    t->equal(sliceB[1], 4, "value at index[1] is correct");
    t->equal(sliceB.length(), 2, "new buffer is correct length");

    std::cout << std::endl << "# " << sliceB;

    auto sliceC = a.slice(2);

    std::cout << std::endl << "# " << sliceC;

    t->ok(sliceB == sliceC, "buffers are the same");
    t->ok(sliceA != sliceC, "buffers are the same");
    t->end();
  });

}
