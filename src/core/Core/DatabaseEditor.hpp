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

struct DatabaseEditor {
  DatabaseEditor() = delete;
  explicit DatabaseEditor(Actors& actors, Classes& classes, Skills& skills, Items& items, Weapons& weapons,
                          Armors& armors, Enemies& enemies, Troops& troops, States& states, Animations& animations,
                          Tilesets& tilesets, CommonEvents& commonEvents, System& system)
  : m_actors(actors)
  , m_classes(classes)
  , m_skills(skills)
  , m_items(items)
  , m_weapons(weapons)
  , m_armors(armors)
  , m_enemies(enemies)
  , m_troops(troops)
  , m_states(states)
  , m_animations(animations)
  , m_tilesets(tilesets)
  , m_commonEvents(commonEvents)
  , m_system(system) {
    m_currentTab = &m_actors;
  }

  void draw();

  void open() { m_isOpen = true; }
private:
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