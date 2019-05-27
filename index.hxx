#ifndef HYPER_UTIL_BUFFER_H
#define HYPER_UTIL_BUFFER_H

#include <algorithm>
#include <vector>
#include <initializer_list>
#include <string>
#include <iostream>
#include <iomanip>
#include <exception>

///
/// namespace Hyper
///
namespace Hyper {

  ///
  /// namespace Util
  ///
  namespace Util {

    ///
    /// class Buffer<T>
    ///
    template<typename T>
    class Buffer {
      public:

        ///
        /// property vector<T> value
        /// comment Contains a vector of the type the constructor was specialized with.
        ///
        std::vector<T> value;

        class OutOfRange : public std::exception {
          virtual const char* what() const throw() {
            return "Out Of Range Exception";
          }
        } EX_OUT_OF_RANGE;

        // bool equals(Buffer b);
        // int indexOf(T value, int byteOffset);

        ///
        /// operator =
        /// comment Assign one buffer to another.
        ///
        Buffer& operator= (const Buffer& buf);

        ///
        /// operator ==
        /// comment Determine if two buffers values are equal.
        ///
        bool operator== (const Buffer<T>& rhs) {
          return std::equal(
            std::begin(this->value),
            std::end(this->value),
            std::begin(rhs.value)
          );
        }

        ///
        /// operator !=
        /// comment Determine if two buffers values are not equal.
        ///
        bool operator!= (const Buffer<T>& rhs) {
          return !std::equal(
            std::begin(this->value),
            std::end(this->value),
            std::begin(rhs.value)
          );
        }


        ///
        /// operator <<
        /// comment Allow buffers to interface with streams. The value of the
        /// comment buffer will be represented as hex in a angle-bracket delimited
        /// comment string.
        ///
        friend std::ostream& operator<< (std::ostream &os, const Buffer<T>& buf) {
          if (buf.value.empty()) {
            os << "";
            return os;
          }

          std::ios_base::fmtflags f(std::cout.flags());

          os << "<Buffer" << std::hex << std::setfill('0');

          for (auto const& c : buf.value) {
            os << " " << std::setw(2) << (int) c;
          }

          std::cout.flags(f);

          os << ">";

          return os;
        };

        ///
        /// operator []
        /// comment Provides access of the buffer via index.
        ///
        T operator[] (int i) {
          return this->value[i];
        }

        ///
        /// constructor Buffer(initializer_list<T> list)
        /// comment Constructs a buffer from an initializer list.
        ///
        Buffer(std::initializer_list<T> l) : value(l) {}

        ///
        /// constructor Buffer()
        /// comment Constructs an empty buffer.
        ///
        Buffer() {
          this->value = std::vector<T>(0);
        };

        ///
        /// constructor Buffer(size_t size)
        /// comment Constructs a buffer with a value of `size`.
        ///
        /// param size The size initially allocated for the buffer value.
        ///
        Buffer(size_t size) {
          this->value = std::vector<T>(size);
        };

        ///
        /// constructor Buffer(const string&)
        /// comment Constructs a buffer from a string.
        ///
        Buffer(const std::string& str) {
          this->value = std::vector<uint8_t>(str.begin(), str.end());
        };

        ///
        /// constructor Buffer(const Buffer&)
        /// comment Constructs a buffer from another buffer.
        ///
        Buffer(const Buffer& buf) {
          this->value = buf.value;
        };

        ///
        /// method toString()
        /// return std::string
        /// comment Get the value of of the buffer as a string.
        ///
        std::string toString() {
          std::string str(this->value.begin(), this->value.end());
          return str;
        };

        ///
        /// method length()
        /// return int
        /// comment Get the size of the buffer in by bytes.
        ///
        int length() {
          return this->value.size();
        };

        ///
        /// method concat(initializer_list<T>& list)
        /// comment Concatinate this buffer with a list of buffers.
        ///
        /// param list A list of buffers to be joined.
        ///
        /// return Buffer
        /// comment Returns a new combined buffer.
        ///
        template<typename LT>
        Buffer concat(const std::initializer_list<LT>& list) {
          using std::end;
          using std::begin;

          unsigned size = 0;
          Buffer cpy(size);

          for (auto& buf : list) {
            size += this->value.size() + buf.value.size();

            this->value.reserve(size);
            this->value.insert(end(this->value), begin(buf.value), end(buf.value));
          }

          cpy.value = this->value;
          return cpy;
        };

        ///
        /// method copy(Buffer& target, size_t targetStart, size_t sourceStart, size_t sourceEnd)
        /// comment Copies this buffer (or part of it) into another buffer.
        ///
        /// param target A Buffer to copy into.
        /// param targetStart The offset within target at which to begin writing.
        /// param sourceStart The offset within buf from which to begin copying.
        /// param sourceEnd The offset within buf at which to stop. copying (not inclusive).
        ///
        /// return size_t
        /// comment Returns how many bytes were written to this buffer.
        ///
        size_t copy (Buffer& buf, size_t targetStart, size_t sourceStart, size_t sourceEnd) {
          using std::end;
          using std::begin;

          auto& source = this->value;
          auto& target = buf.value;

          if (targetStart >= target.size() || sourceStart >= sourceEnd) {
            return 0;
          }

          if (sourceStart > target.size()) {
            throw EX_OUT_OF_RANGE;
          }

          if (sourceEnd - sourceStart > target.size() - targetStart) {
            sourceEnd = sourceStart + target.size() - targetStart;
          }

          unsigned size = std::min(
            std::min(sourceEnd - sourceStart, target.size() - targetStart),
            target.size() - targetStart
          );

          if (buf.value.size() < size) {
            buf.value.resize(size);
          }

          std::copy(
            begin(source) + sourceStart,
            begin(source) + sourceEnd,
            begin(target) + targetStart
          );

          return size;
        };

        ///
        /// overload copy(Buffer& target)
        ///
        /// return size_t
        ///
        size_t copy (Buffer& buf) {
          if (buf.value.size() == 0) {
            return 0;
          }

          buf.value = std::vector<T> (this->value);
          return buf.value.size();
        };

        ///
        /// overload copy(Buffer& target, size_t targetStart)
        ///
        /// return size_t
        ///
        size_t copy (Buffer& buf, size_t targetStart) {
          return copy(buf, targetStart, 0, buf.value.size());
        };

        ///
        /// overload copy(Buffer& target, size_t targetStart, size_t sourceStart)
        ///
        /// return size_t
        ///
        size_t copy (Buffer& buf, size_t targetStart, size_t sourceStart) {
          return copy(buf, targetStart, sourceStart, buf.value.size());
        };

        ///
        /// method slice(size_t from, size_t to)
        /// comment provides a new buffer that represents a slice of this one.
        ///
        /// param from Where the new Buffer will start. Default: 0.
        /// param to Where the new Buffer will end (not inclusive). Default: buf.length.
        ///
        /// return Buffer
        ///
        Buffer slice(size_t from, size_t to) {
          const auto size = this->value.size();

          if (from > size || to > size) {
            Buffer<T> buf;
            return buf;
          }

          auto _from = std::begin(this->value) + from;
          auto _to = std::begin(this->value) + to;

          std::vector<T> value(_from, _to);

          Buffer<T> buf(value.size());
          buf.value = value;
          return buf;
        };

        ///
        /// overload slice(size_t)
        /// return Buffer
        ///
        Buffer slice(size_t from) {
          return slice(from, this->value.size());
        };

        ~Buffer () {
          this->value.clear();
        };
    };
  } // namespace Util
} // namespace Hyper

#endif
