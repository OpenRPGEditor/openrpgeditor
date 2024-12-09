#include "Database/Actor.hpp"

Actor::Actor(const Actor& other)
: m_id(other.m_id)
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
, m_profile(other.m_profile) {}

Actor& Actor::operator=(const Actor& other) {
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
  return *this;
}

Actor::Actor(Actor&& other) noexcept
: m_id(other.m_id)
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
, m_profile(std::move(other.m_profile)) {}

Actor& Actor::operator=(Actor&& other) noexcept {
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
  return *this;
}

int Actor::id() const { return m_id; }
void Actor::setId(const int id) {
  m_id = id;
  onIdModified().fire(this, id);
  setModified();
}

const std::string& Actor::battlerName() const { return m_battlerName; }
void Actor::setBattlerName(const std::string& battlerName) {
  m_battlerName = battlerName;
  onBattlerNameModified().fire(this, m_battlerName);
  setModified();
}

int Actor::characterIndex() const { return m_characterIndex; }
void Actor::setCharacterIndex(const int characterIndex) {
  m_characterIndex = characterIndex;
  onCharacterIndexModified().fire(this, m_characterIndex);
  setModified();
}

const std::string& Actor::characterName() const { return m_characterName; }
void Actor::setCharacterName(const std::string& characterName) {
  m_characterName = characterName;
  onCharacterNameModified().fire(this, m_characterName);
  setModified();
}

int Actor::classId() const { return m_classId; }
void Actor::setClassId(const int classId) {
  m_classId = classId;
  onClassIdModified().fire(this, m_classId);
  setModified();
}

const std::vector<int>& Actor::equips() const { return m_equips; }
void Actor::setEquips(const std::vector<int>& equips) {
  m_equips = equips;
  onEquipsModified().fire(this, m_equips);
  setModified();
}

void Actor::addEquip(const int equip) {
  m_equips.push_back(equip);
  onEquipAdded().fire(this, m_equips.size() - 1, equip);
  setModified();
}

void Actor::setEquip(const int index, const int equip) {
  assert(index < m_equips.size());
  m_equips[index] = equip;
  onEquipAdded().fire(this, index, index);
}

void Actor::removeEquip(const int index) {
  assert(index < m_equips.size());
  int oldEquip = m_equips[index];
  m_equips.erase(m_equips.begin() + index);
  onEquipRemoved().fire(this, index, oldEquip);
}

int Actor::faceIndex() const { return m_faceIndex; }
void Actor::setFaceIndex(const int faceIndex) {
  m_faceIndex = faceIndex;
  onFaceIndexModified().fire(this, m_faceIndex);
  setModified();
}

const std::string& Actor::faceName() const { return m_faceName; }
void Actor::setFaceName(const std::string& faceName) {
  m_faceName = faceName;
  onFaceNameModified().fire(this, m_faceName);
  setModified();
}

std::vector<Trait>& Actor::traits() { return m_traits; }
void Actor::setTraits(const std::vector<Trait>& traits) {
  m_traits = traits;
  onTraitsModified().fire(this, m_traits);
  setModified();
}

Trait& Actor::trait(const int index) {
  assert(index < m_traits.size());
  return m_traits[index];
}

int Actor::initialLevel() const { return m_initialLevel; }
void Actor::setInitialLevel(const int initialLevel) {
  m_initialLevel = initialLevel;
  onInitialLevelModified().fire(this, m_initialLevel);
  setModified();
}

int Actor::maxLevel() const { return m_maxLevel; }
void Actor::setMaxLevel(const int maxLevel) {
  m_maxLevel = maxLevel;
  onMaxLevelModified().fire(this, m_maxLevel);
  setModified();
}

const std::string& Actor::name() const { return m_name; }
void Actor::setName(const std::string& name) {
  m_name = name;
  onNameModified().fire(this, m_name);
  setModified();
}
const std::string& Actor::nickname() const { return m_nickname; }
void Actor::setNickname(const std::string& nickname) {
  m_nickname = nickname;
  onNicknameModified().fire(this, m_nickname);
  setModified();
}

const std::string& Actor::note() const { return m_note; }
void Actor::setNote(const std::string& note) {
  m_note = note;
  onNoteModified().fire(this, m_note);
  setModified();
}

const std::string& Actor::profile() const { return m_profile; }
void Actor::setProfile(const std::string& profile) {
  m_profile = profile;
  onProfileModified().fire(this, m_profile);
  setModified();
}

bool Actor::isValid() const { return m_isValid; }
void Actor::setValid(const bool valid) { m_isValid = valid; }

/* Signals */
signal<void(Actor* actor, int newId)>& Actor::onIdModified() {
  if (!m_onIdModified) {
    m_onIdModified.emplace();
  }
  return *m_onIdModified;
}

signal<void(Actor* actor, const std::string& name)>& Actor::onBattlerNameModified() {
  if (!m_onBattlerNameModified) {
    m_onBattlerNameModified.emplace();
  }
  return *m_onBattlerNameModified;
}

signal<void(Actor* actor, int newIndex)>& Actor::onCharacterIndexModified() {
  if (!m_onCharacterIndexModified) {
    m_onCharacterIndexModified.emplace();
  }

  return *m_onCharacterIndexModified;
}

signal<void(Actor* actor, const std::string& name)>& Actor::onCharacterNameModified() {
  if (!m_onCharacterNameModified) {
    m_onCharacterNameModified.emplace();
  }
  return *m_onCharacterNameModified;
}

signal<void(Actor* actor, int newId)>& Actor::onClassIdModified() {
  if (!m_onClassIdModified) {
    m_onClassIdModified.emplace();
  }

  return *m_onClassIdModified;
}

signal<void(Actor* actor, const std::vector<int>& newEquips)>& Actor::onEquipsModified() {
  if (!m_onEquipsModified) {
    m_onEquipsModified.emplace();
  }
  return *m_onEquipsModified;
}

signal<void(Actor* actor, int index, int newEquip)>& Actor::onEquipAdded() {
  if (!m_onEquipAdded) {
    m_onEquipAdded.emplace();
  }
  return *m_onEquipAdded;
}

signal<void(Actor* actor, int index, int newEquip)>& Actor::onEquipModified() {
  if (!m_onEquipModified) {
    m_onEquipModified.emplace();
  }
  return *m_onEquipModified;
}

signal<void(Actor* actor, int index, int newEquip)>& Actor::onEquipRemoved() {
  if (!m_onEquipRemoved) {
    m_onEquipRemoved.emplace();
  }
  return *m_onEquipRemoved;
}

signal<void(Actor* actor, int newId)>& Actor::onFaceIndexModified() {
  if (!m_onFaceIndexModified) {
    m_onFaceIndexModified.emplace();
  }

  return *m_onFaceIndexModified;
}

signal<void(Actor* actor, const std::string& newFaceName)>& Actor::onFaceNameModified() {
  if (!m_onFaceNameModified) {
    m_onFaceNameModified.emplace();
  }
  return *m_onFaceNameModified;
}

signal<void(Actor* actor, const std::vector<Trait>& newTraits)>& Actor::onTraitsModified() {
  if (!m_onTraitsModified) {
    m_onTraitsModified.emplace();
  }
  return *m_onTraitsModified;
}

signal<void(Actor* actor, const Trait& trait)>& Actor::onTraitAdded() {
  if (!m_onTraitAdded) {
    m_onTraitAdded.emplace();
  }
  return *m_onTraitAdded;
}

signal<void(Actor* actor, int newInitialLevel)>& Actor::onInitialLevelModified() {
  if (!m_onInitialLevelModified) {
    m_onInitialLevelModified.emplace();
  }
  return *m_onInitialLevelModified;
}

signal<void(Actor* actor, int newMaxLevel)>& Actor::onMaxLevelModified() {
  if (!m_onMaxLevelModified) {
    m_onMaxLevelModified.emplace();
  }
  return *m_onMaxLevelModified;
}

signal<void(Actor* actor, const std::string& newName)>& Actor::onNameModified() {
  if (!m_onNameModified) {
    m_onNameModified.emplace();
  }
  return *m_onNameModified;
}

signal<void(Actor* actor, const std::string& newNickname)>& Actor::onNicknameModified() {
  if (!m_onNicknameModified) {
    m_onNicknameModified.emplace();
  }
  return *m_onNicknameModified;
}

signal<void(Actor* actor, const std::string& newNote)>& Actor::onNoteModified() {
  if (!m_onNoteModified) {
    m_onNoteModified.emplace();
  }
  return *m_onNoteModified;
}

signal<void(Actor* actor, const std::string& newProfile)>& Actor::onProfileModified() {
  if (!m_onProfileModified) {
    m_onProfileModified.emplace();
  }
  return *m_onProfileModified;
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