#pragma once
#include "Texture.hpp"
#include "core/Object.hpp"
#include <memory>
#include <vector>
namespace cube::video {
class RenderTarget : public core::Object {
private:
  std::vector<std::shared_ptr<Texture>> _colors;
  std::shared_ptr<Texture> _texture3D;

public:
};
}; // namespace cube::video