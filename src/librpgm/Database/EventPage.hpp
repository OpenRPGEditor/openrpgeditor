#pragma once

#include "Database/Globals.hpp"
#include "Database/EventPage.hpp"
#include "Database/CommandParser.hpp"
#include "Database/MovementRoute.hpp"

#include <string>
#include <vector>

#include "nlohmann/json.hpp"

struct EventCondition {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(EventCondition, actorId, actorValid, itemId, itemValid, selfSwitchCh, selfSwitchValid,
                                 switch1Id, switch1Valid, switch2Id, switch2Valid, variableId, variableValid,
                                 variableValue);
  int actorId{1};
  bool actorValid{};
  int itemId{1};
  bool itemValid{};
  std::string selfSwitchCh = "A";
  bool selfSwitchValid{};
  int switch1Id{1};
  bool switch1Valid{};
  int switch2Id{1};
  bool switch2Valid{};
  int variableId{1};
  bool variableValid{};
  int variableValue{};

  [[nodiscard]] bool isDirty() const { return m_isDirty; }
  bool m_isDirty{false};
};

struct EventImage {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(EventImage, tileId, characterName, direction, pattern, characterIndex);
  int tileId{};
  std::string characterName;
  Direction direction{};
  int pattern{};
  int characterIndex{};

  [[nodiscard]] bool isDirty() const { return m_isDirty; }
  bool m_isDirty{false};
};

struct EventPage {
  friend void to_json(nlohmann::json& json, const EventPage& eventPage);
  friend void from_json(const nlohmann::json& json, EventPage& eventPage);

  EventCondition conditions{};
  bool directionFix{};
  EventImage image;
  std::vector<std::shared_ptr<IEventCommand>> list;
  MovementFrequency moveFrequency{MovementFrequency::Normal};
  MovementRoute moveRoute{};
  MovementSpeed moveSpeed{MovementSpeed::Normal};
  MoveType moveType{MoveType::Fixed};
  EventPriority priorityType{};
  bool stepAnime = false;
  bool through{};
  EventTriggerType trigger{};
  bool walkAnime = true;

  /* OpenRPGMaker Additions */
  std::string name;

  [[nodiscard]] bool isDirty() const {
    m_isDirty |= std::any_of(list.begin(), list.end(), [](const auto& cmd) { return cmd && cmd->isDirty(); });

    m_isDirty |= conditions.isDirty();
    m_isDirty |= image.isDirty();
    m_isDirty |= moveRoute.isDirty();

    return m_isDirty;
  }
  mutable bool m_isDirty{false};
};