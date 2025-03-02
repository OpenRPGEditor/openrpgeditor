#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/ReferenceSearch.hpp"
#include "Database/System.hpp"

class DBMappingsTab final : public IDBEditorTab {
public:
  DBMappingsTab() = delete;
  explicit DBMappingsTab(System& system, DatabaseEditor* parent);

  bool isUnicodeFormatting(const std::string& text);
  void draw() override;
  void drawVariables();
  void drawSwitches();
  void drawCommons();

private:
  System& m_system;
  int m_selectedVariable{0};
  int m_selectedSwitch{0};
  int m_selectedCommon{0};
  int move_to;
  int move_from;

  std::string m_switch_string;
  std::string m_variable_string;
  std::string m_common_string;

  ReferenceSearch reference_from;
  ReferenceSearch reference_to;

  std::vector<int> m_reference_left{};
  std::vector<int> m_reference_right{};

  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
  int m_editMaxVariables{};
  int m_editMaxSwitches{};
};