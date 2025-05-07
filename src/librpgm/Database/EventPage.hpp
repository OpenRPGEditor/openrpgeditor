#pragma once

#include "Database/EventPage.hpp"
#include "Database/Globals.hpp"
#include "Database/IPageEditor.hpp"
#include "Database/MovementRoute.hpp"

#include <string>
#include <vector>

#include "nlohmann/json.hpp"

class EventCondition final : public IModifiable {
  friend void to_json(nlohmann::ordered_json& to, const EventCondition& cond);
  friend void from_json(const nlohmann::ordered_json& from, EventCondition& cond);

public:
  EventCondition() = default;
  EventCondition(const EventCondition& other);
  EventCondition& operator=(const EventCondition& other);
  EventCondition(EventCondition&& other) noexcept;
  EventCondition& operator=(EventCondition&& other) noexcept;
  bool operator==(const EventCondition& other) const;

  int actorId() const;
  void setActorId(int actorId);

  bool actorValid() const;
  void setActorValid(bool actorValid);

  int itemId() const;
  void setItemId(int itemId);

  bool itemValid() const;
  void setItemValid(bool itemValid);

  std::string_view selfSwitchCh() const;
  void setSelfSwitchCh(std::string_view selfSwitchCh);

  bool selfSwitchValid() const;
  void setSelfSwitchValid(bool selfSwitchValid);

  int switch1Id() const;
  void setSwitch1Id(int switch1Id);

  bool switch1Valid() const;
  void setSwitch1Valid(bool switch1Valid);

  int switch2Id() const;
  void setSwitch2Id(int switch2Id);
  bool switch2Valid() const;
  void setSwitch2Valid(bool switch2Valid);

  int variableId() const;
  void setVariableId(int variableId);

  bool variableValid() const;
  void setVariableValid(bool variableValid);

  int variableValue() const;
  void setVariableValue(int variableValue);

  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;

  rpgmutils::signal<void(EventCondition*, int)>& actorIdModified();
  rpgmutils::signal<void(EventCondition*, bool)>& actorValidModified();
  rpgmutils::signal<void(EventCondition*, int)>& itemIdModified();
  rpgmutils::signal<void(EventCondition*, bool)>& itemValidModified();
  rpgmutils::signal<void(EventCondition*, const std::string&)>& selfSwitchModified();
  rpgmutils::signal<void(EventCondition*, bool)>& selfSwitchValidModified();
  rpgmutils::signal<void(EventCondition*, int)>& switch1IdModified();
  rpgmutils::signal<void(EventCondition*, bool)>& switch1ValidModified();
  rpgmutils::signal<void(EventCondition*, int)>& switch2IdModified();
  rpgmutils::signal<void(EventCondition*, bool)>& switch2ValidModified();
  rpgmutils::signal<void(EventCondition*, int)>& variableIdModified();
  rpgmutils::signal<void(EventCondition*, bool)>& variableValidModified();
  rpgmutils::signal<void(EventCondition*, int)>& variableValueModified();

private:
  int m_actorId{1};
  bool m_actorValid{};
  int m_itemId{1};
  bool m_itemValid{};
  std::string m_selfSwitchCh = "A";
  bool m_selfSwitchValid{};
  int m_switch1Id{1};
  bool m_switch1Valid{};
  int m_switch2Id{1};
  bool m_switch2Valid{};
  int m_variableId{1};
  bool m_variableValid{};
  int m_variableValue{};

  std::optional<int> m_oldactorId;
  std::optional<bool> m_oldactorValid;
  std::optional<int> m_olditemId;
  std::optional<bool> m_olditemValid;
  std::optional<std::string> m_oldselfSwitchCh;
  std::optional<bool> m_oldselfSwitchValid;
  std::optional<int> m_oldswitch1Id;
  std::optional<bool> m_oldswitch1Valid;
  std::optional<int> m_oldswitch2Id;
  std::optional<bool> m_oldswitch2Valid;
  std::optional<int> m_oldvariableId;
  std::optional<bool> m_oldvariableValid;
  std::optional<int> m_oldvariableValue;

  std::optional<rpgmutils::signal<void(EventCondition*, int)>> m_actorIdModified;
  std::optional<rpgmutils::signal<void(EventCondition*, bool)>> m_actorValidModified;
  std::optional<rpgmutils::signal<void(EventCondition*, int)>> m_itemIdModified;
  std::optional<rpgmutils::signal<void(EventCondition*, bool)>> m_itemValidModified;
  std::optional<rpgmutils::signal<void(EventCondition*, const std::string&)>> m_selfSwitchModified;
  std::optional<rpgmutils::signal<void(EventCondition*, bool)>> m_selfSwitchValidModified;
  std::optional<rpgmutils::signal<void(EventCondition*, int)>> m_switch1IdModified;
  std::optional<rpgmutils::signal<void(EventCondition*, bool)>> m_switch1ValidModified;
  std::optional<rpgmutils::signal<void(EventCondition*, int)>> m_switch2IdModified;
  std::optional<rpgmutils::signal<void(EventCondition*, bool)>> m_switch2ValidModified;
  std::optional<rpgmutils::signal<void(EventCondition*, int)>> m_variableIdModified;
  std::optional<rpgmutils::signal<void(EventCondition*, bool)>> m_variableValidModified;
  std::optional<rpgmutils::signal<void(EventCondition*, int)>> m_variableValueModified;
};
void to_json(nlohmann::ordered_json& to, const EventCondition& cond);
void from_json(const nlohmann::ordered_json& from, EventCondition& cond);

class EventImage final : public IModifiable {
  friend void to_json(nlohmann::ordered_json& to, const EventImage& image);
  friend void from_json(const nlohmann::ordered_json& from, EventImage& image);

public:
  EventImage() = default;
  EventImage(const EventImage& other);
  EventImage& operator=(const EventImage& other);
  EventImage(EventImage&& other) noexcept;
  EventImage& operator=(EventImage&& other) noexcept;
  bool operator==(const EventImage& other) const;

  int tileId() const;
  void setTileId(int tileId);

  std::string_view characterName() const;
  void setCharacterName(std::string_view characterName);
  Direction direction() const;
  void setDirection(Direction direction);

  int pattern() const;
  void setPattern(int pattern);

  int characterIndex() const;
  void setCharacterIndex(int characterIndex);

  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;

  rpgmutils::signal<void(EventImage*, int)>& tileIdModified();
  rpgmutils::signal<void(EventImage*, const std::string&)>& characterNameModified();
  rpgmutils::signal<void(EventImage*, Direction)>& directionModified();
  rpgmutils::signal<void(EventImage*, int)>& patternModified();
  rpgmutils::signal<void(EventImage*, int)>& characterIndexModified();

private:
  int m_tileId{};
  std::string m_characterName;
  Direction m_direction{Direction::Down};
  int m_pattern{};
  int m_characterIndex{};

  std::optional<int> m_oldtileId;
  std::optional<std::string> m_oldcharacterName;
  std::optional<Direction> m_olddirection;
  std::optional<int> m_oldpattern;
  std::optional<int> m_oldcharacterIndex;

  std::optional<rpgmutils::signal<void(EventImage*, int)>> m_tileIdModified;
  std::optional<rpgmutils::signal<void(EventImage*, const std::string&)>> m_characterNameModified;
  std::optional<rpgmutils::signal<void(EventImage*, Direction)>> m_directionModified;
  std::optional<rpgmutils::signal<void(EventImage*, int)>> m_patternModified;
  std::optional<rpgmutils::signal<void(EventImage*, int)>> m_characterIndexModified;
};

void to_json(nlohmann::ordered_json& to, const EventImage& image);
void from_json(const nlohmann::ordered_json& from, EventImage& image);

class EventPage final : public IModifiable {
public:
  friend void to_json(nlohmann::ordered_json& json, const EventPage& eventPage);
  friend void from_json(const nlohmann::ordered_json& json, EventPage& eventPage);
  EventPage();
  EventPage(const EventPage& other);
  EventPage& operator=(const EventPage& other);
  EventPage(EventPage&& other) noexcept;
  EventPage& operator=(EventPage&& other) noexcept;

  EventCondition& conditions();
  const EventCondition& conditions() const;
  void setCondition(const EventCondition& condition);

  bool directionFix() const;
  void setDirectionFix(bool directionFix);

  EventImage& image();
  const EventImage& image() const;
  void setImage(const EventImage& image);

  std::vector<std::shared_ptr<IEventCommand>>& list();
  const std::vector<std::shared_ptr<IEventCommand>>& list() const;
  void setList(const std::vector<std::shared_ptr<IEventCommand>>& list);

  MovementRoute& moveRoute();
  const MovementRoute& moveRoute() const;
  void setMoveRoute(const MovementRoute& moveRoute);

  MovementSpeed moveSpeed() const;
  void setMoveSpeed(MovementSpeed movementSpeed);

  MovementFrequency moveFrequency() const;
  void setMoveFrequency(MovementFrequency moveFrequency);

  MoveType moveType() const;
  void setMoveType(MoveType movementType);

  EventPriority priorityType() const;
  void setPriorityType(EventPriority priority);

  bool stepAnime() const;
  void setStepAnime(bool stepAnime);

  bool through() const;
  void setThrough(bool through);

  EventTriggerType trigger() const;
  void setTrigger(EventTriggerType trigger);

  bool walkAnime() const;
  void setWalkAnime(bool walkAnime);

  std::string_view name() const;
  void setName(std::string_view name);

  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;

  rpgmutils::signal<void(EventPage*, EventCondition)>& conditionsModified();
  rpgmutils::signal<void(EventPage*, bool)>& directionFixModified();
  rpgmutils::signal<void(EventPage*, EventImage)>& imageModified();
  rpgmutils::signal<void(EventPage*, std::vector<std::shared_ptr<IEventCommand>>)>& listModified();
  rpgmutils::signal<void(EventPage*, MovementFrequency)>& moveFrequencyModified();
  rpgmutils::signal<void(EventPage*, MovementRoute)>& moveRouteModified();
  rpgmutils::signal<void(EventPage*, MovementSpeed)>& moveSpeedModified();
  rpgmutils::signal<void(EventPage*, MoveType)>& moveTypeModified();
  rpgmutils::signal<void(EventPage*, EventPriority)>& priorityTypeModified();
  rpgmutils::signal<void(EventPage*, bool)>& stepAnimeModified();
  rpgmutils::signal<void(EventPage*, bool)>& throughModified();
  rpgmutils::signal<void(EventPage*, EventTriggerType)>& triggerModified();
  rpgmutils::signal<void(EventPage*, bool)>& walkAnimeModified();
  /* OpenRPGMaker Additions */
  rpgmutils::signal<void(EventPage*, const std::string&)>& nameModified();

  [[nodiscard]] bool isModified() const override {
    bool modified = IModifiable::isModified();
    modified |= std::ranges::any_of(m_list, [](const auto& cmd) { return cmd && cmd->isModified(); });

    modified |= m_conditions.isModified();
    modified |= m_image.isModified();
    modified |= m_moveRoute.isModified();
    return modified;
  }

  void clear();
  IPageEditor* editor() const {
    if (!m_editor) {
      m_editor.reset(IPageEditor::create(const_cast<EventPage*>(this)));
    }

    return m_editor.get();
  }

  EventPage clone() const { return EventPage(*this, 1); }
  bool operator==(const EventPage& rhs) const {
    return m_conditions == rhs.m_conditions && m_directionFix == rhs.m_directionFix && m_image == rhs.m_image && m_list == rhs.m_list && m_moveFrequency == rhs.m_moveFrequency &&
           m_moveRoute == rhs.m_moveRoute && m_moveSpeed == rhs.m_moveSpeed && m_moveType == rhs.m_moveType && m_priorityType == rhs.m_priorityType && m_stepAnime == rhs.m_stepAnime &&
           m_through == rhs.m_through && m_trigger == rhs.m_trigger && m_walkAnime && (m_name ? m_name == rhs.m_name : true);
    ;
  }

  void connectAllSignals();

private:
  EventPage(const EventPage& other, int)
  : IModifiable(other) {
    m_conditions = other.m_conditions;
    m_directionFix = other.m_directionFix;
    m_image = other.m_image;
    for (const auto& cmd : other.m_list) {
      m_list.push_back(cmd->clone());
    }
    m_moveRoute = other.m_moveRoute.clone();

    m_moveSpeed = other.m_moveSpeed;
    m_moveFrequency = other.m_moveFrequency;
    m_moveType = other.m_moveType;
    m_priorityType = other.m_priorityType;
    m_stepAnime = other.m_stepAnime;
    m_through = other.m_through;
    m_trigger = other.m_trigger;
    m_walkAnime = other.m_walkAnime;
    m_name = other.m_name;
  };
  EventCondition m_conditions{};
  bool m_directionFix{};
  EventImage m_image;
  std::vector<std::shared_ptr<IEventCommand>> m_list;
  MovementFrequency m_moveFrequency{MovementFrequency::Normal};
  MovementRoute m_moveRoute{};
  MovementSpeed m_moveSpeed{MovementSpeed::x2_Slower};
  MoveType m_moveType{MoveType::Fixed};
  EventPriority m_priorityType{};
  bool m_stepAnime = false;
  bool m_through{};
  EventTriggerType m_trigger{EventTriggerType::Action_Button};
  bool m_walkAnime = true;
  /* OpenRPGMaker Additions */
  std::optional<std::string> m_name;

  std::optional<EventCondition> m_oldconditions;
  std::optional<bool> m_olddirectionFix;
  std::optional<EventImage> m_oldimage;
  std::optional<std::vector<std::shared_ptr<IEventCommand>>> m_oldlist;
  std::optional<MovementFrequency> m_oldmoveFrequency;
  std::optional<MovementRoute> m_oldmoveRoute;
  std::optional<MovementSpeed> m_oldmoveSpeed;
  std::optional<MoveType> m_oldmoveType;
  std::optional<EventPriority> m_oldpriorityType;
  std::optional<bool> m_oldstepAnime;
  std::optional<bool> m_oldthrough;
  std::optional<EventTriggerType> m_oldtrigger;
  std::optional<bool> m_oldwalkAnime;
  std::optional<std::string> m_oldname;

  std::optional<rpgmutils::signal<void(EventPage*, EventCondition)>> m_conditionsModified;
  std::optional<rpgmutils::signal<void(EventPage*, bool)>> m_directionFixModified;
  std::optional<rpgmutils::signal<void(EventPage*, EventImage)>> m_imageModified;
  std::optional<rpgmutils::signal<void(EventPage*, std::vector<std::shared_ptr<IEventCommand>>)>> m_listModified;
  std::optional<rpgmutils::signal<void(EventPage*, MovementFrequency)>> m_moveFrequencyModified;
  std::optional<rpgmutils::signal<void(EventPage*, MovementRoute)>> m_moveRouteModified;
  std::optional<rpgmutils::signal<void(EventPage*, MovementSpeed)>> m_moveSpeedModified;
  std::optional<rpgmutils::signal<void(EventPage*, MoveType)>> m_moveTypeModified;
  std::optional<rpgmutils::signal<void(EventPage*, EventPriority)>> m_priorityTypeModified;
  std::optional<rpgmutils::signal<void(EventPage*, bool)>> m_stepAnimeModified;
  std::optional<rpgmutils::signal<void(EventPage*, bool)>> m_throughModified;
  std::optional<rpgmutils::signal<void(EventPage*, EventTriggerType)>> m_triggerModified;
  std::optional<rpgmutils::signal<void(EventPage*, bool)>> m_walkAnimeModified;
  /* OpenRPGMaker Additions */
  std::optional<rpgmutils::signal<void(EventPage*, const std::string&)>> m_nameModified;

  mutable std::shared_ptr<IPageEditor> m_editor;

  void onValueModified(IModifiable* v);
};
void to_json(nlohmann::ordered_json& json, const EventPage& eventPage);
void from_json(const nlohmann::ordered_json& json, EventPage& eventPage);