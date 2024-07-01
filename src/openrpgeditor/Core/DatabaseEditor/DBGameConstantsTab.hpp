#pragma once
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include <optional>

struct GameConstants;
struct DBGameConstantsTab : IDBEditorTab {
  explicit DBGameConstantsTab(GameConstants& constants, DatabaseEditor* parent)
  : IDBEditorTab(parent), m_constants(&constants) {}

  void drawVariableAliasModal();
  void drawSwitchAliasModal();
  void draw() override;

private:
  GameConstants* m_constants = nullptr;
  std::optional<VariableSwitchPicker> m_picker;
  int m_selectedSwitch = -1;
  int m_selectedVariable = -1;
  std::optional<std::string> m_currentAlias;
  bool m_openPopup = false;
};
