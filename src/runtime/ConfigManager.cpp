#include "runtime/ConfigManager.hpp"
#include "core/Buffer.hpp"
#include "core/Singleton.hpp"
#include "core/Variable.hpp"
#include "runtime/Application.hpp"
#include <exception>
#include <memory>
using namespace cube;
using namespace cube::runtime;
core::Variable &ConfigManager::query(const std::string &name) {
  size_t pos = name.find(":");
  if (pos == std::string::npos) {
    return _nil;
  }
  std::string domain = name.substr(0, pos);
  std::string path = name.substr(pos + 1);
  auto app = core::Singleton<Application>::get();
  if (!_configs.contains(name)) {
    auto src = std::dynamic_pointer_cast<core::Buffer>(_resource->load(
        app->getApplicationName(), std::format("configs/{}/{}", domain, path)));
    _configs[name] = {};
    if (src) {
      std::string source((const char *)src->getData(), src->getSize());
      try {
        _configs[name].parseToml(source, path);
      } catch (std::exception &error) {
        _logger->error("Failed to load config '{}' :{}", name, error.what());
      } catch (...) {
        _logger->error("Failed to load config '{}' :Unknown exception", name);
      }
    }
  }
  return _configs[name];
}

bool ConfigManager::save(const std::string &name) {
  size_t pos = name.find(":");
  if (pos == std::string::npos) {
    _logger->error("Invalid config name: {}", name);
    return false;
  }
  std::string domain = name.substr(0, pos);
  std::string path = name.substr(pos + 1);
  std::shared_ptr<core::Buffer> buf;
  if (_configs.contains(name)) {
    auto &cfg = _configs.at(name);
    auto src = cfg.toToml();
    buf = std::make_shared<core::Buffer>(src.size(), src.data());
  }
  auto app = core::Singleton<Application>::get();
  _resource->save(app->getApplicationName(),
                  std::format("configs/{}/{}", domain, path), buf);
  return true;
}
bool ConfigManager::remove(const std::string &name) {
  if (_configs.contains(name)) {
    _configs.erase(name);
    return true;
  }
  return false;
}
void ConfigManager::clear() { _configs.clear(); }