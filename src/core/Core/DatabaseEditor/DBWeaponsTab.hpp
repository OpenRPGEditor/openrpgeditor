#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/Weapons.hpp"
struct Weapons;
struct DBWeaponsTab : IDBEditorTab {
  DBWeaponsTab() = delete;
  explicit DBWeaponsTab(Weapons& system, DatabaseEditor* parent) : IDBEditorTab(parent), m_weapons(system) {}
  void draw() override;

  [[nodiscard]] std::vector<Weapon>& weapons() { return m_weapons.weapons(); }
  [[nodiscard]] const std::vector<Weapon>& weapons() const { return m_weapons.weapons(); }
  [[nodiscard]] Weapon* weapon(int id) { return m_weapons.weapon(id); }
  [[nodiscard]] const Weapon* weapon(int id) const { return m_weapons.weapon(id); }

private:
  Weapons& m_weapons;
};