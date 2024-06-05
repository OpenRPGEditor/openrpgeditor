#pragma once

#include "Database/Globals.hpp"
#include "Database/EventPage.hpp"
#include "Database/CommandParser.hpp"

#include <string>
#include <vector>

#include "nlohmann/json.hpp"

struct EventCondition {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(EventCondition, actorId, actorValid, itemId, itemValid, selfSwitchCh, selfSwitchValid,
                                 switch1Id, switch1Valid, switch2Id, switch2Valid, variableId, variableValid,
                                 variableValue);
  int actorId;
  bool actorValid;
  int itemId;
  bool itemValid;
  std::string selfSwitchCh;
  bool selfSwitchValid;
  int switch1Id;
  bool switch1Valid;
  int switch2Id;
  bool switch2Valid;
  int variableId;
  bool variableValid;
  int variableValue;
};

struct EventImage {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(EventImage, tileId, characterName, direction, pattern, characterIndex);
  int tileId;
  std::string characterName;
  int direction;
  int pattern;
  int characterIndex;
};

struct EventPage {
  friend void to_json(nlohmann::json& json, const EventPage& eventPage);
  friend void from_json(const nlohmann::json& json, EventPage& eventPage);

  EventCondition conditions;
  bool directionFix;
  EventImage image;
  std::vector<std::shared_ptr<IEventCommand>> list;
};