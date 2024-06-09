#pragma once

#include "Database/Trait.hpp"
#include <vector>

struct DatabaseEditor;

struct TraitsEditor {
  void draw(std::vector<Trait>& traits, DatabaseEditor* dbEditor);

private:
  Trait* m_selectedTrait = nullptr;
};