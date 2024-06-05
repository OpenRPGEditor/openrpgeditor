#pragma once

#include "Database/Globals.hpp"

#include <nlohmann/json.hpp>


class Command {
public:
  EventCode code() const { return m_code; }
  int indent() const { return m_indent; }

  // private:
  EventCode m_code;
  int m_indent;
};

class CommonEvent {
public:
  std::string name;
  int id;
  std::vector<Command> commands;
};

class CommonEvents {
public:
  static CommonEvents load(std::string_view filepath);
  bool serialize(std::string_view filename);

  std::vector<CommonEvent> m_events;
};
