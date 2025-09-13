#include "core/LangLoader.hpp"
#include "core/Variable.hpp"
#include <fstream>
#include <string>
using namespace cube;
using namespace cube::core;
std::shared_ptr<core::Object> LangLoader::resolve(const std::string &filename) {
  std::ifstream in(filename);
  if (!in.is_open()) {
    throw std::runtime_error("Failed to open file: " + filename);
  }
  std::shared_ptr<core::Variable> var = std::make_shared<core::Variable>();
  var->setTable();
  auto &table = var->getTable();
  std::string line;
  while (std::getline(in, line)) {
    size_t pos = line.find('#');
    if (pos != std::string::npos) {
      line = line.substr(0, pos);
    }
    line.erase(0, line.find_first_not_of(" \t\r\n"));
    line.erase(line.find_last_not_of(" \t\r\n") + 1);
    if (line.empty()) {
      continue;
    }
    pos = line.find("=");
    if (pos == std::string::npos) {
      continue;
    }
    std::string key = line.substr(0, pos);
    std::string value = line.substr(pos + 1);
    // trim
    key.erase(0, key.find_first_not_of(" \t\r\n"));
    key.erase(key.find_last_not_of(" \t\r\n") + 1);
    value.erase(0, value.find_first_not_of(" \t\r\n"));
    value.erase(value.find_last_not_of(" \t\r\n") + 1);
    if (value.starts_with("\"") && value.ends_with("\"") && value.size() >= 2) {
      value = value.substr(1, value.size() - 2);
    }
    if (key.empty()) {
      continue;
    }
    table[key] = core::Variable().setString(value);
  }
  in.close();
  return var;
}