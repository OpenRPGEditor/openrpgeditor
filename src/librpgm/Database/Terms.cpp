#include "Database/Terms.hpp"

Terms::Terms(const Terms& other)
: IModifiable(other)
, m_basic(other.m_basic)
, m_commands(other.m_commands)
, m_messages(other.m_messages)
, m_params(other.m_params)
, m_oldbasic(other.m_oldbasic)
, m_oldcommands(other.m_oldcommands)
, m_oldmessages(other.m_oldmessages)
, m_oldparams(other.m_oldparams) {}

Terms& Terms::operator=(const Terms& other) {
  IModifiable::operator=(other);
  m_basic = other.m_basic;
  m_commands = other.m_commands;
  m_messages = other.m_messages;
  m_params = other.m_params;
  m_oldbasic = other.m_oldbasic;
  m_oldcommands = other.m_oldcommands;
  m_oldmessages = other.m_oldmessages;
  m_oldparams = other.m_oldparams;
  return *this;
}

Terms::Terms(Terms&& other) noexcept
: IModifiable(std::move(other))
, m_basic(std::move(other.m_basic))
, m_commands(std::move(other.m_commands))
, m_messages(std::move(other.m_messages))
, m_params(std::move(other.m_params))
, m_oldbasic(std::move(other.m_oldbasic))
, m_oldcommands(std::move(other.m_oldcommands))
, m_oldmessages(std::move(other.m_oldmessages))
, m_oldparams(std::move(other.m_oldparams)) {}

Terms& Terms::operator=(Terms&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_basic = std::move(other.m_basic);
  m_commands = std::move(other.m_commands);
  m_messages = std::move(other.m_messages);
  m_params = std::move(other.m_params);
  m_oldbasic = std::move(other.m_oldbasic);
  m_oldcommands = std::move(other.m_oldcommands);
  m_oldmessages = std::move(other.m_oldmessages);
  m_oldparams = std::move(other.m_oldparams);
  return *this;
}

std::vector<std::optional<std::string>>& Terms::basic() { return m_basic; }
const std::vector<std::optional<std::string>>& Terms::basic() const { return m_basic; }
void Terms::setBasic(std::vector<std::optional<std::string>>& value) {
  if (m_basic == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(basic);
  m_basic = value;
  if (!signalsDisabled()) {
   emit_signal( basicModified(), this, value);
  }
  setModified();
}

std::vector<std::optional<std::string>>& Terms::commands() { return m_commands; }
const std::vector<std::optional<std::string>>& Terms::commands() const { return m_commands; }
void Terms::setCommands(std::vector<std::optional<std::string>>& value) {
  if (m_commands == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(commands);
  m_commands = value;
  if (!signalsDisabled()) {
    emit_signal(commandsModified(), this, value);
  }
  setModified();
}

Messages& Terms::messages() { return m_messages; }
const Messages& Terms::messages() const { return m_messages; }
void Terms::setMessages(const Messages& value) {
  if (m_messages == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(messages);
  m_messages = value;
  if (!signalsDisabled()) {
    emit_signal(messagesModified(), this, value);
  }
  setModified();
}

std::vector<std::optional<std::string>>& Terms::params() { return m_params; }
const std::vector<std::optional<std::string>>& Terms::params() const { return m_params; }
void Terms::setParams(std::vector<std::optional<std::string>>& value) {
  if (m_params == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(params);
  m_params = value;
  if (!signalsDisabled()) {
    emit_signal(paramsModified(), this, value);
  }
  setModified();
}

void Terms::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(basic);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(commands);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(messages);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(params);
  m_messages.restoreOriginal();
}
void Terms::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(basic);
  MODIFIABLE_ACCEPT_VALUE(commands);
  MODIFIABLE_ACCEPT_VALUE(messages);
  MODIFIABLE_ACCEPT_VALUE(params);
  m_messages.acceptChanges();
}
nlohmann::ordered_json Terms::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(basic),
      MODIFIABLE_SERIALIZE_VALUE(commands),
      MODIFIABLE_SERIALIZE_CHILD_VALUE(messages),
      MODIFIABLE_SERIALIZE_VALUE(params),
  };
}

rpgmutils::signal<void(Terms*, const std::vector<std::optional<std::string>>&)>& Terms::basicModified() {
  if (!m_basicModified) {
    m_basicModified.emplace();
  }
  return *m_basicModified;
}

rpgmutils::signal<void(Terms*, const std::vector<std::optional<std::string>>&)>& Terms::commandsModified() {
  if (!m_commandsModified) {
    m_commandsModified.emplace();
  }
  return *m_commandsModified;
}

rpgmutils::signal<void(Terms*, const Messages&)>& Terms::messagesModified() {
  if (!m_messagesModified) {
    m_messagesModified.emplace();
  }
  return *m_messagesModified;
}

rpgmutils::signal<void(Terms*, const std::vector<std::optional<std::string>>&)>& Terms::paramsModified() {
  if (!m_paramsModified) {
    m_paramsModified.emplace();
  }
  return *m_paramsModified;
}

void to_json(nlohmann::ordered_json& j, const Terms& terms) {
  j = {
      {"basic", terms.m_basic},
      {"commands", terms.m_commands},
      {"messages", terms.m_messages},
      {"params", terms.m_params},
  };
}

void from_json(const nlohmann::ordered_json& j, Terms& terms) {
  terms.m_basic = j.value("basic", terms.m_basic);
  terms.m_commands = j.value("commands", terms.m_commands);
  terms.m_messages = j.value("messages", terms.m_messages);
  terms.m_params = j.value("params", terms.m_params);
}
