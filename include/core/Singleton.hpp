#pragma once
#include <memory>
namespace cube::core {
template <class C> class Singleton {
public:
  static std::shared_ptr<C> get() {
    static std::shared_ptr<C> instance = std::make_shared<C>();
    return instance;
  }
};
}; // namespace cube::core