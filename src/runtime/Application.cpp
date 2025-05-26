#include "runtime/Application.hpp"
#include "util/Singleton.hpp"
#include "util/TaskRunner.hpp"
int Application::run(int argc, char *argv[]) {
  for (int i = 0; i < argc; i++) {
    _args.push_back(argv[i]);
  }
  for (auto &module : _modules) {
    module->setup(this);
  }
  auto &runner = Singleton<TaskRunner>::get();
  runner->runTaskLoop();
  for (auto &module : _modules) {
    module->cleanup();
  }
  _modules.clear();
  return 0;
}