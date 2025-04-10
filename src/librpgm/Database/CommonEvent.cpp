#include "Database/CommonEvent.hpp"

CommonEvent::CommonEvent(const CommonEvent& other)
: IModifiable(other)
, m_id(other.m_id)
, m_name(other.m_name)
, m_commands(other.m_commands)
, m_switchId(other.m_switchId)
, m_trigger(other.m_trigger)
, m_oldid(other.m_oldid)
, m_oldname(other.m_oldname)
, m_oldcommands(other.m_oldcommands)
, m_oldswitchId(other.m_oldswitchId)
, m_oldtrigger(other.m_oldtrigger) {}
CommonEvent& CommonEvent::operator=(const CommonEvent& other) {
  IModifiable::operator=(other);
  m_id = other.m_id;
  m_name = other.m_name;
  m_commands = other.m_commands;
  m_switchId = other.m_switchId;
  m_trigger = other.m_trigger;
  m_oldid = other.m_oldid;
  m_oldname = other.m_oldname;
  m_oldcommands = other.m_oldcommands;
  m_oldswitchId = other.m_oldswitchId;
  m_oldtrigger = other.m_oldtrigger;
  return *this;
}
CommonEvent::CommonEvent(CommonEvent&& other) noexcept
: IModifiable(std::move(other))
, m_id(other.m_id)
, m_name(std::move(other.m_name))
, m_commands(std::move(other.m_commands))
, m_switchId(other.m_switchId)
, m_trigger(other.m_trigger)
, m_oldid(other.m_oldid)
, m_oldname(std::move(other.m_oldname))
, m_oldcommands(std::move(other.m_oldcommands))
, m_oldswitchId(other.m_oldswitchId)
, m_oldtrigger(other.m_oldtrigger) {}

CommonEvent& CommonEvent::operator=(CommonEvent&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_id = other.m_id;
  m_name = std::move(other.m_name);
  m_commands = std::move(other.m_commands);
  m_switchId = other.m_switchId;
  m_trigger = other.m_trigger;
  m_oldid = other.m_oldid;
  m_oldname = std::move(other.m_oldname);
  m_oldcommands = std::move(other.m_oldcommands);
  m_oldswitchId = other.m_oldswitchId;
  m_oldtrigger = other.m_oldtrigger;
  return *this;
}

[[nodiscard]] int CommonEvent::id() const { return m_id; }
void CommonEvent::setId(const int id) { m_id = id; }

std::vector<std::shared_ptr<IEventCommand>>& CommonEvent::commands() { return m_commands; }
const std::vector<std::shared_ptr<IEventCommand>>& CommonEvent::commands() const { return m_commands; }
void CommonEvent::setCommands(const std::vector<std::shared_ptr<IEventCommand>>& commands) {
  if (m_commands == commands) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(commands);
  m_commands = commands;
  if (!signalsDisabled()) {
  emit_signal(commandsModified(), this, commands);
  }
  setModified(true);
}

const std::string& CommonEvent::name() const { return m_name; }
void CommonEvent::setName(const std::string& name) {
  if (m_name == name) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(name);
  m_name = name;
  if (!signalsDisabled()) {
  emit_signal(nameModified(), this, name);
  }
  setModified(true);
}

int CommonEvent::switchId() const { return m_switchId; }
void CommonEvent::setSwitchId(const int id) {
  if (m_switchId == id) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(id);
  m_switchId = id;
  if (!signalsDisabled()) {
  emit_signal(switchIdModified(), this, id);
  }
  setModified(true);
}

CommonEventTriggerType CommonEvent::trigger() const { return m_trigger; }
void CommonEvent::setTrigger(const CommonEventTriggerType trigger) {
  if (m_trigger == trigger) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(trigger);
  m_trigger = trigger;
  if (!signalsDisabled()) {
  emit_signal(triggerModified(), this, trigger);
  }
  setModified(true);
}

void CommonEvent::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(id);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(name);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(commands);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(switchId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(trigger);

  for (const auto& command : m_commands) {
    command->restoreOriginal();
  }
}
void CommonEvent::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(id);
  MODIFIABLE_ACCEPT_VALUE(name);
  MODIFIABLE_ACCEPT_VALUE(commands);
  MODIFIABLE_ACCEPT_VALUE(switchId);
  MODIFIABLE_ACCEPT_VALUE(trigger);
  for (const auto& command : m_commands) {
    command->acceptChanges();
  }
}
nlohmann::ordered_json CommonEvent::serializeOldValues() const {
  nlohmann::ordered_json j;
  j["id"] = m_id;
  j["name"] = m_name;
  CommandParser::serialize(j["list"], m_commands, false, true);
  j["switchId"] = m_switchId;
  j["trigger"] = m_trigger;
  return j;
}

rpgmutils::signal<void(CommonEvent*, int)>& CommonEvent::idModified() {
  if (!m_idModified) {
    m_idModified.emplace();
  }
  return *m_idModified;
}
rpgmutils::signal<void(CommonEvent*, const std::vector<std::shared_ptr<IEventCommand>>&)>& CommonEvent::commandsModified() {
  if (!m_commandsModified) {
    m_commandsModified.emplace();
  }
  return *m_commandsModified;
}
rpgmutils::signal<void(CommonEvent*, const std::string&)>& CommonEvent::nameModified() {
  if (!m_nameModified) {
    m_nameModified.emplace();
  }
  return *m_nameModified;
}
rpgmutils::signal<void(CommonEvent*, int)>& CommonEvent::switchIdModified() {
  if (!m_switchIdModified) {
    m_switchIdModified.emplace();
  }
  return *m_switchIdModified;
}
rpgmutils::signal<void(CommonEvent*, CommonEventTriggerType)>& CommonEvent::triggerModified() {
  if (!m_triggerModified) {
    m_triggerModified.emplace();
  }
  return *m_triggerModified;
}

void to_json(nlohmann::ordered_json& to, const CommonEvent& event) {
  CommandParser parser;
  nlohmann::ordered_json commands;
  parser.serialize(commands, event.commands());

  to = nlohmann::ordered_json{
      {"id", event.m_id}, {"list", commands}, {"name", event.m_name}, {"switchId", event.m_switchId}, {"trigger", event.m_trigger},
  };
}
void from_json(const nlohmann::ordered_json& from, CommonEvent& event) {
  event.m_id = from.value("id", event.m_id);
  event.m_name = from.value("name", event.m_name);
  CommandParser parser;
  event.m_commands = parser.parse(from.at("list"));
  event.m_switchId = from.value("switchId", event.m_switchId);
  event.m_trigger = from.value("trigger", event.m_trigger);
}