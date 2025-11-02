#pragma once
#include "Editor/CommonUI/EffectsEditor.hpp"
#include "Editor/CommonUI/ObjectPicker.hpp"
#include "Editor/DatabaseEditor/DBCommonEventsTab.hpp"
#include "Editor/DatabaseEditor/IDBEditorTab.hpp"
#include "Editor/Graphics/CheckeredCompositeTexture.hpp"
#include "Editor/Graphics/IconSheet.hpp"

#include "Database/Animation.hpp"
#include "Database/Skills.hpp"

class DBSkillsTab final : public IDBCoreEditorTab<DBSkillsTab> {
public:
  void draw() override;

  std::string getName(const int index) const override { return m_skills->skill(index)->name(); }
  int getCount() const override { return m_skills->count(); }

  [[nodiscard]] std::string tabName() const override { return tr("Skills"); }

  bool isReady() const override { return !!Database::instance()->skills; }
  bool isInitialized() const override { return m_skills; }
  void initialize() override {
    m_skills = &Database::instance()->skills.value();

    m_selectedSkill = m_skills->skill(1);
    if (m_selectedSkill) {
      m_effectsEditor.setEffects(&m_selectedSkill->effects());
    }
    m_maxSkills = m_skills->count();
  }

private:
  Skills* m_skills{};
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
  std::optional<IconSheet> m_itemSheet;
};