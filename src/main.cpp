#include "runtime/Application.hpp"
#include <memory>
#include <print>

int main(int argc, char *argv[]) {
  return Singleton<Application>::get()
      ->addSystem([](std::unique_ptr<Application> &theApp) {
        for (auto &arg : theApp->getArgs()) {
          std::println("arg: {}", arg);
        }
      })
      ->run(argc, argv);
}