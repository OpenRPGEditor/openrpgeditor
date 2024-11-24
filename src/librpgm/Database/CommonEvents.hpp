#pragma once

#include "Database/CommandParser.hpp"
#include "Database/Globals.hpp"
#include "EventCommands/EventDummy.hpp"

#include <nlohmann/json.hpp>

struct CommonEvent {
  friend void to_json(nlohmann::ordered_json& to, const CommonEvent& event);
  friend void from_json(const nlohmann::ordered_json& to, CommonEvent& event);

  int id{};
  std::vector<std::shared_ptr<IEventCommand>> commands;
  std::string name{};
  int switchId{};
  CommonEventTriggerType trigger;
};

void to_json(nlohmann::ordered_json& to, const CommonEvent& event);
void from_json(const nlohmann::ordered_json& to, CommonEvent& event);

class CommonEvents {
public:
  static CommonEvents load(std::string_view filepath);
  bool serialize(std::string_view filename);

  std::vector<std::optional<CommonEvent>>& events() { return m_events; }
  const std::vector<std::optional<CommonEvent>>& events() const { return m_events; }

  [[nodiscard]] CommonEvent* event(int id) {
    for (auto& event : m_events) {
      if (event && event->id == id) {
        return &event.value();
      }
    }
    return nullptr;
  }

  [[nodiscard]] const CommonEvent* event(int id) const {
    for (const auto& event : m_events) {
      if (event && event->id == id) {
        return &event.value();
      }
    }

    return nullptr;
  }

  int count() const { return m_events.size() - 1; }

  void resize(int newSize) {
    ++newSize;
    int oldSize = m_events.size();
    m_events.resize(newSize);
    if (newSize > oldSize) {
      for (int i = oldSize; i < m_events.size(); ++i) {
        m_events[i] = CommonEvent();
        m_events[i]->commands.emplace_back(new EventDummy());
        m_events[i]->id = i;
      }
    }
  }

private:
  std::vector<std::optional<CommonEvent>> m_events;
};
