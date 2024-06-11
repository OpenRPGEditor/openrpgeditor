#pragma once

#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/DatabaseEditor/DBActorsTab.hpp"
#include "Core/DatabaseEditor/DBClassesTab.hpp"
#include "Core/DatabaseEditor/DBSkillsTab.hpp"
#include "Core/DatabaseEditor/DBItemsTab.hpp"
#include "Core/DatabaseEditor/DBWeaponsTab.hpp"
#include "Core/DatabaseEditor/DBArmorsTab.hpp"
#include "Core/DatabaseEditor/DBEnemiesTab.hpp"
#include "Core/DatabaseEditor/DBTroopsTab.hpp"
#include "Core/DatabaseEditor/DBStatesTab.hpp"
#include "Core/DatabaseEditor/DBAnimationsTab.hpp"
#include "Core/DatabaseEditor/DBTilesetsTab.hpp"
#include "Core/DatabaseEditor/DBCommonEventsTab.hpp"
#include "Core/DatabaseEditor/DBSystemTab.hpp"
#include "Database/Classes.hpp"

struct Project;
struct DatabaseEditor {
  DatabaseEditor() = delete;
  explicit DatabaseEditor(Project* parent, Actors& actors, Classes& classes, Skills& skills, Items& items,
                          Weapons& weapons, Armors& armors, Enemies& enemies, Troops& troops, States& states,
                          Animations& animations, Tilesets& tilesets, CommonEvents& commonEvents, System& system)
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
  , m_system(system, this) {
    m_currentTab = &m_actors;
  }

  void draw();

  void open() { m_isOpen = true; }

  [[nodiscard]] std::string& element(int id) { return m_system.element(id); }
  [[nodiscard]] const std::string& element(int id) const { return m_system.element(id); }

  [[nodiscard]] std::vector<std::string>& elements(int id) { return m_system.elements(); }
  [[nodiscard]] const std::vector<std::string>& elements(int id) const { return m_system.elements(); }

  [[nodiscard]] size_t elementsCount() const { return m_system.elementsCount(); }

  [[nodiscard]] State* state(int id) { return m_states.state(id); }
  [[nodiscard]] const State* state(int id) const { return m_states.state(id); }

  [[nodiscard]] Classes& classes() { return m_classes.classes(); }
  [[nodiscard]] const Classes& classes() const { return m_classes.classes(); }

  [[nodiscard]] std::string variables(int id) { return m_system.variable(id); }
  [[nodiscard]] const std::string variables(int id) const { return m_system.variable(id); }

  [[nodiscard]] std::string& switches(int id) { return m_system.switche(id); }
  [[nodiscard]] const std::string& switches(int id) const { return m_system.switche(id); }

  [[nodiscard]] std::vector<std::string>& skillTypes() { return m_system.skillTypes(); }
  [[nodiscard]] const std::vector<std::string>& skillTypes() const { return m_system.skillTypes(); }

  [[nodiscard]] std::string& skillType(int id) { return m_system.skillType(id); }
  [[nodiscard]] const std::string& skillType(int id) const { return m_system.skillType(id); }

  [[nodiscard]] std::vector<std::string>& equipTypes() { return m_system.equipTypes(); }
  [[nodiscard]] const std::vector<std::string>& equipTypes() const { return m_system.equipTypes(); }

  [[nodiscard]] std::string& equipType(int id) { return m_system.equipType(id); }
  [[nodiscard]] const std::string& equipType(int id) const { return m_system.equipType(id); }

  [[nodiscard]] const std::optional<std::string>& basic(int id) const { return m_system.basic(id); }

  [[nodiscard]] const std::optional<std::string>& command(int id) const { return m_system.command(id); }

  [[nodiscard]] Messages& messages() { return m_system.messages(); }
  [[nodiscard]] const Messages& messages() const { return m_system.messages(); }

  [[nodiscard]] std::vector<Weapon>& weapons() { return m_weapons.weapons(); }
  [[nodiscard]] const std::vector<Weapon>& weapons() const { return m_weapons.weapons(); }

  [[nodiscard]] Weapon* weapon(int id) { return m_weapons.weapon(id); }
  [[nodiscard]] const Weapon* weapon(int id) const { return m_weapons.weapon(id); }

  [[nodiscard]] std::vector<Armor>& armors() { return m_armors.armors(); }
  [[nodiscard]] const std::vector<Armor>& armors() const { return m_armors.armors(); }

  [[nodiscard]] Armor* armor(int id) { return m_armors.armor(id); }
  [[nodiscard]] const Armor* armor(int id) const { return m_armors.armor(id); }

  [[nodiscard]] std::string& armorType(int id) { return m_system.armorType(id); }
  [[nodiscard]] const std::string& armorType(int id) const { return m_system.armorType(id); }

  [[nodiscard]] std::string& weaponType(int id) { return m_system.weaponType(id); }
  [[nodiscard]] const std::string& weaponType(int id) const { return m_system.weaponType(id); }

  [[nodiscard]] Skill* skill(int id) { return m_skills.skill(id); }
  [[nodiscard]] const Skill* skill(int id) const { return m_skills.skill(id); }

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
  IDBEditorTab* m_currentTab;
  bool m_isOpen{};
};