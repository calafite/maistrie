#pragma once

#include <array>
#include <cstdio>
#include <cstring>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#if defined(_WIN32)
#define CALAFITE_FREAD _fread_nolock
#define CALAFITE_FWRITE _fwrite_nolock
#elif defined(__linux__) || defined(__CYGWIN__)
#define CALAFITE_FREAD fread_unlocked
#define CALAFITE_FWRITE fwrite_unlocked
#else
#define CALAFITE_FREAD fread
#define CALAFITE_FWRITE fwrite
#endif

#define CALAFITE_UNLIKELY(x) __builtin_expect(!!(x), 0)

namespace calafite {
namespace io {

struct Scanner {
  static constexpr size_t BUF_SIZE = 1 << 17; // 128KB buffer
  char buf[BUF_SIZE];
  char *ptr = buf;
  char *end = buf;
  bool eof_flag = false;

  inline bool reload() {
    size_t res = CALAFITE_FREAD(buf, 1, BUF_SIZE, stdin);
    ptr = buf;
    end = buf + res;
    if (res == 0)
      eof_flag = true;
    return res > 0;
  }

  inline char get_char() {
    if (CALAFITE_UNLIKELY(ptr == end)) {
      if (!reload())
        return EOF;
    }
    return *ptr++;
  }

  inline char skip_whitespace() {
    char c;
    while ((c = get_char()) != EOF && c <= ' ') {
    }
    return c;
  }

  explicit operator bool() const { return !eof_flag; }

  template <typename T>
  inline std::enable_if_t<std::is_integral_v<T>, Scanner &> operator>>(T &val) {
    val = 0;
    char c = skip_whitespace();
    if (CALAFITE_UNLIKELY(c == EOF))
      return *this;

    bool neg = false;
    if constexpr (std::is_signed_v<T>) {
      if (c == '-') {
        neg = true;
        c = get_char();
      }
    }

    for (; (unsigned char)(c - '0') < 10; c = get_char()) {
      val = val * 10 + (c - '0');
    }

    if constexpr (std::is_signed_v<T>) {
      if (neg)
        val = -val;
    }
    return *this;
  }

  inline Scanner &operator>>(char &c) {
    c = skip_whitespace();
    return *this;
  }

  inline Scanner &operator>>(std::string &s) {
    s.clear();
    char c = skip_whitespace();
    if (CALAFITE_UNLIKELY(c == EOF))
      return *this;
    do {
      s.push_back(c);
      c = get_char();
    } while (c != EOF && c > ' ');
    return *this;
  }

  template <typename T1, typename T2>
  inline Scanner &operator>>(std::pair<T1, T2> &p) {
    return *this >> p.first >> p.second;
  }

  template <typename T> inline Scanner &operator>>(std::vector<T> &v) {
    for (auto &x : v)
      *this >> x;
    return *this;
  }

  template <typename T, size_t N>
  inline Scanner &operator>>(std::array<T, N> &a) {
    for (auto &x : a)
      *this >> x;
    return *this;
  }
};

struct Printer {
  static constexpr size_t BUF_SIZE = 1 << 17;
  char buf[BUF_SIZE];
  char *ptr = buf;
  char num_lut[200];

  Printer() {
    for (int i = 0; i < 100; i++) {
      num_lut[i * 2] = (char)('0' + i / 10);
      num_lut[i * 2 + 1] = (char)('0' + i % 10);
    }
  }

  ~Printer() { flush(); }

  inline void flush() {
    if (ptr != buf) {
      CALAFITE_FWRITE(buf, 1, ptr - buf, stdout);
      ptr = buf;
    }
  }

  inline void put_char(char c) {
    if (CALAFITE_UNLIKELY(ptr - buf >= BUF_SIZE - 1))
      flush();
    *ptr++ = c;
  }

  template <typename T>
  inline std::enable_if_t<std::is_integral_v<T>, Printer &> operator<<(T val) {
    if (CALAFITE_UNLIKELY(ptr - buf >= BUF_SIZE - 32))
      flush();

    using U = std::make_unsigned_t<T>;
    U uval = val;
    if constexpr (std::is_signed_v<T>) {
      if (val < 0) {
        *ptr++ = '-';
        uval = 0 - uval;
      }
    }

    if (uval == 0) {
      *ptr++ = '0';
      return *this;
    }

    char temp[24];
    int idx = 24;

    while (uval >= 100) {
      idx -= 2;
      int rem = uval % 100;
      uval /= 100;
      temp[idx] = num_lut[rem * 2];
      temp[idx + 1] = num_lut[rem * 2 + 1];
    }
    if (uval < 10) {
      temp[--idx] = (char)('0' + uval);
    } else {
      idx -= 2;
      temp[idx] = num_lut[uval * 2];
      temp[idx + 1] = num_lut[uval * 2 + 1];
    }

    int len = 24 - idx;
    std::memcpy(ptr, temp + idx, len);
    ptr += len;
    return *this;
  }

  inline Printer &operator<<(char c) {
    put_char(c);
    return *this;
  }

  inline Printer &operator<<(const char *s) {
    while (*s)
      put_char(*s++);
    return *this;
  }

  inline Printer &operator<<(const std::string &s) {
    for (char c : s)
      put_char(c);
    return *this;
  }

  template <typename T1, typename T2>
  inline Printer &operator<<(const std::pair<T1, T2> &p) {
    return *this << p.first << ' ' << p.second;
  }

  template <typename T> inline Printer &operator<<(const std::vector<T> &v) {
    for (size_t i = 0; i < v.size(); i++) {
      *this << v[i];
      if (i + 1 < v.size())
        put_char(' ');
    }
    return *this;
  }

  template <typename T, size_t N>
  inline Printer &operator<<(const std::array<T, N> &a) {
    for (size_t i = 0; i < N; i++) {
      *this << a[i];
      if (i + 1 < N)
        put_char(' ');
    }
    return *this;
  }
};

inline Scanner in;
inline Printer out;

template <typename... Args> inline void read(Args &...args) {
  (in >> ... >> args);
}

template <typename First, typename... Rest>
inline void print(const First &first, const Rest &...rest) {
  out << first;
  ((out << ' ' << rest), ...);
}
inline void print() {}

template <typename... Args> inline void println(const Args &...args) {
  print(args...);
  out << '\n';
}

} // namespace io
} // namespace calafite
