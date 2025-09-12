#include "runtime/ModManager.hpp"
#include "core/Buffer.hpp"
#include "core/Singleton.hpp"
#include "core/Variable.hpp"
#include "runtime/Application.hpp"
#include "runtime/LocaleManager.hpp"
#include "runtime/ResourceManager.hpp"
#include <algorithm>
#include <exception>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
using namespace cube;
using namespace cube::runtime;
ModManager::ModManager() {
  _locale = core::Singleton<LocaleManager>::get();
  _resource = core::Singleton<ResourceManager>::get();
}
const std::unordered_map<std::string, ModManager::Manifest> &
ModManager::getMods() const {
  return _mods;
}
void ModManager::scanManifests() {
  auto app = core::Singleton<Application>::get();
  auto root = _resource->getDomain(app->getApplicationName());
  root += "mods";
  if (!std::filesystem::exists(root) || !std::filesystem::is_directory(root)) {
    return;
  }
  for (auto &item : std::filesystem::directory_iterator(root)) {
    auto dirname = item.path().filename().string();
    auto manifest = std::format("mods/{}/manifest.json", dirname);
    auto buf = std::dynamic_pointer_cast<core::Buffer>(
        _resource->load(app->getApplicationName(), manifest));
    if (buf) {
      std::string source((const char *)buf->getData(), buf->getSize());
      core::Variable val;
      try {
        val.parseJson(source, dirname);
        if (!val.getField("name").isString() ||
            val.getField("name").getString().empty()) {
          throw std::runtime_error(
              "invalid manifest, field 'name' is required");
        }
        auto name = val.getField("name").getString();
        auto &manifest = _mods[name];
        manifest.name = val.getField("name").getString();
        manifest.path = std::format("mods/{}/", dirname);
        manifest.displayName =
            val.getField("displayName").getString(manifest.name);
        manifest.version = val.getField("version").getString("0.1.0-unknown");
        if (manifest.version.ext == "error") {
          throw std::runtime_error(
              "invalid manifest, field 'version' is malformed");
        }
        manifest.author = val.getField("author").getString();
        manifest.license = val.getField("license").getString();
        manifest.icon = val.getField("icon").getString();
        manifest.engine = val.getField("engine").getString("0.1.0");
        if (manifest.engine.ext == "error") {
          throw std::runtime_error(
              "invalid manifest, field 'engine' is malformed");
        }
        if (val.hasField("dependences") &&
            val.getField("dependences").isTable()) {
          auto &dependences = val.getField("dependences").getTable();
          for (auto &[dep, version] : dependences) {
            manifest.dependences[dep] = version.getString();
            if (manifest.dependences[dep].ext == "error") {
              throw std::runtime_error(std::format(
                  "invalid manifest, dependence '{}' is malformed", dep));
            }
          }
        }
        if (val.hasField("locales") && val.getField("locales").isTable()) {
          auto &locales = val.getField("locales").getTable();
          for (auto &[lang, file] : locales) {
            manifest.locales[lang] = file.getString();
          }
        }
        manifest.status = "init";
        _mods[manifest.name] = manifest;
      } catch (std::exception &e) {
        _logger->error("Failed to load mod manifest: {}", e.what());
      }
    }
  }
}

void ModManager::load() {
  scanManifests();
  for (auto &[_, manifest] : _mods) {
    loadManifest(manifest);
  }
}

void ModManager::loadManifest(const Manifest &manifest,
                              const std::vector<std::string> &paths) {
  if (manifest.status == "ready") {
    return;
  }
  auto it = std::find(paths.begin(), paths.end(), manifest.name);
  if (it != paths.end()) {
    std::string message =
        std::format("Failed to load mod '{}', there is a circular dependency: ",
                    manifest.name);
    while (it != paths.end()) {
      message += *it;
      message += " -> ";
    }
    message += manifest.name;
    throw std::runtime_error(message);
  }
  for (auto &[name, version] : manifest.dependences) {
    if (!_mods.contains(name)) {
      throw std::runtime_error(std::format(
          "Missing mod '{}',dependent by '{}' ", name, manifest.name));
    }
    auto &dep = _mods.at(name);
    if (version > dep.version) {
      throw std::runtime_error(std::format(
          "Mod '{}' version is too low, required '{}',got '{}'", manifest.name,
          version.toString(), manifest.version.toString()));
    }
    std::vector<std::string> next = paths;
    next.push_back(manifest.name);
    loadManifest(dep, next);
  }
  auto app = core::Singleton<Application>::get();
  auto root = _resource->getDomain(app->getApplicationName());
  _resource->setDomain(manifest.name,
                       std::format("{}/{}", root, manifest.path));
  for (auto &[locale, file] : manifest.locales) {
    _locale->registerLocale(locale, file);
  }
}