#pragma once
#include "util/Interface.hpp"
class ITask : public Interface {
public:
  virtual bool next() = 0;
  virtual void destroy() = 0;
};