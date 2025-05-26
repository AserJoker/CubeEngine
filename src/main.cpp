#include "runtime/Application.hpp"
#include <print>
#include <stdexcept>

void printArgs(std::unique_ptr<Application> &theApp) {
  auto &args = theApp->getArgs();
  for (auto &arg : args) {
    std::println("{}", arg);
    throw std::runtime_error("Test error");
  }
}

int main(int argc, char *argv[]) {
  return Singleton<Application>::get()->addSystem(printArgs)->run(argc, argv);
}