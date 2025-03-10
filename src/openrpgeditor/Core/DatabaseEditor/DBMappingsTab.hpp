#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/ReferenceSearch.hpp"
#include "DBCommonEventsTab.hpp"
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
  std::vector<int>& getHeaders() override { return m_headers; }
  int getHeader(int index) override { return m_headers.at(index); }
  bool hasHeader() override { return !m_headers.empty(); }
  void setHeaderRange(int start, int end) override {
    m_categoryStart = start;
    m_categoryEnd = end;
  }
  std::string_view GetFormattingHelpText() { return trNOOP("Reference updating does not support:\n- Ranges\n- Data without game constants\nThese must be updated manually."); }
  std::string getName(int index) override { return ""; }
  int getCount() override { return 0; }

private:
  int m_categoryStart;
  int m_categoryEnd;
  std::vector<int> m_headers;
  System& m_system;
  int m_selectedVariable{0};
  int m_selectedSwitch{0};
  int m_selectedCommon{0};
  int m_selectedTab{0};

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
  int m_selectedUnicode{0};
  std::vector<std::string_view> m_unicodes;
};