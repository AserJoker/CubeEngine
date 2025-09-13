#pragma once
#include "core/Object.hpp"
#include <memory>
#include <string>
namespace cube::core {
class Loader : public core::Object {
public:
  virtual std::shared_ptr<core::Object>
  resolve(const std::string &filename) = 0;
};
} // namespace cube::core