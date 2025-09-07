#pragma once

#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/Trait.hpp"

#include <vector>

class State;
class Skill;

class TraitsEditor final {
public:
  void setTraits(std::vector<Trait>* traits) { m_traits = traits; }
  void draw();
  void drawRadioButton(TraitCode code, int dataId, double initialValue) const;

private:
  void drawPopup();
  std::vector<Trait>* m_traits = nullptr;
  Trait* m_selectedTrait = nullptr;
  bool m_updateTab{false};
  bool m_isNewEntry{false};
  bool m_tempTraitAssigned{false};
  Trait m_tempTrait;
  Trait* m_deletingTrait = nullptr;
  std::optional<StatePicker> m_statePicker;
  std::optional<SkillPicker> m_skillPicker;
};