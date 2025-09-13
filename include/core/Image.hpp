#pragma once
#include "core/Object.hpp"
#include <SDL3/SDL.h>
namespace cube::core {
class Image : public Object {
private:
  SDL_Surface *_surface = nullptr;

public:
  explicit Image(SDL_Surface *surface);
  ~Image() override;
};
}; // namespace cube::core