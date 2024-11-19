
#include "CommonEvents.hpp"
#include <fstream>

using json = nlohmann::json;

void to_json(nlohmann::json& to, const CommonEvent& event) {
  CommandParser parser;
  json commands;
  parser.serialize(commands, event.commands);

  to = json{{"id", event.id},
            {"list", commands},
            {"name", event.name},
            {"switchId", event.switchId},
            {"trigger", event.trigger}};
}
void from_json(const nlohmann::json& from, CommonEvent& event) {
  event.id = from.value("id", event.id);
  event.name = from.value("name", event.name);
  CommandParser parser;
  event.commands = parser.parse(from.at("list"));
  event.switchId = from.value("switchId", event.switchId);
  event.trigger = from.value("trigger", event.trigger);
}

CommonEvents CommonEvents::load(std::string_view filepath) {
  std::ifstream file(filepath.data());
  json data = json::parse(file);
  CommonEvents commonEvents;
  commonEvents.m_events.reserve(data.size());

  for (const auto& [_, value] : data.items()) {
    value.get_to(commonEvents.m_events.emplace_back());
  }
  return commonEvents;
}

bool CommonEvents::serialize(std::string_view filename) {
  // TODO: Temporary file for atomic writes

  std::ofstream file(filename.data());
  json data;

  for (const auto& event : m_events) {
    data.push_back(event);
  }

  file << data.dump(4);

  return true;
}