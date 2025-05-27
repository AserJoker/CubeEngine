#pragma once
#include "util/ITask.hpp"
#include <vector>
class TaskRunner {
private:
  std::vector<ITask *> _tasks;
  bool _running;

public:
  void addTask(ITask *task);
  bool runTask();
  bool runSafeTask();
  void runTaskLoop();
  void stop();
};