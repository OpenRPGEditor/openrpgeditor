#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"

struct Enemies;
struct DBEnemiesTab : IDBEditorTab {
  DBEnemiesTab() = delete;
  explicit DBEnemiesTab(Enemies& enemies, DatabaseEditor* parent) : IDBEditorTab(parent), m_enemies(enemies) {}
  void draw() override;

private:
  Enemies& m_enemies;
};