#pragma once
#include "core/Buffer.hpp"
#include "core/Object.hpp"
#include <memory>
namespace cube::runtime {
class Loader : public core::Object {
public:
  virtual std::shared_ptr<core::Object> resolve(const core::Buffer *buffer) = 0;
};
} // namespace cube::runtime