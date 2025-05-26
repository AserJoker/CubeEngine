#pragma once
#include "util/ITask.hpp"
#include <functional>
class ProxyTask : public ITask {
private:
  std::function<bool()> _function;

public:
  ProxyTask(const std::function<bool()> &function);

  bool next() override;

  void destroy() override;
};