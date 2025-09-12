#include "runtime/AssetManager.hpp"
using namespace cube;
using namespace cube::runtime;

const std::shared_ptr<core::Object> &
AssetManager::query(const std::string &name) {
  if (_assets.contains(name)) {
    return _assets.at(name);
  }
  size_t pos = name.find(":");
  if (pos == std::string::npos) {
    return _nil;
  }
  std::string domain = name.substr(0, pos);
  std::string path = name.substr(pos + 1);
  if (_assets.contains(name)) {
    return _assets.at(name);
  }
  auto obj = _resource->load(domain, std::format("assets/{}", path));
  if (obj) {
    _assets[name] = obj;
    return _assets[name];
  }
  return _nil;
}
bool AssetManager::remove(const std::string &name) {
  if (_assets.contains(name)) {
    _assets.erase(name);
    return true;
  }
  return false;
}

void AssetManager::clear() { _assets.clear(); }