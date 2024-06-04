#pragma once

#include "Database/Globals.hpp"

#include <nlohmann/json.hpp>
#include <unordered_set>

class Parameter {
public:
  enum class Type {
    Null,
    Object,
    Array,
    String,
    Bool,
    Integer,
    Unsigned,
    Float,
    Binary,
  };

  Type type;
  std::unordered_set<std::string, int> object;
  std::vector<Parameter> array_value;
  std::string string_value;
  bool bool_value;
  int integer_value;
  uint32_t unsigned_value;
  std::vector<char> binary_value;
};

class Command {
public:
  EventCode code() const { return m_code; }
  int indent() const { return m_indent; }

  // private:
  EventCode m_code;
  int m_indent;
  std::vector<Parameter> m_parameters;
};

class CommonEvent {
public:
  std::string name;
  int id;
  std::vector<Command> commands;
};

class CommonEvents {
public:
  CommonEvents();

  std::vector<CommonEvent> m_events;
};
