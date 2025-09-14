#pragma once
#include "core/Object.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>
namespace cube::core {
class Image : public Object {
private:
  SDL_Surface *_surface = nullptr;

public:
  explicit Image(SDL_Surface *surface);
  ~Image() override;
  SDL_Surface *getSurface() const;
};
}; // namespace cube::core