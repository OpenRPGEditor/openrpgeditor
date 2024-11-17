#include "Database/Database.hpp"

#include "Serializable/ActorsSerializer.hpp"
#include "Serializable/ClassesSerializer.hpp"
#include "Serializable/DeserializationQueue.hpp"

Database* Database::m_instance = nullptr;

Database::Database(const std::string_view _projectBasePath, const std::string_view _projectFilePath,
                   const std::string_view _projectVersion) {
  m_instance = this;
  projectVersion = _projectVersion;
  basePath = _projectBasePath;
  projectFilePath = _projectFilePath;
  RPGM_INFO("Queue Actor definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<ActorsSerializer>("data/Actors.json"),
                                           [this](const std::shared_ptr<ISerializable>& serializer) {
                                             actors = *std::dynamic_pointer_cast<ActorsSerializer>(serializer)->data();
                                           });
  RPGM_INFO("Queue Class definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<ClassesSerializer>("data/Classes.json"),
                                           [this](const std::shared_ptr<ISerializable>& serializer) {
                                             classes =
                                                 *std::dynamic_pointer_cast<ClassesSerializer>(serializer)->data();
                                           });
  RPGM_INFO("Loading Skill definitions...");
  skills = Skills::load(basePath + "/data/Skills.json");
  RPGM_INFO("Loading Item definitions...");
  items = Items::load(basePath + "/data/Items.json");
  RPGM_INFO("Loading Weapon definitions...");
  weapons = Weapons::load(basePath + "/data/Weapons.json");
  RPGM_INFO("Loading Armor definitions...");
  armors = Armors::load(basePath + "/data/Armors.json");
  RPGM_INFO("Loading Enemy definitions...");
  enemies = Enemies::load(basePath + "/data/Enemies.json");
  RPGM_INFO("Loading Troop definitions...");
  troops = Troops::load(basePath + "/data/Troops.json");
  RPGM_INFO("Loading State definitions...");
  states = States::load(basePath + "/data/States.json");
  RPGM_INFO("Loading Animation definitions...");
  animations = Animations::load(basePath + "/data/Animations.json");
  RPGM_INFO("Loading Tileset definitions...");
  tilesets = Tilesets::load(basePath + "/data/Tilesets.json");
  RPGM_INFO("Loading CommonEvent definitions...");
  commonEvents = CommonEvents::load(basePath + "/data/CommonEvents.json");
  RPGM_INFO("Loading System...");
  system = System::load(basePath + "/data/System.json");
  RPGM_INFO("Loading Plugins...");
  plugins = Plugins::load(basePath + "js/plugins.js");
  RPGM_INFO("Loading GameConstants");
  gameConstants = GameConstants::load(basePath + "/data/Constants.json");
  RPGM_INFO("Loading Templates");
  templates = Templates::load(basePath + "/data/Templates.json");
  RPGM_INFO(std::to_string(templates.commands.size()) + " size of commands");
  mapInfos = MapInfos::load(basePath + "/data/MapInfos.json");
}

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
