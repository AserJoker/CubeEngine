#pragma once
#include "runtime/IModule.hpp"
#include "runtime/System.hpp"
#include "util/Singleton.hpp"
#include "util/TaskRunner.hpp"
#include <string>
#include <vector>
class Application {
private:
  std::vector<std::string> _args;
  std::vector<IModule *> _modules;

public:
  auto addSystem(auto &&fn) {
    auto &runner = Singleton<TaskRunner>::get();
    runner->addTask(new System(std::function(fn)));
    return this;
  }

  template <class T, class... Args> auto addModule(Args... args) {
    auto &module = Singleton<T>::get();
    auto it = std::find(_modules.begin(), _modules.end(), module.get());
    if (it != _modules.end()) {
      return this;
    }
    module->initialize(args...);
    _modules.push_back(module.get());
    return this;
  }

  int run(int argc, char *argv[]);

  inline const std::vector<std::string> &getArgs() const { return _args; };
};