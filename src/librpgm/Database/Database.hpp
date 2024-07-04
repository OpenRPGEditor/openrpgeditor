#pragma once

#include "GameConstants.hpp"
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
#include <string_view>
using namespace std::string_view_literals;

struct Database {
  static constexpr auto InvalidDataName = "?"sv;
  Database() { Instance = this; }
  ~Database() { Instance = nullptr; }
  Database(Database&) = delete;
  Database(Database&&) = delete;
  Database& operator=(Database&) = delete;
  Database& operator=(Database&&) = delete;

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
  GameConstants gameConstants{};
  std::string projectVersion; // As stored in the .rpgproject file
  std::string projectFilePath;
  std::string basePath;

  Map loadMap(int mapId);

  static std::string framesText(const int frames) {
    return std::to_string(frames) + (frames > 1 ? " frames" : " frame");
  }

  static std::string secondsText(const int seconds) {
    return std::to_string(seconds) + (seconds > 1 ? " seconds" : " second");
  }

  static std::string parentheses(const std::string& text) { return "(" + text + ")"; }

  static std::string imageText(const std::string& name, int index = -1) {
    if (!name.empty()) {
      if (index >= 0) {
        return std::format("{}({})", name, index);
      }
      return name;
    }

    return "None";
  }

  static std::string dualImageText(const std::string& name1, const std::string& name2) {
    if (name1.empty() && name2.empty()) {
      return "None";
    }

    std::string text = name1.empty() ? "None" : name1;
    if (!name2.empty()) {
      text += " & " + name2;
    }

    return text;
  }

  static std::string audioText(const Audio& audio) {
    if (!audio.name.empty()) {
      return audio.name + " " + parentheses(std::format("{}, {}, {}", audio.volume, audio.pitch, audio.pan));
    }

    return "None";
  }

  static std::string movieText(const std::string& name) {
    if (!name.empty()) {
      return name;
    }

    return "None";
  }

  std::string actorName(int id) const {
    const auto object = actors.actor(id);
    return object ? object->name : InvalidDataName.data();
  }

  std::string className(int id) const {
    const auto object = classes.classType(id);
    return object ? object->name : InvalidDataName.data();
  }

  std::string skillName(int id) const {
    const auto object = skills.skill(id);
    return object ? object->name : InvalidDataName.data();
  }

  std::string itemName(int id) const {
    const auto object = items.item(id);
    return object ? object->name : InvalidDataName.data();
  }

  std::string weaponName(int id) const {
    const auto object = weapons.weapon(id);
    return object ? object->name : InvalidDataName.data();
  }
  std::string armorName(int id) const {
    const auto object = armors.armor(id);
    return object ? object->name : InvalidDataName.data();
  }

  std::string enemyName(int id) const {
    const auto object = enemies.enemy(id);
    return object ? object->name : InvalidDataName.data();
  }

  std::string troopName(int id) const {
    const auto object = troops.troop(id);
    return object ? object->name : InvalidDataName.data();
  }

  std::string stateName(int id) const {
    const auto object = states.state(id);
    return object ? object->name : InvalidDataName.data();
  }

  std::string animationName(int id) const {
    const auto object = animations.animation(id);
    return object ? object->name : InvalidDataName.data();
  }

  std::string tilesetName(int id) const {
    const auto object = tilesets.tileset(id);
    return object ? object->name : InvalidDataName.data();
  }

  std::string commonEventName(int id) const {
    const auto object = commonEvents.event(id);
    return object ? object->name : InvalidDataName.data();
  }

  std::string switchName(int id) const {
    if (id < 0 || id >= system.switches.size()) {
      return InvalidDataName.data();
    }
    return system.switche(id);
  }

  std::string variableName(int id) const {
    if (id < 0 || id >= system.variables.size()) {
      return InvalidDataName.data();
    }
    return system.variable(id);
  }

  std::string elementName(int id) const {
    if (id < 0 || id >= system.elements.size()) {
      return InvalidDataName.data();
    }
    return *system.element(id);
  }

  std::string skillTypeName(int id) const {
    if (id < 0 || id >= system.skillTypes.size()) {
      return InvalidDataName.data();
    }
    return *system.skillType(id);
  }

  std::string weaponTypeName(int id) const {
    if (id < 0 || id >= system.weaponTypes.size()) {
      return InvalidDataName.data();
    }
    return *system.weaponType(id);
  }

  std::string armorTypeName(int id) const {
    if (id < 0 || id >= system.armorTypes.size()) {
      return InvalidDataName.data();
    }
    return *system.armorType(id);
  }

  std::string equipTypeName(int id) const {
    if (id < 0 || id >= system.equipTypes.size()) {
      return InvalidDataName.data();
    }
    return *system.equipType(id);
  }

  std::string troopMemberName(int id, int index) const {
    const auto object = troops.troop(id);
    const auto member = object ? object->member(index) : nullptr;
    return member ? enemyName(member->enemyId) : InvalidDataName.data();
  }

  std::string mapName(int id) const {
    const auto map = mapInfos.map(id);
    return map ? map->name : InvalidDataName.data();
  }

  static std::string makeIdText(int id, int numDigits) {
    std::string ret = std::to_string(id);
    int digits = std::clamp<int>(numDigits - ret.length(), 0, 4);
    return std::string(digits, '0') + ret;
  }

  static std::string nameOrId(const std::string_view name, const int id, const int numDigits = 4) {
    if (!name.empty()) {
      return name.data();
    };
    return "#" + makeIdText(id, numDigits);
  }

  static std::string nameAndId(const std::string_view name, const int id, const int numDigits = 4) {
    if (!name.empty()) {
      return "#" + makeIdText(id, numDigits) + " " + name.data();
    }

    return "#" + makeIdText(id, numDigits);
  }

  std::string actorNameOrId(const int id) const { return nameOrId(actorName(id), id); }
  std::string classNameOrId(const int id) const { return nameOrId(className(id), id); }
  std::string skillNameOrId(const int id) const { return nameOrId(skillName(id), id); }
  std::string itemNameOrId(const int id) const { return nameOrId(itemName(id), id); }
  std::string weaponNameOrId(const int id) const { return nameOrId(weaponName(id), id); }
  std::string armorNameOrId(const int id) const { return nameOrId(armorName(id), id); }
  std::string enemyNameOrId(const int id) const { return nameOrId(enemyName(id), id); }
  std::string troopNameOrId(const int id) const { return nameOrId(troopName(id), id); }
  std::string stateNameOrId(const int id) const { return nameOrId(stateName(id), id); }
  std::string animationNameOrId(const int id) const { return nameOrId(animationName(id), id); }
  std::string tilesetNameOrId(const int id) const { return nameOrId(tilesetName(id), id); }
  std::string commonEventNameOrId(const int id) const { return nameOrId(commonEventName(id), id); }
  std::string switchNameOrId(const int id) const { return nameOrId(switchName(id), id); }
  std::string variableNameOrId(const int id) const { return nameOrId(variableName(id), id); }
  std::string mapNameOrId(const int id) const { return nameOrId(mapName(id), id); }

  std::pair<Actor*, Class*> featureObjects(const int actorId) const {
    auto actor = actors.actor(actorId);
    if (!actor) {
      return {};
    }

    return std::make_pair(const_cast<Actor*>(actor), const_cast<Class*>(classes.classType(actor->classId)));
  }

  std::vector<Trait*> allTraits(const int actorId) const {
    const auto features = featureObjects(actorId);
    if (features.first == nullptr) {
      return {};
    }
    std::vector<Trait*> ret;
    ret.reserve(features.first->traits.size());
    for (auto& trait : features.first->traits) {
      ret.push_back(&trait);
    }

    return ret;
  }

  std::vector<Trait*> traits(const int actorId, const TraitCode code) const {
    auto traits = allTraits(actorId);
    std::vector<Trait*> ret;
    ret.reserve(traits.size());
    for (auto& trait : traits) {
      if (trait->code == code) {
        ret.push_back(trait);
      }
    }
    ret.shrink_to_fit();
    return ret;
  }

  std::vector<int> traitsSet(const int actorId, const TraitCode code) const {
    const auto tr = traits(actorId, code);
    std::vector<int> ret;
    ret.reserve(tr.size());
    for (auto& trait : tr) {
      if (trait->code == code) {
        ret.push_back(trait->dataId);
      }
    }
    ret.shrink_to_fit();
    return ret;
  }

  bool isEquipWeaponTypeOk(int actorId, int wTypeId) const {
    auto traits = traitsSet(actorId, TraitCode::Equip_Weapon);
    return std::find_if(traits.begin(), traits.end(), [&wTypeId](const auto& id) { return id == wTypeId; }) !=
           traits.end();
  }

  bool isEquipArmorTypeOk(int actorId, int aTypeId) const {
    auto traits = traitsSet(actorId, TraitCode::Equip_Armor);
    return std::find_if(traits.begin(), traits.end(), [&aTypeId](const auto& id) { return id == aTypeId; }) !=
           traits.end();
  }
  bool isEquipTypeSealed(int actorId, int aTypeId) const {
    auto traits = traitsSet(actorId, TraitCode::Seal_Equip);
    return std::find_if(traits.begin(), traits.end(), [&aTypeId](const auto& id) { return id == aTypeId; }) !=
           traits.end();
  }

  int slotType(const int actorId) const {

    if (const std::vector<int> set = traitsSet(actorId, TraitCode::Slot_Type); set.size() > 0) {
      return *std::max_element(set.begin(), set.end());
    }
    return 0;
  }

  bool isDualWield(const int actorId) const { return slotType(actorId) == 1; }

  int slotIdToEquipId(const int actorId, const int slotId) const {
    return slotId == 1 && isDualWield(actorId) ? 1 : slotId + 1;
  }

  static Database* Instance;
};