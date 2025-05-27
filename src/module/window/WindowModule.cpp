#include "module/window/WindowModule.hpp"
#include "module/window/Window.hpp"
#include "module/window/WindowCloseEvent.hpp"
#include "runtime/Application.hpp"
#include "util/EventQueue.hpp"
#include "util/Singleton.hpp"
#include "util/TaskRunner.hpp"
#include <SDL3/SDL.h>
#include <format>
#include <stdexcept>

static bool onWindowClose(EventQueue *queue) {
  auto &windows = Window::getWindows();
  if (queue->hasEvent() &&
      queue->peekEvent()->getType() == std::string("WindowClose")) {
    auto event = queue->pollEvent();
    auto windowCloseEvent = (WindowCloseEvent *)event.get();
    if (windows.contains(windowCloseEvent->getWindowId())) {
      auto win = windows.at(windowCloseEvent->getWindowId());
      win->dispose();
    }
  }
  if (windows.empty()) {
    queue->pushEvent<QuitEvent>();
    return true;
  }
  return false;
}
static bool onQuitEvent(EventQueue *queue) {
  if (queue->hasEvent() &&
      queue->peekEvent()->getType() == std::string("Quit")) {
    auto &runner = Singleton<TaskRunner>::get();
    runner->stop();
  }
  return false;
}

static bool runEventLoop(EventQueue *queue) {
  auto &windows = Window::getWindows();
  SDL_Event event = {};
  if (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED: {
      queue->pushEvent<WindowCloseEvent>(event.window.windowID);
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
    throw std::runtime_error(
        std::format("Failed to initialize SDL: {}", SDL_GetError()));
  }
  for (auto &[attr, val] : _config.glAttributes) {
    if (!SDL_GL_SetAttribute(attr, val)) {
      throw std::runtime_error(
          std::format("Failed to set OpenGL Attribute: {}", SDL_GetError()));
    }
  }
  auto &win = Singleton<MainWindow>::get();
  win->create(_config.title, _config.width, _config.height, _config.flags);
  app->addSystem(onQuitEvent);
  app->addSystem(onWindowClose);
  app->addSystem(runEventLoop);
}

void WindowModule::cleanup() { SDL_Quit(); }

void WindowModule::initialize(const Config &config) {
  _config = config;
  if (_config.title.empty()) {
    _config.title = "Cube";
  }
  if (_config.width == 0) {
    _config.width = 1024;
  }
  if (_config.height == 0) {
    _config.height = 768;
  }
}