#pragma once
#include "core/Object.hpp"
#include "core/Singleton.hpp"
#include "runtime/ResourceManager.hpp"
#include <memory>
#include <string>
#include <unordered_map>
namespace cube::runtime {
class AssetManager : public core::Object {
private:
  std::shared_ptr<ResourceManager> _resource =
      core::Singleton<ResourceManager>::get();
  Logger *_logger = Logger::getLogger("AssetManager");
  std::unordered_map<std::string, std::shared_ptr<core::Object>> _assets;
  std::shared_ptr<core::Object> _nil;

public:
  const std::shared_ptr<core::Object> &query(const std::string &name);
  bool remove(const std::string &name);
  const std::shared_ptr<core::Object> &clear(const std::string &name);

  void clear();
};
} // namespace cube::runtime