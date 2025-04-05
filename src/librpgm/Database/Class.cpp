#include "Database/Class.hpp"

Class::Learning::Learning(const Learning& other)
: IModifiable(other)
, m_level(other.m_level)
, m_note(other.m_note)
, m_skillId(other.m_skillId) {}

Class::Learning& Class::Learning::operator=(const Learning& other) {
  IModifiable::operator=(other);
  m_level = other.m_level;
  m_note = other.m_note;
  m_skillId = other.m_skillId;
  return *this;
}

Class::Learning::Learning(Learning&& other) noexcept
: IModifiable(other)
, m_level(other.m_level)
, m_note(std::move(other.m_note))
, m_skillId(other.m_skillId) {}

int Class::Learning::level() const { return m_level; }
void Class::Learning::setLevel(const int level) {
  MODIFIABLE_SET_OLD_VALUE(level);
  m_level = level;
  if (!signalsDisabled()) {
    levelModified().fire(this, level);
  }
  setHasChanges();
}

const std::string& Class::Learning::note() const { return m_note; }

void Class::Learning::setNote(const std::string& note) {
  MODIFIABLE_SET_OLD_VALUE(note);
  m_note = note;
  if (!signalsDisabled()) {
    noteModified().fire(this, note);
  }
  setHasChanges();
}

int Class::Learning::skillId() const { return m_skillId; }

void Class::Learning::setSkillId(const int skillId) {
  MODIFIABLE_SET_OLD_VALUE(skillId);
  m_skillId = skillId;
  if (!signalsDisabled()) {
    skillIdModified().fire(this, skillId);
  }
  setHasChanges();
}
void Class::Learning::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(level);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(note);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(skillId);
}
void Class::Learning::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(level);
  MODIFIABLE_ACCEPT_VALUE(note);
  MODIFIABLE_ACCEPT_VALUE(skillId);
}
nlohmann::ordered_json Class::Learning::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(level),
      MODIFIABLE_SERIALIZE_VALUE(note),
      MODIFIABLE_SERIALIZE_VALUE(skillId),
  };
}

rpgmutils::signal<void(Class::Learning*, int)>& Class::Learning::levelModified() {
  if (!m_levelModified) {
    m_levelModified.emplace();
  }
  return *m_levelModified;
}

rpgmutils::signal<void(Class::Learning*, const std::string&)>& Class::Learning::noteModified() {
  if (!m_noteModified) {
    m_noteModified.emplace();
  }
  return *m_noteModified;
}

rpgmutils::signal<void(Class::Learning*, int)>& Class::Learning::skillIdModified() {
  if (!m_skillIdModified) {
    m_skillIdModified.emplace();
  }
  return *m_skillIdModified;
}

Class::Learning& Class::Learning::operator=(Learning&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_level = other.m_level;
  m_note = std::move(other.m_note);
  m_skillId = other.m_skillId;
  return *this;
}

Class::Class(const Class& other)
: IModifiable(other)
, m_id(other.m_id)
, m_expParams(other.m_expParams)
, m_traits(other.m_traits)
, m_learnings(other.m_learnings)
, m_name(other.m_name)
, m_note(other.m_note)
, m_params(other.m_params)
, m_oldid(other.m_oldid)
, m_oldexpParams(other.m_oldexpParams)
, m_oldtraits(other.m_oldtraits)
, m_oldlearnings(other.m_oldlearnings)
, m_oldname(other.m_oldname)
, m_oldnote(other.m_oldnote)
, m_oldparams(other.m_oldparams)
, m_isValid(other.m_isValid) {}

Class& Class::operator=(const Class& other) {
  IModifiable::operator=(other);
  m_id = other.m_id;
  m_expParams = other.m_expParams;
  m_traits = other.m_traits;
  m_learnings = other.m_learnings;
  m_name = other.m_name;
  m_note = other.m_note;
  m_params = other.m_params;
  m_oldid = other.m_oldid;
  m_oldexpParams = other.m_oldexpParams;
  m_oldtraits = other.m_oldtraits;
  m_oldlearnings = other.m_oldlearnings;
  m_oldname = other.m_oldname;
  m_oldnote = other.m_oldnote;
  m_oldparams = other.m_oldparams;
  m_isValid = other.m_isValid;
  return *this;
}

Class::Class(Class&& other) noexcept
: IModifiable(other)
, m_id(other.m_id)
, m_expParams(std::move(other.m_expParams))
, m_traits(std::move(other.m_traits))
, m_learnings(std::move(other.m_learnings))
, m_name(std::move(other.m_name))
, m_note(std::move(other.m_note))
, m_params(std::move(other.m_params))
, m_oldid(other.m_oldid)
, m_oldexpParams(std::move(other.m_oldexpParams))
, m_oldtraits(std::move(other.m_oldtraits))
, m_oldlearnings(std::move(other.m_oldlearnings))
, m_oldname(std::move(other.m_oldname))
, m_oldnote(std::move(other.m_oldnote))
, m_oldparams(std::move(other.m_oldparams))
, m_isValid(other.m_isValid) {}

Class& Class::operator=(Class&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_id = other.m_id;
  m_expParams = std::move(other.m_expParams);
  m_traits = std::move(other.m_traits);
  m_learnings = std::move(other.m_learnings);
  m_name = std::move(other.m_name);
  m_note = std::move(other.m_note);
  m_params = std::move(other.m_params);
  m_oldid = other.m_oldid;
  m_oldexpParams = std::move(other.m_oldexpParams);
  m_oldtraits = std::move(other.m_oldtraits);
  m_oldlearnings = std::move(other.m_oldlearnings);
  m_oldname = std::move(other.m_oldname);
  m_oldnote = std::move(other.m_oldnote);
  m_oldparams = std::move(other.m_oldparams);
  m_isValid = other.m_isValid;
  return *this;
}

int Class::id() const { return m_id; }
void Class::setId(const int id) {
  MODIFIABLE_SET_OLD_VALUE(id);
  m_id = id;
  if (!signalsDisabled()) {
    idModified().fire(this, id);
  }
  setHasChanges();
}

const std::array<int, 4>& Class::expParams() const { return m_expParams; }
void Class::setExpParams(const std::array<int, 4>& expParams) {
  MODIFIABLE_SET_OLD_VALUE(expParams);
  m_expParams = expParams;
  if (!signalsDisabled()) {
    expParamsModified().fire(this, expParams);
  }
  setHasChanges();
}

void Class::setExpParam(const int index, const int value) {
  assert(index >= 0 && index < m_expParams.size());
  MODIFIABLE_SET_OLD_VALUE(expParams);
  m_expParams[index] = value;
  if (!signalsDisabled()) {
    expParamModified().fire(this, index, value);
  }
  setHasChanges();
}

std::vector<Trait>& Class::traits() { return m_traits; }
const std::vector<Trait>& Class::traits() const { return m_traits; }
void Class::setTraits(const std::vector<Trait>& traits) {
  MODIFIABLE_SET_OLD_VALUE(traits);
  m_traits = traits;
  if (!signalsDisabled()) {
    traitsModified().fire(this, traits);
  }
  setHasChanges();
}

const std::vector<Class::Learning>& Class::learnings() const { return m_learnings; }
void Class::setLearnings(const std::vector<Learning>& learnings) {
  MODIFIABLE_SET_OLD_VALUE(learnings);
  m_learnings = learnings;
  if (!signalsDisabled()) {
    learningsModified().fire(this, learnings);
  }
  setHasChanges();
}

const std::string& Class::name() const { return m_name; }
void Class::setName(const std::string& name) {
  MODIFIABLE_SET_OLD_VALUE(name);
  m_name = name;
  if (!signalsDisabled()) {
    nameModified().fire(this, name);
  }
  setHasChanges();
}

const std::string& Class::note() const { return m_note; }
void Class::setNote(const std::string& note) {
  MODIFIABLE_SET_OLD_VALUE(note);
  m_note = note;
  if (!signalsDisabled()) {
    noteModified().fire(this, note);
  }
  setHasChanges();
}

[[nodiscard]] const std::array<std::array<int, 99>, 8>& Class::params() const { return m_params; }
void Class::setParams(const std::array<std::array<int, 99>, 8>& params) {
  MODIFIABLE_SET_OLD_VALUE(params);
  m_params = params;
  if (!signalsDisabled()) {
    paramsModified().fire(this, params);
  }
  setHasChanges();
}

bool Class::isValid() const { return m_isValid; }
void Class::setValid(const bool isValid) { m_isValid = isValid; }

void Class::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(id);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(expParams);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(traits);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(learnings);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(name);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(note);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(params);
}

void Class::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(id);
  MODIFIABLE_ACCEPT_VALUE(expParams);
  MODIFIABLE_ACCEPT_VALUE(traits);
  MODIFIABLE_ACCEPT_VALUE(learnings);
  MODIFIABLE_ACCEPT_VALUE(name);
  MODIFIABLE_ACCEPT_VALUE(note);
  MODIFIABLE_ACCEPT_VALUE(params);
}

nlohmann::ordered_json Class::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(id),   MODIFIABLE_SERIALIZE_VALUE(expParams), MODIFIABLE_SERIALIZE_VALUE(traits), MODIFIABLE_SERIALIZE_VALUE_CHILD_ARRAY(learnings),
      MODIFIABLE_SERIALIZE_VALUE(name), MODIFIABLE_SERIALIZE_VALUE(note),      MODIFIABLE_SERIALIZE_VALUE(params),
  };
}

rpgmutils::signal<void(Class*, int)>& Class::idModified() {
  if (!m_idModified) {
    m_idModified.emplace();
  }
  return *m_idModified;
}

rpgmutils::signal<void(Class*, const std::array<int, 4>&)>& Class::expParamsModified() {
  if (!m_expParamsModified) {
    m_expParamsModified.emplace();
  }
  return *m_expParamsModified;
}

rpgmutils::signal<void(Class*, int, int)>& Class::expParamModified() {
  if (!m_expParamModified) {
    m_expParamModified.emplace();
  }
  return *m_expParamModified;
}

rpgmutils::signal<void(Class*, const std::vector<Trait>&)>& Class::traitsModified() {
  if (!m_traitsModified) {
    m_traitsModified.emplace();
  }
  return *m_traitsModified;
}

rpgmutils::signal<void(Class*, const std::vector<Class::Learning>&)>& Class::learningsModified() {
  if (!m_learningsModified) {
    m_learningsModified.emplace();
  }
  return *m_learningsModified;
}

rpgmutils::signal<void(Class*, const std::string&)>& Class::nameModified() {
  if (!m_nameModified) {
    m_nameModified.emplace();
  }
  return *m_nameModified;
}

rpgmutils::signal<void(Class*, const std::string&)>& Class::noteModified() {
  if (!m_noteModified) {
    m_noteModified.emplace();
  }
  return *m_noteModified;
}

rpgmutils::signal<void(Class*, const std::array<std::array<int, 99>, 8>&)>& Class::paramsModified() {
  if (!m_paramsModified) {
    m_paramsModified.emplace();
  }
  return *m_paramsModified;
}

void to_json(nlohmann::ordered_json& to, const Class::Learning& learning) {
  to = {
      {"level", learning.m_level},
      {"note", learning.m_note},
      {"skillId", learning.m_skillId},
  };
}
void from_json(const nlohmann::ordered_json& from, Class::Learning& learning) {
  learning.m_level = from.value("level", learning.m_level);
  learning.m_note = from.value("note", learning.m_note);
  learning.m_skillId = from.value("skillId", learning.m_skillId);
}

void to_json(nlohmann::ordered_json& to, const Class& cls) {
  to = {
      {"id", cls.m_id}, {"expParams", cls.m_expParams}, {"traits", cls.m_traits}, {"learnings", cls.m_learnings}, {"name", cls.m_name}, {"note", cls.m_note}, {"params", cls.m_params},
  };
}

void from_json(const nlohmann::ordered_json& from, Class& cls) {
  cls.m_id = from.value("id", cls.m_id);
  cls.m_expParams = from.value("expParams", cls.m_expParams);
  cls.m_traits = from.value("traits", cls.m_traits);
  cls.m_learnings = from.value("learnings", cls.m_learnings);
  cls.m_name = from.value("name", cls.m_name);
  cls.m_note = from.value("note", cls.m_note);
  cls.m_params = from.value("params", cls.m_params);
}