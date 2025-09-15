#include "ui/Font.hpp"
#include <imgui.h>
using namespace cube;
using namespace cube::ui;
void Font::draw(const Context &ctx) {
  auto font = ctx.getFont(_name);
  if (font) {
    ImGui::PushFont(font);
  }
  Node::draw(ctx);
  if (font) {
    ImGui::PopFont();
  }
}