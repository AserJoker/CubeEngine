#pragma once
namespace cube::core {
class Object {
public:
  virtual ~Object() = default;
  template <class T> T *cast() { return dynamic_cast<T *>(this); }
};
}; // namespace cube::core