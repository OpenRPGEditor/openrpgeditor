#pragma once

#include "Database/Actors.hpp"
#include "Database/Animations.hpp"
#include "Database/Armors.hpp"
#include "Database/Classes.hpp"
#include "Database/CommonEvents.hpp"
#include "Database/Docs.hpp"
#include "Database/Enemies.hpp"
#include "Database/GameConstants.hpp"
#include "Database/Items.hpp"
#include "Database/Map.hpp"
#include "Database/MapInfos.hpp"
#include "Database/Plugins.hpp"
#include "Database/Skills.hpp"
#include "Database/States.hpp"
#include "Database/System.hpp"
#include "Database/Templates.hpp"
#include "Database/Tilesets.hpp"
#include "Database/Troops.hpp"
#include "Database/Weapons.hpp"
#include "Utils/SignalSlot.hpp"

#include <string_view>
using namespace std::string_view_literals;

struct Database {
  static constexpr auto InvalidDataName = "?"sv;
  Database(std::string_view projectBasePath, std::string_view projectFilePath, std::string_view projectVersion);
  ~Database() { m_instance = nullptr; }
  Database(Database&) = delete;
  Database(Database&&) = delete;
  void load();
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
  Docs docs{};
  std::string projectVersion; // As stored in the .rpgproject file
  std::string projectFilePath;
  std::string basePath;

  void serializeProject();

  static std::string framesText(const int frames) { return std::to_string(frames) + (frames > 1 ? " frames" : " frame"); }

  static std::string secondsText(const int seconds) { return std::to_string(seconds) + (seconds > 1 ? " seconds" : " second"); }

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
    if (!audio.name().empty()) {
      return audio.name() + " " + parentheses(std::format("{}, {}, {}", audio.volume(), audio.pitch(), audio.pan()));
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
    return object ? object->name() : InvalidDataName.data();
  }
  [[nodiscard]] std::string actorName(const int id) const {
    const auto object = actors.actor(id);
    return object ? object->name() : InvalidDataName.data();
  }

  [[nodiscard]] std::string className(const int id) const {
    const auto object = classes.classType(id);
    return object ? object->name() : InvalidDataName.data();
  }

  [[nodiscard]] std::string skillName(const int id) const {
    const auto object = skills.skill(id);
    return object ? object->name() : InvalidDataName.data();
  }

  [[nodiscard]] std::string itemName(const int id) const {
    const auto object = items.item(id);
    return object ? object->name() : InvalidDataName.data();
  }

  [[nodiscard]] std::string weaponName(const int id) const {
    const auto object = weapons.weapon(id);
    return object ? object->name() : InvalidDataName.data();
  }
  [[nodiscard]] std::string armorName(const int id) const {
    const auto object = armors.armor(id);
    return object ? object->name() : InvalidDataName.data();
  }

  [[nodiscard]] std::string enemyName(const int id) const {
    const auto object = enemies.enemy(id);
    return object ? object->name() : InvalidDataName.data();
  }

  [[nodiscard]] std::string troopName(const int id) const {
    const auto object = troops.troop(id);
    return object ? object->name() : InvalidDataName.data();
  }

  [[nodiscard]] std::string stateName(const int id) const {
    if (id == 0) {
      return "Normal Attack";
    }

    const auto object = states.state(id);
    return object ? object->name() : InvalidDataName.data();
  }

  [[nodiscard]] std::string animationName(const int id) const {
    const auto object = animations.animation(id);
    return object ? object->name() : InvalidDataName.data();
  }

  [[nodiscard]] std::string tilesetName(const int id) const {
    const auto object = tilesets.tileset(id);
    return object ? object->name() : InvalidDataName.data();
  }

  [[nodiscard]] std::string commonEventName(const int id) const {
    const auto object = commonEvents.event(id);
    return object ? object->name() : InvalidDataName.data();
  }

  [[nodiscard]] std::string switchName(const int id) const {
    if (id < 0 || id >= system.switches().size()) {
      return InvalidDataName.data();
    }
    return system.switche(id);
  }

  [[nodiscard]] std::string variableName(const int id) const {
    if (id < 0 || id >= system.variables().size()) {
      return InvalidDataName.data();
    }
    return system.variable(id);
  }

  [[nodiscard]] std::string elementName(const int id) const {
    if (id < 0 || id >= system.elements().size()) {
      return InvalidDataName.data();
    }
    return system.element(id);
  }

  [[nodiscard]] std::string skillTypeName(const int id) const {
    if (id < 0 || id >= system.skillTypes().size()) {
      return InvalidDataName.data();
    }
    return system.skillType(id);
  }

  [[nodiscard]] std::string weaponTypeName(const int id) const {
    if (id < 0 || id >= system.weaponTypes().size()) {
      return InvalidDataName.data();
    }
    return system.weaponType(id);
  }

  [[nodiscard]] std::string armorTypeName(const int id) const {
    if (id < 0 || id >= system.armorTypes().size()) {
      return InvalidDataName.data();
    }
    return system.armorType(id);
  }

  [[nodiscard]] std::string equipTypeName(const int id) const {
    if (id < 0 || id >= system.equipTypes().size()) {
      return InvalidDataName.data();
    }
    return system.equipType(id);
  }

  [[nodiscard]] std::string troopMemberName(const int id, const int index) const {
    if (index < 0) {
      return "Entire Troop";
    }
    const auto object = troops.troop(id);
    const auto member = object ? object->member(index) : nullptr;
    return member ? enemyName(member->enemyId()) : InvalidDataName.data();
  }

  [[nodiscard]] std::string mapName(const int id) const {
    const auto map = mapInfos.map(id);
    return map ? map->name() : InvalidDataName.data();
  }

  static std::string makeIdText(const int id, const int numDigits) {
    const std::string ret = std::to_string(id);
    const int digits = std::clamp<int>(numDigits - static_cast<int>(ret.length()), 0, 4);
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
  [[nodiscard]] std::string actorNameAndId(const int id) const { return nameAndId(actorName(id), id); }
  [[nodiscard]] std::string classNameOrId(const int id) const { return nameOrId(className(id), id); }
  [[nodiscard]] std::string classNameAndId(const int id) const { return nameAndId(className(id), id); }
  [[nodiscard]] std::string skillNameOrId(const int id) const { return nameOrId(skillName(id), id); }
  [[nodiscard]] std::string itemNameOrId(const int id) const { return nameOrId(itemName(id), id); }
  [[nodiscard]] std::string itemNameAndId(const int id) const { return nameAndId(itemName(id), id); }
  [[nodiscard]] std::string weaponNameOrId(const int id) const { return nameOrId(weaponName(id), id); }
  [[nodiscard]] std::string weaponNameAndId(const int id) const { return nameAndId(weaponName(id), id); }
  [[nodiscard]] std::string armorNameOrId(const int id) const { return nameOrId(armorName(id), id); }
  [[nodiscard]] std::string armorNameAndId(const int id) const { return nameAndId(armorName(id), id); }
  [[nodiscard]] std::string enemyNameOrId(const int id) const { return nameOrId(enemyName(id), id); }
  [[nodiscard]] std::string enemyNameAndId(const int id) const { return nameAndId(enemyName(id), id); }
  [[nodiscard]] std::string troopNameOrId(const int id) const {
    if (id == -1) {
      return "Entire Troop";
    }
    return nameOrId(troopName(id), id);
  }
  [[nodiscard]] std::string troopNameAndId(const int id) const {
    if (id == -1) {
      return "Entire Troop";
    }
    return nameAndId(troopName(id), id);
  }

  [[nodiscard]] std::string stateNameOrId(const int id) const { return nameOrId(stateName(id), id); }
  [[nodiscard]] std::string stateNameAndId(const int id) const { return nameAndId(stateName(id), id); }
  [[nodiscard]] std::string animationNameOrId(const int id) const { return nameOrId(animationName(id), id); }
  [[nodiscard]] std::string animationNameAndId(const int id) const { return nameAndId(animationName(id), id); }
  [[nodiscard]] std::string tilesetNameOrId(const int id) const { return nameOrId(tilesetName(id), id); }
  [[nodiscard]] std::string tilesetNameAndId(const int id) const { return nameAndId(tilesetName(id), id); }
  [[nodiscard]] std::string commonEventNameOrId(const int id) const { return nameOrId(commonEventName(id), id); }
  [[nodiscard]] std::string commonEventNameAndId(const int id) const { return nameAndId(commonEventName(id), id); }
  [[nodiscard]] std::string switchNameOrId(const int id) const { return nameOrId(switchName(id), id); }
  [[nodiscard]] std::string switchNameAndId(const int id) const { return nameAndId(switchName(id), id); }
  [[nodiscard]] std::string variableNameOrId(const int id) const { return nameOrId(variableName(id), id); }
  [[nodiscard]] std::string variableNameAndId(const int id) const { return nameAndId(variableName(id), id); }
  [[nodiscard]] std::string mapNameOrId(const int id) const { return nameOrId(mapName(id), id); }
  [[nodiscard]] std::string mapNameAndId(const int id) const { return nameAndId(mapName(id), id); }
  [[nodiscard]] std::string skillTypeNameOrId(const int id) const { return nameOrId(skillTypeName(id), id, 2); }
  [[nodiscard]] std::string skillTypeNameAndId(const int id) const { return nameAndId(skillTypeName(id), id, 2); }
  [[nodiscard]] std::string weaponTypeNameOrId(const int id) const { return nameOrId(weaponTypeName(id), id, 2); }
  [[nodiscard]] std::string elementNameOrId(const int id) const { return nameOrId(elementName(id), id, 2); }
  [[nodiscard]] std::string elementNameAndId(const int id) const { return nameAndId(elementName(id), id, 2); }
  [[nodiscard]] std::string armorTypeOrId(const int id) const { return nameOrId(armorTypeName(id), id, 2); }
  [[nodiscard]] std::string armorTypeAndId(const int id) const { return nameAndId(armorTypeName(id), id, 2); }
  [[nodiscard]] std::string equipTypeNameOrId(const int id) const { return nameOrId(equipTypeName(id), id, 2); }
  [[nodiscard]] std::string equipTypeNameAndId(const int id) const { return nameAndId(equipTypeName(id), id, 2); }

  [[nodiscard]] std::pair<Actor*, Class*> featureObjects(const int actorId) const {
    const auto actor = actors.actor(actorId);
    if (!actor) {
      return {};
    }

    return std::make_pair(const_cast<Actor*>(actor), const_cast<Class*>(classes.classType(actor->classId())));
  }

  [[nodiscard]] std::vector<const Trait*> allTraits(const int actorId) const {
    const auto [act, cls] = featureObjects(actorId);
    std::vector<const Trait*> ret;
    if (act) {
      for (auto& trait : act->traits()) {
        ret.push_back(&trait);
      }
    }
    if (cls) {
      for (auto& trait : cls->traits()) {
        ret.push_back(&trait);
      }
    }

    return ret;
  }

  [[nodiscard]] std::vector<const Trait*> traits(const int actorId, const TraitCode code) const {
    const auto traits = allTraits(actorId);
    std::vector<const Trait*> ret;
    ret.reserve(traits.size());
    for (auto& trait : traits) {
      if (trait->code() == code) {
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
      if (trait->code() == code) {
        ret.push_back(trait->dataId());
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

    if (const std::vector<int> set = traitsSet(actorId, TraitCode::Slot_Type); !set.empty()) {
      return *std::ranges::max_element(set);
    }
    return 0;
  }
  [[nodiscard]] bool isDualWield(const int actorId) const { return slotType(actorId) == 1; }

  [[nodiscard]] int slotIdToEquipId(const int actorId, const int slotId) const { return slotId == 1 && isDualWield(actorId) ? 1 : slotId + 1; }

  [[nodiscard]] std::vector<char> encryptionKeyAsBytes() const { return system.encryptionKeyAsBytes(); }

  static Database* instance() { return m_instance; }

  rpgmutils::signal<void()>& actorsLoaded() { return m_actorsLoaded; }
  rpgmutils::signal<void()>& classesLoaded() { return m_classesLoaded; }
  rpgmutils::signal<void()>& skillsLoaded() { return m_skillsLoaded; }
  rpgmutils::signal<void()>& itemsLoaded() { return m_itemsLoaded; }
  rpgmutils::signal<void()>& weaponsLoaded() { return m_weaponsLoaded; }
  rpgmutils::signal<void()>& armorsLoaded() { return m_armorsLoaded; }
  rpgmutils::signal<void()>& enemiesLoaded() { return m_enemiesLoaded; }
  rpgmutils::signal<void()>& troopsLoaded() { return m_troopsLoaded; }
  rpgmutils::signal<void()>& statesLoaded() { return m_statesLoaded; }
  rpgmutils::signal<void()>& animationsLoaded() { return m_animationsLoaded; }
  rpgmutils::signal<void()>& tilesetsLoaded() { return m_tilesetsLoaded; }
  rpgmutils::signal<void()>& commonEventsLoaded() { return m_commonEventsLoaded; }
  rpgmutils::signal<void()>& systemLoaded() { return m_systemLoaded; }
  rpgmutils::signal<void()>& pluginsLoaded() { return m_pluginsLoaded; }
  rpgmutils::signal<void()>& mapInfosLoaded() { return m_mapInfosLoaded; }
  rpgmutils::signal<void()>& gameConstantsLoaded() { return m_gameConstantsLoaded; }
  rpgmutils::signal<void()>& templatesLoaded() { return m_templatesLoaded; }
  rpgmutils::signal<void()>& docsLoaded() { return m_docsLoaded; }

private:
  rpgmutils::signal<void()> m_actorsLoaded;
  rpgmutils::signal<void()> m_classesLoaded;
  rpgmutils::signal<void()> m_skillsLoaded;
  rpgmutils::signal<void()> m_itemsLoaded;
  rpgmutils::signal<void()> m_weaponsLoaded;
  rpgmutils::signal<void()> m_armorsLoaded;
  rpgmutils::signal<void()> m_enemiesLoaded;
  rpgmutils::signal<void()> m_troopsLoaded;
  rpgmutils::signal<void()> m_statesLoaded;
  rpgmutils::signal<void()> m_animationsLoaded;
  rpgmutils::signal<void()> m_tilesetsLoaded;
  rpgmutils::signal<void()> m_commonEventsLoaded;
  rpgmutils::signal<void()> m_systemLoaded;
  rpgmutils::signal<void()> m_pluginsLoaded;
  rpgmutils::signal<void()> m_mapInfosLoaded;
  rpgmutils::signal<void()> m_gameConstantsLoaded;
  rpgmutils::signal<void()> m_templatesLoaded;
  rpgmutils::signal<void()> m_docsLoaded;
  static Database* m_instance;
};