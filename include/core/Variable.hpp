#pragma once
#include "core/Object.hpp"
#include <any>
#include <string>
#include <unordered_map>
#include <vector>
namespace cube::core {
class Variable : public Object {
public:
  using Table = std::unordered_map<std::string, Variable>;
  using Array = std::vector<Variable>;

public:
  enum class Type {
    NIL,
    STRING,
    INTEGER,
    NUMBER,
    BOOLEAN,
    TABLE,
    ARRAY,
  };

private:
  std::any _value = nullptr;
  Type _type = Variable::Type::NIL;

public:
  const Type &getType() const;
  Variable &setNil();
  Variable &setString(const std::string &value = "");
  Variable &setNumber(double value = .0f);
  Variable &setInteger(int64_t value = 0);
  Variable &setBoolean(bool value = false);
  Variable &setTable(const Table &value = {});
  Variable &setArray(const Array &value = {});
  bool isNil() const;
  bool isString() const;
  bool isNumber() const;
  bool isInteger() const;
  bool isBoolean() const;
  bool isTable() const;
  bool isArray() const;
  const std::string &getString(const std::string &def = "") const;
  double getNumber(double def = .0f) const;
  int64_t getInteger(int64_t def = 0) const;
  bool getBoolean(bool def = false) const;
  const Table &getTable(const Table &def = {}) const;
  const Array &getArray(const Array &def = {}) const;
  Table &getTable();
  Array &getArray();
  Variable &setField(const std::string &key, const Variable &value);
  Variable &setIndex(size_t idx, const Variable &value);
  const Variable &getField(const std::string &key) const;
  const Variable &get(const std::string &key) const;
  const Variable &getIndex(size_t idx) const;
  Variable &push(const Variable &variable);
  Variable &getField(const std::string &key);
  Variable &getIndex(size_t idx);
  Variable &set(const std::string &key, const Variable &value);
  bool hasField(const std::string &key) const;
  size_t getSize() const;
  Variable &delField(const std::string &key);
  Variable &parseToml(const std::string &source, const std::string &filename);
  std::string toToml() const;
  Variable &parseJson(const std::string &source, const std::string &filename);
  std::string toJson() const;
};
}; // namespace cube::core