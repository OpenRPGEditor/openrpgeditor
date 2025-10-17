#pragma once

#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/CommonEvents.hpp"
#include "Database/Enemies.hpp"
#include "Database/Skills.hpp"
#include "Database/States.hpp"

#include <vector>

class Action;
struct DatabaseEditor;

struct ActionsEditor {
  void setActions(std::vector<Enemy::Action>* actions) { m_actions = actions; }
  void draw();

private:
  void drawPopup();
  std::vector<Enemy::Action>* m_actions = nullptr;
  Enemy::Action* m_selectedAction = nullptr;
  bool m_updateTab{false};
  bool m_isNewEntry{false};
  bool m_tempEffectAssigned{false};
  Enemy::Action m_tempAction;
  Enemy::Action* m_deletingAction = nullptr;
  std::optional<StatePicker> m_statePicker;
  std::optional<SkillPicker> m_skillPicker;
  std::optional<VariableSwitchPicker> picker;
};