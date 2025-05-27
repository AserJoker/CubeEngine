#pragma once
#include "util/ProxyTask.hpp"
#include "util/Singleton.hpp"
#include <functional>
template <class... Args> class System : public ProxyTask {
private:
  template <class T> T *inject() { return Singleton<T>::get().get(); }

  template <class T>
    requires requires(T t) { T::template inject<T>(); }
  T *inject() {
    return T::template inject<T>();
  }

  auto autoInject(const std::function<void(Args *...)> &fn) {
    return [fn, this]() -> bool {
      fn(inject<Args>()...);
      return true;
    };
  }

  auto autoInject(const std::function<bool(Args *...)> &fn) {
    return [fn, this]() -> bool { return fn(inject<Args>()...); };
  }

public:
  System(const std::function<void(Args *...)> &fn)
      : ProxyTask(autoInject(fn)) {}

  System(const std::function<bool(Args *...)> &fn)
      : ProxyTask(autoInject(fn)) {}
};