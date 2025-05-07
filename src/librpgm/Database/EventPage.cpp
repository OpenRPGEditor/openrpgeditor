#include "Database/EventPage.hpp"
#include "Database/CommandParser.hpp"
#include "Database/EventCommands/EventDummy.hpp"
#include <string_view>

using namespace std::string_view_literals;

EventCondition::EventCondition(const EventCondition& other)
: IModifiable(other)
, m_actorId(other.m_actorId)
, m_actorValid(other.m_actorValid)
, m_itemId(other.m_itemId)
, m_itemValid(other.m_itemValid)
, m_selfSwitchCh(other.m_selfSwitchCh)
, m_selfSwitchValid(other.m_selfSwitchValid)
, m_switch1Id(other.m_switch1Id)
, m_switch1Valid(other.m_switch1Valid)
, m_switch2Id(other.m_switch2Id)
, m_switch2Valid(other.m_switch2Valid)
, m_variableId(other.m_variableId)
, m_variableValid(other.m_variableValid)
, m_variableValue(other.m_variableValue)
, m_oldactorId(other.m_oldactorId)
, m_oldactorValid(other.m_oldactorValid)
, m_olditemId(other.m_olditemId)
, m_olditemValid(other.m_olditemValid)
, m_oldselfSwitchCh(other.m_oldselfSwitchCh)
, m_oldselfSwitchValid(other.m_oldselfSwitchValid)
, m_oldswitch1Id(other.m_oldswitch1Id)
, m_oldswitch1Valid(other.m_oldswitch1Valid)
, m_oldswitch2Id(other.m_oldswitch2Id)
, m_oldswitch2Valid(other.m_oldswitch2Valid)
, m_oldvariableId(other.m_oldvariableId)
, m_oldvariableValid(other.m_oldvariableValid)
, m_oldvariableValue(other.m_oldvariableValue) {}

EventCondition& EventCondition::operator=(const EventCondition& other) {
  IModifiable::operator=(other);
  m_actorId = other.m_actorId;
  m_actorValid = other.m_actorValid;
  m_itemId = other.m_itemId;
  m_itemValid = other.m_itemValid;
  m_selfSwitchCh = other.m_selfSwitchCh;
  m_selfSwitchValid = other.m_selfSwitchValid;
  m_switch1Id = other.m_switch1Id;
  m_switch1Valid = other.m_switch1Valid;
  m_switch2Id = other.m_switch2Id;
  m_switch2Valid = other.m_switch2Valid;
  m_variableId = other.m_variableId;
  m_variableValid = other.m_variableValid;
  m_variableValue = other.m_variableValue;
  m_oldactorId = other.m_oldactorId;
  m_oldactorValid = other.m_oldactorValid;
  m_olditemId = other.m_olditemId;
  m_olditemValid = other.m_olditemValid;
  m_oldselfSwitchCh = other.m_oldselfSwitchCh;
  m_oldselfSwitchValid = other.m_oldselfSwitchValid;
  m_oldswitch1Id = other.m_oldswitch1Id;
  m_oldswitch1Valid = other.m_oldswitch1Valid;
  m_oldswitch2Id = other.m_oldswitch2Id;
  m_oldswitch2Valid = other.m_oldswitch2Valid;
  m_oldvariableId = other.m_oldvariableId;
  m_oldvariableValid = other.m_oldvariableValid;
  m_oldvariableValue = other.m_oldvariableValue;
  return *this;
}
EventCondition::EventCondition(EventCondition&& other) noexcept
: IModifiable(std::move(other))
, m_actorId(other.m_actorId)
, m_actorValid(other.m_actorValid)
, m_itemId(other.m_itemId)
, m_itemValid(other.m_itemValid)
, m_selfSwitchCh(std::move(other.m_selfSwitchCh))
, m_selfSwitchValid(other.m_selfSwitchValid)
, m_switch1Id(other.m_switch1Id)
, m_switch1Valid(other.m_switch1Valid)
, m_switch2Id(other.m_switch2Id)
, m_switch2Valid(other.m_switch2Valid)
, m_variableId(other.m_variableId)
, m_variableValid(other.m_variableValid)
, m_variableValue(other.m_variableValue)
, m_oldactorId(other.m_oldactorId)
, m_oldactorValid(other.m_oldactorValid)
, m_olditemId(other.m_olditemId)
, m_olditemValid(other.m_olditemValid)
, m_oldselfSwitchCh(std::move(other.m_oldselfSwitchCh))
, m_oldselfSwitchValid(other.m_oldselfSwitchValid)
, m_oldswitch1Id(other.m_oldswitch1Id)
, m_oldswitch1Valid(other.m_oldswitch1Valid)
, m_oldswitch2Id(other.m_oldswitch2Id)
, m_oldswitch2Valid(other.m_oldswitch2Valid)
, m_oldvariableId(other.m_oldvariableId)
, m_oldvariableValid(other.m_oldvariableValid)
, m_oldvariableValue(other.m_oldvariableValue) {}

EventCondition& EventCondition::operator=(EventCondition&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_actorId = other.m_actorId;
  m_actorValid = other.m_actorValid;
  m_itemId = other.m_itemId;
  m_itemValid = other.m_itemValid;
  m_selfSwitchCh = std::move(other.m_selfSwitchCh);
  m_selfSwitchValid = other.m_selfSwitchValid;
  m_switch1Id = other.m_switch1Id;
  m_switch1Valid = other.m_switch1Valid;
  m_switch2Id = other.m_switch2Id;
  m_switch2Valid = other.m_switch2Valid;
  m_variableId = other.m_variableId;
  m_variableValid = other.m_variableValid;
  m_variableValue = other.m_variableValue;
  m_oldactorId = other.m_oldactorId;
  m_oldactorValid = other.m_oldactorValid;
  m_olditemId = other.m_olditemId;
  m_olditemValid = other.m_olditemValid;
  m_oldselfSwitchCh = std::move(other.m_oldselfSwitchCh);
  m_oldselfSwitchValid = other.m_oldselfSwitchValid;
  m_oldswitch1Id = other.m_oldswitch1Id;
  m_oldswitch1Valid = other.m_oldswitch1Valid;
  m_oldswitch2Id = other.m_oldswitch2Id;
  m_oldswitch2Valid = other.m_oldswitch2Valid;
  m_oldvariableId = other.m_oldvariableId;
  m_oldvariableValid = other.m_oldvariableValid;
  m_oldvariableValue = other.m_oldvariableValue;
  return *this;
}
bool EventCondition::operator==(const EventCondition& other) const {
  return m_actorId == other.m_actorId && m_actorValid == other.m_actorValid && m_itemId == other.m_itemId && m_itemValid == other.m_itemValid && m_selfSwitchCh == other.m_selfSwitchCh &&
         m_switch1Id == other.m_switch1Id && m_switch1Valid == other.m_switch1Valid && m_switch2Id == other.m_switch2Id && m_switch2Valid == other.m_switch2Valid &&
         m_variableId == other.m_variableId && m_variableValid == other.m_variableValid && m_variableValue == other.m_variableValue;
}

int EventCondition::actorId() const { return m_actorId; }
void EventCondition::setActorId(const int actorId) {
  if (actorId == m_actorId) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(actorId);
  m_actorId = actorId;
  if (!signalsDisabled()) {
    emit_signal(actorIdModified(), this, actorId);
  }
  setModified();
}

bool EventCondition::actorValid() const { return m_actorValid; }
void EventCondition::setActorValid(const bool actorValid) {
  if (actorValid == m_actorValid) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(actorValid);
  m_actorValid = actorValid;
  if (!signalsDisabled()) {
    emit_signal(actorValidModified(), this, actorValid);
  }
  setModified();
}

int EventCondition::itemId() const { return m_itemId; }
void EventCondition::setItemId(const int itemId) {
  if (itemId == m_itemId) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(itemId);
  m_itemId = itemId;
  if (!signalsDisabled()) {
    emit_signal(itemIdModified(), this, itemId);
  }
  setModified();
}

bool EventCondition::itemValid() const { return m_itemValid; }
void EventCondition::setItemValid(const bool itemValid) {
  if (itemValid == m_itemValid) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(itemValid);
  m_itemValid = itemValid;
  if (!signalsDisabled()) {
    emit_signal(itemValidModified(), this, itemValid);
  }
  setModified();
}

std::string_view EventCondition::selfSwitchCh() const { return m_selfSwitchCh; }
void EventCondition::setSelfSwitchCh(const std::string_view selfSwitchCh) {
  if (selfSwitchCh == m_selfSwitchCh) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(selfSwitchCh);
  m_selfSwitchCh = selfSwitchCh;
  if (!signalsDisabled()) {
    emit_signal(selfSwitchModified(), this, m_selfSwitchCh);
  }
  setModified();
}

bool EventCondition::selfSwitchValid() const { return m_selfSwitchValid; }
void EventCondition::setSelfSwitchValid(const bool selfSwitchValid) {
  if (selfSwitchValid == m_selfSwitchValid) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(selfSwitchValid);
  m_selfSwitchValid = selfSwitchValid;
  if (!signalsDisabled()) {
    emit_signal(selfSwitchValidModified(), this, m_selfSwitchValid);
  }
  setModified();
}

int EventCondition::switch1Id() const { return m_switch1Id; }

void EventCondition::setSwitch1Id(const int switch1Id) {
  if (switch1Id == m_switch1Id) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(switch1Id);
  m_switch1Id = switch1Id;
  if (!signalsDisabled()) {
    emit_signal(switch1IdModified(), this, m_switch1Id);
  }
  setModified();
}

bool EventCondition::switch1Valid() const { return m_switch1Valid; }
void EventCondition::setSwitch1Valid(const bool switch1Valid) {
  if (switch1Valid == m_switch1Valid) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(switch1Valid);
  m_switch1Valid = switch1Valid;
  if (!signalsDisabled()) {
    emit_signal(switch1ValidModified(), this, m_switch1Valid);
  }
  setModified();
}

int EventCondition::switch2Id() const { return m_switch2Id; }
void EventCondition::setSwitch2Id(const int switch2Id) {
  if (switch2Id == m_switch2Id) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(switch2Id);
  m_switch2Id = switch2Id;
  if (!signalsDisabled()) {
    emit_signal(switch2IdModified(), this, m_switch2Id);
  }
  setModified();
}
bool EventCondition::switch2Valid() const { return m_switch2Valid; }
void EventCondition::setSwitch2Valid(const bool switch2Valid) {
  if (switch2Valid == m_switch2Valid) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(switch2Valid);
  m_switch2Valid = switch2Valid;
  if (!signalsDisabled()) {
    emit_signal(switch2ValidModified(), this, m_switch2Valid);
  }
  setModified();
}

int EventCondition::variableId() const { return m_variableId; }
void EventCondition::setVariableId(const int variableId) {
  if (variableId == m_variableId) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(variableId);
  m_variableId = variableId;
  if (!signalsDisabled()) {
    emit_signal(variableIdModified(), this, m_variableId);
  }
  setModified();
}

bool EventCondition::variableValid() const { return m_variableValid; }
void EventCondition::setVariableValid(const bool variableValid) {
  if (variableValid == m_variableValid) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(variableValid);
  m_variableValid = variableValid;
  if (!signalsDisabled()) {
    emit_signal(variableValidModified(), this, m_variableValid);
  }
  setModified();
}

int EventCondition::variableValue() const { return m_variableValue; }
void EventCondition::setVariableValue(const int variableValue) {
  if (variableValue == m_variableValue) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(variableValue);
  m_variableValue = variableValue;
  if (!signalsDisabled()) {
    emit_signal(variableValueModified(), this, m_variableValue);
  }
  setModified();
}

void EventCondition::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(actorId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(actorValid);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(itemId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(itemValid);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(selfSwitchCh);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(selfSwitchValid);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(switch1Id);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(switch1Valid);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(switch2Id);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(switch2Valid);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(variableId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(variableValid);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(variableValue);
}
void EventCondition::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(actorId);
  MODIFIABLE_ACCEPT_VALUE(actorValid);
  MODIFIABLE_ACCEPT_VALUE(itemId);
  MODIFIABLE_ACCEPT_VALUE(itemValid);
  MODIFIABLE_ACCEPT_VALUE(selfSwitchCh);
  MODIFIABLE_ACCEPT_VALUE(selfSwitchValid);
  MODIFIABLE_ACCEPT_VALUE(switch1Id);
  MODIFIABLE_ACCEPT_VALUE(switch1Valid);
  MODIFIABLE_ACCEPT_VALUE(switch2Id);
  MODIFIABLE_ACCEPT_VALUE(switch2Valid);
  MODIFIABLE_ACCEPT_VALUE(variableId);
  MODIFIABLE_ACCEPT_VALUE(variableValid);
  MODIFIABLE_ACCEPT_VALUE(variableValue);
}
nlohmann::ordered_json EventCondition::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(actorId),      MODIFIABLE_SERIALIZE_VALUE(actorValid), MODIFIABLE_SERIALIZE_VALUE(itemId),        MODIFIABLE_SERIALIZE_VALUE(itemValid),
      MODIFIABLE_SERIALIZE_VALUE(selfSwitchCh), MODIFIABLE_SERIALIZE_VALUE(switch1Id),  MODIFIABLE_SERIALIZE_VALUE(switch1Valid),  MODIFIABLE_SERIALIZE_VALUE(switch2Id),
      MODIFIABLE_SERIALIZE_VALUE(switch2Valid), MODIFIABLE_SERIALIZE_VALUE(variableId), MODIFIABLE_SERIALIZE_VALUE(variableValid), MODIFIABLE_SERIALIZE_VALUE(variableValue),
  };
}

rpgmutils::signal<void(EventCondition*, int)>& EventCondition::actorIdModified() {
  if (!m_actorIdModified) {
    m_actorIdModified.emplace();
  }
  return *m_actorIdModified;
}
rpgmutils::signal<void(EventCondition*, bool)>& EventCondition::actorValidModified() {
  if (!m_actorValidModified) {
    m_actorValidModified.emplace();
  }
  return *m_actorValidModified;
}
rpgmutils::signal<void(EventCondition*, int)>& EventCondition::itemIdModified() {
  if (!m_itemIdModified) {
    m_itemIdModified.emplace();
  }
  return *m_itemIdModified;
}
rpgmutils::signal<void(EventCondition*, bool)>& EventCondition::itemValidModified() {
  if (!m_itemValidModified) {
    m_itemValidModified.emplace();
  }
  return *m_itemValidModified;
}
rpgmutils::signal<void(EventCondition*, const std::string&)>& EventCondition::selfSwitchModified() {
  if (!m_selfSwitchModified) {
    m_selfSwitchModified.emplace();
  }
  return *m_selfSwitchModified;
}
rpgmutils::signal<void(EventCondition*, bool)>& EventCondition::selfSwitchValidModified() {
  if (!m_selfSwitchValidModified) {
    m_selfSwitchValidModified.emplace();
  }
  return *m_selfSwitchValidModified;
}
rpgmutils::signal<void(EventCondition*, int)>& EventCondition::switch1IdModified() {
  if (!m_switch1IdModified) {
    m_switch1IdModified.emplace();
  }
  return *m_switch1IdModified;
}
rpgmutils::signal<void(EventCondition*, bool)>& EventCondition::switch1ValidModified() {
  if (!m_switch1ValidModified) {
    m_switch1ValidModified.emplace();
  }
  return *m_switch1ValidModified;
}
rpgmutils::signal<void(EventCondition*, int)>& EventCondition::switch2IdModified() {
  if (!m_switch2IdModified) {
    m_switch2IdModified.emplace();
  }
  return *m_switch2IdModified;
}
rpgmutils::signal<void(EventCondition*, bool)>& EventCondition::switch2ValidModified() {
  if (!m_switch2ValidModified) {
    m_switch2ValidModified.emplace();
  }
  return *m_switch2ValidModified;
}
rpgmutils::signal<void(EventCondition*, int)>& EventCondition::variableIdModified() {
  if (!m_variableIdModified) {
    m_variableIdModified.emplace();
  }
  return *m_variableIdModified;
}
rpgmutils::signal<void(EventCondition*, bool)>& EventCondition::variableValidModified() {
  if (!m_variableValidModified) {
    m_variableValidModified.emplace();
  }
  return *m_variableValidModified;
}
rpgmutils::signal<void(EventCondition*, int)>& EventCondition::variableValueModified() {
  if (!m_variableValueModified) {
    m_variableValueModified.emplace();
  }
  return *m_variableValueModified;
}

EventImage::EventImage(const EventImage& other)
: IModifiable(other)
, m_tileId(other.m_tileId)
, m_characterName(other.m_characterName)
, m_direction(other.m_direction)
, m_pattern(other.m_pattern)
, m_characterIndex(other.m_characterIndex)
, m_oldtileId(other.m_oldtileId)
, m_oldcharacterName(other.m_oldcharacterName)
, m_oldpattern(other.m_oldpattern)
, m_oldcharacterIndex(other.m_oldcharacterIndex) {}
EventImage& EventImage::operator=(const EventImage& other) {
  IModifiable::operator=(other);
  m_tileId = other.m_tileId;
  m_characterName = other.m_characterName;
  m_direction = other.m_direction;
  m_pattern = other.m_pattern;
  m_characterIndex = other.m_characterIndex;
  m_oldtileId = other.m_oldtileId;
  m_oldcharacterName = other.m_oldcharacterName;
  m_oldpattern = other.m_oldpattern;
  m_oldcharacterIndex = other.m_oldcharacterIndex;
  return *this;
}
EventImage::EventImage(EventImage&& other) noexcept
: IModifiable(std::move(other))
, m_tileId(other.m_tileId)
, m_characterName(std::move(other.m_characterName))
, m_direction(other.m_direction)
, m_pattern(other.m_pattern)
, m_characterIndex(other.m_characterIndex)
, m_oldtileId(other.m_oldtileId)
, m_oldcharacterName(std::move(other.m_oldcharacterName))
, m_oldpattern(other.m_oldpattern)
, m_oldcharacterIndex(other.m_oldcharacterIndex) {}
EventImage& EventImage::operator=(EventImage&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_tileId = other.m_tileId;
  m_characterName = std::move(other.m_characterName);
  m_direction = other.m_direction;
  m_pattern = other.m_pattern;
  m_characterIndex = other.m_characterIndex;
  m_oldtileId = other.m_oldtileId;
  m_oldcharacterName = other.m_oldcharacterName;
  m_oldpattern = other.m_oldpattern;
  m_oldcharacterIndex = other.m_oldcharacterIndex;
  return *this;
}
bool EventImage::operator==(const EventImage& other) const {
  return m_tileId == other.m_tileId && m_characterName == other.m_characterName && m_direction == other.m_direction && m_pattern == other.m_pattern && m_characterIndex == other.m_characterIndex;
}

int EventImage::tileId() const { return m_tileId; }
void EventImage::setTileId(int tileId) {
  if (m_tileId == tileId) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(tileId);
  m_tileId = tileId;
  if (!signalsDisabled()) {
    emit_signal(tileIdModified(), this, tileId);
  }
  setModified();
}

std::string_view EventImage::characterName() const { return m_characterName; }
void EventImage::setCharacterName(std::string_view characterName) {
  if (m_characterName == characterName) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(characterName);
  m_characterName = characterName;
  if (!signalsDisabled()) {
    emit_signal(characterNameModified(), this, m_characterName);
  }
  setModified();
}
Direction EventImage::direction() const { return m_direction; }
void EventImage::setDirection(const Direction direction) {
  if (m_direction == direction) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(direction);
  m_direction = direction;
  if (!signalsDisabled()) {
    emit_signal(directionModified(), this, m_direction);
  }
  setModified();
}

int EventImage::pattern() const { return m_pattern; }
void EventImage::setPattern(const int pattern) {
  if (m_pattern == pattern) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(pattern);
  m_pattern = pattern;
  if (!signalsDisabled()) {
    emit_signal(patternModified(), this, m_pattern);
  }
  setModified();
}

int EventImage::characterIndex() const { return m_characterIndex; }
void EventImage::setCharacterIndex(const int characterIndex) {
  if (m_characterIndex == characterIndex) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(characterIndex);
  m_characterIndex = characterIndex;
  if (!signalsDisabled()) {
    emit_signal(characterIndexModified(), this, m_characterIndex);
  }
  setModified();
}

void EventImage::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(tileId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(characterName);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(direction);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(pattern);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(characterIndex);
}
void EventImage::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(tileId);
  MODIFIABLE_ACCEPT_VALUE(characterName);
  MODIFIABLE_ACCEPT_VALUE(direction);
  MODIFIABLE_ACCEPT_VALUE(pattern);
  MODIFIABLE_ACCEPT_VALUE(characterIndex);
}
nlohmann::ordered_json EventImage::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(tileId),  MODIFIABLE_SERIALIZE_VALUE(characterName),  MODIFIABLE_SERIALIZE_VALUE(direction),
      MODIFIABLE_SERIALIZE_VALUE(pattern), MODIFIABLE_SERIALIZE_VALUE(characterIndex),
  };
}

rpgmutils::signal<void(EventImage*, int)>& EventImage::tileIdModified() {
  if (!m_tileIdModified) {
    m_tileIdModified.emplace();
  }
  return *m_tileIdModified;
}
rpgmutils::signal<void(EventImage*, const std::string&)>& EventImage::characterNameModified() {
  if (!m_characterNameModified) {
    m_characterNameModified.emplace();
  }
  return *m_characterNameModified;
}
rpgmutils::signal<void(EventImage*, Direction)>& EventImage::directionModified() {
  if (!m_directionModified) {
    m_directionModified.emplace();
  }
  return *m_directionModified;
}
rpgmutils::signal<void(EventImage*, int)>& EventImage::patternModified() {
  if (!m_patternModified) {
    m_patternModified.emplace();
  }
  return *m_patternModified;
}
rpgmutils::signal<void(EventImage*, int)>& EventImage::characterIndexModified() {
  if (!m_characterIndexModified) {
    m_characterIndexModified.emplace();
  }
  return *m_characterIndexModified;
}

EventPage::EventPage() { m_list.emplace_back(new EventDummy())->setIndent(0); }

EventPage::EventPage(const EventPage& other)
: IModifiable(other)
, m_conditions(other.m_conditions)
, m_directionFix(other.m_directionFix)
, m_image(other.m_image)
, m_list(other.m_list)
, m_moveFrequency(other.m_moveFrequency)
, m_moveRoute(other.m_moveRoute)
, m_moveSpeed(other.m_moveSpeed)
, m_moveType(other.m_moveType)
, m_priorityType(other.m_priorityType)
, m_stepAnime(other.m_stepAnime)
, m_through(other.m_through)
, m_trigger(other.m_trigger)
, m_walkAnime(other.m_walkAnime)
, m_name(other.m_name)
, m_oldconditions(other.m_oldconditions)
, m_olddirectionFix(other.m_olddirectionFix)
, m_oldimage(other.m_oldimage)
, m_oldlist(other.m_oldlist)
, m_oldmoveFrequency(other.m_oldmoveFrequency)
, m_oldmoveRoute(other.m_oldmoveRoute)
, m_oldmoveSpeed(other.m_oldmoveSpeed)
, m_oldmoveType(other.m_oldmoveType)
, m_oldstepAnime(other.m_oldstepAnime)
, m_oldthrough(other.m_oldthrough)
, m_oldtrigger(other.m_oldtrigger)
, m_oldwalkAnime(other.m_oldwalkAnime)
, m_oldname(other.m_oldname)
, m_editor(other.m_editor) {
  if (m_editor) {
    m_editor->setPagePtr(this);
  }
}

EventPage& EventPage::operator=(const EventPage& other) {
  IModifiable::operator=(other);
  m_conditions = other.m_conditions;
  m_directionFix = other.m_directionFix;
  m_image = other.m_image;
  m_list = other.m_list;
  m_moveFrequency = other.m_moveFrequency;
  m_moveRoute = other.m_moveRoute;
  m_moveSpeed = other.m_moveSpeed;
  m_moveType = other.m_moveType;
  m_priorityType = other.m_priorityType;
  m_stepAnime = other.m_stepAnime;
  m_through = other.m_through;
  m_trigger = other.m_trigger;
  m_walkAnime = other.m_walkAnime;
  m_name = other.m_name;
  m_oldconditions = other.m_oldconditions;
  m_olddirectionFix = other.m_olddirectionFix;
  m_oldimage = other.m_oldimage;
  m_oldlist = other.m_oldlist;
  m_oldmoveFrequency = other.m_oldmoveFrequency;
  m_oldmoveRoute = other.m_oldmoveRoute;
  m_oldmoveSpeed = other.m_oldmoveSpeed;
  m_oldmoveType = other.m_oldmoveType;
  m_oldstepAnime = other.m_oldstepAnime;
  m_oldthrough = other.m_oldthrough;
  m_oldtrigger = other.m_oldtrigger;
  m_oldwalkAnime = other.m_oldwalkAnime;
  m_oldname = other.m_oldname;
  m_editor = other.m_editor;
  if (m_editor) {
    m_editor->setPagePtr(this);
  }
  return *this;
}
EventPage::EventPage(EventPage&& other) noexcept
: IModifiable(std::move(other))
, m_conditions(other.m_conditions)
, m_directionFix(other.m_directionFix)
, m_image(std::move(other.m_image))
, m_list(std::move(other.m_list))
, m_moveFrequency(other.m_moveFrequency)
, m_moveRoute(other.m_moveRoute)
, m_moveSpeed(other.m_moveSpeed)
, m_moveType(other.m_moveType)
, m_priorityType(other.m_priorityType)
, m_stepAnime(other.m_stepAnime)
, m_through(other.m_through)
, m_trigger(other.m_trigger)
, m_walkAnime(other.m_walkAnime)
, m_name(other.m_name)
, m_oldconditions(other.m_oldconditions)
, m_olddirectionFix(other.m_olddirectionFix)
, m_oldimage(other.m_oldimage)
, m_oldlist(other.m_oldlist)
, m_oldmoveFrequency(other.m_oldmoveFrequency)
, m_oldmoveRoute(other.m_oldmoveRoute)
, m_oldmoveSpeed(other.m_oldmoveSpeed)
, m_oldmoveType(other.m_oldmoveType)
, m_oldstepAnime(other.m_oldstepAnime)
, m_oldthrough(other.m_oldthrough)
, m_oldtrigger(other.m_oldtrigger)
, m_oldwalkAnime(other.m_oldwalkAnime)
, m_oldname(other.m_oldname)
, m_editor(std::move(other.m_editor)) {
  if (m_editor) {
    m_editor->setPagePtr(this);
  }
}

EventPage& EventPage::operator=(EventPage&& other) noexcept {
  IModifiable::operator=(other);
  m_conditions = std::move(other.m_conditions);
  m_directionFix = other.m_directionFix;
  m_image = std::move(other.m_image);
  m_list = std::move(other.m_list);
  m_moveFrequency = other.m_moveFrequency;
  m_moveRoute = std::move(other.m_moveRoute);
  m_moveSpeed = other.m_moveSpeed;
  m_moveType = other.m_moveType;
  m_priorityType = other.m_priorityType;
  m_stepAnime = other.m_stepAnime;
  m_through = other.m_through;
  m_trigger = other.m_trigger;
  m_walkAnime = other.m_walkAnime;
  m_name = other.m_name;
  m_oldconditions = other.m_oldconditions;
  m_olddirectionFix = other.m_olddirectionFix;
  m_oldimage = other.m_oldimage;
  m_oldlist = std::move(other.m_oldlist);
  m_oldmoveFrequency = other.m_oldmoveFrequency;
  m_oldmoveRoute = std::move(other.m_oldmoveRoute);
  m_oldmoveSpeed = other.m_oldmoveSpeed;
  m_oldmoveType = other.m_oldmoveType;
  m_oldstepAnime = other.m_oldstepAnime;
  m_oldthrough = other.m_oldthrough;
  m_oldtrigger = other.m_oldtrigger;
  m_oldwalkAnime = other.m_oldwalkAnime;
  m_oldname = std::move(other.m_oldname);
  m_editor = std::move(other.m_editor);
  if (m_editor) {
    m_editor->setPagePtr(this);
  }
  return *this;
}

EventCondition& EventPage::conditions() { return m_conditions; }
const EventCondition& EventPage::conditions() const { return m_conditions; }
void EventPage::setCondition(const EventCondition& condition) {
  if (condition == m_conditions) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(conditions);
  m_conditions = condition;
  if (!signalsDisabled()) {
    emit_signal(conditionsModified(), this, condition);
  }
  setModified();
}

bool EventPage::directionFix() const { return m_directionFix; }
void EventPage::setDirectionFix(const bool directionFix) {
  if (directionFix == m_directionFix) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(directionFix);
  m_directionFix = directionFix;
  if (!signalsDisabled()) {
    emit_signal(directionFixModified(), this, m_directionFix);
  }
  setModified();
}

EventImage& EventPage::image() { return m_image; }
const EventImage& EventPage::image() const { return m_image; }
void EventPage::setImage(const EventImage& image) {
  if (image == m_image) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(image);
  m_image = image;
  if (!signalsDisabled()) {
    emit_signal(imageModified(), this, m_image);
  }
  setModified();
}

std::vector<std::shared_ptr<IEventCommand>>& EventPage::list() { return m_list; }
const std::vector<std::shared_ptr<IEventCommand>>& EventPage::list() const { return m_list; }
void EventPage::setList(const std::vector<std::shared_ptr<IEventCommand>>& list) {
  if (list == m_list) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(list);
  m_list = list;
  if (!signalsDisabled()) {
    emit_signal(listModified(), this, m_list);
  }
  setModified();
}

MovementRoute& EventPage::moveRoute() { return m_moveRoute; }
const MovementRoute& EventPage::moveRoute() const { return m_moveRoute; }
void EventPage::setMoveRoute(const MovementRoute& moveRoute) {
  if (m_moveRoute == moveRoute) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(moveRoute);
  m_moveRoute = moveRoute;
  if (!signalsDisabled()) {
    emit_signal(moveRouteModified(), this, m_moveRoute);
  }
  setModified();
}

MovementSpeed EventPage::moveSpeed() const { return m_moveSpeed; }
void EventPage::setMoveSpeed(const MovementSpeed movementSpeed) {
  if (movementSpeed == m_moveSpeed) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(moveSpeed);
  m_moveSpeed = movementSpeed;
  if (!signalsDisabled()) {
    emit_signal(moveSpeedModified(), this, m_moveSpeed);
  }
  setModified();
}

MovementFrequency EventPage::moveFrequency() const { return m_moveFrequency; }
void EventPage::setMoveFrequency(const MovementFrequency moveFrequency) {
  if (moveFrequency == m_moveFrequency) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(moveFrequency);
  m_moveFrequency = moveFrequency;
  if (!signalsDisabled()) {
    emit_signal(moveFrequencyModified(), this, m_moveFrequency);
  }
  setModified();
}

MoveType EventPage::moveType() const { return m_moveType; }
void EventPage::setMoveType(const MoveType movementType) {
  if (movementType == m_moveType) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(moveType);
  m_moveType = movementType;
  if (!signalsDisabled()) {
    emit_signal(moveTypeModified(), this, m_moveType);
  }
  setModified();
}

EventPriority EventPage::priorityType() const { return m_priorityType; }
void EventPage::setPriorityType(const EventPriority priority) {
  if (priority == m_priorityType) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(priorityType);
  m_priorityType = priority;
  if (!signalsDisabled()) {
    emit_signal(priorityTypeModified(), this, m_priorityType);
  }
  setModified();
}

bool EventPage::stepAnime() const { return m_stepAnime; }
void EventPage::setStepAnime(const bool stepAnime) {
  if (stepAnime == m_stepAnime) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(stepAnime);
  m_stepAnime = stepAnime;
  if (!signalsDisabled()) {
    emit_signal(stepAnimeModified(), this, m_stepAnime);
  }
  setModified();
}

bool EventPage::through() const { return m_through; }
void EventPage::setThrough(const bool through) {
  if (through == m_through) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(through);
  m_through = through;
  if (!signalsDisabled()) {
    emit_signal(throughModified(), this, m_through);
  }
  setModified();
}

EventTriggerType EventPage::trigger() const { return m_trigger; }
void EventPage::setTrigger(const EventTriggerType trigger) {
  if (trigger == m_trigger) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(trigger);
  m_trigger = trigger;
  if (!signalsDisabled()) {
    emit_signal(triggerModified(), this, m_trigger);
  }
  setModified();
}

bool EventPage::walkAnime() const { return m_walkAnime; }
void EventPage::setWalkAnime(const bool walkAnime) {
  if (walkAnime == m_walkAnime) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(walkAnime);
  m_walkAnime = walkAnime;
  if (!signalsDisabled()) {
    emit_signal(walkAnimeModified(), this, m_walkAnime);
  }
  setModified();
}

std::string_view EventPage::name() const { return m_name ? *m_name : ""sv; }
void EventPage::setName(std::string_view name) {
  if (m_name && name == *m_name) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(name);
  m_name = name;
  if (!signalsDisabled()) {
    emit_signal(nameModified(), this, name.data());
  }
  setModified();
}

void EventPage::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(conditions);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(directionFix);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(image);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(list);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(moveRoute);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(moveSpeed);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(moveFrequency);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(moveType);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(priorityType);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(stepAnime);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(walkAnime);
  if (m_oldname) {
    m_name = m_oldname;
    m_oldname.reset();
  }
  for (const auto& ev : m_list) {
    ev->restoreOriginal();
  }
}
void EventPage::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(conditions);
  MODIFIABLE_ACCEPT_VALUE(directionFix);
  MODIFIABLE_ACCEPT_VALUE(image);
  MODIFIABLE_ACCEPT_VALUE(list);
  MODIFIABLE_ACCEPT_VALUE(moveRoute);
  MODIFIABLE_ACCEPT_VALUE(moveSpeed);
  MODIFIABLE_ACCEPT_VALUE(moveFrequency);
  MODIFIABLE_ACCEPT_VALUE(moveType);
  MODIFIABLE_ACCEPT_VALUE(priorityType);
  MODIFIABLE_ACCEPT_VALUE(stepAnime);
  MODIFIABLE_ACCEPT_VALUE(walkAnime);
  MODIFIABLE_ACCEPT_VALUE(through);
  MODIFIABLE_ACCEPT_VALUE(name);

  for (const auto& ev : m_list) {
    ev->acceptChanges();
  }
}
nlohmann::ordered_json EventPage::serializeOldValues() const {
  nlohmann::ordered_json json;
  json["conditions"] = m_oldconditions ? *m_oldconditions : m_conditions;
  json["directionFix"] = m_olddirectionFix ? *m_olddirectionFix : m_directionFix;
  json["image"] = m_oldimage ? *m_oldimage : m_image;
  CommandParser::serialize(json["list"], m_oldlist ? *m_oldlist : m_list, false, true);
  json["moveFrequency"] = m_oldmoveFrequency ? *m_oldmoveFrequency : m_moveFrequency;
  json["moveRoute"] = m_oldmoveRoute ? *m_oldmoveRoute : m_moveRoute;
  json["moveSpeed"] = m_oldmoveSpeed ? *m_oldmoveSpeed : m_moveSpeed;
  json["moveType"] = m_oldmoveType ? *m_oldmoveType : m_moveType;
  json["priorityType"] = m_oldpriorityType ? *m_oldpriorityType : m_priorityType;
  json["stepAnime"] = m_oldstepAnime ? *m_oldstepAnime : m_stepAnime;
  json["through"] = m_oldthrough ? *m_oldthrough : m_through;
  json["trigger"] = m_oldtrigger ? *m_oldtrigger : m_trigger;
  json["walkAnime"] = m_oldwalkAnime ? *m_oldwalkAnime : m_walkAnime;
  auto& tmp = m_oldname ? m_oldname : m_name;
  if (tmp) {
    json["name"] = *tmp;
  }
  return json;
}

rpgmutils::signal<void(EventPage*, EventCondition)>& EventPage::conditionsModified() {
  if (!m_conditionsModified) {
    m_conditionsModified.emplace();
  }
  return *m_conditionsModified;
}
rpgmutils::signal<void(EventPage*, bool)>& EventPage::directionFixModified() {
  if (!m_directionFixModified) {
    m_directionFixModified.emplace();
  }
  return *m_directionFixModified;
}
rpgmutils::signal<void(EventPage*, EventImage)>& EventPage::imageModified() {
  if (!m_imageModified) {
    m_imageModified.emplace();
  }
  return *m_imageModified;
}
rpgmutils::signal<void(EventPage*, std::vector<std::shared_ptr<IEventCommand>>)>& EventPage::listModified() {
  if (!m_listModified) {
    m_listModified.emplace();
  }
  return *m_listModified;
}
rpgmutils::signal<void(EventPage*, MovementFrequency)>& EventPage::moveFrequencyModified() {
  if (!m_moveFrequencyModified) {
    m_moveFrequencyModified.emplace();
  }
  return *m_moveFrequencyModified;
}
rpgmutils::signal<void(EventPage*, MovementRoute)>& EventPage::moveRouteModified() {
  if (!m_moveRouteModified) {
    m_moveRouteModified.emplace();
  }
  return *m_moveRouteModified;
}
rpgmutils::signal<void(EventPage*, MovementSpeed)>& EventPage::moveSpeedModified() {
  if (!m_moveSpeedModified) {
    m_moveSpeedModified.emplace();
  }
  return *m_moveSpeedModified;
}
rpgmutils::signal<void(EventPage*, MoveType)>& EventPage::moveTypeModified() {
  if (!m_moveTypeModified) {
    m_moveTypeModified.emplace();
  }
  return *m_moveTypeModified;
}
rpgmutils::signal<void(EventPage*, EventPriority)>& EventPage::priorityTypeModified() {
  if (!m_priorityTypeModified) {
    m_priorityTypeModified.emplace();
  }
  return *m_priorityTypeModified;
}
rpgmutils::signal<void(EventPage*, bool)>& EventPage::stepAnimeModified() {
  if (!m_stepAnimeModified) {
    m_stepAnimeModified.emplace();
  }
  return *m_stepAnimeModified;
}
rpgmutils::signal<void(EventPage*, bool)>& EventPage::throughModified() {
  if (!m_throughModified) {
    m_throughModified.emplace();
  }
  return *m_throughModified;
}
rpgmutils::signal<void(EventPage*, EventTriggerType)>& EventPage::triggerModified() {
  if (!m_triggerModified) {
    m_triggerModified.emplace();
  }
  return *m_triggerModified;
}
rpgmutils::signal<void(EventPage*, bool)>& EventPage::walkAnimeModified() {
  if (!m_walkAnimeModified) {
    m_walkAnimeModified.emplace();
  }
  return *m_walkAnimeModified;
}
/* OpenRPGMaker Additions */
rpgmutils::signal<void(EventPage*, const std::string&)>& EventPage::nameModified() {
  if (!m_nameModified) {
    m_nameModified.emplace();
  }
  return *m_nameModified;
}

void EventPage::clear() {
  std::vector<std::shared_ptr<IEventCommand>> list;
  list.emplace_back(new EventDummy())->setIndent(0);
  setList(list);
}

void EventPage::connectAllSignals() {
  m_conditions.modified().disconnect<&EventPage::onValueModified>(this);
  m_conditions.modified().connect<&EventPage::onValueModified>(this);
  m_image.modified().disconnect<&EventPage::onValueModified>(this);
  m_image.modified().connect<&EventPage::onValueModified>(this);

  for (const auto& cmd : m_list) {
    cmd->modified().disconnect<&EventPage::onValueModified>(this);
    cmd->modified().connect<&EventPage::onValueModified>(this);
    // TODO: Add this
    // cmd->connectAllSignals();
  }
}

void EventPage::onValueModified(IModifiable* v) { emit_signal(modified(), this); }

void to_json(nlohmann::ordered_json& to, const EventCondition& cond) {
  to = {
      {"actorId", cond.m_actorId},
      {"actorValid", cond.m_actorValid},
      {"itemId", cond.m_itemId},
      {"itemValid", cond.m_itemValid},
      {"selfSwitchCh", cond.m_selfSwitchCh},
      {"selfSwitchValid", cond.m_selfSwitchValid},
      {"switch1Id", cond.m_switch1Id},
      {"switch1Valid", cond.m_switch1Valid},
      {"switch2Id", cond.m_switch2Id},
      {"switch2Valid", cond.m_switch2Valid},
      {"variableId", cond.m_variableId},
      {"variableValid", cond.m_variableValid},
      {"variableValue", cond.m_variableValue},
  };
}
void from_json(const nlohmann::ordered_json& from, EventCondition& cond) {
  cond.m_actorId = from.value("actorId", cond.m_actorId);
  cond.m_actorValid = from.value("actorValid", cond.m_actorValid);
  cond.m_itemId = from.value("itemId", cond.m_itemId);
  cond.m_itemValid = from.value("itemValid", cond.m_itemValid);
  cond.m_selfSwitchCh = from.value("selfSwitchCh", cond.m_selfSwitchCh);
  cond.m_selfSwitchValid = from.value("selfSwitchValid", cond.m_selfSwitchValid);
  cond.m_switch1Id = from.value("switch1Id", cond.m_switch1Id);
  cond.m_switch1Valid = from.value("switch1Valid", cond.m_switch1Valid);
  cond.m_switch2Id = from.value("switch2Id", cond.m_switch2Id);
  cond.m_switch2Valid = from.value("switch2Valid", cond.m_switch2Valid);
  cond.m_variableId = from.value("variableId", cond.m_variableId);
  cond.m_variableValid = from.value("variableValid", cond.m_variableValid);
  cond.m_variableValue = from.value("variableValue", cond.m_variableValue);
}

void to_json(nlohmann::ordered_json& to, const EventImage& image) {
  to = {
      {"characterIndex", image.m_characterIndex}, {"characterName", image.m_characterName}, {"direction", image.m_direction}, {"pattern", image.m_pattern}, {"tileId", image.m_tileId},
  };
}
void from_json(const nlohmann::ordered_json& from, EventImage& image) {
  image.m_characterIndex = from.value("characterIndex", image.m_characterIndex);
  image.m_characterName = from.value("characterName", image.m_characterName);
  image.m_direction = from.value("direction", image.m_direction);
  image.m_pattern = from.value("pattern", image.m_pattern);
  image.m_tileId = from.value("tileId", image.m_tileId);
}

void to_json(nlohmann::ordered_json& json, const EventPage& eventPage) {
  json["conditions"] = eventPage.m_conditions;
  json["directionFix"] = eventPage.m_directionFix;
  json["image"] = eventPage.m_image;
  CommandParser::serialize(json["list"], eventPage.m_list);
  json["moveFrequency"] = eventPage.m_moveFrequency;
  json["moveRoute"] = eventPage.m_moveRoute;
  json["moveSpeed"] = eventPage.m_moveSpeed;
  json["moveType"] = eventPage.m_moveType;
  json["priorityType"] = eventPage.m_priorityType;
  json["stepAnime"] = eventPage.m_stepAnime;
  json["through"] = eventPage.m_through;
  json["trigger"] = eventPage.m_trigger;
  json["walkAnime"] = eventPage.m_walkAnime;
  if (eventPage.m_name) {
    json["name"] = eventPage.m_name;
  }
}

void from_json(const nlohmann::ordered_json& json, EventPage& eventPage) {
  eventPage.m_conditions = json.value("conditions", eventPage.m_conditions);
  eventPage.m_directionFix = json.value("directionFix", eventPage.m_directionFix);
  eventPage.m_image = json.value("image", eventPage.m_image);
  CommandParser cmdParser;
  eventPage.m_list = cmdParser.parse(json["list"]);
  eventPage.m_moveFrequency = json.value("moveFrequency", eventPage.m_moveFrequency);
  eventPage.m_moveRoute = json.value("moveRoute", eventPage.m_moveRoute);
  eventPage.m_moveSpeed = json.value("moveSpeed", eventPage.m_moveSpeed);
  eventPage.m_moveType = json.value("moveType", eventPage.m_moveType);
  eventPage.m_priorityType = json.value("priorityType", eventPage.m_priorityType);
  eventPage.m_stepAnime = json.value("stepAnime", eventPage.m_stepAnime);
  eventPage.m_through = json.value("through", eventPage.m_through);
  eventPage.m_trigger = json.value("trigger", eventPage.m_trigger);
  eventPage.m_walkAnime = json.value("walkAnime", eventPage.m_walkAnime);
  eventPage.m_name = json.value("name", eventPage.m_name);
}
