#include "Database/Database.hpp"

#include "Database/Serializable/DeserializationQueue.hpp"
#include "Database/Serializable/SerializationQueue.hpp"

#include "Database/Serializable/ActorsSerializer.hpp"
#include "Database/Serializable/AnimationsSerializer.hpp"
#include "Database/Serializable/ArmorsSerializer.hpp"
#include "Database/Serializable/ClassesSerializer.hpp"
#include "Database/Serializable/CommonEventsSerializer.hpp"
#include "Database/Serializable/EnemiesSerializer.hpp"
#include "Database/Serializable/ItemsSerializer.hpp"
#include "Database/Serializable/SkillsSerializer.hpp"
#include "Database/Serializable/StatesSerializer.hpp"
#include "Database/Serializable/SystemSerializer.hpp"
#include "Database/Serializable/TilesetsSerializer.hpp"
#include "Database/Serializable/TroopsSerializer.hpp"
#include "Database/Serializable/WeaponsSerializer.hpp"
#include "Serializable/MapSerializer.hpp"

Database* Database::m_instance = nullptr;

Database::Database(const std::string_view _projectBasePath, const std::string_view _projectFilePath, const std::string_view _projectVersion) {
  m_instance = this;
  projectVersion = _projectVersion;
  basePath = _projectBasePath;
  projectFilePath = _projectFilePath;
}

void Database::load() {
  RPGM_INFO("Queue System definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<SystemSerializer>("data/System.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    system = std::dynamic_pointer_cast<SystemSerializer>(serializer)->data();
    RPGM_INFO("System definitions loaded");
  });
  RPGM_INFO("Queue Actor definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<ActorsSerializer>("data/Actors.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    actors = std::dynamic_pointer_cast<ActorsSerializer>(serializer)->data();
    RPGM_INFO("Actor definitions loaded");
  });
  RPGM_INFO("Queue Class definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<ClassesSerializer>("data/Classes.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    classes = std::dynamic_pointer_cast<ClassesSerializer>(serializer)->data();
    RPGM_INFO("Class definitions loaded");
  });
  RPGM_INFO("Queue Skill definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<SkillsSerializer>("data/Skills.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    skills = std::dynamic_pointer_cast<SkillsSerializer>(serializer)->data();
    RPGM_INFO("Skill definitions loaded");
  });
  RPGM_INFO("Queue Item definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<ItemsSerializer>("data/Items.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    items = std::dynamic_pointer_cast<ItemsSerializer>(serializer)->data();
    RPGM_INFO("Item definitions loaded");
  });
  RPGM_INFO("Queue Weapon definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<WeaponsSerializer>("data/Weapons.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    weapons = std::dynamic_pointer_cast<WeaponsSerializer>(serializer)->data();
    RPGM_INFO("Weapon definitions loaded");
  });
  RPGM_INFO("Queue Armor definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<ArmorsSerializer>("data/Armors.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    armors = std::dynamic_pointer_cast<ArmorsSerializer>(serializer)->data();
    RPGM_INFO("Armor definitions loaded");
  });
  RPGM_INFO("Queue Enemy definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<EnemiesSerializer>("data/Enemies.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    enemies = std::dynamic_pointer_cast<EnemiesSerializer>(serializer)->data();
    RPGM_INFO("Enemy definitions loaded");
  });
  RPGM_INFO("Queue Troop definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<TroopsSerializer>("data/Troops.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    troops = std::dynamic_pointer_cast<TroopsSerializer>(serializer)->data();
    RPGM_INFO("Troop definitions loaded");
  });
  RPGM_INFO("Queue State definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<StatesSerializer>("data/States.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    states = std::dynamic_pointer_cast<StatesSerializer>(serializer)->data();
    RPGM_INFO("State definitions loaded");
  });
  RPGM_INFO("Queue Animation definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<AnimationsSerializer>("data/Animations.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    animations = std::dynamic_pointer_cast<AnimationsSerializer>(serializer)->data();
    RPGM_INFO("Animation definitions loaded");
  });
  RPGM_INFO("Queue Tileset definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<TilesetsSerializer>("data/Tilesets.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    tilesets = std::dynamic_pointer_cast<TilesetsSerializer>(serializer)->data();
    RPGM_INFO("Tileset definitions loaded");
  });
  RPGM_INFO("Queue CommonEvent definitions for load...");
  DeserializationQueue::instance().enqueue(std::make_shared<CommonEventsSerializer>("data/CommonEvents.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    commonEvents = std::dynamic_pointer_cast<CommonEventsSerializer>(serializer)->data();
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

void Database::serializeProject() {
  SerializationQueue::instance().enqueue(std::make_shared<CommonEventsSerializer>(commonEvents, "data/CommonEvents.json"), [this](const std::shared_ptr<ISerializable>& serializer) {});
  SerializationQueue::instance().enqueue(std::make_shared<SystemSerializer>(system, "data/System.json"), [this](const std::shared_ptr<ISerializable>& serializer) {});
  SerializationQueue::instance().enqueue(std::make_shared<ActorsSerializer>(actors, "data/Actors.json"), [this](const std::shared_ptr<ISerializable>& serializer) {});
  SerializationQueue::instance().enqueue(std::make_shared<ClassesSerializer>(classes, "data/Classes.json"), [this](const std::shared_ptr<ISerializable>& serializer) {});
  SerializationQueue::instance().enqueue(std::make_shared<SkillsSerializer>(skills, "data/Skills.json"), [this](const std::shared_ptr<ISerializable>& serializer) {});
  SerializationQueue::instance().enqueue(std::make_shared<ItemsSerializer>(items, "data/Items.json"), [this](const std::shared_ptr<ISerializable>& serializer) {});
  SerializationQueue::instance().enqueue(std::make_shared<WeaponsSerializer>(weapons, "data/Weapons.json"), [this](const std::shared_ptr<ISerializable>& serializer) {});
  SerializationQueue::instance().enqueue(std::make_shared<ArmorsSerializer>(armors, "data/Armors.json"), [this](const std::shared_ptr<ISerializable>& serializer) {});
  SerializationQueue::instance().enqueue(std::make_shared<EnemiesSerializer>(enemies, "data/Enemies.json"), [this](const std::shared_ptr<ISerializable>& serializer) {});
  SerializationQueue::instance().enqueue(std::make_shared<TroopsSerializer>(troops, "data/Troops.json"), [this](const std::shared_ptr<ISerializable>& serializer) {});
  SerializationQueue::instance().enqueue(std::make_shared<StatesSerializer>(states, "data/States.json"), [this](const std::shared_ptr<ISerializable>& serializer) {});
  SerializationQueue::instance().enqueue(std::make_shared<AnimationsSerializer>(animations, "data/Animations.json"), [this](const std::shared_ptr<ISerializable>& serializer) {});
  SerializationQueue::instance().enqueue(std::make_shared<TilesetsSerializer>(tilesets, "data/Tilesets.json"), [this](const std::shared_ptr<ISerializable>& serializer) {});

  /* TODO: Implement Serializers */
  plugins.serialize(basePath + "/js/plugins.js");
  mapInfos.serialize(basePath + "/data/MapInfos.json");
  gameConstants.serialize(basePath + "/data/Constants.json");

  for (const auto& map : mapInfos.mapInfos()) {
    if (map->map()) {
      SerializationQueue::instance().enqueue(std::make_shared<MapSerializer>(*map->map(), std::format("data/Map{:03}.json", map->id)), [this](const std::shared_ptr<ISerializable>& serializer) {});
    }
  }
}
