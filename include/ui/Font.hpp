#pragma once
#include "ui/Node.hpp"
#include <imgui.h>
namespace cube::ui {
class Font : public Node {
private:
  std::string _name;
  size_t _size;

public:
  void draw(const Context &ctx) override;
};
} // namespace cube::ui