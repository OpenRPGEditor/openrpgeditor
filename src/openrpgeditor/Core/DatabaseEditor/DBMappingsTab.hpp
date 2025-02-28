#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/System.hpp"

class DBMappingsTab final : public IDBEditorTab {
public:
  DBMappingsTab() = delete;
  explicit DBMappingsTab(System& system, DatabaseEditor* parent);
  void draw() override;

private:
  System& m_system;
  int m_selectedVariable{0};
  int m_selectedSwitch{0};

  std::string m_switch_string;
  std::string m_variable_string;

  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
  int m_editMaxVariables{};
  int m_editMaxSwitches{};
};