#include "runtime/Application.hpp"
#include "util/Singleton.hpp"
#include "util/TaskRunner.hpp"
int Application::run(int argc, char *argv[]) {
  for (int i = 0; i < argc; i++) {
    _args.push_back(argv[i]);
  }
  auto &runner = Singleton<TaskRunner>::get();
  runner->runTaskLoop();
  return 0;
}