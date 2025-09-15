#include "runtime/Window.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <format>
#include <stdexcept>
using namespace cube;
using namespace cube::runtime;
Window::Window(const std::string &title, int width, int height) {
  _window = SDL_CreateWindow(title.c_str(), width, height, 0);
  if (!_window) {
    throw std::runtime_error(
        std::format("Failed to create window: {}", SDL_GetError()));
  }
}
Window::~Window() {
  if (_window) {
    SDL_DestroyWindow(_window);
    _window = nullptr;
  }
}
void Window::onEvent(const SDL_Event &event) {
  if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
    SDL_DestroyWindow(_window);
    _window = nullptr;
  }
}
void Window::onUpdate() {}
bool Window::isActive() const {
  uint32_t flags = SDL_GetWindowFlags(_window);
  return (flags & SDL_WINDOW_INPUT_FOCUS) && (!(flags & SDL_WINDOW_MINIMIZED));
}
SDL_Window *Window::getWindow() const { return _window; }
SDL_WindowID Window::getWindowID() const { return SDL_GetWindowID(_window); }