#include "module/window/Window.hpp"
#include "module/window/WindowModule.hpp"
#include "runtime/Application.hpp"
#include "util/IndexHandle.hpp"
#include <memory>
#include <print>

bool renameWindow(std::unique_ptr<MainWindowHandle> &window) {
  if (window->exists()) {
    auto &win = window->get();
    SDL_SetWindowTitle(win.getSDLWindow(), "Renamed Window");
    return true;
  } else {
    return false;
  }
}

int main(int argc, char *argv[]) {
  return Singleton<Application>::get()
      ->addModule<WindowModule>()
      ->addSystem(renameWindow)
      ->run(argc, argv);
}