#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/Skills.hpp"

struct DBSkillsTab : IDBEditorTab {
  DBSkillsTab() = delete;
  explicit DBSkillsTab(Skills& system, DatabaseEditor* parent) : IDBEditorTab(parent), m_skills(system) {}
  void draw() override;

  [[nodiscard]] std::vector<Skill>& skills() { return m_skills.skills(); }
  [[nodiscard]] const std::vector<Skill>& skills() const { return m_skills.skills(); }
  [[nodiscard]] Skill* skill(int id) { return m_skills.skill(id); }
  [[nodiscard]] const Skill* skill(int id) const { return m_skills.skill(id); }
private:
  Skills& m_skills;
};