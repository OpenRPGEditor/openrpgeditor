#include "Database/Vehicle.hpp"

Vehicle::Vehicle(const Vehicle& other)
: IModifiable(other)
, m_bgm(other.m_bgm)
, m_characterIndex(other.m_characterIndex)
, m_characterName(other.m_characterName)
, m_startMapId(other.m_startMapId)
, m_startX(other.m_startX)
, m_startY(other.m_startY)
, m_oldbgm(other.m_oldbgm)
, m_oldcharacterIndex(other.m_oldcharacterIndex)
, m_oldcharacterName(other.m_oldcharacterName)
, m_oldstartMapId(other.m_oldstartMapId)
, m_oldstartX(other.m_oldstartX)
, m_oldstartY(other.m_oldstartY) {}

Vehicle& Vehicle::operator=(const Vehicle& other) {
  IModifiable::operator=(other);
  m_bgm = other.m_bgm;
  m_characterIndex = other.m_characterIndex;
  m_characterName = other.m_characterName;
  m_startMapId = other.m_startMapId;
  m_startX = other.m_startX;
  m_startY = other.m_startY;
  m_oldbgm = other.m_oldbgm;
  m_oldcharacterIndex = other.m_oldcharacterIndex;
  m_oldcharacterName = other.m_oldcharacterName;
  m_oldstartMapId = other.m_oldstartMapId;
  m_oldstartX = other.m_oldstartX;
  m_oldstartY = other.m_oldstartY;
  return *this;
}

Vehicle::Vehicle(Vehicle&& other) noexcept
: IModifiable(std::move(other))
, m_bgm(std::move(other.m_bgm))
, m_characterIndex(other.m_characterIndex)
, m_characterName(std::move(other.m_characterName))
, m_startMapId(other.m_startMapId)
, m_startX(other.m_startX)
, m_startY(other.m_startY)
, m_oldbgm(std::move(other.m_oldbgm))
, m_oldcharacterIndex(other.m_oldcharacterIndex)
, m_oldcharacterName(std::move(other.m_oldcharacterName))
, m_oldstartMapId(other.m_oldstartMapId)
, m_oldstartX(other.m_oldstartX)
, m_oldstartY(other.m_oldstartY) {}

Vehicle& Vehicle::operator=(Vehicle&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_bgm = std::move(other.m_bgm);
  m_characterIndex = other.m_characterIndex;
  m_characterName = std::move(other.m_characterName);
  m_startMapId = other.m_startMapId;
  m_startX = other.m_startX;
  m_startY = other.m_startY;
  m_oldbgm = std::move(other.m_oldbgm);
  m_oldcharacterIndex = other.m_oldcharacterIndex;
  m_oldcharacterName = std::move(other.m_oldcharacterName);
  m_oldstartMapId = other.m_oldstartMapId;
  m_oldstartX = other.m_oldstartX;
  m_oldstartY = other.m_oldstartY;
  return *this;
}

Audio& Vehicle::bgm() { return m_bgm; }
const Audio& Vehicle::bgm() const { return m_bgm; }
void Vehicle::setBgm(const Audio& bgm) {
  if (m_bgm == bgm) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(bgm);
  m_bgm = bgm;
  if (!signalsDisabled()) {
  emit_signal(bgmModified(), this, bgm);
  }
  setModified();
}

int Vehicle::characterIndex() const { return m_characterIndex; }
void Vehicle::setCharacterIndex(int characterIndex) {
  if (m_characterIndex == characterIndex) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(characterIndex);
  m_characterIndex = characterIndex;
  if (!signalsDisabled()) {
  emit_signal(characterIndexModified(), this, characterIndex);
  }
  setModified();
}

const std::string& Vehicle::characterName() const { return m_characterName; }
void Vehicle::setCharacterName(const std::string& characterName) {
  if (m_characterName == characterName) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(characterName);
  m_characterName = characterName;
  if (!signalsDisabled()) {
  emit_signal(characterNameModified(), this, characterName);
  }
  setModified();
}

int Vehicle::startMapId() const { return m_startMapId; }
void Vehicle::setStartMapId(int startMapId) {
  if (m_startMapId == startMapId) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(startMapId);
  m_startMapId = startMapId;
  if (!signalsDisabled()) {
  emit_signal(startMapIdModified(), this, startMapId);
  }
  setModified();
}

int Vehicle::startX() const { return m_startX; }
void Vehicle::setStartX(int startX) {
  if (m_startX == startX) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(startX);
  m_startX = startX;
  if (!signalsDisabled()) {
  emit_signal(startXModified(), this, startX);
  }
  setModified();
}

int Vehicle::startY() const { return m_startY; }
void Vehicle::setStartY(int startY) {
  if (m_startY == startY) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(startY);
  m_startY = startY;
  if (!signalsDisabled()) {
  emit_signal(startYModified(), this, startY);
  }
  setModified();
}

void Vehicle::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(bgm);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(characterIndex);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(characterName);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(startMapId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(startX);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(startY);
  m_bgm.restoreOriginal();
}

void Vehicle::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(bgm);
  MODIFIABLE_ACCEPT_VALUE(characterIndex);
  MODIFIABLE_ACCEPT_VALUE(characterName);
  MODIFIABLE_ACCEPT_VALUE(startMapId);
  MODIFIABLE_ACCEPT_VALUE(startX);
  MODIFIABLE_ACCEPT_VALUE(startY);
  m_bgm.acceptChanges();
}
nlohmann::ordered_json Vehicle::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_CHILD_VALUE(bgm),  MODIFIABLE_SERIALIZE_VALUE(characterIndex), MODIFIABLE_SERIALIZE_VALUE(characterName),
      MODIFIABLE_SERIALIZE_VALUE(startMapId), MODIFIABLE_SERIALIZE_VALUE(startX),         MODIFIABLE_SERIALIZE_VALUE(startY),
  };
}

rpgmutils::signal<void(Vehicle*, const Audio&)>& Vehicle::bgmModified() {
  if (!m_bgmModified) {
    m_bgmModified.emplace();
  }
  return *m_bgmModified;
}
rpgmutils::signal<void(Vehicle*, int)>& Vehicle::characterIndexModified() {
  if (!m_characterIndexModified) {
    m_characterIndexModified.emplace();
  }
  return *m_characterIndexModified;
}
rpgmutils::signal<void(Vehicle*, const std::string&)>& Vehicle::characterNameModified() {
  if (!m_characterNameModified) {
    m_characterNameModified.emplace();
  }
  return *m_characterNameModified;
}
rpgmutils::signal<void(Vehicle*, int)>& Vehicle::startMapIdModified() {
  if (!m_startMapIdModified) {
    m_startMapIdModified.emplace();
  }
  return *m_startMapIdModified;
}
rpgmutils::signal<void(Vehicle*, int)>& Vehicle::startXModified() {
  if (!m_startXModified) {
    m_startXModified.emplace();
  }
  return *m_startXModified;
}
rpgmutils::signal<void(Vehicle*, int)>& Vehicle::startYModified() {
  if (!m_startYModified) {
    m_startYModified.emplace();
  }
  return *m_startYModified;
}

void to_json(nlohmann::ordered_json& j, const Vehicle& v) {
  j = {
      {"bgm", v.m_bgm}, {"characterIndex", v.m_characterIndex}, {"characterName", v.m_characterName}, {"startMapId", v.m_startMapId}, {"startX", v.m_startX}, {"startY", v.m_startY},
  };
}
void from_json(const nlohmann::ordered_json& j, Vehicle& v) {
  v.m_bgm = j.value("bgm", v.m_bgm);
  v.m_characterIndex = j.value("characterIndex", v.m_characterIndex);
  v.m_characterName = j.value("characterName", v.m_characterName);
  v.m_startMapId = j.value("startMapId", v.m_startMapId);
  v.m_startX = j.value("startX", v.m_startX);
  v.m_startY = j.value("startY", v.m_startY);
}
