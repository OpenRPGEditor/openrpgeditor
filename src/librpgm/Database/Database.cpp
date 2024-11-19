#include "Database/Database.hpp"

#include "Database/Serializable/DeserializationQueue.hpp"
#include "Database/Serializable/SerializationQueue.hpp"

#include "Database/Serializable/SystemSerializer.hpp"
#include "Database/Serializable/ActorsSerializer.hpp"
#include "Database/Serializable/ClassesSerializer.hpp"
#include "Database/Serializable/ItemsSerializer.hpp"
#include "Database/Serializable/SkillsSerializer.hpp"
#include "Database/Serializable/WeaponsSerializer.hpp"
#include "Database/Serializable/ArmorsSerializer.hpp"
#include "Database/Serializable/EnemiesSerializer.hpp"
#include "Database/Serializable/TroopsSerializer.hpp"
#include "Database/Serializable/StatesSerializer.hpp"
#include "Database/Serializable/AnimationsSerializer.hpp"
#include "Database/Serializable/TilesetsSerializer.hpp"
#include "Database/Serializable/CommonEventsSerializer.hpp"

Database* Database::m_instance = nullptr;

Database::Database(const std::string_view _projectBasePath, const std::string_view _projectFilePath,
                   const std::string_view _projectVersion) {
  m_instance = this;
  projectVersion = _projectVersion;
  basePath = _projectBasePath;
  projectFilePath = _projectFilePath;
}

void Database::load() {
  RPGM_INFO("Queue System definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<SystemSerializer>("data/System.json"),
                                           [this](const std::shared_ptr<ISerializable>& serializer) {
                                             system = std::dynamic_pointer_cast<SystemSerializer>(serializer)->data();
                                             RPGM_INFO("System definitions loaded");
                                           });
  RPGM_INFO("Queue Actor definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<ActorsSerializer>("data/Actors.json"),
                                           [this](const std::shared_ptr<ISerializable>& serializer) {
                                             actors = std::dynamic_pointer_cast<ActorsSerializer>(serializer)->data();
                                             RPGM_INFO("Actor definitions loaded");
                                           });
  RPGM_INFO("Queue Class definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<ClassesSerializer>("data/Classes.json"),
                                           [this](const std::shared_ptr<ISerializable>& serializer) {
                                             classes = std::dynamic_pointer_cast<ClassesSerializer>(serializer)->data();
                                             RPGM_INFO("Class definitions loaded");
                                           });
  RPGM_INFO("Queue Skill definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<SkillsSerializer>("data/Skills.json"),
                                           [this](const std::shared_ptr<ISerializable>& serializer) {
                                             skills = std::dynamic_pointer_cast<SkillsSerializer>(serializer)->data();
                                             RPGM_INFO("Skill definitions loaded");
                                           });
  RPGM_INFO("Queue Item definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<ItemsSerializer>("data/Items.json"),
                                           [this](const std::shared_ptr<ISerializable>& serializer) {
                                             items = std::dynamic_pointer_cast<ItemsSerializer>(serializer)->data();
                                             RPGM_INFO("Item definitions loaded");
                                           });
  RPGM_INFO("Queue Weapon definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<WeaponsSerializer>("data/Weapons.json"),
                                           [this](const std::shared_ptr<ISerializable>& serializer) {
                                             weapons = std::dynamic_pointer_cast<WeaponsSerializer>(serializer)->data();
                                             RPGM_INFO("Weapon definitions loaded");
                                           });
  RPGM_INFO("Queue Armor definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<ArmorsSerializer>("data/Armors.json"),
                                           [this](const std::shared_ptr<ISerializable>& serializer) {
                                             armors = std::dynamic_pointer_cast<ArmorsSerializer>(serializer)->data();
                                             RPGM_INFO("Armor definitions loaded");
                                           });
  RPGM_INFO("Queue Enemy definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<EnemiesSerializer>("data/Enemies.json"),
                                           [this](const std::shared_ptr<ISerializable>& serializer) {
                                             enemies = std::dynamic_pointer_cast<EnemiesSerializer>(serializer)->data();
                                             RPGM_INFO("Enemy definitions loaded");
                                           });
  RPGM_INFO("Queue Troop definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<TroopsSerializer>("data/Troops.json"),
                                           [this](const std::shared_ptr<ISerializable>& serializer) {
                                             troops = std::dynamic_pointer_cast<TroopsSerializer>(serializer)->data();
                                             RPGM_INFO("Troop definitions loaded");
                                           });
  RPGM_INFO("Queue State definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<StatesSerializer>("data/States.json"),
                                           [this](const std::shared_ptr<ISerializable>& serializer) {
                                             states = std::dynamic_pointer_cast<StatesSerializer>(serializer)->data();
                                             RPGM_INFO("State definitions loaded");
                                           });
  RPGM_INFO("Queue Animation definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<AnimationsSerializer>("data/Animations.json"),
                                           [this](const std::shared_ptr<ISerializable>& serializer) {
                                             animations =
                                                 std::dynamic_pointer_cast<AnimationsSerializer>(serializer)->data();
                                             RPGM_INFO("Animation definitions loaded");
                                           });
  RPGM_INFO("Queue Tileset definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<TilesetsSerializer>("data/Tilesets.json"),
                                           [this](const std::shared_ptr<ISerializable>& serializer) {
                                             tilesets =
                                                 std::dynamic_pointer_cast<TilesetsSerializer>(serializer)->data();
                                             RPGM_INFO("Tileset definitions loaded");
                                           });
  RPGM_INFO("Queue CommonEvent definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<CommonEventsSerializer>("data/CommonEvents.json"),
                                           [this](const std::shared_ptr<ISerializable>& serializer) {
                                             commonEvents =
                                                 std::dynamic_pointer_cast<CommonEventsSerializer>(serializer)->data();
                                             RPGM_INFO("CommonEvent definitions loaded");
                                           });
  mapInfos = MapInfos::load(basePath + "/data/MapInfos.json");
  mapInfos.loadAllMaps();

  RPGM_INFO("Loading Plugins...");
  plugins = Plugins::load(basePath + "js/plugins.js");
  RPGM_INFO("Loading GameConstants");
  gameConstants = GameConstants::load(basePath + "/data/Constants.json");
  RPGM_INFO("Loading Templates");
  templates = Templates::load(basePath + "/data/Templates.json");
  RPGM_INFO(std::to_string(templates.commands.size()) + " size of commands");
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
