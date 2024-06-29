#pragma once

#include "Database/Trait.hpp"
#include <vector>

struct DatabaseEditor;

struct TraitsEditor {
  void setTraits(std::vector<Trait>* traits) { m_traits = traits; }
  void draw(DatabaseEditor* dbEditor);

private:
  void drawPopup(DatabaseEditor* dbEditor);
  std::vector<Trait>* m_traits = nullptr;
  Trait* m_selectedTrait = nullptr;
  bool m_updateTab{false};
  bool m_isNewEntry{false};
  bool m_tempTraitAssigned{false};
  Trait m_tempTrait;
  Trait* m_deletingTrait = nullptr;
};