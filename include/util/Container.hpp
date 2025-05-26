#pragma once
#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

template <class T> class Container {
private:
  std::vector<T> _items;
  std::unordered_map<std::string, T *> _indexMap;

public:
  Container() = default;
  Container(const Container &) = delete;

  Container &operator=(const Container &) = delete;

  template <class... Args> void spawn(Args... args) {
    _items.emplace_back(args...);
  }

  template <class... Args>
  void spawnNamed(const std::string &name, Args... args) {
    if (_indexMap.find(name) != _indexMap.end()) {
      throw std::runtime_error("Item with name '" + name + "' already exists.");
    }
    _items.emplace_back(args...);
    _indexMap[name] = &_items.back();
  }

  T &get(const std::string &name) {
    auto it = _indexMap.find(name);
    if (it == _indexMap.end()) {
      throw std::runtime_error("Item with name '" + name + "' not found.");
    }
    return *(it->second);
  }

  const T &get(const std::string &name) const {
    auto it = _indexMap.find(name);
    if (it == _indexMap.end()) {
      throw std::runtime_error("Item with name '" + name + "' not found.");
    }
    return *(it->second);
  }

  void remove(const std::string &name) {
    auto it = _indexMap.find(name);
    if (it == _indexMap.end()) {
      throw std::runtime_error("Item with name '" + name + "' not found.");
    }
    auto itemIt = std::find(_items.begin(), _items.end(), *(it->second));
    if (itemIt != _items.end()) {
      _items.erase(itemIt);
    }
    _indexMap.erase(it);
  }

  std::vector<T> &getAll() { return _items; }

  const std::vector<T> &getAll() const { return _items; }

  size_t size() const { return _items.size(); }

  bool empty() const { return _items.empty(); }

  void clear() {
    _items.clear();
    _indexMap.clear();
  }

  void foreach (const std::function<void(T &)> &fn) {
    for (auto &item : _items) {
      fn(item);
    }
  }
};