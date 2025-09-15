#pragma once
#include "runtime/Window.hpp"
#include "ui/Context.hpp"
#include "ui/Document.hpp"
#include <imgui.h>
namespace cube::runtime {
class GUIWindow : public Window {
private:
  SDL_Renderer *_renderer = nullptr;
  ui::Document _document;
  ui::Context _ctx = {};

public:
  GUIWindow(const std::string &title, int width, int height);
  ~GUIWindow() override;
  void onUpdate() override;
  void onEvent(const SDL_Event &event) override;
};
}; // namespace cube::runtime