#pragma once
#include "core/Object.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <string>
namespace cube::runtime {
class Window : public core::Object {
private:
  SDL_Window *_window = nullptr;

public:
  Window(const std::string &title, int width, int height);
  ~Window() override;
  SDL_Window *getWindow() const;
  SDL_WindowID getWindowID() const;
  bool isActive() const;
  virtual void onEvent(const SDL_Event &event);
  virtual void onUpdate();
};
} // namespace cube::runtime