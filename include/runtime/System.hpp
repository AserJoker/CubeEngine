#pragma once
#include "util/ProxyTask.hpp"
#include "util/Singleton.hpp"
#include <memory>

class System : public ProxyTask {
public:
  template <typename Arg>
    requires requires(Arg arg) { Arg::template inject<Arg>(); }
  static std::unique_ptr<Arg> &inject() {
    return Arg::template inject<Arg>();
  }

  template <class Arg> static std::unique_ptr<Arg> &inject() {
    return Singleton<Arg>::get();
  }

protected:
  template <class Fn, class... Args> auto autoInject(Fn func) {
    return std::function(
        [this, func]() -> auto { return func(this->inject<Args>()...); });
  }

public:
  template <class Ret, class... Args>
  System(const std::function<Ret(std::unique_ptr<Args> &...)> &func)
      : ProxyTask(autoInject<decltype(func), Args...>(func)) {}
  
  template <class Ret, class... Args>
  System(Ret (*func)(std::unique_ptr<Args> &...))
      : ProxyTask(autoInject<decltype(func), Args...>(func)) {}
};