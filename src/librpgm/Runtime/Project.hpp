#pragma once

#include "Runtime/DatabaseEditor.hpp"
#include <string_view>
#include <string>

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

class Project {
public:
  static Project load(std::string_view filePath, std::string_view basePath);

  [[nodiscard]] bool isLoaded() const { return m_isLoaded; }
  [[nodiscard]] bool isValid() const { return m_isValid; }
  [[nodiscard]] const std::string& version() const { return m_projectVersion; }

  // Files will inform Project of that they've been modified via a callback,
  // this keeps things simple and avoids delegate overhead
  [[nodiscard]] bool modified() { return m_isModified; }

  void draw();

private:
  bool m_isValid = false;
  bool m_isLoaded = false;
  bool m_isModified = false;

  std::string m_projectVersion; // As stored in the .rpgproject file
  std::string m_projectFilePath;
  std::string m_basePath;

  Actors m_actors;
  Classes m_classes;
  Skills m_skills;
  Items m_items;
  Weapons m_weapons;
  Armors m_armors;
  Enemies m_enemies;
  Troops m_troops;
  States m_states;
  Animations m_animations;
  Tilesets m_tilesets;
  CommonEvents m_commonEvents;
  System m_system;
  MapInfos m_mapInfos;
};