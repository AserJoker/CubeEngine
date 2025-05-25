#pragma once
template <unsigned length> struct TString {
  char value[length + 1]{};
  constexpr TString(char const *source) {
    for (unsigned i = 0; i < length; i++) {
      value[i] = source[i];
    }
  }
  auto operator<=>(const TString &) const = default;
};
template <unsigned length>
TString(char const (&)[length]) -> TString<length - 1>;

template <TString name, class T> struct Named {
  T value{};
};