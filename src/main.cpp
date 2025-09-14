#include "core/Singleton.hpp"
#include "runtime/Application.hpp"
#include "runtime/Window.hpp"
#include <SDL3/SDL_events.h>

using namespace cube;
class MainWindow : public runtime::Window {
public:
  MainWindow() : runtime::Window("Cube Engine", 800, 600) {}
  void onWindowEvent(const SDL_WindowEvent &event) override {
    if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
      SDL_QuitEvent e = {
          .type = SDL_EVENT_QUIT,
          .timestamp = SDL_GetTicks(),
      };
      SDL_PushEvent((SDL_Event *)&e);
    }
    return runtime::Window::onWindowEvent(event);
  }
};
auto main(int argc, char *argv[]) -> int {
  try {
    return core::Singleton<runtime::Application>::get()
        ->setApplicationInfo(APP_NAME, APP_VERSION, APP_ID)
        ->addWindow(std::make_shared<MainWindow>())
        ->addWindow(std::make_shared<runtime::Window>("Debug Window", 800, 600))
        ->run(argc, argv);
  } catch (std::exception &e) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", e.what());
    return -1;
  } catch (...) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unknown exception");
    return -1;
  }
};