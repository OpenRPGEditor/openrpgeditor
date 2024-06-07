#pragma once

#include "Database/CommandParser.hpp"
#include "Database/Globals.hpp"

#include <nlohmann/json.hpp>

struct CommonEvent {
  friend void to_json(nlohmann::json& to, const CommonEvent& event);
  friend void from_json(const nlohmann::json& to, CommonEvent& event);

  int id;
  std::vector<std::shared_ptr<IEventCommand>> commands;
  std::string name;
  int switchId;
  CommonEventTriggerType trigger;
};

class CommonEvents {
public:
  static CommonEvents load(std::string_view filepath);
  bool serialize(std::string_view filename);

  std::vector<CommonEvent> m_events;
};
