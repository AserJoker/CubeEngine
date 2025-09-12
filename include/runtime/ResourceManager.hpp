#pragma once
#include "Loader.hpp"
#include "core/Buffer.hpp"
#include "core/Object.hpp"
#include "runtime/Logger.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
namespace cube::runtime {
class ResourceManager : public core::Object {
private:
  std::unordered_map<std::string, std::shared_ptr<Loader>> _loaders;
  std::unordered_map<std::string, std::string> _domains;
  Logger *_logger = Logger::getLogger("ResourceManager");

public:
  void setDomain(const std::string &name, const std::string &path);
  const std::string &getDomain(const std::string &name,
                               const std::string &def = "./") const;
  std::shared_ptr<core::Object> load(const std::string &domain,
                                     const std::string &url) const;
  bool save(const std::string &domain, const std::string &url,
            const std::shared_ptr<core::Buffer> &buffer = nullptr) const;
  std::vector<std::string> query(const std::string &domain,
                                 const std::string &url) const;
};
} // namespace cube::runtime