#pragma once
#include "util/Singleton.hpp"
template <class T, class... Args> class With : public T {
private:
  template <class Arg> auto &inject() {
    return Singleton<std::decay_t<Arg>>::get();
  }

  template <class Arg>
    requires requires { Arg::template inject<Arg>(); }
  auto &inject() {
    return Arg::template inject<Arg>();
  }

public:
  With() : T(inject<Args>()...) {}
};