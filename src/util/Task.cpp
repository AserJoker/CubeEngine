#include "util/Task.hpp"
bool Task::nextTask() {
  auto &tasks = getTasks();
  if (tasks.empty()) {
    return true;
  }
  auto task = *tasks.begin();
  tasks.erase(tasks.begin());
  if (task->next()) {
    task->destroy();
  } else {
    tasks.push_back(task);
  }
  return false;
}

void Task::runTask() {
  while (!nextTask())
    ;
}