
#include "Database/Map.hpp"

#include "Database/Event.hpp"
#include "Database/EventPage.hpp"
#include <fstream>

Map::Encounter::Encounter(const Encounter& other)
: IModifiable(other)
, m_regionSet(other.m_regionSet)
, m_troopId(other.m_troopId)
, m_weight(other.m_weight)
, m_oldregionSet(other.m_oldregionSet)
, m_oldtroopId(other.m_oldtroopId)
, m_oldweight(other.m_oldweight) {}

Map::Encounter& Map::Encounter::operator=(const Encounter& other) {
  IModifiable::operator=(other);
  m_regionSet = other.m_regionSet;
  m_troopId = other.m_troopId;
  m_weight = other.m_weight;
  m_oldregionSet = other.m_oldregionSet;
  m_oldtroopId = other.m_oldtroopId;
  m_oldweight = other.m_oldweight;
  return *this;
}
Map::Encounter::Encounter(Encounter&& other) noexcept
: IModifiable(std::move(other))
, m_regionSet(std::move(other.m_regionSet))
, m_troopId(other.m_troopId)
, m_weight(other.m_weight)
, m_oldregionSet(std::move(other.m_oldregionSet))
, m_oldtroopId(other.m_oldtroopId)
, m_oldweight(other.m_oldweight) {}
Map::Encounter& Map::Encounter::operator=(Encounter&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_regionSet = std::move(other.m_regionSet);
  m_troopId = other.m_troopId;
  m_weight = other.m_weight;
  m_oldregionSet = std::move(other.m_oldregionSet);
  m_oldtroopId = other.m_oldtroopId;
  m_oldweight = other.m_oldweight;
  return *this;
}

int Map::Encounter::troopId() const { return m_troopId; }

void Map::Encounter::setTroopId(int id) {
  if (m_troopId == id) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(troopId);
  m_troopId = id;
  if (!signalsDisabled()) {
    emit_signal(troopIdModified(), this, id);
  }
  setModified();
}

int Map::Encounter::weight() const { return m_weight; }
void Map::Encounter::setWeight(int weight) {
  if (m_weight == weight) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(weight);
  m_weight = weight;
  if (!signalsDisabled()) {
    emit_signal(weightModified(), this, weight);
  }
  setModified();
}

const std::vector<int>& Map::Encounter::regionSet() { return m_regionSet; }
void Map::Encounter::setRegionSet(const std::vector<int>& regionSet) {
  if (m_regionSet == regionSet) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(regionSet);
  m_regionSet = regionSet;
  if (!signalsDisabled()) {
    emit_signal(regionSetModified(), this, regionSet);
  }
  setModified();
}
void Map::Encounter::setRegion(int idx, int region) {
  if (idx >= m_regionSet.size()) {
    m_regionSet.resize(idx + 1);
  }
  if (region == m_regionSet[idx]) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(regionSet);
  m_regionSet[idx] = region;
  if (!signalsDisabled()) {
    emit_signal(regionModified(), this, idx, region);
  }
  setModified();
}

void Map::Encounter::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(regionSet);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(troopId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(weight);
}
void Map::Encounter::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(regionSet);
  MODIFIABLE_ACCEPT_VALUE(troopId);
  MODIFIABLE_ACCEPT_VALUE(weight);
}
nlohmann::ordered_json Map::Encounter::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(regionSet),
      MODIFIABLE_SERIALIZE_VALUE(troopId),
      MODIFIABLE_SERIALIZE_VALUE(weight),
  };
}

rpgmutils::signal<void(Map::Encounter*, const std::vector<int>&)>& Map::Encounter::regionSetModified() {
  if (!m_regionSetModified) {
    m_regionSetModified.emplace();
  }
  return *m_regionSetModified;
}
rpgmutils::signal<void(Map::Encounter*, int, int)>& Map::Encounter::regionModified() {
  if (!m_regionModified) {
    m_regionModified.emplace();
  }
  return *m_regionModified;
}
rpgmutils::signal<void(Map::Encounter*, int)>& Map::Encounter::troopIdModified() {
  if (!m_troopIdModified) {
    m_troopIdModified.emplace();
  }
  return *m_troopIdModified;
}
rpgmutils::signal<void(Map::Encounter*, int)>& Map::Encounter::weightModified() {
  if (!m_weightModified) {
    m_weightModified.emplace();
  }
  return *m_weightModified;
}

Map::Map(const Map& other)
: IModifiable(other)
, m_autoplayBgm(other.m_autoplayBgm)
, m_autoplayBgs(other.m_autoplayBgs)
, m_battleback1Name(other.m_battleback1Name)
, m_battleback2Name(other.m_battleback2Name)
, m_bgm(other.m_bgm)
, m_bgs(other.m_bgs)
, m_disableDashing(other.m_disableDashing)
, m_encounterList(other.m_encounterList)
, m_encounterStep(other.m_encounterStep)
, m_note(other.m_note)
, m_parallaxLoopX(other.m_parallaxLoopX)
, m_parallaxLoopY(other.m_parallaxLoopY)
, m_parallaxName(other.m_parallaxName)
, m_parallaxShow(other.m_parallaxShow)
, m_parallaxSx(other.m_parallaxSx)
, m_parallaxSy(other.m_parallaxSy)
, m_scrollType(other.m_scrollType)
, m_specifyBattleback(other.m_specifyBattleback)
, m_tilesetId(other.m_tilesetId)
, m_width(other.m_width)
, m_height(other.m_height)
, m_data(other.m_data)
, m_events(other.m_events)
, m_oldautoplayBgm(other.m_oldautoplayBgm)
, m_oldautoplayBgs(other.m_oldautoplayBgs)
, m_oldbattleback1Name(other.m_oldbattleback1Name)
, m_oldbattleback2Name(other.m_oldbattleback2Name)
, m_oldbgm(other.m_oldbgm)
, m_oldbgs(other.m_oldbgs)
, m_olddisableDashing(other.m_olddisableDashing)
, m_oldencounterList(other.m_oldencounterList)
, m_oldencounterStep(other.m_oldencounterStep)
, m_oldnote(other.m_oldnote)
, m_oldparallaxLoopX(other.m_oldparallaxLoopX)
, m_oldparallaxLoopY(other.m_oldparallaxLoopY)
, m_oldparallaxName(other.m_oldparallaxName)
, m_oldparallaxShow(other.m_oldparallaxShow)
, m_oldparallaxSx(other.m_oldparallaxSx)
, m_oldparallaxSy(other.m_oldparallaxSy)
, m_oldscrollType(other.m_oldscrollType)
, m_oldspecifyBattleback(other.m_oldspecifyBattleback)
, m_oldtilesetId(other.m_oldtilesetId)
, m_oldwidth(other.m_oldwidth)
, m_oldheight(other.m_oldheight)
, m_olddata(other.m_olddata)
, m_oldevents(std::move(other.m_oldevents)) {}

Map& Map::operator=(const Map& other) {
  IModifiable::operator=(other);
  m_autoplayBgm = other.m_autoplayBgm;
  m_autoplayBgs = other.m_autoplayBgs;
  m_battleback1Name = other.m_battleback1Name;
  m_battleback2Name = other.m_battleback2Name;
  m_bgm = other.m_bgm;
  m_bgs = other.m_bgs;
  m_disableDashing = other.m_disableDashing;
  m_encounterList = other.m_encounterList;
  m_encounterStep = other.m_encounterStep;
  m_note = other.m_note;
  m_parallaxLoopX = other.m_parallaxLoopX;
  m_parallaxLoopY = other.m_parallaxLoopY;
  m_parallaxName = other.m_parallaxName;
  m_parallaxShow = other.m_parallaxShow;
  m_parallaxSx = other.m_parallaxSx;
  m_parallaxSy = other.m_parallaxSy;
  m_scrollType = other.m_scrollType;
  m_specifyBattleback = other.m_specifyBattleback;
  m_tilesetId = other.m_tilesetId;
  m_width = other.m_width;
  m_height = other.m_height;
  m_data = other.m_data;
  m_events = other.m_events;
  m_oldautoplayBgm = other.m_oldautoplayBgm;
  m_oldautoplayBgs = other.m_oldautoplayBgs;
  m_oldbattleback1Name = other.m_oldbattleback1Name;
  m_oldbattleback2Name = other.m_oldbattleback2Name;
  m_oldbgm = other.m_oldbgm;
  m_oldbgs = other.m_oldbgs;
  m_olddisableDashing = other.m_olddisableDashing;
  m_oldencounterList = other.m_oldencounterList;
  m_oldencounterStep = other.m_oldencounterStep;
  m_oldnote = other.m_oldnote;
  m_oldparallaxLoopX = other.m_oldparallaxLoopX;
  m_oldparallaxLoopY = other.m_oldparallaxLoopY;
  m_oldparallaxName = other.m_oldparallaxName;
  m_oldparallaxShow = other.m_oldparallaxShow;
  m_oldparallaxSx = other.m_oldparallaxSx;
  m_oldparallaxSy = other.m_oldparallaxSy;
  m_oldscrollType = other.m_oldscrollType;
  m_oldspecifyBattleback = other.m_oldspecifyBattleback;
  m_oldtilesetId = other.m_oldtilesetId;
  m_oldwidth = other.m_oldwidth;
  m_oldheight = other.m_oldheight;
  m_olddata = other.m_olddata;
  return *this;
}
Map::Map(Map&& other) noexcept
: IModifiable(std::move(other))
, m_autoplayBgm(other.m_autoplayBgm)
, m_autoplayBgs(other.m_autoplayBgs)
, m_battleback1Name(std::move(other.m_battleback1Name))
, m_battleback2Name(std::move(other.m_battleback2Name))
, m_bgm(std::move(other.m_bgm))
, m_bgs(std::move(other.m_bgs))
, m_disableDashing(other.m_disableDashing)
, m_encounterList(std::move(other.m_encounterList))
, m_encounterStep(other.m_encounterStep)
, m_note(std::move(other.m_note))
, m_parallaxLoopX(other.m_parallaxLoopX)
, m_parallaxLoopY(other.m_parallaxLoopY)
, m_parallaxName(std::move(other.m_parallaxName))
, m_parallaxShow(other.m_parallaxShow)
, m_parallaxSx(other.m_parallaxSx)
, m_parallaxSy(other.m_parallaxSy)
, m_scrollType(other.m_scrollType)
, m_specifyBattleback(other.m_specifyBattleback)
, m_tilesetId(other.m_tilesetId)
, m_width(other.m_width)
, m_height(other.m_height)
, m_data(std::move(other.m_data))
, m_events(std::move(other.m_events))
, m_oldautoplayBgm(other.m_oldautoplayBgm)
, m_oldautoplayBgs(other.m_oldautoplayBgs)
, m_oldbattleback1Name(std::move(other.m_oldbattleback1Name))
, m_oldbattleback2Name(std::move(other.m_oldbattleback2Name))
, m_oldbgm(std::move(other.m_oldbgm))
, m_oldbgs(std::move(other.m_oldbgs))
, m_olddisableDashing(other.m_olddisableDashing)
, m_oldencounterList(std::move(other.m_oldencounterList))
, m_oldencounterStep(other.m_oldencounterStep)
, m_oldnote(std::move(other.m_oldnote))
, m_oldparallaxLoopX(other.m_oldparallaxLoopX)
, m_oldparallaxLoopY(other.m_oldparallaxLoopY)
, m_oldparallaxName(std::move(other.m_oldparallaxName))
, m_oldparallaxShow(other.m_oldparallaxShow)
, m_oldparallaxSx(other.m_oldparallaxSx)
, m_oldparallaxSy(other.m_oldparallaxSy)
, m_oldscrollType(other.m_oldscrollType)
, m_oldspecifyBattleback(other.m_oldspecifyBattleback)
, m_oldtilesetId(other.m_oldtilesetId)
, m_oldwidth(other.m_oldwidth)
, m_oldheight(other.m_oldheight)
, m_olddata(other.m_olddata)
, m_oldevents(std::move(other.m_oldevents)) {};
Map& Map::operator=(Map&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_autoplayBgm = other.m_autoplayBgm;
  m_autoplayBgs = other.m_autoplayBgs;
  m_battleback1Name = std::move(other.m_battleback1Name);
  m_battleback2Name = std::move(other.m_battleback2Name);
  m_bgm = std::move(other.m_bgm);
  m_bgs = std::move(other.m_bgs);
  m_disableDashing = other.m_disableDashing;
  m_encounterList = std::move(other.m_encounterList);
  m_encounterStep = other.m_encounterStep;
  m_note = std::move(other.m_note);
  m_parallaxLoopX = other.m_parallaxLoopX;
  m_parallaxLoopY = other.m_parallaxLoopY;
  m_parallaxName = std::move(other.m_parallaxName);
  m_parallaxShow = other.m_parallaxShow;
  m_parallaxSx = other.m_parallaxSx;
  m_parallaxSy = other.m_parallaxSy;
  m_scrollType = other.m_scrollType;
  m_specifyBattleback = other.m_specifyBattleback;
  m_tilesetId = other.m_tilesetId;
  m_width = other.m_width;
  m_height = other.m_height;
  m_data = std::move(other.m_data);
  m_events = std::move(other.m_events);
  m_oldautoplayBgm = other.m_oldautoplayBgm;
  m_oldautoplayBgs = other.m_oldautoplayBgs;
  m_oldbattleback1Name = std::move(other.m_oldbattleback1Name);
  m_oldbattleback2Name = std::move(other.m_oldbattleback2Name);
  m_oldbgm = std::move(other.m_oldbgm);
  m_oldbgs = std::move(other.m_oldbgs);
  m_olddisableDashing = other.m_olddisableDashing;
  m_oldencounterList = std::move(other.m_oldencounterList);
  m_oldencounterStep = other.m_oldencounterStep;
  m_oldnote = std::move(other.m_oldnote);
  m_oldparallaxLoopX = other.m_oldparallaxLoopX;
  m_oldparallaxLoopY = other.m_oldparallaxLoopY;
  m_oldparallaxName = std::move(other.m_oldparallaxName);
  m_oldparallaxShow = other.m_oldparallaxShow;
  m_oldparallaxSx = other.m_oldparallaxSx;
  m_oldparallaxSy = other.m_oldparallaxSy;
  m_oldscrollType = other.m_oldscrollType;
  m_oldspecifyBattleback = other.m_oldspecifyBattleback;
  m_oldtilesetId = other.m_oldtilesetId;
  m_oldwidth = other.m_oldwidth;
  m_oldheight = other.m_oldheight;
  return *this;
}

[[nodiscard]] bool Map::autoplayBgm() const { return m_autoplayBgm; }
void Map::setAutoplayBgm(const bool value) {
  if (value == m_autoplayBgm) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(autoplayBgm);
  m_autoplayBgm = value;
  if (!signalsDisabled()) {
    emit_signal(autoplayBgmModified(), this, value);
  }
  setModified();
}

[[nodiscard]] bool Map::autoplayBgs() const { return m_autoplayBgs; }
void Map::setAutoplayBgs(const bool value) {
  if (value == m_autoplayBgs) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(autoplayBgs);
  m_autoplayBgs = value;
  if (!signalsDisabled()) {
    emit_signal(autoplayBgsModified(), this, value);
  }
  setModified();
}

const std::string& Map::battleback1Name() const { return m_battleback1Name; }
void Map::setBattleback1Name(const std::string& value) {
  if (value == m_battleback1Name) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(battleback1Name);
  m_battleback1Name = value;
  if (!signalsDisabled()) {
    emit_signal(battleback1NameModified(), this, value);
  }
  setModified();
}

const std::string& Map::battleback2Name() const { return m_battleback2Name; }
void Map::setBattleback2Name(const std::string& value) {
  if (value == m_battleback2Name) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(battleback2Name);
  m_battleback2Name = value;
  if (!signalsDisabled()) {
    emit_signal(battleback2NameModified(), this, value);
  }
  setModified();
}

Audio& Map::bgm() { return m_bgm; }
void Map::setBgm(const Audio& bgm) {
  if (bgm == m_bgm) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(bgm);
  m_bgm = bgm;
  if (!signalsDisabled()) {
    emit_signal(bgmModified(), this, bgm);
  }
  setModified();
}

Audio& Map::bgs() { return m_bgs; }
void Map::setBgs(const Audio& bgs) {
  if (bgs == m_bgs) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(bgs);
  m_bgs = bgs;
  if (!signalsDisabled()) {
    emit_signal(bgsModified(), this, bgs);
  }
  setModified();
}

bool Map::disableDashing() const { return m_disableDashing; }
void Map::setDisableDashing(bool value) {
  if (value == m_disableDashing) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(disableDashing);
  m_disableDashing = value;
  if (!signalsDisabled()) {
    emit_signal(disableDashingModified(), this, value);
  }
  setModified();
}

const std::string& Map::displayName() const { return m_displayName; }
void Map::setDisplayName(const std::string& value) {
  if (value == m_displayName) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(displayName);
  m_displayName = value;
  if (!signalsDisabled()) {
    emit_signal(displayNameModified(), this, value);
  }
  setModified();
}

std::vector<Map::Encounter>& Map::encounterList() { return m_encounterList; }
void Map::setEncounterList(const std::vector<Encounter>& value) {
  if (value == m_encounterList) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(encounterList);
  m_encounterList = value;
  if (!signalsDisabled()) {
    emit_signal(encounterListModified(), this, value);
  }
  setModified();
}

Map::Encounter& Map::encounter(int id) {
  assert(id >= 0 && id < m_encounterList.size());
  return m_encounterList[id];
}

void Map::setEncounter(int id, const Encounter& encounter) {
  assert(id >= 0 && id < m_encounterList.size());
  if (m_encounterList[id] == encounter) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(encounterList);
  m_encounterList[id] = encounter;
  if (!signalsDisabled()) {
    emit_signal(encounterListModified(), this, m_encounterList);
  }
  setModified();
}

[[nodiscard]] int Map::encounterStep() const { return m_encounterStep; }
void Map::setEncounterStep(int value) {
  if (value == m_encounterStep) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(encounterStep);
  m_encounterStep = value;
  if (!signalsDisabled()) {
    emit_signal(encounterStepModified(), this, value);
  }
  setModified();
}

const std::string& Map::note() const { return m_note; }
void Map::setNote(const std::string& value) {
  if (value == m_note) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(note);
  m_note = value;
  if (!signalsDisabled()) {
    emit_signal(noteModified(), this, m_note);
  }
  setModified();
}

[[nodiscard]] bool Map::parallaxLoopX() const { return m_parallaxLoopX; }
void Map::setParallaxLoopX(bool value) {
  if (value == m_parallaxLoopX) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(parallaxLoopX);
  m_parallaxLoopX = value;
  if (!signalsDisabled()) {
    emit_signal(parallaxLoopXModified(), this, value);
  }
  setModified();
}

[[nodiscard]] bool Map::parallaxLoopY() const { return m_parallaxLoopY; }
void Map::setParallaxLoopY(bool value) {
  if (value == m_parallaxLoopY) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(parallaxLoopY);
  m_parallaxLoopY = value;
  if (!signalsDisabled()) {
    emit_signal(parallaxLoopYModified(), this, value);
  }
  setModified();
}

const std::string& Map::parallaxName() const { return m_parallaxName; }
void Map::setParallaxName(const std::string& value) {
  if (value == m_parallaxName) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(parallaxName);
  m_parallaxName = value;
  if (!signalsDisabled()) {
    emit_signal(parallaxNameModified(), this, value);
  }
  setModified();
}

[[nodiscard]] bool Map::parallaxShow() const { return m_parallaxShow; }
void Map::setParallaxShow(bool value) {
  if (value == m_parallaxShow) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(parallaxShow);
  m_parallaxShow = value;
  if (!signalsDisabled()) {
    emit_signal(parallaxShowModified(), this, value);
  }
  setModified();
}

[[nodiscard]] int Map::parallaxSx() const { return m_parallaxSx; }
void Map::setParallaxSx(int value) {
  if (value == m_parallaxSx) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(parallaxSx);
  m_parallaxSx = value;
  if (!signalsDisabled()) {
    emit_signal(parallaxSxModified(), this, value);
  }
  setModified();
}

[[nodiscard]] int Map::parallaxSy() const { return m_parallaxSy; }
void Map::setParallaxSy(int value) {
  if (value == m_parallaxSy) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(parallaxSy);
  m_parallaxSy = value;
  if (!signalsDisabled()) {
    emit_signal(parallaxSyModified(), this, value);
  }
  setModified();
}

[[nodiscard]] ScrollType Map::scrollType() const { return m_scrollType; }
void Map::setScrollType(ScrollType value) {
  if (value == m_scrollType) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(scrollType);
  m_scrollType = value;
  if (!signalsDisabled()) {
    emit_signal(scrollTypeModified(), this, value);
  }
  setModified();
}

[[nodiscard]] bool Map::specifyBattleback() const { return m_specifyBattleback; }
void Map::setSpecifyBattleback(bool value) {
  if (value == m_specifyBattleback) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(specifyBattleback);
  m_specifyBattleback = value;
  if (!signalsDisabled()) {
    emit_signal(specifyBattlebackModified(), this, value);
  }
  setModified();
}

[[nodiscard]] int Map::tilesetId() const { return m_tilesetId; }
void Map::setTilesetId(int value) {
  if (value == m_tilesetId) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(tilesetId);
  m_tilesetId = value;
  if (!signalsDisabled()) {
    emit_signal(tilesetIdModified(), this, value);
  }
  setModified();
}

[[nodiscard]] int Map::width() const { return m_width; }
void Map::setWidth(int value) {
  if (value == m_width) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(width);
  m_width = value;
  if (!signalsDisabled()) {
    emit_signal(widthModified(), this, value);
  }
  setModified();
}

[[nodiscard]] int Map::height() const { return m_height; }
void Map::setHeight(int value) {
  if (value == m_height) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(height);
  m_height = value;
  if (!signalsDisabled()) {
    emit_signal(heightModified(), this, value);
  }
  setModified();
}

const std::vector<std::optional<int>>& Map::data() const { return m_data; }
void Map::setData(const std::vector<std::optional<int>>& value) {
  if (value == m_data) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(data);
  m_data = value;
  if (!signalsDisabled()) {
    emit_signal(dataModified(), this, value);
  }
  setModified();
}
void Map::setTileAt(const int id, const int x, const int y, const int layer) {
  if (x < 0 || x >= m_width) {
    return;
  }
  if (y < 0 || y >= m_height) {
    return;
  }
  if (layer >= 6) {
    return;
  }
  auto& tile = m_data[(layer * m_height + y) * m_width + x];
  if (tile && *tile == id) {
    return;
  }
  tile = id;
  if (!signalsDisabled()) {
    emit_signal(dataModified(), this, m_data);
  }
  setModified();
}

int Map::tileAt(const int x, const int y, const int layer) const {
  if (x < 0 || x >= m_width) {
    return 0;
  }
  if (y < 0 || y >= m_height) {
    return 0;
  }
  if (layer >= 6) {
    return 0;
  }
  auto& tile = m_data[(layer * m_height + y) * m_width + x];
  return tile.value_or(0);
}

const std::vector<std::optional<Event>>& Map::events() const { return m_events; }
void Map::setEvents(const std::vector<std::optional<Event>>& value) {
  if (value == m_events) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(events);
  m_events = value;
  if (!signalsDisabled()) {
    emit_signal(eventsModified(), this, value);
  }
  setModified();
}

void Map::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(autoplayBgm);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(autoplayBgs);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(battleback1Name);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(battleback2Name);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(bgm);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(bgs);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(disableDashing);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(displayName);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(encounterList);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(note);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(parallaxLoopX);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(parallaxLoopY);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(parallaxName);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(parallaxSx);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(parallaxSy);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(scrollType);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(specifyBattleback);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(tilesetId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(width);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(height);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(data);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(events);

  m_bgm.restoreOriginal();
  m_bgs.restoreOriginal();
  std::ranges::for_each(m_encounterList, [&](auto& e) { e.restoreOriginal(); });
  std::ranges::for_each(m_events, [&](auto& e) {
    if (e)
      e->restoreOriginal();
  });
}
void Map::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(autoplayBgm);
  MODIFIABLE_ACCEPT_VALUE(autoplayBgs);
  MODIFIABLE_ACCEPT_VALUE(battleback1Name);
  MODIFIABLE_ACCEPT_VALUE(battleback2Name);
  MODIFIABLE_ACCEPT_VALUE(bgm);
  MODIFIABLE_ACCEPT_VALUE(bgs);
  MODIFIABLE_ACCEPT_VALUE(disableDashing);
  MODIFIABLE_ACCEPT_VALUE(displayName);
  MODIFIABLE_ACCEPT_VALUE(encounterList);
  MODIFIABLE_ACCEPT_VALUE(note);
  MODIFIABLE_ACCEPT_VALUE(parallaxLoopX);
  MODIFIABLE_ACCEPT_VALUE(parallaxLoopY);
  MODIFIABLE_ACCEPT_VALUE(parallaxName);
  MODIFIABLE_ACCEPT_VALUE(parallaxSx);
  MODIFIABLE_ACCEPT_VALUE(parallaxSy);
  MODIFIABLE_ACCEPT_VALUE(scrollType);
  MODIFIABLE_ACCEPT_VALUE(specifyBattleback);
  MODIFIABLE_ACCEPT_VALUE(tilesetId);
  MODIFIABLE_ACCEPT_VALUE(width);
  MODIFIABLE_ACCEPT_VALUE(height);
  MODIFIABLE_ACCEPT_VALUE(data);
  MODIFIABLE_ACCEPT_VALUE(events);

  m_bgm.acceptChanges();
  m_bgs.acceptChanges();
  std::ranges::for_each(m_encounterList, [&](auto& e) { e.acceptChanges(); });
  std::ranges::for_each(m_events, [&](auto& e) {
    if (e)
      e->acceptChanges();
  });
}
nlohmann::ordered_json Map::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(autoplayBgm),
      MODIFIABLE_SERIALIZE_VALUE(autoplayBgs),
      MODIFIABLE_SERIALIZE_VALUE(battleback1Name),
      MODIFIABLE_SERIALIZE_VALUE(battleback2Name),
      MODIFIABLE_SERIALIZE_CHILD_VALUE(bgm),
      MODIFIABLE_SERIALIZE_CHILD_VALUE(bgs),
      MODIFIABLE_SERIALIZE_VALUE(disableDashing),
      MODIFIABLE_SERIALIZE_VALUE(displayName),
      MODIFIABLE_SERIALIZE_VALUE_CHILD_ARRAY(encounterList),
      MODIFIABLE_SERIALIZE_VALUE(encounterStep),
      MODIFIABLE_SERIALIZE_VALUE(note),
      MODIFIABLE_SERIALIZE_VALUE(parallaxLoopX),
      MODIFIABLE_SERIALIZE_VALUE(parallaxLoopY),
      MODIFIABLE_SERIALIZE_VALUE(parallaxName),
      MODIFIABLE_SERIALIZE_VALUE(parallaxSx),
      MODIFIABLE_SERIALIZE_VALUE(parallaxSx),
      MODIFIABLE_SERIALIZE_VALUE(scrollType),
      MODIFIABLE_SERIALIZE_VALUE(specifyBattleback),
      MODIFIABLE_SERIALIZE_VALUE(tilesetId),
      MODIFIABLE_SERIALIZE_VALUE(width),
      MODIFIABLE_SERIALIZE_VALUE(height),
      MODIFIABLE_SERIALIZE_VALUE(data),
      MODIFIABLE_SERIALIZE_VALUE_CHILD_ARRAY_OPTIONAL(events),
  };
}

rpgmutils::signal<void(Map*, bool)>& Map::autoplayBgmModified() {
  if (!m_autoplayBgmModified) {
    m_autoplayBgmModified.emplace();
  }
  return *m_autoplayBgmModified;
}
rpgmutils::signal<void(Map*, bool)>& Map::autoplayBgsModified() {
  if (!m_autoplayBgsModified) {
    m_autoplayBgsModified.emplace();
  }
  return *m_autoplayBgsModified;
}
rpgmutils::signal<void(Map*, const std::string&)>& Map::battleback1NameModified() {
  if (!m_battleback1NameModified) {
    m_battleback1NameModified.emplace();
  }
  return *m_battleback1NameModified;
}
rpgmutils::signal<void(Map*, const std::string&)>& Map::battleback2NameModified() {
  if (!m_battleback2NameModified) {
    m_battleback2NameModified.emplace();
  }
  return *m_battleback2NameModified;
}
rpgmutils::signal<void(Map*, Audio)>& Map::bgmModified() {
  if (!m_bgmModified) {
    m_bgmModified.emplace();
  }
  return *m_bgmModified;
}
rpgmutils::signal<void(Map*, Audio)>& Map::bgsModified() {
  if (!m_bgsModified) {
    m_bgsModified.emplace();
  }
  return *m_bgsModified;
}
rpgmutils::signal<void(Map*, bool)>& Map::disableDashingModified() {
  if (!m_disableDashingModified) {
    m_disableDashingModified.emplace();
  }
  return *m_disableDashingModified;
}
rpgmutils::signal<void(Map*, const std::string&)>& Map::displayNameModified() {
  if (!m_displayNameModified) {
    m_displayNameModified.emplace();
  }
  return *m_displayNameModified;
}
rpgmutils::signal<void(Map*, const std::vector<Map::Encounter>&)>& Map::encounterListModified() {
  if (!m_encounterListModified) {
    m_encounterListModified.emplace();
  }
  return *m_encounterListModified;
}
rpgmutils::signal<void(Map*, int)>& Map::encounterStepModified() {
  if (!m_encounterStepModified) {
    m_encounterStepModified.emplace();
  }
  return *m_encounterStepModified;
}
rpgmutils::signal<void(Map*, const std::string&)>& Map::noteModified() {
  if (!m_noteModified) {
    m_noteModified.emplace();
  }
  return *m_noteModified;
}
rpgmutils::signal<void(Map*, bool)>& Map::parallaxLoopXModified() {
  if (!m_parallaxLoopXModified) {
    m_parallaxLoopXModified.emplace();
  }
  return *m_parallaxLoopXModified;
}
rpgmutils::signal<void(Map*, bool)>& Map::parallaxLoopYModified() {
  if (!m_parallaxLoopYModified) {
    m_parallaxLoopYModified.emplace();
  }
  return *m_parallaxLoopYModified;
}
rpgmutils::signal<void(Map*, const std::string&)>& Map::parallaxNameModified() {
  if (!m_parallaxNameModified) {
    m_parallaxNameModified.emplace();
  }
  return *m_parallaxNameModified;
}
rpgmutils::signal<void(Map*, bool)>& Map::parallaxShowModified() {
  if (!m_parallaxShowModified) {
    m_parallaxShowModified.emplace();
  }
  return *m_parallaxShowModified;
}
rpgmutils::signal<void(Map*, int)>& Map::parallaxSxModified() {
  if (!m_parallaxSxModified) {
    m_parallaxSxModified.emplace();
  }
  return *m_parallaxSxModified;
}
rpgmutils::signal<void(Map*, int)>& Map::parallaxSyModified() {
  if (!m_parallaxSyModified) {
    m_parallaxSyModified.emplace();
  }
  return *m_parallaxSyModified;
}
rpgmutils::signal<void(Map*, ScrollType)>& Map::scrollTypeModified() {
  if (!m_scrollTypeModified) {
    m_scrollTypeModified.emplace();
  }
  return *m_scrollTypeModified;
}
rpgmutils::signal<void(Map*, bool)>& Map::specifyBattlebackModified() {
  if (!m_specifyBattlebackModified) {
    m_specifyBattlebackModified.emplace();
  }
  return *m_specifyBattlebackModified;
}
rpgmutils::signal<void(Map*, int)>& Map::tilesetIdModified() {
  if (!m_tilesetIdModified) {
    m_tilesetIdModified.emplace();
  }
  return *m_tilesetIdModified;
}
rpgmutils::signal<void(Map*, int)>& Map::widthModified() {
  if (!m_widthModified) {
    m_widthModified.emplace();
  }
  return *m_widthModified;
}
rpgmutils::signal<void(Map*, int)>& Map::heightModified() {
  if (!m_heightModified) {
    m_heightModified.emplace();
  }
  return *m_heightModified;
}
rpgmutils::signal<void(Map*, const std::vector<std::optional<int>>&)>& Map::dataModified() {
  if (!m_dataModified) {
    m_dataModified.emplace();
  }
  return *m_dataModified;
}
rpgmutils::signal<void(Map*, const std::vector<std::optional<Event>>&)>& Map::eventsModified() {
  if (!m_eventsModified) {
    m_eventsModified.emplace();
  }
  return *m_eventsModified;
}

void Map::resize(const int newWidth, const int newHeight) {
  static constexpr int MaxLayers = 6;
  std::vector<std::optional<int>> newData;
  newData.resize(MaxLayers * newWidth * newHeight);
  std::ranges::fill(newData.begin(), newData.end(), 0);

  for (int y = 0; y < newHeight; ++y) {
    for (int x = 0; x < newWidth; ++x) {
      if (x < m_width && y < m_height) {
        for (int z = 0; z < MaxLayers; ++z) {
          const int si = (z * m_height + y) * m_width + x;
          const int di = (z * newHeight + y) * newWidth + x;
          newData[di] = m_data[si];
        }
      }
    }
  }

  std::ranges::for_each(m_events, [&](auto& event) {
    if (event->x() >= newWidth || event->y() >= newHeight) {
      event.reset();
    }
  });

  m_width = newWidth;
  m_height = newHeight;
  m_data = std::move(newData);
}

[[nodiscard]] std::vector<Event*> Map::getSorted() {
  std::vector<Event*> ret;
  for (auto& e : m_events) {
    if (e) {
      ret.push_back(&e.value());
    }
  }
  std::ranges::sort(ret, [](const Event* a, const Event* b) {
    const int aZ = static_cast<int>(a->page(0)->priorityType()) * 2 + 1;
    const int bZ = static_cast<int>(b->page(0)->priorityType()) * 2 + 1;
    if (aZ != bZ) {
      return aZ < bZ;
    }

    if (a->y() != b->y()) {
      return a->y() < b->y();
    }
    return a->renderer()->spriteId() < b->renderer()->spriteId();
  });
  return ret;
}

[[nodiscard]] std::vector<Event*> Map::getRenderSorted() {
  std::vector<Event*> ret;
  for (auto& e : m_events) {
    if (e) {
      ret.push_back(&e.value());
    }
  }
  std::ranges::sort(ret, [](const Event* a, const Event* b) {
    if (a->renderer()->screenZ() != b->renderer()->screenZ()) {
      return a->renderer()->screenZ() < b->renderer()->screenZ();
    }
    if (a->renderer()->y() != b->renderer()->y()) {
      return a->renderer()->y() < b->renderer()->y();
    }
    return a->renderer()->spriteId() < b->renderer()->spriteId();
  });
  return ret;
}

std::vector<Event*> Map::eventsAtNoThrough(const int x, const int y) {
  std::vector<Event*> ret;
  std::ranges::for_each(m_events, [&ret, &x, &y](auto& ev) {
    if (ev && ev->x() == x && ev->y() == y && !ev->renderer()->isThrough()) {
      ret.push_back(&ev.value());
    }
  });
  return ret;
}

std::vector<Event*> Map::eventsAtRenderPosNoThrough(const int x, const int y) {
  std::vector<Event*> ret;
  std::ranges::for_each(m_events, [&ret, &x, &y](auto& ev) {
    if (ev && ev->renderer()->x() == x && ev->renderer()->y() == y && !ev->renderer()->isThrough()) {
      ret.push_back(&ev.value());
    }
  });
  return ret;
}

void Map::setDummyEvent(const EventImage& image, const int x, const int y) {
  if (m_events.empty()) {
    m_events.emplace_back();
  }
  if (!m_events[0]) {
    m_events[0].emplace();
  }
  EventPage page;
  /* We don't care about the state of this object, so disable signals */
  m_events[0]->disableSignals();
  page.setImage(image);
  m_events[0]->setId(0);
  m_events[0]->setPages({page});
  m_events[0]->setX(x);
  m_events[0]->setY(y);
}

Event* Map::event(int id) {

  if (const auto it = std::ranges::find_if(m_events, [&id](const auto& ev) { return ev && ev->id() == id; }); it != m_events.end()) {
    return &it->value();
  }
  return nullptr;
}

Event* Map::eventAt(int x, int y) {
  if (const auto it = std::ranges::find_if(m_events, [&x, &y](const auto& ev) { return ev && ev->x() == x && ev->y() == y; }); it != m_events.end()) {
    return &it->value();
  }

  return nullptr;
}

std::vector<Event*> Map::eventsAt(const int x, const int y) {
  std::vector<Event*> ret;
  std::ranges::for_each(m_events, [&ret, &x, &y](auto& ev) {
    if (ev && ev->x() == x && ev->y() == y) {
      ret.push_back(&ev.value());
    }
  });
  return ret;
}

const std::vector<const Event*> Map::eventsAt(const int x, const int y) const {
  std::vector<const Event*> ret;
  std::ranges::for_each(m_events, [&ret, &x, &y](auto& ev) {
    if (ev && ev->x() == x && ev->y() == y) {
      ret.push_back(&ev.value());
    }
  });
  return ret;
}

Event* Map::createNewEvent() {
  if (m_events.empty()) {
    m_events.emplace_back();
  }
  const auto it = std::find_if(m_events.begin() + 1, m_events.end(), [](const auto& ev) { return !ev; });
  Event* ret;
  if (it != m_events.end()) {
    *it = Event();
    (*it)->setId(it - m_events.begin());
    ret = &it->value();
  } else {
    ret = &m_events.emplace_back(Event()).value();
    ret->setId(m_events.size() - 1);
  }
  ret->setName(std::format("EV{:03}", ret->id()));
  ret->addPage({});
  ret->modified().connect<&Map::onEventModified>(this);
  ret->connectAllSignals();

  m_isDirty = true;
  return ret;
}
Event* Map::createEventFromTemplate(const Event& ev) {
  // Inserts new template
  bool isNull{false};
  if (ev.id() == m_events.size()) {
    m_events.emplace_back(ev);
  } else {
    if (m_events.size() < ev.id()) {
      while (m_events.size() <= ev.id()) {
        m_events.emplace_back(std::nullopt);
      }
    }
    if (m_events.at(ev.id()).has_value()) {
      m_events.insert(m_events.begin() + ev.id(), ev);
    } else {
      isNull = true;
      m_events.at(ev.id()) = ev;
    }
  }
  // Resort and rename
  if (!isNull) {
    for (int i = ev.id() + 1; i < m_events.size(); ++i) {
      if (m_events.at(i).has_value()) {
        m_events.at(i)->setId(i);
        if (m_events.at(i)->name().contains("EV")) {
          m_events.at(i)->setName(std::format("EV{:03} ", i));
        }
      }
    }
  }
  return &*m_events.at(ev.id());
}

void Map::deleteEvent(int id) {
  if (const auto it = std::ranges::find_if(m_events, [&id](const auto& ev) { return ev && ev->id() == id; }); it != m_events.end()) {
    it->reset();
    m_isDirty = true;
  }
}

int Map::findOrMakeFreeId() const {
  if (m_events.empty()) {
    return 1;
  }

  for (int i = 1; i < m_events.size(); ++i) {
    if (!m_events[i]) {
      return i;
    }
  }

  return m_events.size();
}

void Map::connectAllEvents() {
  for (auto& ev : m_events) {
    if (!ev || ev->id() == 0) {
      continue;
    }

    // Disconnect just in case
    ev->modified().disconnect<&Map::onEventModified>(this);
    ev->modified().connect<&Map::onEventModified>(this);
    ev->connectAllSignals();
  }
}

void Map::onEventModified(IModifiable*) { emit_signal(modified(), this); }

void to_json(nlohmann::ordered_json& json, const Map& map) {
  json = {
      {"autoplayBgm", map.m_autoplayBgm},
      {"autoplayBgs", map.m_autoplayBgs},
      {"battleback1Name", map.m_battleback1Name},
      {"battleback2Name", map.m_battleback2Name},
      {"bgm", map.m_bgm},
      {"bgs", map.m_bgs},
      {"disableDashing", map.m_disableDashing},
      {"displayName", map.m_displayName},
      {"encounterList", map.m_encounterList},
      {"encounterStep", map.m_encounterStep},
      {"height", map.m_height},
      {"note", map.m_note},
      {"parallaxLoopX", map.m_parallaxLoopX},
      {"parallaxLoopY", map.m_parallaxLoopY},
      {"parallaxName", map.m_parallaxName},
      {"parallaxShow", map.m_parallaxShow},
      {"parallaxSx", map.m_parallaxSx},
      {"parallaxSy", map.m_parallaxSy},
      {"scrollType", map.m_scrollType},
      {"specifyBattleback", map.m_specifyBattleback},
      {"tilesetId", map.m_tilesetId},
      {"width", map.m_width},
      {"data", map.m_data},
  };
  for (const auto& event : map.m_events) {
    if (!event || event->id() == 0) {
      json["events"].push_back(nullptr);
    } else {
      json["events"].push_back(event->clone());
    }
  }
}
void from_json(const nlohmann::ordered_json& json, Map& map) {
  map.m_autoplayBgm = json.value("autoplayBgm", map.m_autoplayBgm);
  map.m_autoplayBgs = json.value("autoplayBgs", map.m_autoplayBgs);
  map.m_battleback1Name = json.value("battleback1Name", map.m_battleback1Name);
  map.m_battleback2Name = json.value("battleback2Name", map.m_battleback2Name);
  map.m_bgm = json.value("bgm", map.m_bgm);
  map.m_bgs = json.value("bgs", map.m_bgs);
  map.m_disableDashing = json.value("disableDashing", map.m_disableDashing);
  map.m_displayName = json.value("displayName", map.m_displayName);
  map.m_encounterList = json.value("encounterList", map.m_encounterList);
  map.m_encounterStep = json.value("encounterStep", map.m_encounterStep);
  map.m_height = json.value("height", map.m_height);
  map.m_note = json.value("note", map.m_note);
  map.m_parallaxLoopX = json.value("parallaxLoopX", map.m_parallaxLoopX);
  map.m_parallaxLoopY = json.value("parallaxLoopY", map.m_parallaxLoopY);
  map.m_parallaxName = json.value("parallaxName", map.m_parallaxName);
  map.m_parallaxShow = json.value("parallaxShow", map.m_parallaxShow);
  map.m_parallaxSx = json.value("parallaxSx", map.m_parallaxSx);
  map.m_parallaxSy = json.value("parallaxSy", map.m_parallaxSy);
  map.m_scrollType = json.value("scrollType", map.m_scrollType);
  map.m_specifyBattleback = json.value("specifyBattleback", map.m_specifyBattleback);
  map.m_tilesetId = json.value("tilesetId", map.m_tilesetId);
  map.m_width = json.value("width", map.m_width);
  map.m_data = json.value("data", map.m_data);
  map.m_events = json.value("events", map.m_events);
}

void to_json(nlohmann::ordered_json& json, const Map::Encounter& encounter) {
  json = {
      {"regionSet", encounter.m_regionSet},
      {"troopId", encounter.m_troopId},
      {"weight", encounter.m_weight},
  };
}
void from_json(const nlohmann::ordered_json& json, Map::Encounter& encounter) {
  encounter.m_regionSet = json.value("regionSet", encounter.m_regionSet);
  encounter.m_troopId = json.value("troopId", encounter.m_troopId);
  encounter.m_weight = json.value("weight", encounter.m_weight);
}
