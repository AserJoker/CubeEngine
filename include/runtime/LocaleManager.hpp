#pragma once
#include "core/Object.hpp"
#include "core/Singleton.hpp"
#include "runtime/AssetManager.hpp"
#include <string>
#include <unordered_map>
#include <vector>
namespace cube::runtime {
class LocaleManager : public core::Object {
private:
  std::unordered_map<std::string, std::string> _locales;
  std::unordered_map<std::string, std::string> _defs;
  std::unordered_map<std::string, std::vector<std::string>> _languages;
  std::string _language;
  std::string _def;
  std::shared_ptr<AssetManager> _asset = core::Singleton<AssetManager>::get();
  Logger *_logger = Logger::getLogger("LocaleManager");

public:
  bool setDefaultLanguage(const std::string &language);
  bool setLanguage(const std::string &language);
  std::string
  i18n(const std::string &key,
       const std::unordered_map<std::string, std::string> &args = {});
  const std::unordered_map<std::string, std::vector<std::string>> &
  getLanguages() const;
  void registerLocale(const std::string &language, const std::string &file);
  const std::string &getLanguage() const;
};
} // namespace cube::runtime