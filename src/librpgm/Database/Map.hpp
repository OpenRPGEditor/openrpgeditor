#pragma once

#include "Database/Audio.hpp"
#include "Database/Event.hpp"
#include "Database/EventCommands/EventDummy.hpp"
#include "Database/Globals.hpp"

#include <format>
#include <string>

#include "nlohmann/json.hpp"

class Map {
  friend class MapSerializer;

public:
  friend void to_json(nlohmann::ordered_json& json, const Map& map);
  friend void from_json(const nlohmann::ordered_json& json, Map& map);
  struct Encounter {
    friend void to_json(nlohmann::ordered_json& json, const Encounter& encounter);
    friend void from_json(const nlohmann::ordered_json& json, Encounter& encounter);
    std::vector<int> regionSet{};
    int troopId{};
    int weight{};
  };

  [[nodiscard]] bool autoplayBgm() const { return m_autoplayBgm; }
  void setAutoplayBgm(const bool value) { m_autoplayBgm = value; }

  [[nodiscard]] bool autoplayBgs() const { return m_autoplayBgs; }
  void setAutoplayBgs(const bool value) { m_autoplayBgs = value; }

  const std::string& battleback1Name() const { return m_battleback1Name; }
  void setBattleback1Name(const std::string& value) { m_battleback1Name = value; }

  const std::string& battleback2Name() const { return m_battleback2Name; }
  void setBattleback2Name(const std::string& value) { m_battleback2Name = value; }

  Audio& bgm() { return m_bgm; }
  void setBgm(const Audio& bgm) { m_bgm = bgm; }

  Audio& bgs() { return m_bgs; }
  void setBgs(const Audio& bgs) { m_bgs = bgs; }

  bool disableDashing() const { return m_disableDashing; }
  void setDisableDashing(const bool value) { m_disableDashing = value; }

  const std::string& displayName() const { return m_displayName; }
  void setDisplayName(const std::string& value) { m_displayName = value; }

  std::vector<Encounter>& encounterList() { return m_encounterList; }
  void setEncounterList(const std::vector<Encounter>& value) { m_encounterList = value; }

  Encounter& encounter(const int id) {
    assert(id >= 0 && id < m_encounterList.size());
    return m_encounterList.at(id);
  }

  void setEncounter(const int id, const Encounter& encounter) {
    assert(id >= 0 && id < m_encounterList.size());
    m_encounterList.at(id) = encounter;
  }

  [[nodiscard]] int encounterStep() const { return m_encounterStep; }
  void setEncounterStep(const int value) { m_encounterStep = value; }

  const std::string& note() const { return m_note; }
  void setNote(const std::string& value) { m_note = value; }

  [[nodiscard]] bool parallaxLoopX() const { return m_parallaxLoopX; }
  void setParallaxLoopX(const bool value) { m_parallaxLoopX = value; }

  [[nodiscard]] bool parallaxLoopY() const { return m_parallaxLoopY; }
  void setParallaxLoopY(const bool value) { m_parallaxLoopY = value; }

  const std::string& parallaxName() const { return m_parallaxName; }
  void setParallaxName(const std::string& value) { m_parallaxName = value; }

  [[nodiscard]] bool parallaxShow() const { return m_parallaxShow; }
  void setParallaxShow(const bool value) { m_parallaxShow = value; }

  [[nodiscard]] int parallaxSx() const { return m_parallaxSx; }
  void setParallaxSx(const int value) { m_parallaxSx = value; }

  [[nodiscard]] int parallaxSy() const { return m_parallaxSy; }
  void setParallaxSy(const int value) { m_parallaxSy = value; }

  [[nodiscard]] ScrollType scrollType() const { return m_scrollType; }
  void setScrollType(const ScrollType value) { m_scrollType = value; }

  [[nodiscard]] bool specifyBattleback() const { return m_specifyBattleback; }
  void setSpecifyBattleback(const bool value) { m_specifyBattleback = value; }

  [[nodiscard]] int tilesetId() const { return m_tilesetId; }
  void setTilesetId(const int value) { m_tilesetId = value; }

  [[nodiscard]] int width() const { return m_width; }
  void setWidth(const int value) { m_width = value; }

  [[nodiscard]] int height() const { return m_height; }
  void setHeight(const int value) { m_height = value; }

  std::vector<std::optional<int>>& data() { return m_data; }
  const std::vector<std::optional<int>>& data() const { return m_data; }
  void setData(const std::vector<std::optional<int>>& value) { m_data = value; }

  std::vector<std::optional<Event>>& events() { return m_events; }
  const std::vector<std::optional<Event>>& events() const { return m_events; }
  void setEvents(const std::vector<std::optional<Event>>& value) { m_events = value; }

  [[nodiscard]] std::vector<Event*> getSorted();

  [[nodiscard]] std::vector<Event*> getRenderSorted();

  Event* event(int id);

  Event* eventAt(int x, int y);

  std::vector<Event*> eventsAt(const int x, const int y);

  std::vector<Event*> eventsAtNoThrough(int x, int y);
  std::vector<Event*> eventsAtRenderPosNoThrough(int x, int y);

  Event* createNewEvent();
  Event* createEventFromTemplate(const Event& ev);

  void deleteEvent(int id);

  int findOrMakeFreeId() const;

  void resize(int newWidth, int newHeight);

  bool isValid() const { return m_isValid; }
  void setValid(const bool valid) { m_isValid = valid; }

  bool isModified() {
    m_isDirty |= std::ranges::any_of(m_events, [](const auto& e) { return e && e->isModified(); });

    return m_isDirty;
  }

private:
  bool m_autoplayBgm{};
  bool m_autoplayBgs{};
  std::string m_battleback1Name;
  std::string m_battleback2Name;
  Audio m_bgm;
  Audio m_bgs;
  bool m_disableDashing{};
  std::string m_displayName;
  std::vector<Encounter> m_encounterList;
  int m_encounterStep{};
  std::string m_note;
  bool m_parallaxLoopX{};
  bool m_parallaxLoopY{};
  std::string m_parallaxName;
  bool m_parallaxShow{};
  int m_parallaxSx{};
  int m_parallaxSy{};
  ScrollType m_scrollType{};
  bool m_specifyBattleback{};
  int m_tilesetId{};
  int m_width{};
  int m_height{};

  std::vector<std::optional<int>> m_data;
  std::vector<std::optional<Event>> m_events;

  bool m_isDirty = false;
  bool m_isValid{false};
};

void to_json(nlohmann::ordered_json& json, const Map& map);
void from_json(const nlohmann::ordered_json& json, Map& map);
void to_json(nlohmann::ordered_json& json, const Map::Encounter& encounter);
void from_json(const nlohmann::ordered_json& json, Map::Encounter& encounter);