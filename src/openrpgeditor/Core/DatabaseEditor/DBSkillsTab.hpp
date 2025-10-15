#pragma once
#include "Core/CommonUI/EffectsEditor.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/DatabaseEditor/DBCommonEventsTab.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/Graphics/CheckeredCompositeTexture.hpp"
#include "Core/Graphics/IconSheet.hpp"

#include "Database/Animation.hpp"
#include "Database/Skills.hpp"

class DBSkillsTab final : public IDBEditorTab {
public:
  DBSkillsTab() = delete;
  explicit DBSkillsTab(DatabaseEditor* parent);
  void draw() override;

  std::vector<int>& getHeaders() override { return m_headers; }
  int getHeader(const int index) override { return m_headers.at(index); }
  bool hasHeader() override { return !m_headers.empty(); }
  void setHeaderRange(const int start, const int end) override {
    m_categoryStart = start;
    m_categoryEnd = end;
  }

  std::string getName(int index) override { return m_skills->skill(index)->name(); }
  int getCount() override { return m_skills->count(); }

  [[nodiscard]] std::string tabName() const override { return tr("Skills"); }
  [[nodiscard]] constexpr std::string_view tabId() const override { return "##DBSkillsTab"sv; };

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
  int m_categoryStart;
  int m_categoryEnd;
  std::vector<int> m_headers;
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