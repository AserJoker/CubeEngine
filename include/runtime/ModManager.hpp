#pragma once
#include "core/Object.hpp"
#include "core/Version.hpp"
#include "runtime/LocaleManager.hpp"
#include "runtime/ResourceManager.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace cube::runtime {
class ModManager : public core::Object {
public:
  struct Manifest {
    std::string path;
    std::string name;
    core::Version version;
    std::string displayName;
    std::string description;
    std::string license;
    std::string author;
    std::string icon;
    core::Version engine;
    std::unordered_map<std::string, core::Version> dependences;
    std::unordered_map<std::string, std::string> locales;
    std::string status;
  };

private:
  std::shared_ptr<ResourceManager> _resource;
  std::shared_ptr<LocaleManager> _locale;
  Logger *_logger = Logger::getLogger("ModManager");
  std::unordered_map<std::string, Manifest> _mods;

private:
  void scanManifests();
  void loadManifest(const Manifest &manifest,
                    const std::vector<std::string> &paths = {});

public:
  ModManager();
  void load();
  const std::unordered_map<std::string, Manifest> &getMods() const;
};
}; // namespace cube::runtime