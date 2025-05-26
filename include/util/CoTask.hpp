#pragma once
#include "util/ITask.hpp"
#include "util/PromiseType.hpp"
#include <coroutine>
#include <future>
template <class T> class CoTask : public ITask {
private:
  std::coroutine_handle<PromiseType<T>> _handle;
  std::promise<T> _promise;

public:
  CoTask(const std::coroutine_handle<PromiseType<T>> &handle)
      : _handle(handle) {}
  bool next() {
    if (_handle.done()) {
      return true;
    }
    _handle.resume();
    if (_handle.done()) {
      if (_handle.promise().exception) {
        _promise.set_exception(_handle.promise().exception);
      } else {
        _promise.set_value(_handle.promise().value);
      }
    }
    return false;
  }

  void destroy() { _handle.destroy(); }

  std::future<T> future() { return _promise.get_future(); }
};

template <> class CoTask<void> : public ITask {
private:
  std::coroutine_handle<PromiseType<void>> _handle;
  std::promise<void> _promise;

public:
  CoTask(const std::coroutine_handle<PromiseType<void>> &handle)
      : _handle(handle) {}
  bool next() {
    if (_handle.done()) {
      return true;
    }
    _handle.resume();
    if (_handle.done()) {
      if (_handle.promise().exception) {
        _promise.set_exception(_handle.promise().exception);
      } else {
        _promise.set_value();
      }
    }
    return false;
  }

  void destroy() { _handle.destroy(); }

  std::future<void> future() { return _promise.get_future(); }
};