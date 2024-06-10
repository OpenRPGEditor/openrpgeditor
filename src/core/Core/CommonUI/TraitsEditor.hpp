#pragma once

#include "Database/Trait.hpp"
#include <vector>

struct DatabaseEditor;

struct TraitsEditor {
  void draw(std::vector<Trait>& traits, DatabaseEditor* dbEditor);

private:
  void drawPopup(std::vector<Trait>& traits, DatabaseEditor* dbEditor);
  Trait* m_selectedTrait = nullptr;
  bool m_isNewEntry{false};
  Trait m_tempTrait;
};