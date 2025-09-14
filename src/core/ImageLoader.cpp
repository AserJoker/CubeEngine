#include "core/ImageLoader.hpp"
#include "core/Image.hpp"
#include "core/Object.hpp"
#include <SDL3_image/SDL_image.h>
#include <format>
#include <memory>
#include <stdexcept>
using namespace cube;
using namespace cube::core;
std::shared_ptr<core::Object>
ImageLoader::resolve(const std::string &filename) {
  SDL_Surface *surface = IMG_Load(filename.c_str());
  if (!surface) {
    throw std::runtime_error(std::format("Failed to load image '{}': {} ",
                                         filename, SDL_GetError()));
  }
  return std::make_shared<core::Image>(surface);
}