#pragma once
class Application;
class IModule {
public:
  virtual ~IModule() = default;

  virtual void setup(Application *app) = 0;

  virtual void cleanup() = 0;
};