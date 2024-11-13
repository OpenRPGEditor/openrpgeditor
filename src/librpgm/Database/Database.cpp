#include "Database/Database.hpp"

Database* Database::Instance = nullptr;
Map Database::loadMap(int mapId) {
  std::string path = std::format("{}data/Map{:03}.json", basePath, mapId);
  RPGM_DEBUG("Loading map {}", path);
  Map ret{};
#ifndef DEBUG
  try {
#endif
    ret = Map::load(path);
    RPGM_DEBUG("Map Loaded");
#ifndef DEBUG
  } catch (const std::exception& e) { RPGM_DEBUG("Map Failed To Load! Got exception {}", e.what()); }
#endif

  return ret;
}

bool Database::serializeProject(std::string_view tmpPath) {
  // TODO: Proper atomic implementation with fallback
  (void)tmpPath;

  /* TODO: For now we're going to just flat serialize everything without care for data integrity
   *  for now this is ok, but this *must* be changed before a non-alpha release.
   *  This should also be threaded so we can monitor save progress and inform the user, we also need
   *  modification status so we only save files that have been modified at any point.
   *  At some point we'll also need to implement local file monitoring so we can reload files that get modified
   *  externally, the details of that need to be worked out so the editor doesn't DDoS itself
   */

  /* TODO: try/catch */
  {
    std::string projectBasePath = basePath;
    actors.serialize(projectBasePath + "/data/Actors.json");
    classes.serialize(projectBasePath + "/data/Classes.json");
    skills.serialize(projectBasePath + "/data/Skills.json");
    items.serialize(projectBasePath + "/data/Items.json");
    weapons.serialize(projectBasePath + "/data/Weapons.json");
    armors.serialize(projectBasePath + "/data/Armors.json");
    enemies.serialize(projectBasePath + "/data/Enemies.json");
    troops.serialize(projectBasePath + "/data/Troops.json");
    states.serialize(projectBasePath + "/data/States.json");
    animations.serialize(projectBasePath + "/data/Animations.json");
    tilesets.serialize(projectBasePath + "/data/Tilesets.json");
    commonEvents.serialize(projectBasePath + "/data/CommonEvents.json");
    system.serialize(projectBasePath + "/data/System.json");
    /* TODO: Implement */
    plugins.serialize(projectBasePath + "/js/plugins.js");
    mapInfos.serialize(projectBasePath + "/data/MapInfos.json");
    gameConstants.serialize(projectBasePath + "/data/Constants.json");

    for (const auto& map : mapInfos.mapInfos()) {
      if (map->map()) {
        map->map()->serialize(std::format("{}data/Map{:03}.json", projectBasePath, map->id));
      }
    }
  }

  return true;
}
