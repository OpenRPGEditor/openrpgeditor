#pragma once

#include "Database/Actors.hpp"
#include "Database/Classes.hpp"
#include "Database/Skills.hpp"
#include "Database/Items.hpp"
#include "Database/Weapons.hpp"
#include "Database/Armors.hpp"
#include "Database/Enemies.hpp"
#include "Database/Troops.hpp"
#include "Database/States.hpp"
#include "Database/Animations.hpp"
#include "Database/Tilesets.hpp"
#include "Database/CommonEvents.hpp"
#include "Database/System.hpp"
#include "Database/MapInfos.hpp"
#include "Database/Map.hpp"
#include "Database/Plugins.hpp"

struct Database {
  Database() {
    Instance = this;
  }
  ~Database() {
    Instance = nullptr;
  }
  Database(Database&) =delete;
  Database(Database&&) =delete;
  Database& operator=(Database&)=delete;
  Database& operator=(Database&&)=delete;

  Actors actors{};
  Classes classes{};
  Skills skills{};
  Items items{};
  Weapons weapons{};
  Armors armors{};
  Enemies enemies{};
  Troops troops{};
  States states{};
  Animations animations{};
  Tilesets tilesets{};
  CommonEvents commonEvents{};
  System system{};
  Plugins plugins{};
  MapInfos mapInfos{};
  std::string projectVersion; // As stored in the .rpgproject file
  std::string projectFilePath;
  std::string basePath;

  Map loadMap(int mapId);

  static Database* Instance;
};