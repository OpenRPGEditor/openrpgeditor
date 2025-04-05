#include "Database/Battler.hpp"

Battler::Battler(const Battler& other)
: IModifiable(other)
, m_actorId(other.m_actorId)
, m_equips(other.m_equips)
, m_level(other.m_level)
, m_oldactorId(other.m_oldactorId)
, m_oldequips(other.m_oldequips)
, m_oldlevel(other.m_oldlevel) {}

Battler& Battler::operator=(const Battler& other) {
  IModifiable::operator=(other);
  m_actorId = other.m_actorId;
  m_equips = other.m_equips;
  m_level = other.m_level;
  m_oldactorId = other.m_oldactorId;
  m_oldequips = other.m_oldequips;
  m_oldlevel = other.m_oldlevel;
  return *this;
}

Battler::Battler(Battler&& other) noexcept
: IModifiable(std::move(other))
, m_actorId(other.m_actorId)
, m_equips(std::move(other.m_equips))
, m_level(other.m_level)
, m_oldactorId(other.m_oldactorId)
, m_oldequips(std::move(other.m_oldequips))
, m_oldlevel(other.m_oldlevel) {}

Battler& Battler::operator=(Battler&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_actorId = other.m_actorId;
  m_equips = std::move(other.m_equips);
  m_level = other.m_level;
  m_oldactorId = other.m_oldactorId;
  m_oldequips = std::move(other.m_oldequips);
  m_oldlevel = other.m_oldlevel;
  return *this;
}
int Battler::actorId() const { return m_actorId; }
void Battler::setActorId(int actorId) {
  MODIFIABLE_SET_OLD_VALUE(actorId);
  m_actorId = actorId;
  if (!signalsDisabled()) {
    actorIdModified().fire(this, actorId);
  }
  setHasChanges();
}

const std::vector<int>& Battler::equips() const { return m_equips; }
void Battler::setEquips(const std::vector<int>& equips) {
  MODIFIABLE_SET_OLD_VALUE(equips);
  m_equips = equips;
  if (!signalsDisabled()) {
    equipsModified().fire(this, m_equips);
  }
  setHasChanges();
}
void Battler::setEquip(int index, int equip) {
  assert(index >= 0 && index < m_equips.size());
  MODIFIABLE_SET_OLD_VALUE(equips);
  m_equips[index] = equip;
  if (!signalsDisabled()) {
    equipModified().fire(this, index, equip);
  }
  setHasChanges();
}

int Battler::level() const { return m_level; }
void Battler::setLevel(int level) {
  MODIFIABLE_SET_OLD_VALUE(level);
  m_level = level;
  if (!signalsDisabled()) {
    levelModified().fire(this, level);
  }
  setHasChanges();
}

void Battler::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(actorId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(equips);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(level);
}

void Battler::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(actorId);
  MODIFIABLE_ACCEPT_VALUE(equips);
  MODIFIABLE_ACCEPT_VALUE(level);
}

nlohmann::ordered_json Battler::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(actorId),
      MODIFIABLE_SERIALIZE_VALUE(equips),
      MODIFIABLE_SERIALIZE_VALUE(level),
  };
}

rpgmutils::signal<void(Battler*, int)>& Battler::actorIdModified() {
  if (!m_actorIdModified) {
    m_actorIdModified.emplace();
  }
  return *m_actorIdModified;
}

rpgmutils::signal<void(Battler*, const std::vector<int>&)>& Battler::equipsModified() {
  if (!m_equipsModified) {
    m_equipsModified.emplace();
  }
  return *m_equipsModified;
}
rpgmutils::signal<void(Battler*, int, int)>& Battler::equipModified() {
  if (!m_equipModified) {
    m_equipModified.emplace();
  }
  return *m_equipModified;
}

rpgmutils::signal<void(Battler*, int)>& Battler::levelModified() {
  if (!m_levelModified) {
    m_levelModified.emplace();
  }
  return *m_levelModified;
}

void to_json(nlohmann::ordered_json& out, const Battler& battler) {
  out = {
      {"actorId", battler.m_actorId},
      {"equips", battler.m_equips},
      {"level", battler.m_level},
  };
}
void from_json(const nlohmann::ordered_json& from, Battler& battler) {
  battler.m_actorId = from.value("actorId", battler.m_actorId);
  battler.m_equips = from.value("equips", battler.m_equips);
  battler.m_level = from.value("level", battler.m_level);
}