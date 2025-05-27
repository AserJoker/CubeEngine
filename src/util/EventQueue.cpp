#include "util/EventQueue.hpp"
void EventQueue::pushEvent(const std::shared_ptr<IEvent> &event) {
  _events.push_front(event);
}

const std::shared_ptr<IEvent> &EventQueue::peekEvent() const {
  return _events.back();
}

std::shared_ptr<IEvent> EventQueue::pollEvent() {
  if (_events.empty()) {
    return nullptr;
  }
  auto event = _events.back();
  _events.pop_back();
  return event;
}

bool EventQueue::hasEvent() const { return !_events.empty(); }
bool EventQueue::is(const std::string &type) const {
  return !_events.empty() && _events.back()->getType() == type;
}