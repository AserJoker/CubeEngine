#pragma once
#include "util/PromiseType.hpp"
#include "util/Task.hpp"
#include <exception>
#include <future>
#include <iostream>
#include <ostream>
template <class T> struct CoTask : public Task {
  std::shared_ptr<std::promise<T>> promise;
  std::coroutine_handle<PromiseType<T>> handle;
  bool next() {
    if (handle.done()) {
      return true;
    }
    handle.resume();
    if (handle.done()) {
      if (handle.promise().exception) {
        promise->set_exception(handle.promise().exception);
      } else {
        promise->set_value(handle.promise().value);
      }
      return true;
    }
    return false;
  }
  
  void destroy() {
    if (handle) {
      handle.destroy();
      handle = nullptr;
      try {
        promise->get_futrue().get();
      } catch (std::exception &e) {
        std::println(std::cerr, "Uncaught exception: {}", e.what());
      }
    }
  }

  CoTask(const std::coroutine_handle<PromiseType<T>> &handle,
         const std::shared_ptr<std::promise<T>> &promise)
      : handle(handle), promise(promise) {}
};