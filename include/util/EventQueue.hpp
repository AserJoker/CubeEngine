#pragma once
#include "util/IEvent.hpp"
#include <list>
#include <memory>
#include <string>
class EventQueue {
private:
  std::list<std::shared_ptr<IEvent>> _events;

public:
  template <class E, class... Args> void pushEvent(Args... args) {
    _events.emplace_back(std::make_shared<E>(args...));
  }
  void pushEvent(const std::shared_ptr<IEvent> &event);
  const std::shared_ptr<IEvent> &peekEvent() const;
  std::shared_ptr<IEvent> pollEvent();
  bool hasEvent() const;
  bool is(const std::string &type) const;
};