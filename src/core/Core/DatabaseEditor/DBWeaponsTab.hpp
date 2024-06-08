#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"

struct Weapons;
struct DBWeaponsTab : IDBEditorTab {
  DBWeaponsTab() = delete;
  explicit DBWeaponsTab(Weapons& system) : m_weapons(system) {}
  void draw() override;

private:
  Weapons& m_weapons;
};