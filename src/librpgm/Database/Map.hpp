#pragma once

#include "Database/Audio.hpp"
#include "Database/Event.hpp"
#include "Database/EventCommands/EventDummy.hpp"
#include "Database/Globals.hpp"

#include <format>
#include <string>

#include "nlohmann/json.hpp"

/**
 * @class Map
 * A discrete map unit, contains all information for a given area such as events, tiles, and encounters
 */
class Map final : IModifiable {
  friend class MapSerializer;

public:
  friend void to_json(nlohmann::ordered_json& json, const Map& map);
  friend void from_json(const nlohmann::ordered_json& json, Map& map);
  /**
   * @class Encounter
   * Defines encounter probabilities for troops in a given set of regions
   * Regions are a group of tiles defined in the map data and applied to the map
   * by the map creator using the Region tileset (Tab "R")
   */
  class Encounter final : public IModifiable {
    friend void to_json(nlohmann::ordered_json& json, const Encounter& encounter);
    friend void from_json(const nlohmann::ordered_json& json, Encounter& encounter);

  public:
    Encounter() = default;
    Encounter(const Encounter& other);
    Encounter& operator=(const Encounter& other);
    Encounter(Encounter&& other) noexcept;
    Encounter& operator=(Encounter&& other) noexcept;

    int troopId() const;
    void setTroopId(int id);

    int weight() const;
    void setWeight(int weight);

    const std::vector<int>& regionSet();
    void setRegionSet(const std::vector<int>& regionSet);
    void setRegion(int idx, int region);

    rpgmutils::signal<void(Encounter*, const std::vector<int>&)>& regionSetModified();
    rpgmutils::signal<void(Encounter*, int, int)>& regionModified();
    rpgmutils::signal<void(Encounter*, int)>& troopIdModified();
    rpgmutils::signal<void(Encounter*, int)>& weightModified();

    void restoreOriginal() override;
    void acceptChanges() override;
    nlohmann::ordered_json serializeOldValues() const override;

    bool operator==(const Encounter& other) const { return m_regionSet == other.m_regionSet && m_troopId == other.m_troopId && m_weight == other.m_weight; }

  private:
    std::vector<int> m_regionSet{};
    int m_troopId{};
    int m_weight{};
    std::optional<std::vector<int>> m_oldregionSet;
    std::optional<int> m_oldtroopId;
    std::optional<int> m_oldweight;
    std::optional<rpgmutils::signal<void(Encounter*, const std::vector<int>&)>> m_regionSetModified;
    std::optional<rpgmutils::signal<void(Encounter*, int, int)>> m_regionModified;
    std::optional<rpgmutils::signal<void(Encounter*, int)>> m_troopIdModified;
    std::optional<rpgmutils::signal<void(Encounter*, int)>> m_weightModified;
  };
  Map() = default;
  Map(const Map& other);
  Map& operator=(const Map& other);
  Map(Map&& other) noexcept;
  Map& operator=(Map&& other) noexcept;

  /* Functions */
  /**
   * Specifies whether the Background Music defined in autoPlayBgm plays when the map is loaded.
   * @sa autoPlayBgm
   * @return True if autoplay is enabled False otherwise.
   */
  [[nodiscard]] bool autoplayBgm() const;
  /**
   * Sets whether the music defined in autoplayBgm plays on map load.
   * @param value True if the bgm should play automatically, False otherwise
   */
  void setAutoplayBgm(const bool value);

  [[nodiscard]] bool autoplayBgs() const;
  void setAutoplayBgs(const bool value);

  const std::string& battleback1Name() const;
  void setBattleback1Name(const std::string& value);

  const std::string& battleback2Name() const;
  void setBattleback2Name(const std::string& value);

  Audio& bgm();
  void setBgm(const Audio& bgm);

  Audio& bgs();
  void setBgs(const Audio& bgs);

  bool disableDashing() const;
  void setDisableDashing(bool value);

  const std::string& displayName() const;
  void setDisplayName(const std::string& value);

  std::vector<Encounter>& encounterList();
  void setEncounterList(const std::vector<Encounter>& value);

  Encounter& encounter(int id);

  void setEncounter(int id, const Encounter& encounter);

  [[nodiscard]] int encounterStep() const;
  void setEncounterStep(int value);

  const std::string& note() const;
  void setNote(const std::string& value);

  [[nodiscard]] bool parallaxLoopX() const;
  void setParallaxLoopX(bool value);

  [[nodiscard]] bool parallaxLoopY() const;
  void setParallaxLoopY(bool value);

  const std::string& parallaxName() const;
  void setParallaxName(const std::string& value);

  [[nodiscard]] bool parallaxShow() const;
  void setParallaxShow(bool value);

  [[nodiscard]] int parallaxSx() const;
  void setParallaxSx(int value);

  [[nodiscard]] int parallaxSy() const;
  void setParallaxSy(int value);

  [[nodiscard]] ScrollType scrollType() const;
  void setScrollType(ScrollType value);

  [[nodiscard]] bool specifyBattleback() const;
  void setSpecifyBattleback(bool value);

  [[nodiscard]] int tilesetId() const;
  void setTilesetId(int value);

  [[nodiscard]] int width() const;
  void setWidth(int value);

  [[nodiscard]] int height() const;
  void setHeight(int value);

  const std::vector<std::optional<int>>& data() const;
  void setData(const std::vector<std::optional<int>>& value);
  void setTileAt(int id, int x, int y, int layer);

  const std::vector<std::optional<Event>>& events() const;
  void setEvents(const std::vector<std::optional<Event>>& value);

  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;
  bool isModified() const override {
    bool isModified = IModifiable::isModified();
    isModified |= std::ranges::any_of(m_events, [](const std::optional<Event>& value) { return value && value->isModified(); });
    isModified |= std::ranges::any_of(m_encounterList, [](const Encounter& value) { return value.isModified(); });
    return isModified;
  }

  rpgmutils::signal<void(Map*, bool)>& autoplayBgmModified();
  rpgmutils::signal<void(Map*, bool)>& autoplayBgsModified();
  rpgmutils::signal<void(Map*, const std::string&)>& battleback1NameModified();
  rpgmutils::signal<void(Map*, const std::string&)>& battleback2NameModified();
  rpgmutils::signal<void(Map*, Audio)>& bgmModified();
  rpgmutils::signal<void(Map*, Audio)>& bgsModified();
  rpgmutils::signal<void(Map*, bool)>& disableDashingModified();
  rpgmutils::signal<void(Map*, const std::string&)>& displayNameModified();
  rpgmutils::signal<void(Map*, const std::vector<Encounter>&)>& encounterListModified();
  rpgmutils::signal<void(Map*, int)>& encounterStepModified();
  rpgmutils::signal<void(Map*, const std::string&)>& noteModified();
  rpgmutils::signal<void(Map*, bool)>& parallaxLoopXModified();
  rpgmutils::signal<void(Map*, bool)>& parallaxLoopYModified();
  rpgmutils::signal<void(Map*, const std::string&)>& parallaxNameModified();
  rpgmutils::signal<void(Map*, bool)>& parallaxShowModified();
  rpgmutils::signal<void(Map*, int)>& parallaxSxModified();
  rpgmutils::signal<void(Map*, int)>& parallaxSyModified();
  rpgmutils::signal<void(Map*, ScrollType)>& scrollTypeModified();
  rpgmutils::signal<void(Map*, bool)>& specifyBattlebackModified();
  rpgmutils::signal<void(Map*, int)>& tilesetIdModified();
  rpgmutils::signal<void(Map*, int)>& widthModified();
  rpgmutils::signal<void(Map*, int)>& heightModified();
  rpgmutils::signal<void(Map*, const std::vector<std::optional<int>>&)>& dataModified();
  rpgmutils::signal<void(Map*, const std::vector<std::optional<Event>>&)>& eventsModified();

  /* Utility functions */
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

  std::optional<bool> m_oldautoplayBgm;
  std::optional<bool> m_oldautoplayBgs;
  std::optional<std::string> m_oldbattleback1Name;
  std::optional<std::string> m_oldbattleback2Name;
  std::optional<Audio> m_oldbgm;
  std::optional<Audio> m_oldbgs;
  std::optional<bool> m_olddisableDashing;
  std::optional<std::string> m_olddisplayName;
  std::optional<std::vector<Encounter>> m_oldencounterList;
  std::optional<int> m_oldencounterStep;
  std::optional<std::string> m_oldnote;
  std::optional<bool> m_oldparallaxLoopX;
  std::optional<bool> m_oldparallaxLoopY;
  std::optional<std::string> m_oldparallaxName;
  std::optional<bool> m_oldparallaxShow;
  std::optional<int> m_oldparallaxSx;
  std::optional<int> m_oldparallaxSy;
  std::optional<ScrollType> m_oldscrollType;
  std::optional<bool> m_oldspecifyBattleback;
  std::optional<int> m_oldtilesetId;
  std::optional<int> m_oldwidth;
  std::optional<int> m_oldheight;
  std::optional<std::vector<std::optional<int>>> m_olddata;
  std::optional<std::vector<std::optional<Event>>> m_oldevents;

  std::optional<rpgmutils::signal<void(Map*, bool)>> m_autoplayBgmModified;
  std::optional<rpgmutils::signal<void(Map*, bool)>> m_autoplayBgsModified;
  std::optional<rpgmutils::signal<void(Map*, const std::string&)>> m_battleback1NameModified;
  std::optional<rpgmutils::signal<void(Map*, const std::string&)>> m_battleback2NameModified;
  std::optional<rpgmutils::signal<void(Map*, Audio)>> m_bgmModified;
  std::optional<rpgmutils::signal<void(Map*, Audio)>> m_bgsModified;
  std::optional<rpgmutils::signal<void(Map*, bool)>> m_disableDashingModified;
  std::optional<rpgmutils::signal<void(Map*, const std::string&)>> m_displayNameModified;
  std::optional<rpgmutils::signal<void(Map*, const std::vector<Encounter>&)>> m_encounterListModified;
  std::optional<rpgmutils::signal<void(Map*, int)>> m_encounterStepModified;
  std::optional<rpgmutils::signal<void(Map*, const std::string&)>> m_noteModified;
  std::optional<rpgmutils::signal<void(Map*, bool)>> m_parallaxLoopXModified;
  std::optional<rpgmutils::signal<void(Map*, bool)>> m_parallaxLoopYModified;
  std::optional<rpgmutils::signal<void(Map*, const std::string&)>> m_parallaxNameModified;
  std::optional<rpgmutils::signal<void(Map*, bool)>> m_parallaxShowModified;
  std::optional<rpgmutils::signal<void(Map*, int)>> m_parallaxSxModified;
  std::optional<rpgmutils::signal<void(Map*, int)>> m_parallaxSyModified;
  std::optional<rpgmutils::signal<void(Map*, ScrollType)>> m_scrollTypeModified;
  std::optional<rpgmutils::signal<void(Map*, bool)>> m_specifyBattlebackModified;
  std::optional<rpgmutils::signal<void(Map*, int)>> m_tilesetIdModified;
  std::optional<rpgmutils::signal<void(Map*, int)>> m_widthModified;
  std::optional<rpgmutils::signal<void(Map*, int)>> m_heightModified;
  std::optional<rpgmutils::signal<void(Map*, const std::vector<std::optional<int>>&)>> m_dataModified;
  std::optional<rpgmutils::signal<void(Map*, const std::vector<std::optional<Event>>&)>> m_eventsModified;

  bool m_isDirty = false;
  bool m_isValid{false};
};

void to_json(nlohmann::ordered_json& json, const Map& map);
void from_json(const nlohmann::ordered_json& json, Map& map);
void to_json(nlohmann::ordered_json& json, const Map::Encounter& encounter);
void from_json(const nlohmann::ordered_json& json, Map::Encounter& encounter);