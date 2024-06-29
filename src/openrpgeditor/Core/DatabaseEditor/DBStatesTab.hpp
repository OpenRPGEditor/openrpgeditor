#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/States.hpp"

struct DBStatesTab : IDBEditorTab {
  DBStatesTab() = delete;
  explicit DBStatesTab(States& system, DatabaseEditor* parent) : IDBEditorTab(parent), m_states(system) {}
  void draw() override;

  [[nodiscard]] State* state(int id) { return m_states.state(id); }
  [[nodiscard]] const State* state(int id) const { return m_states.state(id); }

  [[nodiscard]] States& states() { return m_states; }
  [[nodiscard]] const States& states() const { return m_states; }

private:
  States& m_states;
};