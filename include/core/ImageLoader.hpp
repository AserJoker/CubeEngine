#pragma once
#include "core/Loader.hpp"
namespace cube::core {
class ImageLoader : public core::Loader {
public:
  std::shared_ptr<core::Object> resolve(const std::string &filename) override;
};
} // namespace cube::core