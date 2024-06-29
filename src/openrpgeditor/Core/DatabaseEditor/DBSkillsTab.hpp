#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/Skills.hpp"

struct DBSkillsTab : IDBEditorTab {
  DBSkillsTab() = delete;
  explicit DBSkillsTab(Skills& system, DatabaseEditor* parent) : IDBEditorTab(parent), m_skills(system) {}
  void draw() override;

  [[nodiscard]] Skill* skill(int id) { return m_skills.skill(id); }
  [[nodiscard]] const Skill* skill(int id) const { return m_skills.skill(id); }
private:
  Skills& m_skills;
};