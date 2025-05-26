#pragma once
#include <algorithm>
template <size_t N> struct TString {
  constexpr TString(const char (&str)[N]) { std::copy_n(str, N, value); }
  char value[N];
};
template <unsigned length>
TString(char const (&)[length]) -> TString<length - 1>;
template <TString name, class T> struct Named {
  T value{};
};