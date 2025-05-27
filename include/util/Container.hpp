#pragma once
#include <format>
#include <functional>
#include <list>
#include <stdexcept>
#include <string>
#include <unordered_map>

template <class T, class Name = std::string> class Container {
private:
  std::list<T> _items;
  std::unordered_map<Name, T *> _indexMap;

public:
  Container() = default;

  Container(const Container &) = delete;

  Container &operator=(const Container &) = delete;

  template <class... Args> void spawn(Args... args) {
    _items.emplace_back(args...);
  }

  template <class... Args> T &spawnNamed(const Name &name, Args... args) {
    if (_indexMap.find(name) != _indexMap.end()) {
      throw std::runtime_error(
          std::format("Item with name '{}' already exists.", name));
    }
    _items.emplace_back(args...);
    _indexMap[name] = &_items.back();
    return _items.back();
  }

  template <class... Args>
  T &spawnNamed(const std::function<Name(T &)> &nameFn, Args... args) {
    T item(args...);
    Name name = nameFn(item);
    if (_indexMap.find(name) != _indexMap.end()) {
      throw std::runtime_error(
          std::format("Item with name '{}' already exists.", name));
    }
    _items.push_back(std::move(item));
    _indexMap[name] = &_items.back();
    return _items.back();
  }

  T &get(const Name &name) {
    auto it = _indexMap.find(name);
    if (it == _indexMap.end()) {
      throw std::runtime_error(
          std::format("Item with name '{}' not found.", name));
    }
    return *(it->second);
  }

  const T &get(const Name &name) const {
    auto it = _indexMap.find(name);
    if (it == _indexMap.end()) {
      throw std::runtime_error(
          std::format("Item with name '{}' not found.", name));
    }
    return *(it->second);
  }

  bool has(const Name &name) const {
    return _indexMap.find(name) != _indexMap.end();
  }

  void remove(const Name &name) {
    auto it = _indexMap.find(name);
    if (it == _indexMap.end()) {
      throw std::runtime_error(
          std::format("Item with name '{}' not found.", name));
    }
    for (auto itemIt = _items.begin(); itemIt != _items.end(); itemIt++) {
      if (*itemIt == *(it->second)) {
        _items.erase(itemIt);
        break;
      }
    }
    _indexMap.erase(it);
  }

  std::list<T> &getAll() { return _items; }

  const std::list<T> &getAll() const { return _items; }

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