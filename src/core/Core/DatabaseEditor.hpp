#pragma once

struct Actors;
struct IDBEditorTab {
  virtual ~IDBEditorTab() = default;
  virtual void draw() = 0;
};

struct DBActorsTab : IDBEditorTab {
  DBActorsTab() = delete;
  explicit DBActorsTab(Actors& system) : m_actors(system) {}
  void draw() override;

private:
  Actors& m_actors;
};

struct Classes;
struct DBClassesTab : IDBEditorTab {
  DBClassesTab() = delete;
  explicit DBClassesTab(Classes& classes) : m_classes(classes) {}
  void draw() override;

private:
  Classes& m_classes;
};
struct Skills;

struct DBSkillsTab : IDBEditorTab {
  DBSkillsTab() = delete;
  explicit DBSkillsTab(Skills& system) : m_skills(system) {}
  void draw() override;

private:
  Skills& m_skills;
};

struct Items;
struct DBItemsTab : IDBEditorTab {
  DBItemsTab() = delete;
  explicit DBItemsTab(Items& items) : m_items(items) {}
  void draw() override;

private:
  Items& m_items;
};

struct Weapons;
struct DBWeaponsTab : IDBEditorTab {
  DBWeaponsTab() = delete;
  explicit DBWeaponsTab(Weapons& system) : m_weapons(system) {}
  void draw() override;

private:
  Weapons& m_weapons;
};

struct Armors;
struct DBArmorsTab : IDBEditorTab {
  DBArmorsTab() = delete;
  explicit DBArmorsTab(Armors& armors) : m_armors(armors) {}
  void draw() override;

private:
  Armors& m_armors;
};

struct Enemies;
struct DBEnemiesTab : IDBEditorTab {
  DBEnemiesTab() = delete;
  explicit DBEnemiesTab(Enemies& enemies) : m_enemies(enemies) {}
  void draw() override;

private:
  Enemies& m_enemies;
};
struct Troops;
struct DBTroopsTab : IDBEditorTab {
  DBTroopsTab() = delete;
  explicit DBTroopsTab(Troops& troops) : m_troops(troops) {}
  void draw() override;

private:
  Troops& m_troops;
};

struct States;
struct DBStatesTab : IDBEditorTab {
  DBStatesTab() = delete;
  explicit DBStatesTab(States& system) : m_states(system) {}
  void draw() override;

private:
  States& m_states;
};

struct Animations;
struct DBAnimationsTab : IDBEditorTab {
  DBAnimationsTab() = delete;
  explicit DBAnimationsTab(Animations& system) : m_animations(system) {}
  void draw() override;

private:
  Animations& m_animations;
};

struct Tilesets;
struct DBTilesetsTab : IDBEditorTab {
  DBTilesetsTab() = delete;
  explicit DBTilesetsTab(Tilesets& tilesets) : m_tilesets(tilesets) {}
  void draw() override;

private:
  Tilesets& m_tilesets;
};

struct CommonEvents;
struct DBCommonEventsTab : IDBEditorTab {
  DBCommonEventsTab() = delete;
  explicit DBCommonEventsTab(CommonEvents& events) : m_events(events) {}
  void draw() override;

private:
  CommonEvents& m_events;
};
struct System;
struct DBSystemTab : IDBEditorTab {
  DBSystemTab() = delete;
  explicit DBSystemTab(System& system) : m_system(system) {}
  void draw() override;

private:
  System& m_system;
};

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