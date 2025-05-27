#pragma once
#include "SDL3/SDL_video.h"
#include "util/IEvent.hpp"
#include <SDL3/SDL.h>
class WindowCloseEvent : public Event<"WindowClose"> {
private:
  SDL_WindowID _id;

public:
  WindowCloseEvent(SDL_WindowID id) : _id(id) {}

  SDL_WindowID getWindowId() const { return _id; };
};