#include "Database/Template.hpp"

Template::Template(const Template& other)
: IModifiable(other)
, m_id(other.m_id)
, m_name(other.m_name)
, m_note(other.m_note)
, m_type(other.m_type)
, m_commands(other.m_commands)
, m_parameters(other.m_parameters)
, m_oldid(other.m_oldid)
, m_oldname(other.m_oldname)
, m_oldnote(other.m_oldnote)
, m_oldcommands(other.m_oldcommands)
, m_oldparameters(other.m_oldparameters) {}

Template& Template::operator=(const Template& other) {
  IModifiable::operator=(other);
  m_id = other.m_id;
  m_name = other.m_name;
  m_note = other.m_note;
  m_type = other.m_type;
  m_commands = other.m_commands;
  m_parameters = other.m_parameters;
  m_oldid = other.m_oldid;
  m_oldname = other.m_oldname;
  m_oldnote = other.m_oldnote;
  m_oldcommands = other.m_oldcommands;
  m_oldparameters = other.m_oldparameters;
  return *this;
}

Template::Template(Template&& other) noexcept
: IModifiable(other)
, m_id(other.m_id)
, m_name(std::move(other.m_name))
, m_note(std::move(other.m_note))
, m_type(other.m_type)
, m_commands(std::move(other.m_commands))
, m_parameters(std::move(other.m_parameters))
, m_oldid(other.m_oldid)
, m_oldname(std::move(other.m_oldname))
, m_oldnote(std::move(other.m_oldnote))
, m_oldcommands(std::move(other.m_oldcommands))
, m_oldparameters(std::move(other.m_oldparameters)) {}

Template& Template::operator=(Template&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_id = other.m_id;
  m_name = std::move(other.m_name);
  m_note = std::move(other.m_note);
  m_type = other.m_type;
  m_commands = std::move(other.m_commands);
  m_parameters = std::move(other.m_parameters);
  m_oldid = other.m_oldid;
  m_oldname = std::move(other.m_oldname);
  m_oldnote = std::move(other.m_oldnote);
  m_oldcommands = std::move(other.m_oldcommands);
  m_oldparameters = std::move(other.m_oldparameters);
  return *this;
}

[[nodiscard]] int Template::id() const { return m_id; }
void Template::setId(const int id) {
  if (m_id == id) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(id);
  m_id = id;
  if (!signalsDisabled()) {
    emit_signal(idModified(), this, id);
  }
  setModified();
}

[[nodiscard]] const std::string& Template::name() const { return m_name; }
void Template::setName(const std::string& name) {
  if (m_name == name) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(name);
  m_name = name;
  if (!signalsDisabled()) {
    emit_signal(nameModified(), this, name);
  }
  setModified();
}

const std::string& Template::note() const { return m_note; }
void Template::setNote(const std::string& note) {
  if (m_note == note) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(note);
  m_note = note;
  if (!signalsDisabled()) {
   emit_signal( noteModified(), this, note);
  }
  setModified();
}

Template::TemplateType Template::type() const { return m_type; }
void Template::setType(const TemplateType type) {
  if (m_type == type) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(type);
  m_type = type;
  if (!signalsDisabled()) {
    emit_signal(typeModified(), this, type);
  }
  setModified();
}

const std::string& Template::commands() const { return m_commands; }
void Template::setCommands(const std::string& commands) {
  if (m_commands == commands) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(commands);
  m_commands = commands;
  if (!signalsDisabled()) {
    emit_signal(commandsModified(), this, commands);
  }
  setModified();
}

[[nodiscard]] std::vector<int>& Template::parameters() { return m_parameters; }
[[nodiscard]] const std::vector<int>& Template::parameters() const { return m_parameters; }
void Template::setParameters(const std::vector<int>& parameters) {
  if (m_parameters == parameters) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(parameters);
  m_parameters = parameters;
  if (!signalsDisabled()) {
    emit_signal(parametersModified(), this, parameters);
  }
  setModified();
}

void Template::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(id);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(name);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(note);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(type);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(commands);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(parameters);
}
void Template::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(id);
  MODIFIABLE_ACCEPT_VALUE(name);
  MODIFIABLE_ACCEPT_VALUE(note);
  MODIFIABLE_ACCEPT_VALUE(type);
  MODIFIABLE_ACCEPT_VALUE(commands);
  MODIFIABLE_ACCEPT_VALUE(parameters);
}
nlohmann::ordered_json Template::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(id),   MODIFIABLE_SERIALIZE_VALUE(name),     MODIFIABLE_SERIALIZE_VALUE(note),
      MODIFIABLE_SERIALIZE_VALUE(type), MODIFIABLE_SERIALIZE_VALUE(commands), MODIFIABLE_SERIALIZE_VALUE(parameters),
  };
}

rpgmutils::signal<void(Template*, int)>& Template::idModified() {
  if (!m_idModified) {
    m_idModified.emplace();
  }
  return *m_idModified;
}
rpgmutils::signal<void(Template*, const std::string&)>& Template::nameModified() {
  if (!m_nameModified) {
    m_nameModified.emplace();
  }
  return *m_nameModified;
}
rpgmutils::signal<void(Template*, const std::string&)>& Template::noteModified() {
  if (!m_noteModified) {
    m_noteModified.emplace();
  }
  return *m_noteModified;
}
rpgmutils::signal<void(Template*, Template::TemplateType)>& Template::typeModified() {
  if (!m_typeModified) {
    m_typeModified.emplace();
  }
  return *m_typeModified;
}
rpgmutils::signal<void(Template*, const std::string&)>& Template::commandsModified() {
  if (!m_commandsModified) {
    m_commandsModified.emplace();
  }
  return *m_commandsModified;
}
rpgmutils::signal<void(Template*, const std::vector<int>&)>& Template::parametersModified() {
  if (!m_parametersModified) {
    m_parametersModified.emplace();
  }
  return *m_parametersModified;
}

void to_json(nlohmann::ordered_json& j, const Template& templ) {
  j = {
      {"id", templ.m_id}, {"name", templ.m_name}, {"note", templ.m_note}, {"type", templ.m_type}, {"commands", templ.m_commands}, {"parameters", templ.m_parameters},
  };
}

void from_json(const nlohmann::ordered_json& j, Template& templ) {
  templ.m_id = j.value("id", templ.m_id);
  templ.m_name = j.value("name", templ.m_name);
  templ.m_note = j.value("note", templ.m_note);
  templ.m_type = j.value("type", templ.m_type);
  templ.m_commands = j.value("commands", templ.m_commands);
  templ.m_parameters = j.value("parameters", templ.m_parameters);
}