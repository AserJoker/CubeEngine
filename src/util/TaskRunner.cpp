#include "util/TaskRunner.hpp"
#include <exception>
#include <iostream>
#include <ostream>
void TaskRunner::addTask(ITask *task) { _tasks.push_back(task); }

bool TaskRunner::runTask() {
  if (_tasks.empty()) {
    return true;
  }
  auto task = *_tasks.begin();
  _tasks.erase(_tasks.begin());
  if (task->next()) {
    task->destroy();
    delete task;
  } else {
    _tasks.push_back(task);
  }
  return false;
}

bool TaskRunner::runSafeTask() {
  try {
    return runTask();
  } catch (std::exception &e) {
    std::println(std::cerr, "Uncaught error: {}", e.what());
  }
  return _tasks.empty();
}
void TaskRunner::runTaskLoop() {
  while (!_tasks.empty()) {
    runSafeTask();
  }
}