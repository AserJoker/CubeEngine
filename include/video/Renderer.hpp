#pragma once
#include "RenderTarget.hpp"
#include "core/Object.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <cstdint>
#include <memory>
#include <unordered_map>
namespace cube::video {
class Renderer : public core::Object {
private:
  SDL_Window *_window = nullptr;
  SDL_GPUDevice *_device = nullptr;
  std::unordered_map<uint32_t, SDL_GPUGraphicsPipeline *> _pipelines;
  std::shared_ptr<RenderTarget> _target;
  std::shared_ptr<RenderTarget> _default;

public:
  Renderer(SDL_Window *window);
  ~Renderer() override;
  SDL_GPUDevice *getDevice() const;
  void present();
};
} // namespace cube::video