#pragma once
#include "core/Object.hpp"
#include <SDL3/SDL_gpu.h>
namespace cube::video {
class Texture : public core::Object {
private:
  SDL_GPUTexture *_texture = nullptr;
  SDL_GPUDevice *_device = nullptr;

public:
  Texture(SDL_GPUDevice *device, const SDL_GPUTextureCreateInfo &info);
  ~Texture() override;
};
}; // namespace cube::video