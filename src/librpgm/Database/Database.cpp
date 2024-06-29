#include "Database/Database.hpp"

Database* Database::Instance = nullptr;
Map Database::loadMap(int mapId) {
  std::string path = std::format("{}/data/Map{:03}.json", basePath, mapId);
  RPGM_DEBUG("Loading map {}", path);
  Map ret{};
#if !DEBUG
   try {
#endif
  ret = Map::load(path);
  RPGM_DEBUG("Map Loaded");
#if !DEBUG
  } catch (const std::exception& e) { RPGM_DEBUG("Map Failed To Load! Got exception {}", e.what()); }
#endif

  return ret;
}
