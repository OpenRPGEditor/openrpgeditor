#pragma once

#include "Core/DatabaseEditor/DBActorsTab.hpp"
#include "Core/DatabaseEditor/DBAnimationsTab.hpp"
#include "Core/DatabaseEditor/DBArmorsTab.hpp"
#include "Core/DatabaseEditor/DBClassesTab.hpp"
#include "Core/DatabaseEditor/DBCommonEventsTab.hpp"
#include "Core/DatabaseEditor/DBDocTab.hpp"
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

#include "DatabaseEditor/DBLocaleTab.hpp"
#include "DatabaseEditor/DBMappingsTab.hpp"

struct MainWindow;
class DatabaseEditor {
public:
  DatabaseEditor() = delete;
  explicit DatabaseEditor(MainWindow* parent)
  : m_parent(parent) {}

  void draw();

  void open() { m_isOpen = true; }

  MainWindow* project() { return m_parent; }
  const MainWindow* project() const { return m_parent; }

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
    m_mappings.emplace(system, this);
  }
  void setGameConstants(GameConstants& gameConstants) { m_gameConstants.emplace(gameConstants, this); }
  void setTemplates(Templates& templates) { m_templates.emplace(templates, this); }
  void setDocs(Docs& docs) { m_docs.emplace(docs, this); }
  void setLocales(Locales& locales) { m_locales.emplace(locales, this); }

  bool isReady() const {
    return m_actors && m_classes && m_skills && m_items && m_weapons && m_armors && m_enemies && m_troops && m_states && m_animations && m_tilesets && m_commonEvents && m_system && m_terms &&
           m_types && m_gameConstants && m_templates && m_docs && m_mappings && m_locales;
  }
  const IconSheet* getIconSheet();

  std::string& getFilterString() { return m_searchString; } // TODO: Search by string
  bool isFilteredByCategory() const { return m_filterByHeader && m_selectedHeaderIndex != -1; }

  bool isOpen() { return m_isOpen; }
  DBCommonEventsTab* commonEvents() { return m_commonEvents.has_value() ? &m_commonEvents.value() : nullptr; }
  IDBEditorTab* getCurrentTab() { return m_currentTab; }

  rpgmutils::signal<void()> onReady;

private:
  MainWindow* m_parent;
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
  std::optional<DBDocTab> m_docs;
  std::optional<DBLocaleTab> m_locales;
  std::optional<DBMappingsTab> m_mappings;
  std::optional<IconSheet> m_iconSheet;

  std::string m_searchString;
  bool m_filterByHeader{false};
  int m_selectedHeaderIndex{-1};
  std::vector<std::string> m_headers;

  IDBEditorTab* m_currentTab = nullptr;
  bool m_isOpen{};
};