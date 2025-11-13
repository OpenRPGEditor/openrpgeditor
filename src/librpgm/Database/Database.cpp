#include "Database/Database.hpp"

#include "Database/Serializable/FileQueue.hpp"

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
#include "Serializable/MapInfosSerializer.hpp"
#include "Serializable/MapSerializer.hpp"

Database* Database::m_instance = nullptr;

Database::Database(const std::string_view _projectBasePath, const std::string_view _projectFilePath, const std::string_view _projectVersion, const bool isMZ)
: projectVersion(_projectVersion)
, projectFilePath(_projectFilePath)
, basePath(_projectBasePath)
, m_isMZ(isMZ) {
  m_instance = this;
}

void Database::load() {
  config.load(basePath / "editor/config.json");
  transient.load(basePath / ".ore/transient.json");

  RPGM_INFO("Queue MapInfo definitions for load...");
  FileQueue::instance().enqueue(std::make_shared<MapInfosSerializer>("data/MapInfos.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    mapInfos = std::dynamic_pointer_cast<MapInfosSerializer>(serializer)->data();
    mapInfos->buildTree(true);
    mapInfos->modified().connect<&Database::onAnyModifiableModified>(this);
    mapInfos->connectSignals();
    emit_signal(mapInfosLoaded());
    RPGM_INFO("MapInfo definitions loaded");
  });

  RPGM_INFO("Queue Actor definitions for load...");
  FileQueue::instance().enqueue(std::make_shared<ActorsSerializer>("data/Actors.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    actors = std::dynamic_pointer_cast<ActorsSerializer>(serializer)->data();
    actors->modified().connect<&Database::onAnyModifiableModified>(this);
    emit_signal(actorsLoaded());
    RPGM_INFO("Actor definitions loaded");
  });

  RPGM_INFO("Queue Class definitions for load...");
  FileQueue::instance().enqueue(std::make_shared<ClassesSerializer>("data/Classes.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    classes = std::dynamic_pointer_cast<ClassesSerializer>(serializer)->data();
    classes->modified().connect<&Database::onAnyModifiableModified>(this);
    emit_signal(classesLoaded());
    RPGM_INFO("Class definitions loaded");
  });

  RPGM_INFO("Queue Skill definitions for load...");
  FileQueue::instance().enqueue(std::make_shared<SkillsSerializer>("data/Skills.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    skills = std::dynamic_pointer_cast<SkillsSerializer>(serializer)->data();
    skills->modified().connect<&Database::onAnyModifiableModified>(this);
    emit_signal(skillsLoaded());
    RPGM_INFO("Skill definitions loaded");
  });

  RPGM_INFO("Queue Item definitions for load...");
  FileQueue::instance().enqueue(std::make_shared<ItemsSerializer>("data/Items.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    items = std::dynamic_pointer_cast<ItemsSerializer>(serializer)->data();
    items->modified().connect<&Database::onAnyModifiableModified>(this);
    emit_signal(itemsLoaded());
    RPGM_INFO("Item definitions loaded");
  });

  RPGM_INFO("Queue Weapon definitions for load...");
  FileQueue::instance().enqueue(std::make_shared<WeaponsSerializer>("data/Weapons.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    weapons = std::dynamic_pointer_cast<WeaponsSerializer>(serializer)->data();
    weapons->modified().connect<&Database::onAnyModifiableModified>(this);
    emit_signal(weaponsLoaded());
    RPGM_INFO("Weapon definitions loaded");
  });

  RPGM_INFO("Queue Armor definitions for load...");
  FileQueue::instance().enqueue(std::make_shared<ArmorsSerializer>("data/Armors.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    armors = std::dynamic_pointer_cast<ArmorsSerializer>(serializer)->data();
    armors->modified().connect<&Database::onAnyModifiableModified>(this);
    emit_signal(armorsLoaded());
    RPGM_INFO("Armor definitions loaded");
  });

  RPGM_INFO("Queue Enemy definitions for load...");
  FileQueue::instance().enqueue(std::make_shared<EnemiesSerializer>("data/Enemies.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    enemies = std::dynamic_pointer_cast<EnemiesSerializer>(serializer)->data();
    enemies->modified().connect<&Database::onAnyModifiableModified>(this);
    emit_signal(enemiesLoaded());
    RPGM_INFO("Enemy definitions loaded");
  });

  RPGM_INFO("Queue Troop definitions for load...");
  FileQueue::instance().enqueue(std::make_shared<TroopsSerializer>("data/Troops.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    troops = std::dynamic_pointer_cast<TroopsSerializer>(serializer)->data();
    troops->modified().connect<&Database::onAnyModifiableModified>(this);
    emit_signal(troopsLoaded());
    RPGM_INFO("Troop definitions loaded");
  });

  RPGM_INFO("Queue State definitions for load...");
  FileQueue::instance().enqueue(std::make_shared<StatesSerializer>("data/States.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    states = std::dynamic_pointer_cast<StatesSerializer>(serializer)->data();
    states->modified().connect<&Database::onAnyModifiableModified>(this);
    emit_signal(statesLoaded());
    RPGM_INFO("State definitions loaded");
  });

  RPGM_INFO("Queue Animation definitions for load...");
  FileQueue::instance().enqueue(std::make_shared<AnimationsSerializer>("data/Animations.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    animations = std::dynamic_pointer_cast<AnimationsSerializer>(serializer)->data();
    animations->modified().connect<&Database::onAnyModifiableModified>(this);
    emit_signal(animationsLoaded());
    RPGM_INFO("Animation definitions loaded");
  });

  RPGM_INFO("Queue Tileset definitions for load...");
  FileQueue::instance().enqueue(std::make_shared<TilesetsSerializer>("data/Tilesets.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    tilesets = std::dynamic_pointer_cast<TilesetsSerializer>(serializer)->data();
    tilesets->modified().connect<&Database::onAnyModifiableModified>(this);
    emit_signal(tilesetsLoaded());
    RPGM_INFO("Tileset definitions loaded");
  });

  RPGM_INFO("Queue CommonEvent definitions for load...");
  FileQueue::instance().enqueue(std::make_shared<CommonEventsSerializer>("data/CommonEvents.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    commonEvents = std::dynamic_pointer_cast<CommonEventsSerializer>(serializer)->data();
    commonEvents->modified().connect<&Database::onAnyModifiableModified>(this);
    emit_signal(commonEventsLoaded());
    RPGM_INFO("CommonEvent definitions loaded");
  });

  RPGM_INFO("Queue System definitions for load...");
  FileQueue::instance().enqueue(std::make_shared<SystemSerializer>("data/System.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    system = std::dynamic_pointer_cast<SystemSerializer>(serializer)->data();
    system->modified().connect<&Database::onAnyModifiableModified>(this);
    emit_signal(systemLoaded());
    RPGM_INFO("System definitions loaded");
  });

  RPGM_INFO("Queue Map001 localization file...");
  locales = Locales();
  locales->loadMap(basePath / "locales/en/Map0001.json");
  // locales.onModified().connect<&Database::onAnyModifiableModified>(this);
  emit_signal(localesLoaded());

  RPGM_INFO("Loading Plugins...");
  plugins = Plugins::load(basePath / "js/plugins.js");
  // plugins.onModified().connect<&Database::onAnyModifiableModified>(this);
  emit_signal(pluginsLoaded());
  RPGM_INFO("Loading GameConstants");
  gameConstants = GameConstants::load(basePath / "/data/Constants.json");
  // gameConstants.onModified().connect<&Database::onAnyModifiableModified>(this);
  emit_signal(gameConstantsLoaded());
  RPGM_INFO("Loading Templates");
  templates = Templates::load(basePath / "data/Templates.json");
  templates->modified().connect<&Database::onAnyModifiableModified>(this);
  emit_signal(templatesLoaded());

  RPGM_INFO("Loading Documentation");
  docs = Docs::load(basePath / "editor/Documentation.json");
  docs->modified().connect<&Database::onAnyModifiableModified>(this);
  emit_signal(docsLoaded());
}

void Database::serializeProject() {
  if (!actors || !classes || !skills || !items || !weapons || !armors || !enemies || !troops || !states || !animations || !tilesets || !commonEvents || !system || !mapInfos || !plugins ||
      !gameConstants || !docs || !templates || !locales) {
    return;
  }

  serializeSettings();
  if (gameConstants->generateJS) {
    gameConstants->generateConstantsJS(basePath / "js/Constants.js");
  }

  FileQueue::instance().enqueue(std::make_shared<CommonEventsSerializer>(*commonEvents, "data/CommonEvents.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    commonEvents->acceptChanges();
    RPGM_INFO("CommonEvents saved");
  });
  FileQueue::instance().enqueue(std::make_shared<SystemSerializer>(*system, "data/System.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    system->acceptChanges();
    RPGM_INFO("System saved");
  });
  FileQueue::instance().enqueue(std::make_shared<MapInfosSerializer>(*mapInfos, "data/MapInfos.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    mapInfos->acceptChanges();
    RPGM_INFO("MapInfos saved");
  });
  FileQueue::instance().enqueue(std::make_shared<ActorsSerializer>(*actors, "data/Actors.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    actors->acceptChanges();
    RPGM_INFO("Actors saved");
  });
  FileQueue::instance().enqueue(std::make_shared<ClassesSerializer>(*classes, "data/Classes.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    classes->acceptChanges();
    RPGM_INFO("Classes saved");
  });
  FileQueue::instance().enqueue(std::make_shared<SkillsSerializer>(*skills, "data/Skills.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    skills->acceptChanges();
    RPGM_INFO("Skills saved");
  });
  FileQueue::instance().enqueue(std::make_shared<ItemsSerializer>(*items, "data/Items.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    items->acceptChanges();
    RPGM_INFO("Items saved");
  });
  FileQueue::instance().enqueue(std::make_shared<WeaponsSerializer>(*weapons, "data/Weapons.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    weapons->acceptChanges();
    RPGM_INFO("Weapons saved");
  });
  FileQueue::instance().enqueue(std::make_shared<ArmorsSerializer>(*armors, "data/Armors.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    armors->acceptChanges();
    RPGM_INFO("Armors saved");
  });
  FileQueue::instance().enqueue(std::make_shared<EnemiesSerializer>(*enemies, "data/Enemies.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    enemies->acceptChanges();
    RPGM_INFO("Enemies saved");
  });
  FileQueue::instance().enqueue(std::make_shared<TroopsSerializer>(*troops, "data/Troops.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    troops->acceptChanges();
    RPGM_INFO("Troops saved");
  });
  FileQueue::instance().enqueue(std::make_shared<StatesSerializer>(*states, "data/States.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    states->acceptChanges();
    RPGM_INFO("States saved");
  });
  FileQueue::instance().enqueue(std::make_shared<AnimationsSerializer>(*animations, "data/Animations.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    animations->acceptChanges();
    RPGM_INFO("Animations saved");
  });
  FileQueue::instance().enqueue(std::make_shared<TilesetsSerializer>(*tilesets, "data/Tilesets.json"), [this](const std::shared_ptr<ISerializable>& serializer) {
    tilesets->acceptChanges();
    RPGM_INFO("Tilesets saved");
  });

  /* TODO: Implement Serializers */
  plugins->serialize(basePath / "js/plugins.js");
  gameConstants->serialize(basePath / "data/Constants.json");

  for (const auto& map : mapInfos->mapInfos()) {
    if (map->mapLoaded() && map->isModified()) {
      FileQueue::instance().enqueue(std::make_shared<MapSerializer>(map->map()->clone(), map->id(), std::format("data/Map{:03}.json", map->id())),
                                    [this](const std::shared_ptr<ISerializable>& serializer) {
                                      int mapId = std::dynamic_pointer_cast<MapSerializer>(serializer)->mapId();
                                      if (mapInfos->map(mapId)->mapLoaded()) {
                                        mapInfos->map(mapId)->acceptChanges();
                                      }
                                      RPGM_INFO("Map{:03} saved", mapId);
                                    });
    }
  }
}

void Database::createEmptyProject(const std::string_view gameTitle, const std::string_view locale) {
  gameConstants.emplace();
  commonEvents.emplace();
  system.emplace();
  system->setGameTitle(gameTitle.data());
  system->setLocale(locale.data());
  mapInfos.emplace();
  actors.emplace();
  classes.emplace();
  skills.emplace();
  items.emplace();
  weapons.emplace();
  armors.emplace();
  enemies.emplace();
  troops.emplace();
  states.emplace();
  animations.emplace();
  tilesets.emplace();
  plugins.emplace();
}

