#include "core/Singleton.hpp"
#include "runtime/Application.hpp"

using namespace cube;

auto main(int argc, char *argv[]) -> int {
  return core::Singleton<runtime::Application>::get()
      ->setApplicationInfo(APP_NAME, APP_VERSION, APP_ID)
      ->run(argc, argv);
};