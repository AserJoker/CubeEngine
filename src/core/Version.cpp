#include "core/Version.hpp"
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <format>
#include <string>
using namespace cube;
using namespace cube::core;
Version::Version() {}
Version::Version(const std::string &str) {
  std::string v = str;
  if (v.starts_with('v')) {
    v = v.substr(1);
  }
  size_t pos = v.find('-');
  if (pos != std::string::npos) {
    ext = v.substr(pos + 1);
    v = v.substr(0, pos);
  }
  pos = v.find(".");
  std::string major;
  std::string minor;
  std::string patch;
  if (pos == std::string::npos) {
    major = v;
    minor = "0";
    patch = "0";
  } else {
    major = v.substr(0, pos);
    v = v.substr(pos + 1);
    pos = v.find(".");
    if (pos == std::string::npos) {
      minor = v.substr(0, pos);
      patch = v.substr(pos + 1);
    } else {
      minor = v;
      patch = "0";
    }
  }
  try {
    this->major = std::stoi(major);
    this->minor = std::stoi(minor);
    this->patch = std::stoi(patch);
  } catch (std::exception &e) {
    this->ext = "error";
  }
}
bool Version::operator>(const Version &another) const {
  if (major > another.major) {
    return true;
  }
  if (major < another.major) {
    return false;
  }
  if (minor > another.minor) {
    return true;
  }
  if (minor < another.minor) {
    return false;
  }
  return patch > another.patch;
}
std::string Version::toString() const {
  std::string str = std::format("v{}.{}.{}", major, minor, patch);
  if (!ext.empty()) {
    str += "-" + ext;
  }
  return str;
}