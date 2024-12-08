#pragma once

#include "Core/DatabaseEditor/DBActorsTab.hpp"
#include "Core/DatabaseEditor/DBAnimationsTab.hpp"
#include "Core/DatabaseEditor/DBArmorsTab.hpp"
#include "Core/DatabaseEditor/DBClassesTab.hpp"
#include "Core/DatabaseEditor/DBCommonEventsTab.hpp"
#include "Core/DatabaseEditor/DBEnemiesTab.hpp"
#include "Core/DatabaseEditor/DBGameConstantsTab.hpp"
#include "Core/DatabaseEditor/DBItemsTab.hpp"
#include "Core/DatabaseEditor/DBSkillsTab.hpp"
#include "Core/DatabaseEditor/DBStatesTab.hpp"
#include "Core/DatabaseEditor/DBSystemTab.hpp"
#include "Core/DatabaseEditor/DBTemplatesTab.hpp"
#include "Core/DatabaseEditor/DBTermsTab.hpp"
#include "Core/DatabaseEditor/DBTilesetsTab.hpp"
#include "Core/DatabaseEditor/DBTroopsTab.hpp"
#include "Core/DatabaseEditor/DBTypesTab.hpp"
#include "Core/DatabaseEditor/DBWeaponsTab.hpp"
#include "Core/Graphics/IconSheet.hpp"

#include "Database/Class.hpp"

struct Project;
class DatabaseEditor {
public:
  DatabaseEditor() = delete;
  explicit DatabaseEditor(Project* parent) : m_parent(parent) {}

  void draw();

  void open() { m_isOpen = true; }

  [[nodiscard]] std::string* element(const int id) { return m_system->element(id); }
  [[nodiscard]] const std::string* element(const int id) const { return m_system->element(id); }

  [[nodiscard]] std::vector<std::string>& elements() { return m_system->elements(); }
  [[nodiscard]] const std::vector<std::string>& elements() const { return m_system->elements(); }

  [[nodiscard]] size_t elementsCount() const { return m_system->elementsCount(); }

  [[nodiscard]] State* state(const int id) { return m_states->state(id); }
  [[nodiscard]] const State* state(const int id) const { return m_states->state(id); }

  [[nodiscard]] States& states() { return m_states->states(); }
  [[nodiscard]] const States& states() const { return m_states->states(); }

  [[nodiscard]] Actors& actors() { return m_actors->actors(); }
  [[nodiscard]] const Actors& actors() const { return m_actors->actors(); }

  [[nodiscard]] Actor* actor(const int id) { return m_actors->actor(id); }
  [[nodiscard]] const Actor* actor(const int id) const { return m_actors->actor(id); }

  [[nodiscard]] Classes& classes() { return m_classes->classes(); }
  [[nodiscard]] const Classes& classes() const { return m_classes->classes(); }

  [[nodiscard]] Class* classType(const int id) { return m_classes->classType(id); }
  [[nodiscard]] const Class* classType(const int id) const { return m_classes->classType(id); }

  [[nodiscard]] std::string variables(const int id) { return m_system->variable(id); }
  [[nodiscard]] const std::string variables(const int id) const { return m_system->variable(id); }

  [[nodiscard]] std::string& switches(const int id) { return m_system->switche(id); }
  [[nodiscard]] const std::string& switches(const int id) const { return m_system->switche(id); }

  [[nodiscard]] std::vector<std::string>& skillTypes() { return m_system->skillTypes(); }
  [[nodiscard]] const std::vector<std::string>& skillTypes() const { return m_system->skillTypes(); }

  [[nodiscard]] std::string* skillType(const int id) { return m_system->skillType(id); }
  [[nodiscard]] const std::string* skillType(const int id) const { return m_system->skillType(id); }

  [[nodiscard]] std::vector<std::string>& equipTypes() { return m_system->equipTypes(); }
  [[nodiscard]] const std::vector<std::string>& equipTypes() const { return m_system->equipTypes(); }

  [[nodiscard]] std::string* equipType(const int id) { return m_system->equipType(id); }
  [[nodiscard]] const std::string* equipType(const int id) const { return m_system->equipType(id); }

  [[nodiscard]] const std::optional<std::string>& basic(const int id) const { return m_system->basic(id); }

  [[nodiscard]] const std::optional<std::string>& command(const int id) const { return m_system->command(id); }

  [[nodiscard]] Messages& messages() { return m_system->messages(); }
  [[nodiscard]] const Messages& messages() const { return m_system->messages(); }

  [[nodiscard]] std::vector<Skill>& skills() { return m_skills->skills(); }
  [[nodiscard]] const std::vector<Skill>& skills() const { return m_skills->skills(); }

  [[nodiscard]] Skill* skill(const int id) { return m_skills->skill(id); }
  [[nodiscard]] const Skill* skill(const int id) const { return m_skills->skill(id); }

  [[nodiscard]] std::vector<Item>& items() { return m_items->items(); }
  [[nodiscard]] const std::vector<Item>& items() const { return m_items->items(); }

  [[nodiscard]] Item* item(const int id) { return m_items->item(id); }
  [[nodiscard]] const Item* item(const int id) const { return m_items->item(id); }

  [[nodiscard]] std::vector<Weapon>& weapons() { return m_weapons->weapons(); }
  [[nodiscard]] const std::vector<Weapon>& weapons() const { return m_weapons->weapons(); }

  [[nodiscard]] Weapon* weapon(const int id) { return m_weapons->weapon(id); }
  [[nodiscard]] const Weapon* weapon(const int id) const { return m_weapons->weapon(id); }

  [[nodiscard]] std::vector<Armor>& armors() { return m_armors->armors(); }
  [[nodiscard]] const std::vector<Armor>& armors() const { return m_armors->armors(); }

  [[nodiscard]] Armor* armor(const int id) { return m_armors->armor(id); }
  [[nodiscard]] const Armor* armor(const int id) const { return m_armors->armor(id); }

  [[nodiscard]] std::vector<Enemy>& enemies() { return m_enemies->enemies(); }
  [[nodiscard]] const std::vector<Enemy>& enemies() const { return m_enemies->enemies(); }

  [[nodiscard]] Enemy* enemy(const int id) { return m_enemies->enemy(id); }
  [[nodiscard]] const Enemy* enemy(const int id) const { return m_enemies->enemy(id); }

  [[nodiscard]] std::vector<Troop>& troops() { return m_troops->troops(); }
  [[nodiscard]] const std::vector<Troop>& troops() const { return m_troops->troops(); }

  [[nodiscard]] Troop* troop(const int id) { return m_troops->troop(id); }
  [[nodiscard]] const Troop* troop(const int id) const { return m_troops->troop(id); }

  [[nodiscard]] std::vector<Animation>& animations() { return m_animations->animations(); }
  [[nodiscard]] const std::vector<Animation>& animations() const { return m_animations->animations(); }

  [[nodiscard]] Animation* animation(const int id) { return m_animations->animation(id); }
  [[nodiscard]] const Animation* animation(const int id) const { return m_animations->animation(id); }

  [[nodiscard]] std::vector<Tileset>& tilesets() { return m_tilesets->tilesets(); }
  [[nodiscard]] const std::vector<Tileset>& tilesets() const { return m_tilesets->tilesets(); }

  [[nodiscard]] Tileset* tileset(const int id) { return m_tilesets->tileset(id); }
  [[nodiscard]] const Tileset* tileset(const int id) const { return m_tilesets->tileset(id); }

  [[nodiscard]] std::vector<std::optional<CommonEvent>>& commonEvents() { return m_commonEvents->events(); }
  [[nodiscard]] const std::vector<std::optional<CommonEvent>>& commonEvents() const { return m_commonEvents->events(); }

  [[nodiscard]] CommonEvent* commonEvent(const int id) { return m_commonEvents->event(id); }
  [[nodiscard]] const CommonEvent* commonEvent(const int id) const { return m_commonEvents->event(id); }

  [[nodiscard]] std::string* armorType(const int id) { return m_system->armorType(id); }
  [[nodiscard]] const std::string* armorType(const int id) const { return m_system->armorType(id); }

  [[nodiscard]] std::string* weaponType(const int id) { return m_system->weaponType(id); }
  [[nodiscard]] const std::string* weaponType(const int id) const { return m_system->weaponType(id); }

  Project* project() { return m_parent; }
  const Project* project() const { return m_parent; }

  void setActors(Actors& actors) { m_actors.emplace(actors, this); }
  void setClasses(Classes& classes) { m_classes.emplace(classes, this); }
  void setSkills(Skills& skills) { m_skills.emplace(skills, this); }
  void setItems(Items& items) { m_items.emplace(items, this); }
  void setWeapons(Weapons& weapons) { m_weapons.emplace(weapons, this); }
  void setArmors(Armors& armors) { m_armors.emplace(armors, this); }
  void setEnemies(Enemies& enemies) { m_enemies.emplace(enemies, this); }
  void setTroops(Troops& troops) { m_troops.emplace(troops, this); }
  void setStates(States& states) { m_states.emplace(states, this); }
  void setAnimations(Animations& animations) { m_animations.emplace(animations, this); }
  void setTilesets(Tilesets& tilesets) { m_tilesets.emplace(tilesets, this); }
  void setCommonEvents(CommonEvents& commonEvents) { m_commonEvents.emplace(commonEvents, this); }
  void setSystem(System& system) {
    m_system.emplace(system, this);
    m_types.emplace(system, this);
    m_terms.emplace(system, this);
  }
  void setGameConstants(GameConstants& gameConstants) { m_gameConstants.emplace(gameConstants, this); }
  void setTemplates(Templates& templates) { m_templates.emplace(templates, this); }

  bool isReady() const {
    return m_actors && m_classes && m_skills && m_items && m_weapons && m_armors && m_enemies && m_troops && m_states && m_animations && m_tilesets && m_commonEvents && m_system && m_terms &&
           m_types && m_gameConstants && m_templates;
  }
  const IconSheet* getIconSheet();

  signal<void()> onReady;

private:
  Project* m_parent;
  std::optional<DBActorsTab> m_actors;
  std::optional<DBClassesTab> m_classes;
  std::optional<DBSkillsTab> m_skills;
  std::optional<DBItemsTab> m_items;
  std::optional<DBWeaponsTab> m_weapons;
  std::optional<DBArmorsTab> m_armors;
  std::optional<DBEnemiesTab> m_enemies;
  std::optional<DBTroopsTab> m_troops;
  std::optional<DBStatesTab> m_states;
  std::optional<DBAnimationsTab> m_animations;
  std::optional<DBTilesetsTab> m_tilesets;
  std::optional<DBCommonEventsTab> m_commonEvents;
  std::optional<DBSystemTab> m_system;
  std::optional<DBTypesTab> m_types;
  std::optional<DBTermsTab> m_terms;
  std::optional<DBGameConstantsTab> m_gameConstants;
  std::optional<DBTemplatesTab> m_templates;
  std::optional<IconSheet> m_iconSheet;
  IDBEditorTab* m_currentTab = nullptr;
  bool m_isOpen{};
};