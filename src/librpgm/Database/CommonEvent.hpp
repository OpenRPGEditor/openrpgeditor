#pragma once

#include "Database/CommandParser.hpp"
#include "Database/Globals.hpp"
#include "EventCommands/EventDummy.hpp"

class CommonEvent {
  friend class CommonEventsSerializer;
  friend void to_json(nlohmann::ordered_json& to, const CommonEvent& event);
  friend void from_json(const nlohmann::ordered_json& from, CommonEvent& event);

public:
  [[nodiscard]] int id() const { return m_id; }
  void setId(const int id) { m_id = id; }

  std::vector<std::shared_ptr<IEventCommand>>& commands() { return m_commands; }
  const std::vector<std::shared_ptr<IEventCommand>>& commands() const { return m_commands; }
  void setCommands(const std::vector<std::shared_ptr<IEventCommand>>& commands) { m_commands = commands; }

  const std::string& name() const { return m_name; }
  void setName(const std::string& name) { m_name = name; }

  int switchId() const { return m_switchId; }
  void setSwitchId(const int id) { m_switchId = id; }

  CommonEventTriggerType trigger() const { return m_trigger; }
  void setTrigger(const CommonEventTriggerType trigger) { m_trigger = trigger; }

private:
  int m_id{};
  std::vector<std::shared_ptr<IEventCommand>> m_commands;
  std::string m_name{};
  int m_switchId{};
  CommonEventTriggerType m_trigger;
};

void to_json(nlohmann::ordered_json& to, const CommonEvent& event);
void from_json(const nlohmann::ordered_json& from, CommonEvent& event);