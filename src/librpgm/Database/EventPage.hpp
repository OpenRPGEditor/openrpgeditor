#pragma once

#include "Database/EventPage.hpp"
#include "Database/Globals.hpp"
#include "Database/IPageEditor.hpp"
#include "Database/MovementRoute.hpp"

#include <string>
#include <vector>

#include "nlohmann/json.hpp"

struct EventCondition {
  friend void to_json(nlohmann::ordered_json& to, const EventCondition& cond);
  friend void from_json(const nlohmann::ordered_json& from, EventCondition& cond);
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
void to_json(nlohmann::ordered_json& to, const EventCondition& cond);
void from_json(const nlohmann::ordered_json& from, EventCondition& cond);

struct EventImage {
  friend void to_json(nlohmann::ordered_json& to, const EventImage& image);
  friend void from_json(const nlohmann::ordered_json& from, EventImage& image);
  int tileId{};
  std::string characterName;
  Direction direction{Direction::Down};
  int pattern{};
  int characterIndex{};

  [[nodiscard]] bool isDirty() const { return m_isDirty; }
  bool m_isDirty{false};
};
void to_json(nlohmann::ordered_json& to, const EventImage& image);
void from_json(const nlohmann::ordered_json& from, EventImage& image);

class EventPage final : public IModifiable {
public:
  friend void to_json(nlohmann::ordered_json& json, const EventPage& eventPage);
  friend void from_json(const nlohmann::ordered_json& json, EventPage& eventPage);

  EventPage();
  void clear();

  IPageEditor* editor() const {
    if (!m_editor) {
      m_editor.reset(IPageEditor::create(const_cast<EventPage*>(this)));
    }

    return m_editor.get();
  }

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

  [[nodiscard]] bool isModified() const override {
    bool modified = IModifiable::isModified();
    modified |= std::ranges::any_of(list, [](const auto& cmd) { return cmd && cmd->isModified(); });

    modified |= conditions.isDirty();
    modified |= image.isDirty();
    modified |= moveRoute.isModified();
    return modified;
  }

  mutable std::shared_ptr<IPageEditor> m_editor;
};
void to_json(nlohmann::ordered_json& json, const EventPage& eventPage);
void from_json(const nlohmann::ordered_json& json, EventPage& eventPage);