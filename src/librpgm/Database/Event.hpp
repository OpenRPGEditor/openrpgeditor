#pragma once

#include "Database/Globals.hpp"
#include "Database/EventPage.hpp"
#include <string>
#include <vector>

class Event {
public:
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Event, id, name, note, pages, x, y);
  int id{};
  std::string name{};
  std::string note{};
  std::vector<EventPage> pages{};
  int x{};
  int y{};

  bool isDirty() const {
    m_isDirty |= std::any_of(pages.begin(), pages.end(), [](const auto& page) {
      return page.isDirty();
    });

    return m_isDirty;
  }

  mutable bool m_isDirty{false};
};
