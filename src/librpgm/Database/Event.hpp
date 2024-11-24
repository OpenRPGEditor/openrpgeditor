#pragma once

#include "Database/EventPage.hpp"
#include "Database/Globals.hpp"
#include <string>
#include <vector>

class Event {
public:
  friend void to_json(nlohmann::ordered_json& to, const Event& event);
  friend void from_json(const nlohmann::ordered_json& from, Event& event);
  int id{};
  std::string name{};
  std::string note{};
  std::vector<EventPage> pages{};
  int x{};
  int y{};

  bool isDirty() const {
    m_isDirty |= std::any_of(pages.begin(), pages.end(), [](const auto& page) { return page.isDirty(); });

    return m_isDirty;
  }

  mutable bool m_isDirty{false};
};

void to_json(nlohmann::ordered_json& to, const Event& event);
void from_json(const nlohmann::ordered_json& from, Event& event);