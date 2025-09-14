#include "core/Image.hpp"
#include <SDL3/SDL.h>
using namespace cube;
using namespace cube::core;
Image::Image(SDL_Surface *surface) : _surface(surface) {}
Image::~Image() {
  if (_surface) {
    SDL_DestroySurface(_surface);
    _surface = nullptr;
  }
}
SDL_Surface *Image::getSurface() const { return _surface; }