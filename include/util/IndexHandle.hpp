#pragma once
#include "Singleton.hpp"
#include <stdexcept>
template <class Container, size_t idx> class IndexHandle {
public:
  auto &get() {
    auto &container = Singleton<Container>::get();
    if (idx >= container->getAll().size()) {
      throw std::out_of_range("Index out of range in IndexHandle");
    }
    return container->getAll()[idx];
  }

  const auto &get() const {
    auto &container = Singleton<Container>::get();
    if (idx >= container->getALl().size()) {
      throw std::out_of_range("Index out of range in IndexHandle");
    }
    return container->getAll()[idx];
  }

  bool exists() const {
    auto &container = Singleton<Container>::get();
    return idx < container->getAll().size();
  }
};