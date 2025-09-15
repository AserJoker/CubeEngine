#pragma once
#include "core/Object.hpp"
#include <imgui.h>
#include <string>
#include <unordered_map>
namespace cube::ui {
class Context : public core::Object {
private:
  std::unordered_map<std::string, ImFont *> _fonts;

public:
  void removeFont(const std::string &name);
  void clearFont();
  void setFont(const std::string &name, ImFont *font);
  ImFont *getFont(const std::string &name) const;
};
} // namespace cube::ui