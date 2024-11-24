#include "Database/Map.hpp"
#include "Database/Event.hpp"
#include <fstream>

void to_json(nlohmann::ordered_json& json, const Map& map) {
  json = {
      {"autoplayBgm", map.autoplayBgm},
      {"autoplayBgs", map.autoplayBgs},
      {"battleback1Name", map.battleback1Name},
      {"battleback2Name", map.battleback2Name},
      {"bgm", map.bgm},
      {"bgs", map.bgs},
      {"disableDashing", map.disableDashing},
      {"displayName", map.displayName},
      {"encounterList", map.encounterList},
      {"encounterStep", map.encounterStep},
      {"height", map.height},
      {"note", map.note},
      {"parallaxLoopX", map.parallaxLoopX},
      {"parallaxLoopY", map.parallaxLoopY},
      {"parallaxName", map.parallaxName},
      {"parallaxShow", map.parallaxShow},
      {"parallaxSx", map.parallaxSx},
      {"parallaxSy", map.parallaxSy},
      {"scrollType", map.scrollType},
      {"specifyBattleback", map.specifyBattleback},
      {"tilesetId", map.tilesetId},
      {"width", map.width},
      {"data", map.data},
      {"events", map.events},
  };
}
void from_json(const nlohmann::ordered_json& json, Map& map) {
  map.autoplayBgm = json.value("autoplayBgm", map.autoplayBgm);
  map.autoplayBgs = json.value("autoplayBgs", map.autoplayBgs);
  map.battleback1Name = json.value("battleback1Name", map.battleback1Name);
  map.battleback2Name = json.value("battleback2Name", map.battleback2Name);
  map.bgm = json.value("bgm", map.bgm);
  map.bgs = json.value("bgs", map.bgs);
  map.disableDashing = json.value("disableDashing", map.disableDashing);
  map.displayName = json.value("displayName", map.displayName);
  map.encounterList = json.value("encounterList", map.encounterList);
  map.encounterStep = json.value("encounterStep", map.encounterStep);
  map.height = json.value("height", map.height);
  map.note = json.value("note", map.note);
  map.parallaxLoopX = json.value("parallaxLoopX", map.parallaxLoopX);
  map.parallaxLoopY = json.value("parallaxLoopY", map.parallaxLoopY);
  map.parallaxName = json.value("parallaxName", map.parallaxName);
  map.parallaxShow = json.value("parallaxShow", map.parallaxShow);
  map.parallaxSx = json.value("parallaxSx", map.parallaxSx);
  map.parallaxSy = json.value("parallaxSy", map.parallaxSy);
  map.scrollType = json.value("scrollType", map.scrollType);
  map.specifyBattleback = json.value("specifyBattleback", map.specifyBattleback);
  map.tilesetId = json.value("tilesetId", map.tilesetId);
  map.width = json.value("width", map.width);
  map.data = json.value("data", map.data);
  map.events = json.value("events", map.events);
}

void to_json(nlohmann::ordered_json& json, const Map::Encounter& encounter) {
  json = {
      {"regionSet", encounter.regionSet},
      {"troopId", encounter.troopId},
      {"weight", encounter.weight},
  };
}
void from_json(const nlohmann::ordered_json& json, Map::Encounter& encounter) {
  encounter.regionSet = json.value("regionSet", encounter.regionSet);
  encounter.troopId = json.value("troopId", encounter.troopId);
  encounter.weight = json.value("weight", encounter.weight);
}

void Map::resize(const int newWidth, const int newHeight) {
  static constexpr int MaxLayers = 6;
  std::vector<std::optional<int>> newData;
  newData.resize(MaxLayers * newWidth * newHeight);
  std::ranges::fill(newData.begin(), newData.end(), 0);

  for (int y = 0; y < newHeight; ++y) {
    for (int x = 0; x < newWidth; ++x) {
      if (x < width && y < height) {
        for (int z = 0; z < MaxLayers; ++z) {
          const int si = (z * height + y) * width + x;
          const int di = (z * newHeight + y) * newWidth + x;
          newData[di] = data[si];
        }
      }
    }
  }

  std::ranges::for_each(events, [&](auto& event) {
    if (event->x >= newWidth || event->y >= newHeight) {
      event.reset();
    }
  });

  width = newWidth;
  height = newHeight;
  data = std::move(newData);
}

Map Map::load(std::string_view filepath) {
  std::ifstream file(filepath.data());
  if (file.is_open()) {
    Map ret = nlohmann::ordered_json::parse(file).get<Map>();
    ret.m_isValid = true;
    return ret;
  }
  return Map();
}

bool Map::serialize(std::string_view filepath) const {
  std::ofstream file(filepath.data());

  if (file.is_open()) {
    nlohmann::ordered_json data = *this;
    file << data.dump(4);
    return true;
  }

  return false;
}