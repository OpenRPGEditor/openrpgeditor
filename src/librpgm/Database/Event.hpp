#pragma once

#include "Database/Globals.hpp"
#include "Database/EventPage.hpp"
#include <string>
#include <vector>

class Event {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Event, id, name, note, pages, x, y);
  int id;
  std::string name;
  std::string note;
  std::vector<EventPage> pages;
  int x;
  int y;
};
