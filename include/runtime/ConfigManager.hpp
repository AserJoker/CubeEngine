#pragma once
#include "ResourceManager.hpp"
#include "core/Object.hpp"
#include "core/Singleton.hpp"
#include "core/Variable.hpp"
#include "runtime/Logger.hpp"
#include <memory>
#include <string>
#include <unordered_map>
namespace cube::runtime {
class ConfigManager : public core::Object {

private:
  std::shared_ptr<ResourceManager> _resource =
      core::Singleton<ResourceManager>::get();
  Logger *_logger = Logger::getLogger("ConfigManager");

  std::unordered_map<std::string, core::Variable> _configs;
  core::Variable _nil;

public:
  core::Variable &query(const std::string &name);
  bool save(const std::string &name);
  bool remove(const std::string &name);
  void clear();
};
} // namespace cube::runtime