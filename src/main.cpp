#include "runtime/Application.hpp"
#include <print>

void printArgs(std::unique_ptr<Application> &theApp) {
  auto &args = theApp->getArgs();
  for (auto &arg : args) {
    std::println("{}", arg);
  }
}

int main(int argc, char *argv[]) {
  return Singleton<Application>::get()
      ->addSystem(printArgs)
      ->addSystem(printArgs)
      ->addSystem(printArgs)
      ->addSystem(printArgs)
      ->run(argc, argv);
}