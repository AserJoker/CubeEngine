#include "runtime/ResourceManager.hpp"
#include "core/Buffer.hpp"
#include "core/ImageLoader.hpp"
#include "core/JsonLoader.hpp"
#include "core/LangLoader.hpp"
#include "core/Object.hpp"
#include "core/TomlLoader.hpp"
#include <exception>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

using namespace cube;
using namespace cube::runtime;
ResourceManager::ResourceManager() {
  _loaders[".toml"] = std::make_shared<core::TomlLoader>();
  _loaders[".json"] = std::make_shared<core::JsonLoader>();
  _loaders[".lang"] = std::make_shared<core::LangLoader>();
  auto imgLoader = std::make_shared<core::ImageLoader>();
  _loaders[".jpg"] = imgLoader;
  _loaders[".jpeg"] = imgLoader;
  _loaders[".png"] = imgLoader;
  _loaders[".bmp"] = imgLoader;
  _loaders[".tga"] = imgLoader;
  _loaders[".gif"] = imgLoader;
  _loaders[".webp"] = imgLoader;
}
void ResourceManager::setDomain(const std::string &name,
                                const std::string &path) {
  if (path.ends_with("/")) {
    _domains[name] = path;
  } else {
    _domains[name] = path + "/";
  }
}
const std::string &ResourceManager::getDomain(const std::string &name,
                                              const std::string &def) const {
  if (_domains.contains(name)) {
    return _domains.at(name);
  }
  return def;
}
std::shared_ptr<core::Object>
ResourceManager::load(const std::string &domain,
                      const std::string &path) const {
  if (!_domains.contains(domain)) {
    _logger->error("Failed to load resource '{}': unknown domain '{}'", path,
                   domain);
    return nullptr;
  }
  std::string domain_path = _domains.at(domain);
  std::string fullpath = domain_path + path;
  if (!std::filesystem::exists(fullpath)) {
    _logger->error("Failed to load resource '{}:{}': file is not exist", domain,
                   path);
    return nullptr;
  }
  std::filesystem::path p = path;
  if (p.has_extension() && _loaders.contains(p.extension().string())) {
    auto type = p.extension().string();
    auto &loader = _loaders.at(type);
    try {
      return loader->resolve(fullpath);
    } catch (std::exception &e) {
      _logger->error("Failed to load '{}:{}': {}", domain, path, e.what());
    } catch (...) {
      _logger->error("Failed to load '{}:{}': unknown exception", domain, path);
    }
    return nullptr;
  }
  std::ifstream file(fullpath);
  if (!file.is_open()) {
    _logger->error("Failed to load resource '{}:{}': invalid file", domain,
                   path);
    return nullptr;
  }
  file.seekg(0, std::ios::end);
  size_t size = file.tellg();
  file.seekg(0, std::ios::beg);
  std::shared_ptr<core::Buffer> buf = std::make_shared<core::Buffer>(size);
  file.read((char *)buf->getData(), size);
  file.close();
  return buf;
}
bool ResourceManager::save(const std::string &domain, const std::string &path,
                           const std::shared_ptr<core::Buffer> &buffer) const {
  if (!_domains.contains(domain)) {
    _logger->error("Failed to save resource '{}': unknown domain '{}'", path,
                   domain);
    return false;
  }
  auto domain_path = _domains.at(domain);
  auto current_path = domain_path + path;
  std::filesystem::path p = current_path;
  auto dir = p.parent_path();
  if (std::filesystem::exists(dir) && !std::filesystem::is_directory(dir)) {
    _logger->error("Failed to save resource '{}:{}': invalid path", domain,
                   path);
    return false;
  }
  if (!std::filesystem::exists(dir)) {
    if (!std::filesystem::create_directory(dir)) {
      _logger->error("Failed to save resource '{}:{}': invalid url", domain,
                     path);
      return false;
    }
  }
  std::ofstream file(p);
  if (!file.is_open()) {
    _logger->error("Failed to save resource '{}:{}': invalid file", domain,
                   path);
    return false;
  }
  if (buffer && buffer->getSize()) {
    file.write((const char *)buffer->getData(), buffer->getSize());
  }
  file.close();
  return true;
}
std::vector<std::string> ResourceManager::query(const std::string &domain,
                                                const std::string &url) const {
  if (!_domains.contains(domain)) {
    _logger->error("Failed to query resource '{}': unknown domain '{}'", url,
                   domain);
    return {};
  }
  auto domain_path = _domains.at(domain);
  auto current_path = domain_path + url;
  std::filesystem::path p = current_path;
  if (!std::filesystem::exists(p) || !std::filesystem::is_directory(p)) {
    return {};
  }
  std::vector<std::string> result;
  for (auto &item : std::filesystem::directory_iterator(p)) {
    result.push_back(item.path().filename().string());
  }
  return result;
}