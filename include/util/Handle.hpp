#pragma once
#include "Container.hpp"
#include "Singleton.hpp"
#include <string>

template <class T, class Name = std::string> class Handle {
private:
  Name _name;

public:
  using ContainerType = Container<T, Name>;
  Handle(const Name &name) : _name(name) {}

  T &get() {
    auto &container = Singleton<ContainerType>::get();
    return container.get(_name);
  }

  const T &get() const {
    auto &container = Singleton<ContainerType>::get();
    return container.get(_name);
  }

  bool exists() const {
    auto &container = Singleton<ContainerType>::get();
    return container.has(_name);
  }

  void remove() {
    auto &container = Singleton<ContainerType>::get();
    if (container.has(_name)) {
      container.remove(_name);
    }
  }
};