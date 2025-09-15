#include "ui/Node.hpp"
using namespace cube;
using namespace cube::ui;

const std::vector<std::shared_ptr<Node>> &Node::getChildren() const {
  return _children;
}
Node *Node::getParent() const { return _parent; }
const std::string &Node::getId() const { return _id; }
void Node::setId(const std::string &id) { _id = id; }
void Node::append(const std::shared_ptr<Node> &child) {
  if (child->_parent != this) {
    if (child->_parent) {
      child->_parent->remove(child);
    }
    _children.push_back(child);
    child->_parent = this;
  }
}
void Node::remove(const std::shared_ptr<Node> &child) {
  if (child->_parent != this) {
    return;
  }
  std::erase(_children, child);
  child->_parent = nullptr;
}
void Node::clear() { _children.clear(); }
void Node::draw(const Context &ctx) {
  for (auto &child : _children) {
    child->draw(ctx);
  }
}