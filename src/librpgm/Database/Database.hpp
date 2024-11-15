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
#include "Database/Templates.hpp"
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
  Templates templates{};
  std::string projectVersion; // As stored in the .rpgproject file
  std::string projectFilePath;
  std::string basePath;

  Map loadMap(int mapId);

  bool serializeProject(std::string_view tmpPath);

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

  [[nodiscard]] std::string eventName(const int id) const {
    const auto map = mapInfos.currentMap();
    const auto object = map ? map->event(id) : nullptr;
    return object ? object->name : InvalidDataName.data();
  }
  [[nodiscard]] std::string actorName(const int id) const {
    const auto object = actors.actor(id);
    return object ? object->name : InvalidDataName.data();
  }

  [[nodiscard]] std::string className(const int id) const {
    const auto object = classes.classType(id);
    return object ? object->name : InvalidDataName.data();
  }

  [[nodiscard]] std::string skillName(const int id) const {
    const auto object = skills.skill(id);
    return object ? object->name : InvalidDataName.data();
  }

  [[nodiscard]] std::string itemName(const int id) const {
    const auto object = items.item(id);
    return object ? object->name : InvalidDataName.data();
  }

  [[nodiscard]] std::string weaponName(const int id) const {
    const auto object = weapons.weapon(id);
    return object ? object->name : InvalidDataName.data();
  }
  [[nodiscard]] std::string armorName(const int id) const {
    const auto object = armors.armor(id);
    return object ? object->name : InvalidDataName.data();
  }

  [[nodiscard]] std::string enemyName(const int id) const {
    const auto object = enemies.enemy(id);
    return object ? object->name : InvalidDataName.data();
  }

  [[nodiscard]] std::string troopName(const int id) const {
    const auto object = troops.troop(id);
    return object ? object->name : InvalidDataName.data();
  }

  [[nodiscard]] std::string stateName(const int id) const {
    const auto object = states.state(id);
    return object ? object->name : InvalidDataName.data();
  }

  [[nodiscard]] std::string animationName(const int id) const {
    const auto object = animations.animation(id);
    return object ? object->name : InvalidDataName.data();
  }

  [[nodiscard]] std::string tilesetName(const int id) const {
    const auto object = tilesets.tileset(id);
    return object ? object->name : InvalidDataName.data();
  }

  [[nodiscard]] std::string commonEventName(const int id) const {
    const auto object = commonEvents.event(id);
    return object ? object->name : InvalidDataName.data();
  }

  [[nodiscard]] std::string switchName(const int id) const {
    if (id < 0 || id >= system.switches.size()) {
      return InvalidDataName.data();
    }
    return system.switche(id);
  }

  [[nodiscard]] std::string variableName(const int id) const {
    if (id < 0 || id >= system.variables.size()) {
      return InvalidDataName.data();
    }
    return system.variable(id);
  }

  [[nodiscard]] std::string elementName(const int id) const {
    if (id < 0 || id >= system.elements.size()) {
      return InvalidDataName.data();
    }
    return *system.element(id);
  }

  [[nodiscard]] std::string skillTypeName(const int id) const {
    if (id < 0 || id >= system.skillTypes.size()) {
      return InvalidDataName.data();
    }
    return *system.skillType(id);
  }

  [[nodiscard]] std::string weaponTypeName(const int id) const {
    if (id < 0 || id >= system.weaponTypes.size()) {
      return InvalidDataName.data();
    }
    return *system.weaponType(id);
  }

  [[nodiscard]] std::string armorTypeName(const int id) const {
    if (id < 0 || id >= system.armorTypes.size()) {
      return InvalidDataName.data();
    }
    return *system.armorType(id);
  }

  [[nodiscard]] std::string equipTypeName(const int id) const {
    if (id < 0 || id >= system.equipTypes.size()) {
      return InvalidDataName.data();
    }
    return *system.equipType(id);
  }

  [[nodiscard]] std::string troopMemberName(const int id, const int index) const {
    if (index < 0) {
      return "Entire Troop";
    }
    const auto object = troops.troop(id);
    const auto member = object ? object->member(index) : nullptr;
    return member ? enemyName(member->enemyId) : InvalidDataName.data();
  }

  [[nodiscard]] std::string mapName(const int id) const {
    const auto map = mapInfos.map(id);
    return map ? map->name : InvalidDataName.data();
  }

  static std::string makeIdText(const int id, const int numDigits) {
    std::string ret = std::to_string(id);
    int digits = std::clamp<int>(numDigits - static_cast<int>(ret.length()), 0, 4);
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

  [[nodiscard]] std::string eventNameOrId(const int id) const {
    if (id == -1) {
      return "Player";
    }
    if (id == 0) {
      return "This Event";
    }
    return nameOrId(eventName(id), id, 3);
  }
  [[nodiscard]] std::string actorNameOrId(const int id) const { return nameOrId(actorName(id), id); }
  [[nodiscard]] std::string classNameOrId(const int id) const { return nameOrId(className(id), id); }
  [[nodiscard]] std::string skillNameOrId(const int id) const { return nameOrId(skillName(id), id); }
  [[nodiscard]] std::string itemNameOrId(const int id) const { return nameOrId(itemName(id), id); }
  [[nodiscard]] std::string weaponNameOrId(const int id) const { return nameOrId(weaponName(id), id); }
  [[nodiscard]] std::string armorNameOrId(const int id) const { return nameOrId(armorName(id), id); }
  [[nodiscard]] std::string enemyNameOrId(const int id) const { return nameOrId(enemyName(id), id); }
  [[nodiscard]] std::string troopNameOrId(const int id) const {
    if (id == -1) {
      return "Entire Troop";
    }
    return nameOrId(troopName(id), id);
  }
  [[nodiscard]] std::string stateNameOrId(const int id) const { return nameOrId(stateName(id), id); }
  [[nodiscard]] std::string animationNameOrId(const int id) const { return nameOrId(animationName(id), id); }
  [[nodiscard]] std::string tilesetNameOrId(const int id) const { return nameOrId(tilesetName(id), id); }
  [[nodiscard]] std::string commonEventNameOrId(const int id) const { return nameOrId(commonEventName(id), id); }
  [[nodiscard]] std::string switchNameOrId(const int id) const { return nameOrId(switchName(id), id); }
  [[nodiscard]] std::string variableNameOrId(const int id) const { return nameOrId(variableName(id), id); }
  [[nodiscard]] std::string variableNameAndId(const int id) const { return nameAndId(variableName(id), id); }
  [[nodiscard]] std::string mapNameOrId(const int id) const { return nameOrId(mapName(id), id); }

  [[nodiscard]] std::pair<Actor*, Class*> featureObjects(const int actorId) const {
    const auto actor = actors.actor(actorId);
    if (!actor) {
      return {};
    }

    return std::make_pair(const_cast<Actor*>(actor), const_cast<Class*>(classes.classType(actor->classId)));
  }

  [[nodiscard]] std::vector<Trait*> allTraits(const int actorId) const {
    const auto [act, cls] = featureObjects(actorId);
    std::vector<Trait*> ret;
    if (act) {
      for (auto& trait : act->traits) {
        ret.push_back(&trait);
      }
    }
    if (cls) {
      for (auto& trait : cls->traits) {
        ret.push_back(&trait);
      }
    }

    return ret;
  }

  [[nodiscard]] std::vector<Trait*> traits(const int actorId, const TraitCode code) const {
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

  [[nodiscard]] std::vector<int> traitsSet(const int actorId, const TraitCode code) const {
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

  [[nodiscard]] bool isEquipWeaponTypeOk(const int actorId, const int wTypeId) const {
    auto traits = traitsSet(actorId, TraitCode::Equip_Weapon);
    return std::ranges::find_if(traits, [&wTypeId](const auto& id) { return id == wTypeId; }) != traits.end();
  }

  [[nodiscard]] bool isEquipArmorTypeOk(const int actorId, const int aTypeId) const {
    auto traits = traitsSet(actorId, TraitCode::Equip_Armor);
    return std::ranges::find_if(traits, [&aTypeId](const auto& id) { return id == aTypeId; }) != traits.end();
  }

  [[nodiscard]] bool isEquipTypeSealed(const int actorId, const int aTypeId) const {
    auto traits = traitsSet(actorId, TraitCode::Seal_Equip);
    return std::ranges::find_if(traits, [&aTypeId](const auto& id) { return id == aTypeId; }) != traits.end();
  }

  [[nodiscard]] int slotType(const int actorId) const {

    if (const std::vector<int> set = traitsSet(actorId, TraitCode::Slot_Type); set.size() > 0) {
      return *std::ranges::max_element(set);
    }
    return 0;
  }

  [[nodiscard]] bool isDualWield(const int actorId) const { return slotType(actorId) == 1; }

  [[nodiscard]] int slotIdToEquipId(const int actorId, const int slotId) const {
    return slotId == 1 && isDualWield(actorId) ? 1 : slotId + 1;
  }

  [[nodiscard]] std::vector<char> encryptionKeyAsBytes() const { return system.encryptionKeyAsBytes(); }


  static Database* Instance;
};