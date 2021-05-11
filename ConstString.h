#pragma once

#include <iostream>

class ConstString {
  const char* mCStr;
  std::size_t mSize;

public:
  template<std::size_t N>
  constexpr ConstString(const char(&cstr)[N]): mCStr(cstr), mSize(N - 1) {}
  constexpr auto const &size() const { return mSize; }
  constexpr auto const *cstr() const { return mCStr; }

};

std::ostream &operator<<(std::ostream &os, ConstString const &s) {
  return os << s.cstr();
}
