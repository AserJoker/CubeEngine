#include "util/ProxyTask.hpp"
#include <functional>
ProxyTask::ProxyTask(const std::function<bool()> &function)
    : _function(function) {}

bool ProxyTask::next() { return _function(); }

void ProxyTask::destroy() {}