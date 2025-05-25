#pragma once
#include "PromiseType.hpp"
#include "util/CoTask.hpp"
#include <coroutine>
#include <future>

template <class T> class Generator {

public:
  using promise_type = PromiseType<T>;

private:
  std::shared_ptr<std::promise<T>> _promise =
      std::make_shared<std::promise<T>>();
  std::future<T> _future = _promise->get_future();

public:
  Generator<T>(const std::coroutine_handle<promise_type> &handle) {
    Task::addTask(new CoTask{handle, _promise});
  }

  auto &future() { return _future; }

  auto &future() const { return _future; }

  bool ready() const {
    return _future.wait_for(std::chrono::seconds(0)) ==
           std::future_status::ready;
  }

  bool await_ready() { return ready(); }

  template <class Any>
  void await_suspend(const std::coroutine_handle<Any> &handle) {
    while(!ready()){
      Task::nextTask();
    }
  }

  T await_resume() { return _future.get(); }
};