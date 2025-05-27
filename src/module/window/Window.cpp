#include "module/window/Window.hpp"
#include "SDL3/SDL_video.h"
#include <stdexcept>

Window::Window() : _window(nullptr) {}

Window::Window(Window &&other) noexcept : _window(other._window) {
  other._window = nullptr;
  if (_window) {
    getWindows()[SDL_GetWindowID(_window)] = this;
  }
}

Window::~Window() { dispose(); }

void Window::create(const std::string &title, int width, int height,
                    SDL_WindowFlags flags) {
  _window = SDL_CreateWindow(title.c_str(), width, height, flags);
  if (!_window) {
    throw std::runtime_error("Failed to create _window: " +
                             std::string(SDL_GetError()));
  }
  getWindows()[SDL_GetWindowID(_window)] = this;
}

Window &Window::operator=(Window &&other) {
  if (this != &other) {
    if (_window) {
      SDL_DestroyWindow(_window);
    }
    _window = other._window;
    other._window = nullptr;
  }
  if (_window) {
    getWindows()[SDL_GetWindowID(_window)] = this;
  }
  return *this;
}

SDL_Window *Window::getSDLWindow() { return _window; }

const SDL_Window *Window::getSDLWindow() const { return _window; }

void Window::dispose() {
  if (_window) {
    getWindows().erase(SDL_GetWindowID(_window));
    SDL_DestroyWindow(_window);
    _window = nullptr;
  }
}

std::unordered_map<SDL_WindowID, Window *> &Window::getWindows() {
  static std::unordered_map<SDL_WindowID, Window *> windows;
  return windows;
}
