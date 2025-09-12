#include "core/Buffer.hpp"
#include <algorithm>
#include <cstring>
#include <vcruntime_new.h>
using namespace cube;
using namespace cube::core;

Buffer::Buffer(size_t size, void *data) : _size(size), _data(nullptr) {
  if (size) {
    _data = ::operator new(size);
    std::memset(_data, 0, size);
    if (data) {
      std::memcpy(_data, data, size);
    }
  }
}
Buffer::~Buffer() {
  if (_data) {
    ::operator delete(_data);
    _data = nullptr;
    _size = 0;
  }
}
const void *Buffer::getData() const { return _data; }
size_t Buffer::getSize() const { return _size; }
void *Buffer::getData() { return _data; }
void Buffer::resize(size_t size) {
  if (_size != size) {
    void *data = ::operator new(size);
    std::memset(data, 0, size);
    if (_data) {
      std::memcpy(data, _data, std::max(size, _size));
      ::operator delete(_data);
    }
    _data = data;
    _size = size;
  }
}