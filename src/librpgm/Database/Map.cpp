#include "Database/Map.hpp"
#include <fstream>

void to_json(nlohmann::json& json, const Map& map) {
  json = {
      {"autoPlayBgm", map.autoPlayBgm},
      {"autoPlayBgs", map.autoPlayBgs},
      {"battleBack1Name", map.battleBack1Name},
      {"battleBack2Name", map.battleBack2Name},
      {"bgm", map.bgm},
      {"bgs", map.bgs},
      {"disableDashing", map.disableDashing},
      {"displayName", map.displayName},
      {"encounters", map.encounters},
      {"encounterStep", map.encounterStep},
      {"note", map.note},
      {"parallaxLoopX", map.parallaxLoopX},
      {"parallaxLoopY", map.parallaxLoopY},
      {"parallaxName", map.parallaxName},
      {"parallaxShow", map.parallaxShow},
      {"parallaxSx", map.parallaxSx},
      {"parallaxSy", map.parallaxSy},
      {"scrollType", map.scrollType},
      {"specifyBattleBack", map.specifyBattleBack},
      {"tilesetId", map.tilesetId},
      {"width", map.width},
      {"height", map.height},
      {"data", map.data},
      {"events", map.events},
  };
}
void from_json(const nlohmann::json& json, Map& map) {
  map.autoPlayBgm = json.value("autoPlayBgm", map.autoPlayBgm);
  map.autoPlayBgs = json.value("autoPlayBgs", map.autoPlayBgs);
  map.battleBack1Name = json.value("battleBack1Name", map.battleBack1Name);
  map.battleBack2Name = json.value("battleBack2Name", map.battleBack2Name);
  map.bgm = json.value("bgm", map.bgm);
  map.bgs = json.value("bgs", map.bgs);
  map.disableDashing = json.value("disableDashing", map.disableDashing);
  map.displayName = json.value("displayName", map.displayName);
  map.encounters = json.value("encounters", map.encounters);
  map.encounterStep = json.value("encounterStep", map.encounterStep);
  map.note = json.value("note", map.note);
  map.parallaxLoopX = json.value("parallaxLoopX", map.parallaxLoopX);
  map.parallaxLoopY = json.value("parallaxLoopY", map.parallaxLoopY);
  map.parallaxName = json.value("parallaxName", map.parallaxName);
  map.parallaxShow = json.value("parallaxShow", map.parallaxShow);
  map.parallaxSx = json.value("parallaxSx", map.parallaxSx);
  map.parallaxSy = json.value("parallaxSy", map.parallaxSy);
  map.scrollType = json.value("scrollType", map.scrollType);
  map.specifyBattleBack = json.value("specifyBattleBack", map.specifyBattleBack);
  map.tilesetId = json.value("tilesetId", map.tilesetId);
  map.width = json.value("width", map.width);
  map.height = json.value("height", map.height);
  map.data = json.value("data", map.data);
  map.events = json.value("events", map.events);
}

void to_json(nlohmann::json& json, const Map::Encounter& encounter) {
  json = {
      {"regionSet", encounter.regionSet},
      {"troopId", encounter.troopId},
      {"weight", encounter.weight},
  };
}
void from_json(const nlohmann::json& json, Map::Encounter& encounter) {
  encounter.regionSet = json.value("regionSet", encounter.regionSet);
  encounter.troopId = json.value("troopId", encounter.troopId);
  encounter.weight = json.value("weight", encounter.weight);
}

Map Map::load(std::string_view filepath) {
  std::ifstream file(filepath.data());
  if (file.is_open()) {
    Map ret = nlohmann::json::parse(file).get<Map>();
    ret.m_isValid = true;
    return ret;
  }
  return Map();
}

bool Map::serialize(std::string_view filepath) const {
  std::ofstream file(filepath.data());

  if (file.is_open()) {
    nlohmann::json data = *this;
    file << data.dump(4);
    return true;
  }

  return false;
}
