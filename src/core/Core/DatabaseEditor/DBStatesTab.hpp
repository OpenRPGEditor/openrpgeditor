#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"

struct States;
struct DBStatesTab : IDBEditorTab {
  DBStatesTab() = delete;
  explicit DBStatesTab(States& system, DatabaseEditor* parent) : IDBEditorTab(parent), m_states(system) {}
  void draw() override;

private:
  States& m_states;
};