#include "video/Renderer.hpp"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_gpu.h>
#include <cstddef>
#include <format>
#include <stdexcept>
using namespace cube;
using namespace cube::video;
Renderer::Renderer(SDL_Window *window) : _window(window) {
  _device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, NULL);
  if (!_device) {
    throw std::runtime_error(
        std::format("Failed to create gpu device: {}", SDL_GetError()));
  }
  SDL_ClaimWindowForGPUDevice(_device, window);
}
Renderer::~Renderer() {
  if (_device) {
    SDL_ReleaseWindowFromGPUDevice(_device, _window);
    SDL_DestroyGPUDevice(_device);
    _device = nullptr;
  }
}
SDL_GPUDevice *Renderer::getDevice() const { return _device; }
void Renderer::present() {
  SDL_GPUCommandBuffer *cmdbuf = SDL_AcquireGPUCommandBuffer(_device);
  SDL_GPUTexture *swapchain = nullptr;
  SDL_WaitAndAcquireGPUSwapchainTexture(cmdbuf, _window, &swapchain, NULL,
                                        NULL);
  if (swapchain) {
    SDL_GPUColorTargetInfo colorTargetInfo = {};
    colorTargetInfo.texture = swapchain;
    colorTargetInfo.clear_color = (SDL_FColor){0.3f, 0.4f, 0.5f, 1.0f};
    colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
    colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;
    SDL_GPUDepthStencilTargetInfo depthStencilTargetInfo = {};
    depthStencilTargetInfo.stencil_load_op = SDL_GPU_LOADOP_CLEAR;
    depthStencilTargetInfo.stencil_store_op = SDL_GPU_STOREOP_STORE;
    depthStencilTargetInfo.clear_stencil = 0.0f;
    depthStencilTargetInfo.clear_depth = 0.0f;
    depthStencilTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
    depthStencilTargetInfo.store_op = SDL_GPU_STOREOP_STORE;
    SDL_GPURenderPass *renderPass =
        SDL_BeginGPURenderPass(cmdbuf, &colorTargetInfo, 1, NULL);

    SDL_BindGPUGraphicsPipeline(renderPass, NULL);
    SDL_EndGPURenderPass(renderPass);
  }
  SDL_SubmitGPUCommandBuffer(cmdbuf);
}