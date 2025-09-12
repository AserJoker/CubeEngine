#pragma once
#include "core/Object.hpp"
namespace cube::core {
class Buffer : public Object {
private:
  void *_data;
  size_t _size;

public:
  Buffer(size_t size = 0, void *data = nullptr);
  ~Buffer() override;
  const void *getData() const;
  size_t getSize() const;
  void *getData();
  void resize(size_t size);
};
} // namespace cube::core