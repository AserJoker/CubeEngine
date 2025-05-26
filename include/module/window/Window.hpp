#pragma once
#include "util/Container.hpp"
#include "util/IndexHandle.hpp"
#include <SDL3/SDL.h>
class Window {
private:
  SDL_Window *window;

public:
  Window(const char *title, int width, int height);

  Window(SDL_Window *sdlWindow);

  Window(const Window &other) = delete;

  Window &operator=(Window &&other);

  Window(Window &&other) noexcept : window(other.window) {
    other.window = nullptr;
  }

  ~Window();

  bool operator==(const Window &other) const { return window == other.window; }

  SDL_Window *getSDLWindow();

  const SDL_Window *getSDLWindow() const;
};

using WindowContainer = Container<Window, SDL_WindowID>;
using MainWindowHandle = IndexHandle<WindowContainer, 0>;