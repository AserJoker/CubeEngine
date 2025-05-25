#pragma once
#include "runtime/IModule.hpp"
#include "runtime/System.hpp"
#include "util/Generator.hpp"
#include <memory>
#include <string>
#include <vector>
class Application {
private:
  std::vector<std::string> _args;
  std::vector<IModule *> _modules;

public:
  template <class Ret, class... Args>
  Application *addSystem(Ret (*func)(std::unique_ptr<Args> &...)) {
    Task::addTask(new System(func));
    return this;
  }

  template <class Ret, class... Args>
  Application *
  addSystem(const std::function<Ret(std::unique_ptr<Args> &...)> &func) {
    Task::addTask(new System(func));
    return this;
  }

  template <class... Args>
  Application *addSystem(Generator<Void> (*func)(Args...)) {
    func(System::inject<Args>()...);
    return this;
  }

  template <class Module> Application *addModule() {
    _modules.push_back(Singleton<Module>::get().get());
    return this;
  }

  int32_t run(int argc, char *argv[]);

  const std::vector<std::string> &getArgs() const;
};