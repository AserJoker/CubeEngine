#pragma once
#include <string>
namespace cube::core {
struct Version {
  int major{};
  int minor{};
  int patch{};
  std::string ext;
  Version(const std::string &version);
  Version();
  bool operator>(const Version &another) const;
  std::string toString() const;
};
}; // namespace cube::core