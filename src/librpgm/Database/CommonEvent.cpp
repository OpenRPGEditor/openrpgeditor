#include "Database/CommonEvent.hpp"

void to_json(nlohmann::ordered_json& to, const CommonEvent& event) {
  CommandParser parser;
  nlohmann::ordered_json commands;
  parser.serialize(commands, event.commands());

  to = nlohmann::ordered_json{{"id", event.m_id}, {"list", commands}, {"name", event.m_name}, {"switchId", event.m_switchId}, {"trigger", event.m_trigger}};
}
void from_json(const nlohmann::ordered_json& from, CommonEvent& event) {
  event.m_id = from.value("id", event.m_id);
  event.m_name = from.value("name", event.m_name);
  CommandParser parser;
  event.m_commands = parser.parse(from.at("list"));
  event.m_switchId = from.value("switchId", event.m_switchId);
  event.m_trigger = from.value("trigger", event.m_trigger);
}