#pragma once
#include "Context.hpp"
#include "core/Object.hpp"
#include <memory>
#include <string>
#include <vector>
namespace cube::ui {
class Node : public core::Object {
private:
  std::vector<std::shared_ptr<Node>> _children;
  std::string _id;
  Node *_parent = nullptr;

public:
  const std::vector<std::shared_ptr<Node>> &getChildren() const;
  Node *getParent() const;
  const std::string &getId() const;
  void setId(const std::string &id);
  void append(const std::shared_ptr<Node> &child);
  void remove(const std::shared_ptr<Node> &child);
  void clear();
  virtual void draw(const Context &ctx);
};
}; // namespace cube::ui