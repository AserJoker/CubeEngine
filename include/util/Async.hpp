#pragma once
#include "util/CoTask.hpp"
#include "util/PromiseType.hpp"
#include "util/Singleton.hpp"
#include "util/TaskRunner.hpp"
#include <chrono>
#include <coroutine>
#include <future>
template <class T> class Async {
private:
  std::future<T> _future;

public:
  using promise_type = PromiseType<T>;

public:
  Async(const std::coroutine_handle<promise_type> &handle) {
    auto &runner = Singleton<TaskRunner>::get();
    auto task = new CoTask(handle);
    runner->addTask(task);
    _future = task->future();
  }
  bool await_ready() {
    return _future.wait_for(std::chrono::seconds(0)) ==
           std::future_status::ready;
  }

  void await_suspend(const auto &) {
    auto &runner = Singleton<TaskRunner>::get();
    while (!await_ready()) {
      runner->runSafeTask();
    }
  }

  T await_resume() { return _future.get(); }
};