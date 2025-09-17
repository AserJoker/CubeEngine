#include "video/Texture.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_gpu.h>
#include <format>
#include <stdexcept>
using namespace cube;
using namespace cube::video;
Texture::Texture(SDL_GPUDevice *device, const SDL_GPUTextureCreateInfo &info)
    : _device(device) {
  _texture = SDL_CreateGPUTexture(_device, &info);
  if (!_texture) {
    throw std::runtime_error(
        std::format("Failed to create texture: {}", SDL_GetError()));
  }
}
Texture::~Texture() {
  if (_texture) {
    SDL_ReleaseGPUTexture(_device, _texture);
    _texture = nullptr;
  }
}