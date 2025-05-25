#pragma once
#include <memory>
template <class T> class Singleton {
public:
  static std::unique_ptr<T> &get() {
    static std::unique_ptr<T> instance{nullptr};
    if (instance == nullptr) {
      instance = std::make_unique<T>();
    }
    return instance;
  }
};