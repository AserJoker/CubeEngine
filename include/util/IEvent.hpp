#pragma once
#include "util/Interface.hpp"
#include "util/TString.hpp"
class IEvent : public Interface {
public:
  virtual const char *getType() const = 0;
};

template <TString type> class Event : public IEvent {
public:
  const char *getType() const { return type.value; }
};