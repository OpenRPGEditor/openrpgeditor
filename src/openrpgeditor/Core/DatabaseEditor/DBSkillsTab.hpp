#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/Graphics/IconSheet.hpp"
#include "Database/Animation.hpp"
#include "Database/Skills.hpp"

struct DBSkillsTab : IDBEditorTab {
  DBSkillsTab() = delete;
  explicit DBSkillsTab(Skills& system, DatabaseEditor* parent);
  void draw() override;

  [[nodiscard]] std::vector<Skill>& skills() { return m_skills.skills(); }
  [[nodiscard]] const std::vector<Skill>& skills() const { return m_skills.skills(); }
  [[nodiscard]] Skill* skill(int id) { return m_skills.skill(id); }
  [[nodiscard]] const Skill* skill(int id) const { return m_skills.skill(id); }

private:
  Skills& m_skills;
  Skill* m_selectedSkill{};
  int m_maxSkills{};
  int m_editMaxSkills{};
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
  int m_message_template{0};
  std::vector<std::string> m_message_templateList{"casts %1!", "does %1!", "uses %1"};
  std::optional<ObjectPicker<Animation>> m_animationPicker;
};