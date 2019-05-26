#ifndef HYPER_UTIL_BUFFER_H
#define HYPER_UTIL_BUFFER_H

#include <algorithm>
#include <vector>
#include <initializer_list>
#include <string>
#include <iostream>
#include <iomanip>
#include <exception>

namespace Hyper {
  namespace Util {
    template<typename T>
    class Buffer {
      public:
        std::vector<T> value;

        class OutOfRange : public std::exception {
          virtual const char* what() const throw() {
            return "Out Of Range Exception";
          }
        } EX_OUT_OF_RANGE;

        // bool equals(Buffer b);
        // int indexOf(T value, int byteOffset);

        Buffer& operator= (const Buffer& buf);

        T operator[] (int i) {
          return this->value[i];
        }

        Buffer(std::initializer_list<T> l) : value(l) {}

        Buffer() {
          this->value = std::vector<T>(0);
        };

        Buffer(size_t size) {
          this->value = std::vector<T>(size);
        };

        Buffer(const std::string& str) {
          this->value = std::vector<uint8_t>(str.begin(), str.end());
        };

        Buffer(const Buffer& buf) {
          this->value = buf.value;
        };

        std::string toString() {
          std::string str(this->value.begin(), this->value.end());
          return str;
        };

        int length() {
          return this->value.size();
        };

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

        size_t copy (Buffer& buf) {
          if (buf.value.size() == 0) {
            return 0;
          }

          buf.value = std::vector<T> (this->value);
          return buf.value.size();
        };

        size_t copy (Buffer& buf, size_t targetStart) {
          return copy(buf, targetStart, 0, buf.value.size());
        };

        size_t copy (Buffer& buf, size_t targetStart, size_t sourceStart) {
          return copy(buf, targetStart, sourceStart, buf.value.size());
        };

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

        Buffer slice(size_t from) {
          return slice(from, this->value.size());
        };

        ~Buffer () {
          this->value.clear();
        };
    };

    template <typename T>
    bool operator== (const Buffer<T>& lhs, const Buffer<T>& rhs) {
      return std::equal(
        std::begin(lhs.value),
        std::end(lhs.value),
        std::begin(rhs.value)
      );
    }

    template <typename T>
    bool operator!= (const Buffer<T>& lhs, const Buffer<T>& rhs) {
      return !std::equal(
        std::begin(lhs.value),
        std::end(lhs.value),
        std::begin(rhs.value)
      );
    }

    template <typename T>
    std::ostream& operator<< (std::ostream &os, const Buffer<T>& buf) {
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
  } // namespace Util
} // namespace Hyper

#endif
