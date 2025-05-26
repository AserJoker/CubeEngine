#pragma once
#include "util/Task.hpp"
#include <exception>
#include <functional>
#include <iostream>
#include <ostream>

class ProxyTask : public Task {
private:
  std::function<bool()> _function;

public:
  bool next() override {
    try {
      return _function();
    } catch (std::exception &e) {
      std::println(std::cerr, "Uncaught exception: {}", e.what());
    }
    return true;
  }
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