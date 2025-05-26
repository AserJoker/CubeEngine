#include "runtime/Application.hpp"
#include <memory>
#include <print>

int main(int argc, char *argv[]) {
  return Singleton<Application>::get()->run(argc, argv);
}