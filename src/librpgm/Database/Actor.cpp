#include "Database/Actor.hpp"

Actor::Actor(const Actor& other)
: IModifiable(other)
, m_id(other.m_id)
, m_battlerName(other.m_battlerName)
, m_characterIndex(other.m_characterIndex)
, m_characterName(other.m_characterName)
, m_classId(other.m_classId)
, m_equips(other.m_equips)
, m_faceIndex(other.m_faceIndex)
, m_faceName(other.m_faceName)
, m_traits(other.m_traits)
, m_initialLevel(other.m_initialLevel)
, m_maxLevel(other.m_maxLevel)
, m_name(other.m_name)
, m_nickname(other.m_nickname)
, m_note(other.m_note)
, m_profile(other.m_profile)
, m_oldid(other.m_oldid)
, m_oldbattlerName(other.m_oldbattlerName)
, m_oldcharacterIndex(other.m_oldcharacterIndex)
, m_oldcharacterName(other.m_oldcharacterName)
, m_oldclassId(other.m_oldclassId)
, m_oldequips(other.m_oldequips)
, m_oldfaceIndex(other.m_oldfaceIndex)
, m_oldfaceName(other.m_oldfaceName)
, m_oldtraits(other.m_oldtraits)
, m_oldinitialLevel(other.m_oldinitialLevel)
, m_oldmaxLevel(other.m_oldmaxLevel)
, m_oldname(other.m_oldname)
, m_oldnickname(other.m_oldnickname)
, m_oldnote(other.m_oldnote)
, m_oldprofile(other.m_oldprofile)
, m_isValid(other.m_isValid) {}

Actor& Actor::operator=(const Actor& other) {
  IModifiable::operator=(other);
  m_id = other.m_id;
  m_battlerName = other.m_battlerName;
  m_characterIndex = other.m_characterIndex;
  m_characterName = other.m_characterName;
  m_classId = other.m_classId;
  m_equips = other.m_equips;
  m_faceIndex = other.m_faceIndex;
  m_faceName = other.m_faceName;
  m_traits = other.m_traits;
  m_initialLevel = other.m_initialLevel;
  m_maxLevel = other.m_maxLevel;
  m_name = other.m_name;
  m_nickname = other.m_nickname;
  m_note = other.m_note;
  m_profile = other.m_profile;

  m_oldid = other.m_oldid;
  m_oldbattlerName = other.m_oldbattlerName;
  m_oldcharacterIndex = other.m_oldcharacterIndex;
  m_oldcharacterName = other.m_oldcharacterName;
  m_oldclassId = other.m_oldclassId;
  m_oldequips = other.m_oldequips;
  m_oldfaceIndex = other.m_oldfaceIndex;
  m_oldfaceName = other.m_oldfaceName;
  m_oldtraits = other.m_oldtraits;
  m_oldinitialLevel = other.m_oldinitialLevel;
  m_oldmaxLevel = other.m_oldmaxLevel;
  m_oldname = other.m_oldname;
  m_oldnickname = other.m_oldnickname;
  m_oldnote = other.m_oldnote;
  m_oldprofile = other.m_oldprofile;
  m_isValid = other.m_isValid;
  return *this;
}

Actor::Actor(Actor&& other) noexcept
: IModifiable(std::move(other))
, m_id(other.m_id)
, m_battlerName(std::move(other.m_battlerName))
, m_characterIndex(other.m_characterIndex)
, m_characterName(std::move(other.m_characterName))
, m_classId(other.m_classId)
, m_equips(std::move(other.m_equips))
, m_faceIndex(other.m_faceIndex)
, m_faceName(std::move(other.m_faceName))
, m_traits(std::move(other.m_traits))
, m_initialLevel(other.m_initialLevel)
, m_maxLevel(other.m_maxLevel)
, m_name(std::move(other.m_name))
, m_nickname(std::move(other.m_nickname))
, m_note(std::move(other.m_note))
, m_profile(std::move(other.m_profile))
, m_oldid(other.m_oldid)
, m_oldbattlerName(std::move(other.m_oldbattlerName))
, m_oldcharacterIndex(other.m_oldcharacterIndex)
, m_oldcharacterName(std::move(other.m_oldcharacterName))
, m_oldclassId(other.m_oldclassId)
, m_oldequips(std::move(other.m_oldequips))
, m_oldfaceIndex(other.m_oldfaceIndex)
, m_oldfaceName(std::move(other.m_oldfaceName))
, m_oldtraits(std::move(other.m_oldtraits))
, m_oldinitialLevel(other.m_oldinitialLevel)
, m_oldmaxLevel(other.m_oldmaxLevel)
, m_oldname(std::move(other.m_oldname))
, m_oldnickname(std::move(other.m_oldnickname))
, m_oldnote(std::move(other.m_oldnote))
, m_oldprofile(std::move(other.m_oldprofile))
, m_isValid(other.m_isValid) {}

Actor& Actor::operator=(Actor&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_id = other.m_id;
  m_battlerName = std::move(other.m_battlerName);
  m_characterIndex = other.m_characterIndex;
  m_characterName = std::move(other.m_characterName);
  m_classId = other.m_classId;
  m_equips = std::move(other.m_equips);
  m_faceIndex = other.m_faceIndex;
  m_faceName = std::move(other.m_faceName);
  m_traits = std::move(other.m_traits);
  m_initialLevel = other.m_initialLevel;
  m_maxLevel = other.m_maxLevel;
  m_name = std::move(other.m_name);
  m_nickname = std::move(other.m_nickname);
  m_note = std::move(other.m_note);
  m_profile = std::move(other.m_profile);

  m_oldid = other.m_oldid;
  m_oldbattlerName = std::move(other.m_oldbattlerName);
  m_oldcharacterIndex = other.m_oldcharacterIndex;
  m_oldcharacterName = std::move(other.m_oldcharacterName);
  m_oldclassId = other.m_oldclassId;
  m_oldequips = std::move(other.m_oldequips);
  m_oldfaceIndex = other.m_oldfaceIndex;
  m_oldfaceName = std::move(other.m_oldfaceName);
  m_oldtraits = std::move(other.m_oldtraits);
  m_oldinitialLevel = other.m_oldinitialLevel;
  m_oldmaxLevel = other.m_oldmaxLevel;
  m_oldname = std::move(other.m_oldname);
  m_oldnickname = std::move(other.m_oldnickname);
  m_oldnote = std::move(other.m_oldnote);
  m_oldprofile = std::move(other.m_oldprofile);

  m_isValid = other.m_isValid;
  return *this;
}

int Actor::id() const { return m_id; }
void Actor::setId(const int id) {
  MODIFIABLE_SET_OLD_VALUE(id);
  m_id = id;
  if (!signalsDisabled()) {
    idModified().fire(this, id);
  }
  setModified();
}

const std::string& Actor::battlerName() const { return m_battlerName; }
void Actor::setBattlerName(const std::string& battlerName) {
  MODIFIABLE_SET_OLD_VALUE(battlerName);
  m_battlerName = battlerName;
  if (!signalsDisabled()) {
    battlerNameModified().fire(this, m_battlerName);
  }
  setModified();
}
int Actor::characterIndex() const { return m_characterIndex; }
void Actor::setCharacterIndex(const int characterIndex) {
  MODIFIABLE_SET_OLD_VALUE(characterIndex);
  m_characterIndex = characterIndex;
  if (!signalsDisabled()) {
    characterIndexModified().fire(this, m_characterIndex);
  }
  setModified();
}
const std::string& Actor::characterName() const { return m_characterName; }
void Actor::setCharacterName(const std::string& characterName) {
  MODIFIABLE_SET_OLD_VALUE(characterName);
  m_characterName = characterName;
  if (!signalsDisabled()) {
    characterNameModified().fire(this, m_characterName);
  }
  setHasChanges();
}
int Actor::classId() const { return m_classId; }
void Actor::setClassId(const int classId) {
  MODIFIABLE_SET_OLD_VALUE(classId);
  m_classId = classId;
  if (!signalsDisabled()) {
    classIdModified().fire(this, m_classId);
  }
  setModified();
}
const std::vector<int>& Actor::equips() const { return m_equips; }
void Actor::setEquips(const std::vector<int>& equips) {
  MODIFIABLE_SET_OLD_VALUE(equips);
  m_equips = equips;
  if (!signalsDisabled()) {
    equipsModified().fire(this, m_equips);
  }
  setModified();
}
void Actor::addEquip(const int equip) {
  MODIFIABLE_SET_OLD_VALUE(equips);
  m_equips.push_back(equip);
  if (!signalsDisabled()) {
    equipAdded().fire(this, m_equips.size() - 1, equip);
  }
  setModified();
}

void Actor::setEquip(const int index, const int equip) {
  assert(index < m_equips.size());
  MODIFIABLE_SET_OLD_VALUE(equips);
  if (m_equips.size() <= index) {
    m_equips.resize(index - m_equips.size());
  }
  m_equips[index] = equip;
  if (!signalsDisabled()) {
    equipAdded().fire(this, index, index);
  }
  setModified();
}

void Actor::removeEquip(const int index) {
  assert(index < m_equips.size());
  MODIFIABLE_SET_OLD_VALUE(equips);
  int oldEquip = m_equips[index];
  m_equips.erase(m_equips.begin() + index);
  if (!signalsDisabled()) {
    equipRemoved().fire(this, index, oldEquip);
  }
  setModified();
}

int Actor::faceIndex() const { return m_faceIndex; }
void Actor::setFaceIndex(const int faceIndex) {
  MODIFIABLE_SET_OLD_VALUE(faceIndex);
  m_faceIndex = faceIndex;
  if (!signalsDisabled()) {
    faceIndexModified().fire(this, m_faceIndex);
  }
  setModified();
}

const std::string& Actor::faceName() const { return m_faceName; }
void Actor::setFaceName(const std::string& faceName) {
  MODIFIABLE_SET_OLD_VALUE(faceName);
  m_faceName = faceName;
  if (!signalsDisabled()) {
    faceNameModified().fire(this, m_faceName);
  }
  setModified();
}

std::vector<Trait>& Actor::traits() { return m_traits; }
void Actor::setTraits(const std::vector<Trait>& traits) {
  MODIFIABLE_SET_OLD_VALUE(traits);
  m_traits = traits;
  if (!signalsDisabled()) {
    traitsModified().fire(this, m_traits);
  }
  setModified();
}

Trait& Actor::trait(const int index) {
  assert(index < m_traits.size());
  MODIFIABLE_SET_OLD_VALUE(traits);
  return m_traits[index];
}

int Actor::initialLevel() const { return m_initialLevel; }
void Actor::setInitialLevel(const int initialLevel) {
  MODIFIABLE_SET_OLD_VALUE(initialLevel);
  m_initialLevel = initialLevel;
  if (!signalsDisabled()) {
    initialLevelModified().fire(this, m_initialLevel);
  }
  setModified();
}

int Actor::maxLevel() const { return m_maxLevel; }
void Actor::setMaxLevel(const int maxLevel) {
  MODIFIABLE_SET_OLD_VALUE(maxLevel);
  m_maxLevel = maxLevel;
  if (!signalsDisabled()) {
    maxLevelModified().fire(this, m_maxLevel);
  }
  setModified();
}

const std::string& Actor::name() const { return m_name; }
void Actor::setName(const std::string& name) {
  MODIFIABLE_SET_OLD_VALUE(name);
  m_name = name;
  if (!signalsDisabled()) {
    nameModified().fire(this, m_name);
  }
  setModified();
}
const std::string& Actor::nickname() const { return m_nickname; }
void Actor::setNickname(const std::string& nickname) {
  MODIFIABLE_SET_OLD_VALUE(nickname);
  m_nickname = nickname;

  if (!signalsDisabled()) {
    nicknameModified().fire(this, m_nickname);
  }
  setModified();
}

const std::string& Actor::note() const { return m_note; }
void Actor::setNote(const std::string& note) {
  MODIFIABLE_SET_OLD_VALUE(note);
  m_note = note;
  if (!signalsDisabled()) {
    noteModified().fire(this, m_note);
  }
  setModified();
}

const std::string& Actor::profile() const { return m_profile; }
void Actor::setProfile(const std::string& profile) {
  MODIFIABLE_SET_OLD_VALUE(profile);
  m_profile = profile;
  if (!signalsDisabled()) {
    profileModified().fire(this, m_profile);
  }
  setModified();
}

bool Actor::isValid() const { return m_isValid; }
void Actor::setValid(const bool valid) { m_isValid = valid; }

/* Signals */
rpgmutils::signal<void(Actor* actor, int newId)>& Actor::idModified() {
  if (!m_idModified) {
    m_idModified.emplace();
  }
  return *m_idModified;
}

rpgmutils::signal<void(Actor* actor, const std::string& name)>& Actor::battlerNameModified() {
  if (!m_battlerNameModified) {
    m_battlerNameModified.emplace();
  }
  return *m_battlerNameModified;
}

rpgmutils::signal<void(Actor* actor, int newIndex)>& Actor::characterIndexModified() {
  if (!m_characterIndexModified) {
    m_characterIndexModified.emplace();
  }

  return *m_characterIndexModified;
}

rpgmutils::signal<void(Actor* actor, const std::string& name)>& Actor::characterNameModified() {
  if (!m_characterNameModified) {
    m_characterNameModified.emplace();
  }
  return *m_characterNameModified;
}

rpgmutils::signal<void(Actor* actor, int newId)>& Actor::classIdModified() {
  if (!m_onClassIdModified) {
    m_onClassIdModified.emplace();
  }

  return *m_onClassIdModified;
}

rpgmutils::signal<void(Actor* actor, const std::vector<int>& newEquips)>& Actor::equipsModified() {
  if (!m_equipsModified) {
    m_equipsModified.emplace();
  }
  return *m_equipsModified;
}

rpgmutils::signal<void(Actor* actor, int index, int newEquip)>& Actor::equipAdded() {
  if (!m_equipAdded) {
    m_equipAdded.emplace();
  }
  return *m_equipAdded;
}

rpgmutils::signal<void(Actor* actor, int index, int newEquip)>& Actor::equipModified() {
  if (!m_equipModified) {
    m_equipModified.emplace();
  }
  return *m_equipModified;
}

rpgmutils::signal<void(Actor* actor, int index, int newEquip)>& Actor::equipRemoved() {
  if (!m_equipRemoved) {
    m_equipRemoved.emplace();
  }
  return *m_equipRemoved;
}

rpgmutils::signal<void(Actor* actor, int newId)>& Actor::faceIndexModified() {
  if (!m_faceIndexModified) {
    m_faceIndexModified.emplace();
  }

  return *m_faceIndexModified;
}

rpgmutils::signal<void(Actor* actor, const std::string& newFaceName)>& Actor::faceNameModified() {
  if (!m_faceNameModified) {
    m_faceNameModified.emplace();
  }
  return *m_faceNameModified;
}

rpgmutils::signal<void(Actor* actor, const std::vector<Trait>& newTraits)>& Actor::traitsModified() {
  if (!m_traitsModified) {
    m_traitsModified.emplace();
  }
  return *m_traitsModified;
}

rpgmutils::signal<void(Actor* actor, const Trait& trait)>& Actor::traitAdded() {
  if (!m_traitAdded) {
    m_traitAdded.emplace();
  }
  return *m_traitAdded;
}

rpgmutils::signal<void(Actor* actor, int newInitialLevel)>& Actor::initialLevelModified() {
  if (!m_initialLevelModified) {
    m_initialLevelModified.emplace();
  }
  return *m_initialLevelModified;
}

rpgmutils::signal<void(Actor* actor, int newMaxLevel)>& Actor::maxLevelModified() {
  if (!m_maxLevelModified) {
    m_maxLevelModified.emplace();
  }
  return *m_maxLevelModified;
}

rpgmutils::signal<void(Actor* actor, const std::string& newName)>& Actor::nameModified() {
  if (!m_nameModified) {
    m_nameModified.emplace();
  }
  return *m_nameModified;
}

rpgmutils::signal<void(Actor* actor, const std::string& newNickname)>& Actor::nicknameModified() {
  if (!m_nicknameModified) {
    m_nicknameModified.emplace();
  }
  return *m_nicknameModified;
}

rpgmutils::signal<void(Actor* actor, const std::string& newNote)>& Actor::noteModified() {
  if (!m_noteModified) {
    m_noteModified.emplace();
  }
  return *m_noteModified;
}

rpgmutils::signal<void(Actor* actor, const std::string& newProfile)>& Actor::profileModified() {
  if (!m_profileModified) {
    m_profileModified.emplace();
  }
  return *m_profileModified;
}

void Actor::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(id);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(battlerName);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(characterIndex);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(characterName);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(classId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(equips);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(faceIndex);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(faceName);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(traits);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(initialLevel);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(maxLevel);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(name);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(nickname);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(note);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(profile);
}
void Actor::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(id);
  MODIFIABLE_ACCEPT_VALUE(battlerName);
  MODIFIABLE_ACCEPT_VALUE(characterIndex);
  MODIFIABLE_ACCEPT_VALUE(characterName);
  MODIFIABLE_ACCEPT_VALUE(classId);
  MODIFIABLE_ACCEPT_VALUE(equips);
  MODIFIABLE_ACCEPT_VALUE(faceIndex);
  MODIFIABLE_ACCEPT_VALUE(faceName);
  MODIFIABLE_ACCEPT_VALUE(traits);
  MODIFIABLE_ACCEPT_VALUE(initialLevel);
  MODIFIABLE_ACCEPT_VALUE(maxLevel);
  MODIFIABLE_ACCEPT_VALUE(name);
  MODIFIABLE_ACCEPT_VALUE(nickname);
  MODIFIABLE_ACCEPT_VALUE(note);
  MODIFIABLE_ACCEPT_VALUE(profile);
}

nlohmann::ordered_json Actor::serializeOldValues() const {
  const nlohmann::ordered_json j = {
      MODIFIABLE_SERIALIZE_VALUE(id),           MODIFIABLE_SERIALIZE_VALUE(battlerName), MODIFIABLE_SERIALIZE_VALUE(characterIndex), MODIFIABLE_SERIALIZE_VALUE(characterName),
      MODIFIABLE_SERIALIZE_VALUE(equips),       MODIFIABLE_SERIALIZE_VALUE(faceIndex),   MODIFIABLE_SERIALIZE_VALUE(faceName),       MODIFIABLE_SERIALIZE_VALUE_CHILD_ARRAY(traits),
      MODIFIABLE_SERIALIZE_VALUE(initialLevel), MODIFIABLE_SERIALIZE_VALUE(maxLevel),    MODIFIABLE_SERIALIZE_VALUE(name),           MODIFIABLE_SERIALIZE_VALUE(note),
      MODIFIABLE_SERIALIZE_VALUE(profile),
  };

  return j;
}

void to_json(nlohmann::ordered_json& to, const Actor& actor) {
  to = {
      {"id", actor.m_id},
      {"battlerName", actor.m_battlerName},
      {"characterIndex", actor.m_characterIndex},
      {"characterName", actor.m_characterName},
      {"classId", actor.m_classId},
      {"equips", actor.m_equips},
      {"faceIndex", actor.m_faceIndex},
      {"faceName", actor.m_faceName},
      {"traits", actor.m_traits},
      {"initialLevel", actor.m_initialLevel},
      {"maxLevel", actor.m_maxLevel},
      {"name", actor.m_name},
      {"nickname", actor.m_nickname},
      {"note", actor.m_note},
      {"profile", actor.m_profile},
  };
}
void from_json(const nlohmann::ordered_json& from, Actor& actor) {
  actor.m_id = from.value("id", actor.m_id);
  actor.m_battlerName = from.value("battlerName", actor.m_battlerName);
  actor.m_characterIndex = from.value("characterIndex", actor.m_characterIndex);
  actor.m_characterName = from.value("characterName", actor.m_characterName);
  actor.m_classId = from.value("classId", actor.m_classId);
  actor.m_equips = from.value("equips", actor.m_equips);
  actor.m_faceIndex = from.value("faceIndex", actor.m_faceIndex);
  actor.m_faceName = from.value("faceName", actor.m_faceName);
  actor.m_traits = from.value("traits", actor.m_traits);
  actor.m_initialLevel = from.value("initialLevel", actor.m_initialLevel);
  actor.m_maxLevel = from.value("maxLevel", actor.m_maxLevel);
  actor.m_name = from.value("name", actor.m_name);
  actor.m_nickname = from.value("nickname", actor.m_nickname);
  actor.m_profile = from.value("profile", actor.m_profile);
}