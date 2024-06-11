#pragma once

#include "Database/Globals.hpp"
#include "Database/Audio.hpp"
#include "Database/Event.hpp"

#include <string>
#include "nlohmann/json.hpp"

class Map {
public:
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Map, autoPlayBgm, autoPlayBgs, battleBack1Name, battleBack2Name, bgm, bgs,
                                 disableDashing, displayName, encounters, encounterStep, note, parallaxLoopX,
                                 parallaxLoopY, parallaxName, parallaxShow, parallaxSx, parallaxSy, scrollType,
                                 specifyBattleBack, tilesetId, width, height, data, events)
  struct Encounter {
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Encounter, regionSet, troopId, weight);
    std::array<int, 3> regionSet{};
    int troopId{};
    int weight{};
  };
  bool autoPlayBgm{};
  bool autoPlayBgs{};
  std::string battleBack1Name;
  std::string battleBack2Name;
  Audio bgm;
  Audio bgs;
  bool disableDashing{};
  std::string displayName;
  std::vector<Encounter> encounters;
  int encounterStep{};
  std::string note;
  bool parallaxLoopX{};
  bool parallaxLoopY{};
  std::string parallaxName;
  bool parallaxShow{};
  int parallaxSx{};
  int parallaxSy{};
  ScrollType scrollType{};
  bool specifyBattleBack{};
  int tilesetId{};
  int width{};
  int height{};
  std::vector<int> data;
  std::vector<std::optional<Event>> events;

  static Map load(std::string_view filepath);
  bool serialize(std::string_view filepath);

  [[nodiscard]] std::vector<std::optional<Event>> getSortedBy() const {
    std::vector<std::optional<Event>> ret = events;
    std::sort(ret.begin(), ret.end(), [](const std::optional<Event>& a, const std::optional<Event>& b) {
      if (!a || !b) {
        return false;
      }
      if (a->y != b->y) {
        return a->y < b->y;
      }
      return a->id < b->id;
    });
    return std::move(ret);
  }

  Event* event(int id) {
    auto it = std::find_if(events.begin(), events.end(), [&id](const auto& ev) {
      return ev && ev->id == id;
    });

    if (it != events.end()) {
      return &it->value();
    }
    return nullptr;
  }

private:
};
