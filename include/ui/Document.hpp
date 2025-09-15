#pragma once
#include "Node.hpp"
namespace cube::ui {
class Document : public Node {
public:
  void draw(const Context &ctx) override;
  void parse(Context &ctx, const std::string &source,
             const std::string &filename);
};
}; // namespace cube::ui