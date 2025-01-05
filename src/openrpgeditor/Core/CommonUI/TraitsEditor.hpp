#pragma once

#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/Trait.hpp"

#include <vector>

class State;
class Skill;
class DatabaseEditor;

class TraitsEditor final {
public:
  void setTraits(std::vector<Trait>* traits) { m_traits = traits; }
  void draw(DatabaseEditor* dbEditor);
  void drawRadioButton(std::string_view name, TraitCode code, int dataId, double initialValue) const;

private:
  void drawPopup(DatabaseEditor* dbEditor);
  std::vector<Trait>* m_traits = nullptr;
  Trait* m_selectedTrait = nullptr;
  bool m_updateTab{false};
  bool m_isNewEntry{false};
  bool m_tempTraitAssigned{false};
  Trait m_tempTrait;
  Trait* m_deletingTrait = nullptr;
  std::optional<ObjectPicker<State>> m_statePicker;
  std::optional<ObjectPicker<Skill>> m_skillPicker;
};