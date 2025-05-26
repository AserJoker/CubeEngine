#include "module/window/WindowModule.hpp"
#include "module/window/Window.hpp"
#include "runtime/Application.hpp"
#include "util/Container.hpp"
#include <SDL3/SDL.h>
#include <stdexcept>

SDL_WindowID getWindowID(Window &window) {
  return SDL_GetWindowID(window.getSDLWindow());
}

static void createMainWindow(std::unique_ptr<WindowContainer> &windows) {
  windows->spawnNamed(getWindowID, "Cube", 1024, 768);
}

static bool EventLoop(std::unique_ptr<WindowContainer> &windows) {
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_EVENT_QUIT:
      return true;
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED: {
      if (windows->has(event.window.windowID)) {
        auto &win = windows->get(event.window.windowID);
        SDL_DestroyWindow(win.getSDLWindow());
        windows->remove(event.window.windowID);
      }
      if (windows->getAll().empty()) {
        return true; // Exit if no windows left
      }
      break;
    }
    default:
      break;
    }
  }
  return false;
}

void WindowModule::setup(Application *app) {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
    throw std::runtime_error("Failed to initialize SDL: " +
                             std::string(SDL_GetError()));
  }
  app->addSystem(createMainWindow);
  app->addSystem(EventLoop);
}

void WindowModule::cleanup() { SDL_Quit(); }