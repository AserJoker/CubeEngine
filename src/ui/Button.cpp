#include "ui/Button.hpp"
#include <imgui.h>
using namespace cube;
using namespace cube::ui;
void Button::draw(const Context &ctx) { ImGui::Button(_label.c_str()); }
void Button::setLabel(const std::string &label) { _label = label; }
const std::string &Button::getLabel() const { return _label; }