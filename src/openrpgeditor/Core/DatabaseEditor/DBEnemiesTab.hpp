#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/Enemies.hpp"

struct Enemies;
struct DBEnemiesTab : IDBEditorTab {
  DBEnemiesTab() = delete;
  explicit DBEnemiesTab(Enemies& enemies, DatabaseEditor* parent) : IDBEditorTab(parent), m_enemies(enemies) {}
  void draw() override;

  [[nodiscard]] std::vector<Enemy>& enemies() { return m_enemies.enemies(); }
  [[nodiscard]] const std::vector<Enemy>& enemies() const { return m_enemies.enemies(); }

  [[nodiscard]] Enemy* enemy(int id) { return m_enemies.enemy(id); }
  [[nodiscard]] const Enemy* enemy(int id) const { return m_enemies.enemy(id); }

private:
  Enemies& m_enemies;
};