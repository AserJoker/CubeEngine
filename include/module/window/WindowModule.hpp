#pragma once
#include "runtime/IModule.hpp"
class WindowModule : public IModule {
public:
  void setup(Application *app) override;
  void cleanup() override;
};