#pragma once

#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/CommonEvents.hpp"
#include "Database/Skills.hpp"
#include "Database/States.hpp"

#include <vector>

class Effect;
struct DatabaseEditor;

struct EffectsEditor {
  void setEffects(std::vector<Effect>* effects) { m_effects = effects; }
  void draw();

private:
  void drawPopup();
  std::vector<Effect>* m_effects = nullptr;
  Effect* m_selectedEffect = nullptr;
  bool m_updateTab{false};
  bool m_isNewEntry{false};
  bool m_tempEffectAssigned{false};
  Effect m_tempEffect;
  Effect* m_deletingEffect = nullptr;
  std::optional<StatePicker> m_statePicker;
  std::optional<SkillPicker> m_skillPicker;
  std::optional<CommonEventPicker> m_commonEventPicker;
};