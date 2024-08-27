#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/States.hpp"

struct DBStatesTab : IDBEditorTab {
  DBStatesTab() = delete;
  explicit DBStatesTab(States& system, DatabaseEditor* parent);
  void draw() override;

  [[nodiscard]] State* state(int id) { return m_states.state(id); }
  [[nodiscard]] const State* state(int id) const { return m_states.state(id); }

  [[nodiscard]] States& states() { return m_states; }
  [[nodiscard]] const States& states() const { return m_states; }

private:
  States& m_states;
  State* m_selectedState{};
  int m_maxStates{};
  int m_editMaxStates;
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
};