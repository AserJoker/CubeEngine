#pragma once
#include "util/ProxyTask.hpp"
#include "util/Singleton.hpp"
#include <functional>
#include <memory>
template <class... Args> class System : public ProxyTask {
private:
  template <class T> std::unique_ptr<T> &inject() {
    return Singleton<T>::get();
  }

  template <class T>
    requires requires(T t) { T::template inject<T>(); }
  std::unique_ptr<T> &inject() {
    return T::template inject<T>();
  }

  auto autoInject(const std::function<void(std::unique_ptr<Args> &...)> &fn) {
    return [fn, this]() -> bool {
      fn(inject<Args>()...);
      return true;
    };
  }

  auto autoInject(const std::function<bool(std::unique_ptr<Args> &...)> &fn) {
    return [fn, this]() -> bool { return fn(inject<Args>()...); };
  }

public:
  System(const std::function<void(std::unique_ptr<Args> &...)> &fn)
      : ProxyTask(autoInject(fn)) {}
  System(const std::function<bool(std::unique_ptr<Args> &...)> &fn)
      : ProxyTask(autoInject(fn)) {}
};