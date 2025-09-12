#include "core/Variable.hpp"
#include <any>
#include <cjson/cJSON.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <toml++/toml.h>

using namespace cube::core;
static Variable _nil;
const Variable::Type &Variable::getType() const { return _type; }
Variable &Variable::setNil() {
  _value = nullptr;
  _type = Type::NIL;
  return *this;
}
Variable &Variable::setString(const std::string &value) {
  _value = value;
  _type = Type::STRING;
  return *this;
}
Variable &Variable::setNumber(double value) {
  _value = value;
  _type = Type::NUMBER;
  return *this;
}
Variable &Variable::setInteger(int64_t value) {
  _value = value;
  _type = Type::INTEGER;
  return *this;
}
Variable &Variable::setBoolean(bool value) {
  _value = value;
  _type = Type::BOOLEAN;
  return *this;
}
Variable &Variable::setTable(const Table &value) {
  _value = value;
  _type = Type::TABLE;
  return *this;
}
Variable &Variable::setArray(const Array &value) {
  _value = value;
  _type = Type::ARRAY;
  return *this;
}

bool Variable::isNil() const { return _type == Type::NIL; }
bool Variable::isString() const { return _type == Type::STRING; }
bool Variable::isNumber() const { return _type == Type::NUMBER; }
bool Variable::isInteger() const { return _type == Type::INTEGER; }
bool Variable::isBoolean() const { return _type == Type::BOOLEAN; }
bool Variable::isTable() const { return _type == Type::TABLE; }
bool Variable::isArray() const { return _type == Type::ARRAY; }

const std::string &Variable::getString(const std::string &def) const {
  if (_type != Type::STRING) {
    return def;
  }
  return std::any_cast<const std::string &>(_value);
}
double Variable::getNumber(double def) const {
  if (_type != Type::NUMBER) {
    return def;
  }
  return std::any_cast<double>(_value);
}
int64_t Variable::getInteger(int64_t def) const {
  if (_type != Type::INTEGER) {
    return def;
  }
  return std::any_cast<int64_t>(_value);
}
bool Variable::getBoolean(bool def) const {
  if (_type != Type::BOOLEAN) {
    return def;
  }
  return std::any_cast<bool>(_value);
}
const Variable::Table &Variable::getTable(const Table &def) const {
  if (_type != Type::TABLE) {
    return def;
  }
  return std::any_cast<const Variable::Table &>(_value);
}
const Variable::Array &Variable::getArray(const Array &def) const {
  if (_type != Type::ARRAY) {
    return def;
  }
  return std::any_cast<const Variable::Array &>(_value);
}
Variable::Table &Variable::getTable() {
  if (_type != Type::TABLE) {
    setTable();
  }
  return std::any_cast<Variable::Table &>(_value);
}
Variable::Array &Variable::getArray() {
  if (_type != Type::ARRAY) {
    setArray();
  }
  return std::any_cast<Variable::Array &>(_value);
}
Variable &Variable::setField(const std::string &key, const Variable &value) {
  if (_type != Type::TABLE) {
    setTable();
  }
  auto &table = std::any_cast<Table &>(_value);
  table[key] = value;
  return *this;
}
Variable &Variable::setIndex(size_t idx, const Variable &value) {
  if (_type != Type::ARRAY) {
    setArray();
  }
  auto &array = std::any_cast<Array &>(_value);
  if (idx >= array.size()) {
    array.resize(idx + 1);
    array[idx] = value;
  }
  return *this;
}
const Variable &Variable::getField(const std::string &key) const {
  if (_type != Type::TABLE) {
    return _nil;
  }
  auto &table = std::any_cast<const Table &>(_value);
  if (!table.contains(key)) {
    return _nil;
  }
  return table.at(key);
}
const Variable &Variable::get(const std::string &key) const {
  if (_type != Type::TABLE) {
    return _nil;
  }
  std::string field = key;
  size_t pos = field.find(".");
  auto *val = this;
  while (pos != std::string::npos) {
    std::string name = field.substr(0, pos);
    val = &val->getField(name);
    field = field.substr(pos + 1);
    pos = field.find(".");
  }
  return val->getField(field);
}
const Variable &Variable::getIndex(size_t idx) const {
  if (_type != Type::ARRAY) {
    return _nil;
  }
  auto &array = std::any_cast<const Array &>(_value);
  if (idx >= array.size()) {
    return _nil;
  }
  return array.at(idx);
}
Variable &Variable::push(const Variable &variable) {
  if (_type != Type::ARRAY) {
    setArray();
  }
  auto &array = std::any_cast<Array &>(_value);
  array.push_back(variable);
  return *this;
}

Variable &Variable::getField(const std::string &key) {
  if (_type != Type::TABLE) {
    setTable();
  }
  auto &table = std::any_cast<Table &>(_value);
  return table[key];
}
Variable &Variable::getIndex(size_t idx) {
  if (_type != Type::ARRAY) {
    setArray();
  }
  auto &array = std::any_cast<Array &>(_value);
  if (idx >= array.size()) {
    array.resize(idx + 1);
  }
  return array[idx];
}
Variable &Variable::set(const std::string &key, const Variable &value) {
  if (_type != Type::TABLE) {
    setTable();
  }
  std::string field = key;
  size_t pos = field.find(".");
  auto *val = this;
  while (pos != std::string::npos) {
    std::string name = field.substr(0, pos);
    val = &val->getField(name);
    field = field.substr(pos + 1);
    pos = field.find(".");
  }
  val->setField(field, value);
  return *this;
}
bool Variable::hasField(const std::string &key) const {
  if (_type != Type::TABLE) {
    return false;
  }
  auto &table = std::any_cast<const Table &>(_value);
  return table.contains(key);
}
size_t Variable::getSize() const {
  if (_type != Type::ARRAY) {
    return (size_t)-1;
  }
  auto &array = std::any_cast<const Array &>(_value);
  return array.size();
}
Variable &Variable::delField(const std::string &key) {
  if (_type != Type::TABLE) {
    return *this;
  }
  auto &table = std::any_cast<Table &>(_value);
  table.erase(key);
  return *this;
}

static void resolveToml(Variable &variable, toml::node &node) {
  if (node.is_string()) {
    variable.setString(node.as_string()->get());
  } else if (node.is_number()) {
    variable.setNumber(node.as_floating_point()->get());
  } else if (node.is_boolean()) {
    variable.setBoolean(node.as_boolean()->get());
  } else if (node.is_array() || node.is_array_of_tables()) {
    variable.setArray();
    auto arr = node.as_array();
    for (auto &item : *arr) {
      variable.getArray().push_back({});
      resolveToml(variable.getArray().back(), item);
    }
  } else if (node.is_table()) {
    variable.setTable();
    for (auto &[key, value] : *node.as_table()) {
      resolveToml(variable.getField(key.data()), value);
    }
  }
}

Variable &Variable::parseToml(const std::string &source,
                              const std::string &filename) {
  try {
    auto node = toml::parse(source.c_str(), filename);
    resolveToml(*this, node);
  } catch (toml::parse_error &error) {
    std::stringstream ss;
    ss << error;
    throw std::runtime_error(ss.str());
  }
  return *this;
}

static void resolveToToml(toml::node &node, const Variable &variable) {
  if (variable.getType() == Variable::Type::TABLE) {
    auto &table = variable.getTable();
    for (auto &[key, value] : table) {
      switch (value.getType()) {
      case Variable::Type::STRING:
        node.as_table()->insert(key, value.getString());
        break;
      case Variable::Type::INTEGER:
        node.as_table()->insert(key, value.getInteger());
        break;
      case Variable::Type::NUMBER:
        node.as_table()->insert(key, value.getNumber());
        break;
      case Variable::Type::BOOLEAN:
        node.as_table()->insert(key, value.getBoolean());
        break;
      case Variable::Type::TABLE: {
        toml::table tb;
        resolveToToml(tb, value);
        node.as_table()->insert(key, tb);
        break;
      }
      case Variable::Type::ARRAY: {
        toml::array arr;
        resolveToToml(arr, value);
        node.as_table()->insert(key, arr);
      } break;
      default:
        break;
      }
    }
  } else {
    auto &arr = variable.getArray();
    for (auto &item : arr) {
      switch (item.getType()) {
      case Variable::Type::STRING:
        node.as_array()->push_back(item.getString());
        break;
      case Variable::Type::INTEGER:
        node.as_array()->push_back(item.getInteger());
        break;
      case Variable::Type::NUMBER:
        node.as_array()->push_back(item.getNumber());
        break;
      case Variable::Type::BOOLEAN:
        node.as_array()->push_back(item.getBoolean());
        break;
      case Variable::Type::TABLE: {
        toml::table tb;
        resolveToToml(tb, item);
        node.as_array()->push_back(tb);
      } break;
      case Variable::Type::ARRAY: {
        toml::array arr;
        resolveToToml(arr, item);
        node.as_array()->push_back(arr);
      } break;
      default:
        break;
      }
    }
  }
}

std::string Variable::toToml() const {
  if (_type != Type::TABLE) {
    throw std::runtime_error("cannot convert non-table variable to toml");
  }
  toml::table tb;
  resolveToToml(tb, *this);
  std::stringstream ss;
  ss << tb << std::endl;
  return ss.str();
}

static void resolveJson(Variable &var, cJSON *node) {
  if (cJSON_IsNull(node)) {
    var.setNil();
  } else if (cJSON_IsNumber(node)) {
    var.setNumber(node->valuedouble);
  } else if (cJSON_IsBool(node)) {
    var.setBoolean(node->valueint);
  } else if (cJSON_IsString(node)) {
    var.setString(node->valuestring);
  } else if (cJSON_IsArray(node)) {
    var.setArray();
    size_t size = cJSON_GetArraySize(node);
    for (size_t idx = 0; idx < size; idx++) {
      Variable item;
      resolveJson(item, cJSON_GetArrayItem(node, idx));
      var.push(item);
    }
  } else if (cJSON_IsObject(node)) {
    var.setTable();
    cJSON *item = node->child;
    while (item) {
      Variable value;
      resolveJson(value, item);
      var.setField(item->string, value);
      item = item->next;
    }
  }
}

Variable &Variable::parseJson(const std::string &source,
                              const std::string &filename) {
  cJSON *root = cJSON_Parse(source.c_str());
  if (!root) {
    throw std::runtime_error(cJSON_GetErrorPtr());
  }
  resolveJson(*this, root);
  cJSON_free(root);
  return *this;
}

static cJSON *resolveToJson(const Variable &var) {
  switch (var.getType()) {
  case Variable::Type::NIL:
    return cJSON_CreateNull();
  case Variable::Type::STRING:
    return cJSON_CreateString(var.getString().c_str());
  case Variable::Type::INTEGER:
    return cJSON_CreateNumber(var.getInteger());
  case Variable::Type::NUMBER:
    return cJSON_CreateNumber(var.getNumber());
  case Variable::Type::BOOLEAN:
    return cJSON_CreateBool(var.getBoolean());
  case Variable::Type::TABLE: {
    cJSON *obj = cJSON_CreateObject();
    auto &table = var.getTable();
    for (auto &[k, v] : table) {
      cJSON_AddItemToObject(obj, k.c_str(), resolveToJson(v));
    }
    return obj;
  }
  case Variable::Type::ARRAY: {
    cJSON *arr = cJSON_CreateArray();
    auto &array = var.getArray();
    for (auto &item : array) {
      cJSON_AddItemToArray(arr, resolveToJson(item));
    }
    return arr;
  }
  }
}

std::string Variable::toJson() const {
  cJSON *node = resolveToJson(*this);
  char *s = cJSON_Print(node);
  std::string result = s;
  cJSON_free(s);
  return result;
}