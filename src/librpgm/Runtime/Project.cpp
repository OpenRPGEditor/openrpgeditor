#include "Project.hpp"

#include "imgui.h"

#include <fstream>

#include <array>
#include <string_view>
#include <algorithm>

using namespace std::literals::string_view_literals;

constexpr std::array KnownRPGMVVersions = {
    "RPGMV 1.6.1"sv,
    "RPGMV 1.6.2"sv,
};
#include <iostream>

Project Project::load(std::string_view filePath, std::string_view basePath) {
  std::string version;
  std::ifstream file(filePath.data());
  file >> version;

  auto it = std::find(KnownRPGMVVersions.begin(), KnownRPGMVVersions.end(), version);

  /* TODO: This is completely wrong and is only for testing */
  Project ret;
  ret.m_isValid = it != KnownRPGMVVersions.end();
  ret.m_projectVersion = version;
  ret.m_basePath = basePath;
  ret.m_projectFilePath = filePath;
  ret.m_actors = Actors::load(ret.m_basePath + "/data/Actors.json");
  ret.m_actors.serialize("Actors.json");
  ret.m_classes = Classes::load(ret.m_basePath + "/data/Classes.json");
  ret.m_classes.serialize("Classes.json");
  ret.m_skills = Skills::load(ret.m_basePath + "/data/Skills.json");
  ret.m_skills.serialize("Skills.json");
  ret.m_items = Items::load(ret.m_basePath + "/data/Items.json");
  ret.m_items.serialize("Items.json");
  ret.m_weapons = Weapons::load(ret.m_basePath + "/data/Weapons.json");
  ret.m_weapons.serialize("Weapons.json");
  ret.m_armors = Armors::load(ret.m_basePath + "/data/Armors.json");
  ret.m_armors.serialize("Armors.json");
  ret.m_enemies = Enemies::load(ret.m_basePath + "/data/Enemies.json");
  ret.m_enemies.serialize("Enemies.json");
  // ret.m_troops = Troops::load(ret.m_basePath + "/data/Troops.json");
  ret.m_states = States::load(ret.m_basePath + "/data/States.json");
  ret.m_states.serialize("States.json");
  ret.m_animations = Animations::load(ret.m_basePath + "/data/Animations.json");
  ret.m_animations.serialize("Animations.json");
  ret.m_tilesets = Tilesets::load(ret.m_basePath + "/data/Tilesets.json");
  ret.m_tilesets.serialize("Tilesets.json");
  // ret.m_commonEvents = CommonEvents::load(ret.m_basePath + "/data/CommonEvents.json);
  ret.m_system = System::load(ret.m_basePath + "/data/System.json");
  ret.m_system.serialize("System.json");
  // ret.m_types = Types::load(ret.m_basePath + "/data/Types.json");
  return ret;
}

void Project::draw() {}