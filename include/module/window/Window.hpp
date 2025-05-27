#pragma once
#include "SDL3/SDL_video.h"
#include <SDL3/SDL.h>
#include <string>
#include <unordered_map>
class Window {
private:
  SDL_Window *_window;

public:
  Window();

  Window(const Window &other) = delete;

  Window &operator=(Window &&other);

  Window(Window &&other) noexcept;

  virtual ~Window();

  constexpr inline bool operator==(const Window &other) const {
    return _window == other._window;
  }

  void create(const std::string &title, int width, int height,
              SDL_WindowFlags = 0);

  SDL_Window *getSDLWindow();

  const SDL_Window *getSDLWindow() const;

  void dispose();

public:
  static std::unordered_map<SDL_WindowID, Window *> &getWindows();
};

class MainWindow : public Window {};
