#pragma once
#include "util/Interface.hpp"
class Application;
class IModule : public Interface {
public:
  virtual void setup(Application *) = 0;
  virtual void cleanup() = 0;
  virtual void initialize() {};
};