#include "core/JsonLoader.hpp"
#include "core/Object.hpp"
#include "core/Variable.hpp"
#include <fstream>
#include <memory>
using namespace cube;
using namespace cube::core;
std::shared_ptr<core::Object> JsonLoader::resolve(const std::string &filename) {
  std::ifstream in(filename);
  if (!in.is_open()) {
    throw std::runtime_error("Failed to open file: " + filename);
  }
  size_t pos = in.tellg();
  in.seekg(0, std::ios::end);
  size_t size = (size_t)in.tellg() - pos;
  in.seekg(pos, std::ios::beg);
  std::string source(size, '\0');
  in.read(source.data(), size);
  in.close();
  std::shared_ptr<core::Variable> var = std::make_shared<core::Variable>();
  var->parseJson(source, "json");
  return var;
}