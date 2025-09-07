#pragma once
#include "Core/CommonUI/EffectsEditor.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/DatabaseEditor/DBCommonEventsTab.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/Graphics/CheckeredCompositeTexture.hpp"

#include "Database/Animation.hpp"
#include "Database/Skills.hpp"

class DBSkillsTab final : public IDBEditorTab {
public:
  DBSkillsTab() = delete;
  explicit DBSkillsTab(Skills& skills, DatabaseEditor* parent);
  void draw() override;

  [[nodiscard]] std::vector<Skill>& skills() { return m_skills.skills(); }
  [[nodiscard]] const std::vector<Skill>& skills() const { return m_skills.skills(); }
  [[nodiscard]] Skill* skill(const int id) { return m_skills.skill(id); }
  [[nodiscard]] const Skill* skill(const int id) const { return m_skills.skill(id); }
  std::vector<int>& getHeaders() override { return m_headers; }
  int getHeader(int index) override { return m_headers.at(index); }
  bool hasHeader() override { return !m_headers.empty(); }
  void setHeaderRange(int start, int end) override {
    m_categoryStart = start;
    m_categoryEnd = end;
  }

  std::string getName(int index) override { return m_skills.skill(index)->name(); }
  int getCount() override { return m_skills.count(); }

  [[nodiscard]] std::string tabName() const override { return tr("Skills"); }
  [[nodiscard]] constexpr std::string_view tabId() const override { return "##DBSkillsTab"sv; };

private:
  int m_categoryStart;
  int m_categoryEnd;
  std::vector<int> m_headers;
  Skills& m_skills;
  Skill* m_selectedSkill{};
  int m_maxSkills{};
  int m_editMaxSkills{};
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
  std::vector<Effect> m_effects;
  EffectsEditor m_effectsEditor;
  int m_message_template{0};
  std::vector<std::string> m_message_templateList{"casts %1!", "does %1!", "uses %1"};
  std::optional<AnimationPicker> m_animationPicker;
  std::optional<CheckeredCompositeTexture> m_iconButtonTexture;
};