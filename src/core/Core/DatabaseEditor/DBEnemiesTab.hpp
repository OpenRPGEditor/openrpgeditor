#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"

struct Enemies;
struct DBEnemiesTab : IDBEditorTab {
  DBEnemiesTab() = delete;
  explicit DBEnemiesTab(Enemies& enemies) : m_enemies(enemies) {}
  void draw() override;

private:
  Enemies& m_enemies;
};