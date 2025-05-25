#pragma once
#include <vector>
class Task {
private:
  static auto &getTasks() {
    static std::vector<Task *> tasks;
    return tasks;
  }

public:
  virtual ~Task() = default;
  virtual bool next() = 0;
  virtual void destroy() = 0;

public:
  static bool nextTask();
  static void runTask();
  static inline void addTask(Task *task) { getTasks().push_back(task); }
};
