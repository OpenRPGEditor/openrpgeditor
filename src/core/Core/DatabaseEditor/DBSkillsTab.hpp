#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"

struct Skills;
struct DBSkillsTab : IDBEditorTab {
  DBSkillsTab() = delete;
  explicit DBSkillsTab(Skills& system) : m_skills(system) {}
  void draw() override;

private:
  Skills& m_skills;
};