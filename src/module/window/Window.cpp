#include "module/window/Window.hpp"
#include <stdexcept>

Window::Window(const char *title, int width, int height) {
  window = SDL_CreateWindow(title, width, height,
                            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |
                                SDL_WINDOW_HIGH_PIXEL_DENSITY);
  if (!window) {
    throw std::runtime_error("Failed to create window: " +
                             std::string(SDL_GetError()));
  }
}

Window::Window(SDL_Window *sdlWindow) : window(sdlWindow) {
  if (!window) {
    throw std::runtime_error("Invalid SDL_Window pointer provided.");
  }
}

Window::~Window() {
  if (window) {
    SDL_DestroyWindow(window);
    window = nullptr;
  }
}

Window &Window::operator=(Window &&other) {
  if (this != &other) {
    if (window) {
      SDL_DestroyWindow(window);
    }
    window = other.window;
    other.window = nullptr;
  }
  return *this;
}

SDL_Window *Window::getSDLWindow() { return window; }

const SDL_Window *Window::getSDLWindow() const { return window; }