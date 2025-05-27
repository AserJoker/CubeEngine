#pragma once
#include <algorithm>
#include <cstddef>
template <size_t length> struct TString {
  char value[length];
  constexpr TString(const char (&source)[length]) {
    std::copy_n(source, length, value);
  }
};