#include "runtime/GUIWindow.hpp"
#include "runtime/Window.hpp"
#include "ui/Button.hpp"
#include "ui/Document.hpp"
#include <SDL3/SDL.h>
#include <format>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <memory>
#include <stdexcept>

using namespace cube;
using namespace cube::runtime;
bool show_demo = true;
GUIWindow::GUIWindow(const std::string &title, int width, int height)
    : Window(title, width, height) {
  _renderer = SDL_CreateRenderer(getWindow(), NULL);
  if (!_renderer) {
    throw std::runtime_error(
        std::format("Failed to create renderer: {}", SDL_GetError()));
  }
  SDL_SetRenderVSync(_renderer, true);
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavNoCaptureKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  io.ConfigDebugIniSettings = false;
  ImGui_ImplSDL3_InitForSDLRenderer(getWindow(), _renderer);
  ImGui_ImplSDLRenderer3_Init(_renderer);
  auto btn = std::make_shared<ui::Button>();
  btn->setLabel("Test");
  _document.append(btn);
}
GUIWindow::~GUIWindow() {
  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();
  if (_renderer) {
    SDL_DestroyRenderer(_renderer);
    _renderer = nullptr;
  }
}
void GUIWindow::onUpdate() {
  auto &io = ImGui::GetIO();
  ImGui_ImplSDLRenderer3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();
  _document.draw(_ctx);
  ImGui::Render();
  SDL_SetRenderScale(_renderer, io.DisplayFramebufferScale.x,
                     io.DisplayFramebufferScale.y);
  SDL_SetRenderDrawColorFloat(_renderer, 0.45f, 0.55f, 0.60f, 1.00f);
  SDL_RenderClear(_renderer);
  ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), _renderer);
  SDL_RenderPresent(_renderer);
}

void GUIWindow::onEvent(const SDL_Event &event) {
  ImGui_ImplSDL3_ProcessEvent(&event);
  runtime::Window::onEvent(event);
}