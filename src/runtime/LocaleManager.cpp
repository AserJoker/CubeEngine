#include "runtime/LocaleManager.hpp"
#include "core/Variable.hpp"
#include <memory>
#include <string>
using namespace cube;
using namespace cube::runtime;

bool parse(std::shared_ptr<AssetManager> asset, const std::string &file,
           std::unordered_map<std::string, std::string> &locales) {
  auto lang = std::dynamic_pointer_cast<core::Variable>(asset->query(file));
  if (!lang) {
    return false;
  }
  if (!lang->isTable()) {
    return false;
  }
  for (auto &[k, v] : lang->getTable()) {
    if (v.isString()) {
      locales[k] = v.getString();
    }
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