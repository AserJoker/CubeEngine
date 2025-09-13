#pragma once
#include "core/Loader.hpp"
#include <string>
namespace cube::core {
class JsonLoader : public core::Loader {
public:
  std::shared_ptr<core::Object> resolve(const std::string &filename) override;
};
} // namespace cube::core