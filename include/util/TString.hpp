#pragma once
#include <algorithm>
#include <cstddef>
template <size_t length> struct TString {
  char value[length];
  constexpr TString(char (&source)[length]) {
    std::copy_n(value, source, length);
  }
};