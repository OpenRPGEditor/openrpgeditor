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

  void drawCategoryHeaders();
  void draw();

  void open() { m_isOpen = true; }

  MainWindow* project() { return m_parent; }
  const MainWindow* project() const { return m_parent; }

  void setActors(Actors& actors) {
    m_actors = std::make_shared<DBActorsTab>(actors, this);
    m_editorTabs.emplace_back(m_actors);
  }
  void setClasses(Classes& classes) {
    m_classes = std::make_shared<DBClassesTab>(classes, this);
    m_editorTabs.emplace_back(m_classes);
  }
  void setSkills(Skills& skills) {
    m_skills = std::make_shared<DBSkillsTab>(skills, this);
    m_editorTabs.emplace_back(m_skills);
  }
  void setItems(Items& items) {
    m_items = std::make_shared<DBItemsTab>(items, this);
    m_editorTabs.emplace_back(m_items);
  }
  void setWeapons(Weapons& weapons) {
    m_weapons = std::make_shared<DBWeaponsTab>(weapons, this);
    m_editorTabs.emplace_back(m_weapons);
  }
  void setArmors(Armors& armors) {
    m_armors = std::make_shared<DBArmorsTab>(armors, this);
    m_editorTabs.emplace_back(m_armors);
  }
  void setEnemies(Enemies& enemies) {
    m_enemies = std::make_shared<DBEnemiesTab>(enemies, this);
    m_editorTabs.emplace_back(m_enemies);
  }
  void setTroops(Troops& troops) {
    m_troops = std::make_shared<DBTroopsTab>(troops, this);
    m_editorTabs.emplace_back(m_troops);
  }
  void setStates(States& states) {
    m_states = std::make_shared<DBStatesTab>(states, this);
    m_editorTabs.emplace_back(m_states);
  }
  void setAnimations(Animations& animations) {
    m_animations = std::make_shared<DBAnimationsTab>(animations, this);
    m_editorTabs.emplace_back(m_animations);
  }
  void setTilesets(Tilesets& tilesets) {
    m_tilesets = std::make_shared<DBTilesetsTab>(tilesets, this);
    m_editorTabs.emplace_back(m_tilesets);
  }
  void setCommonEvents(CommonEvents& commonEvents) {
    m_commonEvents = std::make_shared<DBCommonEventsTab>(commonEvents, this);
    m_editorTabs.emplace_back(m_commonEvents);
  }
  void setSystem(System& system) {
    m_system = std::make_shared<DBSystemTab>(system, this);
    m_editorTabs.emplace_back(m_system);
    m_types = std::make_shared<DBTypesTab>(system, this);
    m_editorTabs.emplace_back(m_types);
    m_terms = std::make_shared<DBTermsTab>(system, this);
    m_editorTabs.emplace_back(m_terms);
    m_mappings = std::make_shared<DBMappingsTab>(system, this);
    m_editorTabs.emplace_back(m_mappings);
  }
  void setGameConstants(GameConstants& gameConstants) {
    m_gameConstants = std::make_shared<DBGameConstantsTab>(gameConstants, this);
    m_editorTabs.emplace_back(m_gameConstants);
  }
  void setTemplates(Templates& templates) {
    m_templates = std::make_shared<DBTemplatesTab>(templates, this);
    m_editorTabs.emplace_back(m_templates);
  }
  void setDocs(Docs& docs) {
    m_docs = std::make_shared<DBDocTab>(docs, this);
    m_editorTabs.emplace_back(m_docs);
  }
  void setLocales(Locales& locales) {
    m_locales = std::make_shared<DBLocaleTab>(locales, this);
    m_editorTabs.emplace_back(m_locales);
  }

  bool isReady() const {
    // TODO: Clean up initial database status so we can get rid of the individual class variables
    // return std::ranges::all_of(m_editorTabs, [](const auto& v) { return v->isReady(); });
    return m_actors && m_classes && m_skills && m_items && m_weapons && m_armors && m_enemies && m_troops && m_states && m_animations && m_tilesets && m_commonEvents && m_system && m_terms &&
           m_types && m_gameConstants && m_templates && m_docs && m_mappings && m_locales;
  }
  const IconSheet* getIconSheet();

  std::string& getFilterString() { return m_searchString; } // TODO: Search by string
  bool isFilteredByCategory() const { return m_filterByHeader && m_selectedHeaderIndex != -1; }

  bool isOpen() { return m_isOpen; }
  DBCommonEventsTab* commonEvents() { return m_commonEvents.get(); }
  IDBEditorTab* getCurrentTab() { return m_currentTab; }

  rpgmutils::signal<void()> onReady;

  void addTab(const std::shared_ptr<IDBEditorTab>& tab) { m_editorTabs.emplace_back(tab); }

private:
  MainWindow* m_parent;
  std::vector<std::shared_ptr<IDBEditorTab>> m_editorTabs;
  std::shared_ptr<DBActorsTab> m_actors;
  std::shared_ptr<DBClassesTab> m_classes;
  std::shared_ptr<DBSkillsTab> m_skills;
  std::shared_ptr<DBItemsTab> m_items;
  std::shared_ptr<DBWeaponsTab> m_weapons;
  std::shared_ptr<DBArmorsTab> m_armors;
  std::shared_ptr<DBEnemiesTab> m_enemies;
  std::shared_ptr<DBTroopsTab> m_troops;
  std::shared_ptr<DBStatesTab> m_states;
  std::shared_ptr<DBAnimationsTab> m_animations;
  std::shared_ptr<DBTilesetsTab> m_tilesets;
  std::shared_ptr<DBCommonEventsTab> m_commonEvents;
  std::shared_ptr<DBSystemTab> m_system;
  std::shared_ptr<DBTypesTab> m_types;
  std::shared_ptr<DBTermsTab> m_terms;
  std::shared_ptr<DBGameConstantsTab> m_gameConstants;
  std::shared_ptr<DBTemplatesTab> m_templates;
  std::shared_ptr<DBDocTab> m_docs;
  std::shared_ptr<DBLocaleTab> m_locales;
  std::shared_ptr<DBMappingsTab> m_mappings;

  std::optional<IconSheet> m_iconSheet;

  std::string m_searchString;
  bool m_filterByHeader{false};
  int m_selectedHeaderIndex{-1};
  std::vector<std::string> m_headers;

  IDBEditorTab* m_currentTab = nullptr;
  bool m_isOpen{};
};