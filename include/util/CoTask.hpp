#pragma once
#include "util/PromiseType.hpp"
#include "util/Task.hpp"
#include <future>
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
    }
    return false;
  }
  void destroy() { handle.destroy(); }
  CoTask(const std::coroutine_handle<PromiseType<T>> &handle,
         const std::shared_ptr<std::promise<T>> &promise)
      : handle(handle), promise(promise) {}
};