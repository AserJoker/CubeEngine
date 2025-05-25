#include "runtime/Application.hpp"
#include "util/Task.hpp"
int32_t Application::run(int argc, char *argv[]) {
  for (int idx = 0; idx < argc; idx++) {
    _args.push_back(argv[idx]);
  }
  for (auto &m : _modules) {
    m->setup(this);
  }
  Task::runTask();
  for (auto &m : _modules) {
    m->cleanup();
  }
  return 0;
}

const std::vector<std::string> &Application::getArgs() const { return _args; }
