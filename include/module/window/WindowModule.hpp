#pragma once
#include "runtime/IModule.hpp"
#include "util/IEvent.hpp"
#include <SDL3/SDL.h>
#include <string>
#include <unordered_map>

using QuitEvent = Event<"Quit">;

class WindowModule : public IModule {
public:
  struct Config {
    std::string title;
    uint32_t width;
    uint32_t height;
    SDL_WindowFlags flags;
    std::unordered_map<SDL_GLAttr, int> glAttributes;
  };

private:
  Config _config;

public:
  void setup(Application *app) override;
  void cleanup() override;
  void initialize(const Config &config = {});
};