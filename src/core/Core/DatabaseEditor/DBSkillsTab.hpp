#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"

struct Skills;
struct DBSkillsTab : IDBEditorTab {
  DBSkillsTab() = delete;
  explicit DBSkillsTab(Skills& system, DatabaseEditor* parent) : IDBEditorTab(parent), m_skills(system) {}
  void draw() override;

private:
  Skills& m_skills;
};