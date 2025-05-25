#pragma once
#include "util/Task.hpp"
#include <functional>

class ProxyTask : public Task {
private:
  std::function<bool()> _function;

public:
  bool next() override { return _function(); }
  void destroy() override {}
  ProxyTask(bool (*function)()) : _function(function) {}
  ProxyTask(const std::function<bool()> &function) : _function(function) {}
  ProxyTask(void (*function)())
      : _function([=]() -> bool {
          function();
          return true;
        }) {}
  ProxyTask(const std::function<void()> &function)
      : _function([=]() -> bool {
          function();
          return true;
        }) {}
};