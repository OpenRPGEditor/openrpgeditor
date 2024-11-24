#pragma once

#include "Core/DatabaseEditor/DBActorsTab.hpp"
#include "Core/DatabaseEditor/DBAnimationsTab.hpp"
#include "Core/DatabaseEditor/DBArmorsTab.hpp"
#include "Core/DatabaseEditor/DBClassesTab.hpp"
#include "Core/DatabaseEditor/DBCommonEventsTab.hpp"
#include "Core/DatabaseEditor/DBEnemiesTab.hpp"
#include "Core/DatabaseEditor/DBItemsTab.hpp"
#include "Core/DatabaseEditor/DBSkillsTab.hpp"
#include "Core/DatabaseEditor/DBStatesTab.hpp"
#include "Core/DatabaseEditor/DBSystemTab.hpp"
#include "Core/DatabaseEditor/DBTemplatesTab.hpp"
#include "Core/DatabaseEditor/DBTilesetsTab.hpp"
#include "Core/DatabaseEditor/DBTroopsTab.hpp"
#include "Core/DatabaseEditor/DBWeaponsTab.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/Classes.hpp"
#include "DatabaseEditor/DBGameConstantsTab.hpp"

struct Project;
struct DatabaseEditor {
  DatabaseEditor() = delete;
  explicit DatabaseEditor(Project* parent, Actors& actors, Classes& classes, Skills& skills, Items& items, Weapons& weapons, Armors& armors, Enemies& enemies, Troops& troops, States& states,
                          Animations& animations, Tilesets& tilesets, CommonEvents& commonEvents, System& system, GameConstants& constants, Templates& templates)
  : m_parent(parent)
  , m_actors(actors, this)
  , m_classes(classes, this)
  , m_skills(skills, this)
  , m_items(items, this)
  , m_weapons(weapons, this)
  , m_armors(armors, this)
  , m_enemies(enemies, this)
  , m_troops(troops, this)
  , m_states(states, this)
  , m_animations(animations, this)
  , m_tilesets(tilesets, this)
  , m_commonEvents(commonEvents, this)
  , m_system(system, this)
  , m_gameConstants(constants, this)
  , m_templates(templates, this) {
    m_currentTab = &m_actors;
  }

  void draw();

  void open() { m_isOpen = true; }

  [[nodiscard]] std::string* element(int id) { return m_system.element(id); }
  [[nodiscard]] const std::string* element(int id) const { return m_system.element(id); }

  [[nodiscard]] std::vector<std::string>& elements() { return m_system.elements(); }
  [[nodiscard]] const std::vector<std::string>& elements() const { return m_system.elements(); }

  [[nodiscard]] size_t elementsCount() const { return m_system.elementsCount(); }

  [[nodiscard]] State* state(int id) { return m_states.state(id); }
  [[nodiscard]] const State* state(int id) const { return m_states.state(id); }

  [[nodiscard]] States& states() { return m_states.states(); }
  [[nodiscard]] const States& states() const { return m_states.states(); }

  [[nodiscard]] Actors& actors() { return m_actors.actors(); }
  [[nodiscard]] const Actors& actors() const { return m_actors.actors(); }

  [[nodiscard]] Actor* actor(int id) { return m_actors.actor(id); }
  [[nodiscard]] const Actor* actor(int id) const { return m_actors.actor(id); }

  [[nodiscard]] Classes& classes() { return m_classes.classes(); }
  [[nodiscard]] const Classes& classes() const { return m_classes.classes(); }

  [[nodiscard]] Class* classType(int id) { return m_classes.classType(id); }
  [[nodiscard]] const Class* classType(int id) const { return m_classes.classType(id); }

  [[nodiscard]] std::string variables(int id) { return m_system.variable(id); }
  [[nodiscard]] const std::string variables(int id) const { return m_system.variable(id); }

  [[nodiscard]] std::string& switches(int id) { return m_system.switche(id); }
  [[nodiscard]] const std::string& switches(int id) const { return m_system.switche(id); }

  [[nodiscard]] std::vector<std::string>& skillTypes() { return m_system.skillTypes(); }
  [[nodiscard]] const std::vector<std::string>& skillTypes() const { return m_system.skillTypes(); }

  [[nodiscard]] std::string* skillType(int id) { return m_system.skillType(id); }
  [[nodiscard]] const std::string* skillType(int id) const { return m_system.skillType(id); }

  [[nodiscard]] std::vector<std::string>& equipTypes() { return m_system.equipTypes(); }
  [[nodiscard]] const std::vector<std::string>& equipTypes() const { return m_system.equipTypes(); }

  [[nodiscard]] std::string* equipType(int id) { return m_system.equipType(id); }
  [[nodiscard]] const std::string* equipType(int id) const { return m_system.equipType(id); }

  [[nodiscard]] const std::optional<std::string>& basic(int id) const { return m_system.basic(id); }

  [[nodiscard]] const std::optional<std::string>& command(int id) const { return m_system.command(id); }

  [[nodiscard]] Messages& messages() { return m_system.messages(); }
  [[nodiscard]] const Messages& messages() const { return m_system.messages(); }

  [[nodiscard]] std::vector<Skill>& skills() { return m_skills.skills(); }
  [[nodiscard]] const std::vector<Skill>& skills() const { return m_skills.skills(); }

  [[nodiscard]] Skill* skill(int id) { return m_skills.skill(id); }
  [[nodiscard]] const Skill* skill(int id) const { return m_skills.skill(id); }

  [[nodiscard]] std::vector<Item>& items() { return m_items.items(); }
  [[nodiscard]] const std::vector<Item>& items() const { return m_items.items(); }

  [[nodiscard]] Item* item(int id) { return m_items.item(id); }
  [[nodiscard]] const Item* item(int id) const { return m_items.item(id); }

  [[nodiscard]] std::vector<Weapon>& weapons() { return m_weapons.weapons(); }
  [[nodiscard]] const std::vector<Weapon>& weapons() const { return m_weapons.weapons(); }

  [[nodiscard]] Weapon* weapon(int id) { return m_weapons.weapon(id); }
  [[nodiscard]] const Weapon* weapon(int id) const { return m_weapons.weapon(id); }

  [[nodiscard]] std::vector<Armor>& armors() { return m_armors.armors(); }
  [[nodiscard]] const std::vector<Armor>& armors() const { return m_armors.armors(); }

  [[nodiscard]] Armor* armor(int id) { return m_armors.armor(id); }
  [[nodiscard]] const Armor* armor(int id) const { return m_armors.armor(id); }

  [[nodiscard]] std::vector<Enemy>& enemies() { return m_enemies.enemies(); }
  [[nodiscard]] const std::vector<Enemy>& enemies() const { return m_enemies.enemies(); }

  [[nodiscard]] Enemy* enemy(int id) { return m_enemies.enemy(id); }
  [[nodiscard]] const Enemy* enemy(int id) const { return m_enemies.enemy(id); }

  [[nodiscard]] std::vector<Troop>& troops() { return m_troops.troops(); }
  [[nodiscard]] const std::vector<Troop>& troops() const { return m_troops.troops(); }

  [[nodiscard]] Troop* troop(int id) { return m_troops.troop(id); }
  [[nodiscard]] const Troop* troop(int id) const { return m_troops.troop(id); }

  [[nodiscard]] std::vector<Animation>& animations() { return m_animations.animations(); }
  [[nodiscard]] const std::vector<Animation>& animations() const { return m_animations.animations(); }

  [[nodiscard]] Animation* animation(int id) { return m_animations.animation(id); }
  [[nodiscard]] const Animation* animation(int id) const { return m_animations.animation(id); }

  [[nodiscard]] std::vector<Tileset>& tilesets() { return m_tilesets.tilesets(); }
  [[nodiscard]] const std::vector<Tileset>& tilesets() const { return m_tilesets.tilesets(); }

  [[nodiscard]] Tileset* tileset(int id) { return m_tilesets.tileset(id); }
  [[nodiscard]] const Tileset* tileset(int id) const { return m_tilesets.tileset(id); }

  [[nodiscard]] std::vector<std::optional<CommonEvent>>& commonEvents() { return m_commonEvents.events(); }
  [[nodiscard]] const std::vector<std::optional<CommonEvent>>& commonEvents() const { return m_commonEvents.events(); }

  [[nodiscard]] CommonEvent* commonEvent(int id) { return m_commonEvents.event(id); }
  [[nodiscard]] const CommonEvent* commonEvent(int id) const { return m_commonEvents.event(id); }

  [[nodiscard]] std::string* armorType(int id) { return m_system.armorType(id); }
  [[nodiscard]] const std::string* armorType(int id) const { return m_system.armorType(id); }

  [[nodiscard]] std::string* weaponType(int id) { return m_system.weaponType(id); }
  [[nodiscard]] const std::string* weaponType(int id) const { return m_system.weaponType(id); }

  Project* project() { return m_parent; }
  const Project* project() const { return m_parent; }

private:
  Project* m_parent;
  DBActorsTab m_actors;
  DBClassesTab m_classes;
  DBSkillsTab m_skills;
  DBItemsTab m_items;
  DBWeaponsTab m_weapons;
  DBArmorsTab m_armors;
  DBEnemiesTab m_enemies;
  DBTroopsTab m_troops;
  DBStatesTab m_states;
  DBAnimationsTab m_animations;
  DBTilesetsTab m_tilesets;
  DBCommonEventsTab m_commonEvents;
  DBSystemTab m_system;
  DBGameConstantsTab m_gameConstants;
  DBTemplatesTab m_templates;
  IDBEditorTab* m_currentTab;
  bool m_isOpen{};
};