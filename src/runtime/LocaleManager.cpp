#include "runtime/LocaleManager.hpp"
#include "core/Buffer.hpp"
#include <memory>
#include <sstream>
#include <string>
using namespace cube;
using namespace cube::runtime;

bool parse(std::shared_ptr<AssetManager> asset, const std::string &file,
           std::unordered_map<std::string, std::string> &locales) {
  auto buf = std::dynamic_pointer_cast<core::Buffer>(asset->query(file));
  if (!buf) {
    return false;
  }
  std::string source((const char *)buf->getData(), buf->getSize());
  std::stringstream ss(source);
  std::string line;
  while (std::getline(ss, line)) {
    for (auto &ch : line) {
      if (ch == '\t') {
        ch = ' ';
      }
    }
    line.erase(0, line.find_first_not_of(' '));
    line.erase(line.find_last_not_of(' ') + 1);
    size_t pos = line.find('#');
    if (pos != std::string::npos) {
      line = line.substr(0, pos);
    }
    if (line.empty()) {
      continue;
    }
    pos = line.find('=');
    if (pos == std::string::npos) {
      return false;
    }
    std::string name = line.substr(0, pos);
    std::string value = line.substr(pos + 1);
    name.erase(name.find_last_not_of(' ') + 1);
    value.erase(0, value.find_first_not_of(' '));
    if (value[0] == '"') {
      value = value.substr(1, value.length() - 2);
    }
    locales[name] = value;
  }
  return true;
}
bool LocaleManager::setDefaultLanguage(const std::string &language) {
  if (_languages.contains(language)) {
    _locales.clear();
    for (auto &file : _languages[language]) {
      if (!parse(_asset, file, _defs)) {
        _logger->warn("Failed to load locale: {}", file);
      }
    }
    _def = language;
    return true;
  }
  return false;
}
bool LocaleManager::setLanguage(const std::string &language) {
  if (_languages.contains(language)) {
    _locales.clear();
    for (auto &file : _languages[language]) {
      if (!parse(_asset, file, _locales)) {
        _logger->warn("Failed to load locale: {}", file);
      }
    }
    _language = language;
    return true;
  }
  return false;
}
std::string
LocaleManager::i18n(const std::string &key,
                    const std::unordered_map<std::string, std::string> &args) {
  std::string res = key;
  if (_locales.contains(key)) {
    res = _locales.at(key);
  } else if (_defs.contains(key)) {
    res = _defs.at(key);
  }
  for (auto &[k, v] : args) {
    std::string format = "{" + k + "}";
    size_t pos = res.find(format);
    while (pos != std::string::npos) {
      res.replace(pos, format.length(), v);
      pos = res.find(format);
    }
  }
  return res;
}
const std::unordered_map<std::string, std::vector<std::string>> &
LocaleManager::getLanguages() const {
  return _languages;
}
void LocaleManager::registerLocale(const std::string &language,
                                   const std::string &file) {
  auto &languages = _languages[language];
  if (std::find(languages.begin(), languages.end(), file) == languages.end()) {
    _languages[language].push_back(file);
  }
}
const std::string &LocaleManager::getLanguage() const { return _language; }