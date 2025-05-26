#pragma once
#include "runtime/IModule.hpp"
#include "runtime/System.hpp"
#include "util/Singleton.hpp"
#include "util/TaskRunner.hpp"
#include <map>
#include <string>
#include <vector>
class Application {
private:
  std::vector<std::string> _args;
  std::map<std::string, IModule *> _modules;

public:
  auto addSystem(auto &&fn) {
    auto &runner = Singleton<TaskRunner>::get();
    runner->addTask(new System(std::function(fn)));
    return this;
  }

  template <class T> auto addModule(T *module) {
    _modules.insert(typeid(T).name(), module);
    return this;
  }

  int run(int argc, char *argv[]);

  inline const std::vector<std::string> &getArgs() const { return _args; };
};