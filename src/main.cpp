#include "SDL3/SDL_messagebox.h"
#include "SDL3/SDL_video.h"
#include "module/window/Window.hpp"
#include "module/window/WindowCloseEvent.hpp"
#include "module/window/WindowModule.hpp"
#include "runtime/Application.hpp"
#include "util/EventQueue.hpp"
#include <memory>
#include <print>

bool onWindowClose(EventQueue *queue) {
  if (queue->is("WindowClose")) {
    auto event = queue->pollEvent();
    auto &win = Window::getWindows().at(
        ((WindowCloseEvent *)event.get())->getWindowId());
    int btnId = 0;
    SDL_MessageBoxData msg;
    SDL_MessageBoxButtonData btn[2];
    btn[0].buttonID = 1;
    btn[0].text = "Ok";
    btn[1].buttonID = 2;
    btn[1].text = "Cancel";
    msg.message = "Are you sure to close window";
    msg.window = win->getSDLWindow();
    msg.title = "Confirm";
    msg.numbuttons = 2;
    msg.buttons = btn;
    SDL_ShowMessageBox(&msg, &btnId);
    if (btnId == 1) {
      queue->pushEvent(event);
    }
  }
  return false;
}

int main(int argc, char *argv[]) {
  return Singleton<Application>::get()
      ->addModule<WindowModule>(WindowModule::Config{
          .title = "Main Window",
          .flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE,
          .glAttributes =
              {
                  {SDL_GL_CONTEXT_MAJOR_VERSION, 4},
                  {SDL_GL_CONTEXT_MINOR_VERSION, 2},
                  {SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE},
              },
      })
      ->addSystem(onWindowClose)
      ->run(argc, argv);
}