#pragma once
#include "core/Object.hpp"
#include <SDL3/SDL.h>
namespace cube::video {
class Renderer : public core::Object {
private:
  SDL_GPUDevice *_device = nullptr;

public:
  Renderer(SDL_Window *window);
  ~Renderer() override;
  SDL_GPUDevice *getDevice() const;
  void present();
};
} // namespace cube::video