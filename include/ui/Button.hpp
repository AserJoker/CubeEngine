#pragma once
#include "Node.hpp"
#include <string>
namespace cube::ui {
class Button : public Node {
private:
  std::string _label;

public:
  void draw(const Context &ctx) override;
  void setLabel(const std::string &label);
  const std::string &getLabel() const;
};
}; // namespace cube::ui