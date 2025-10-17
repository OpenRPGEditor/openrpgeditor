#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/ReferenceSearch.hpp"
#include "Database/System.hpp"
#include "DBCommonEventsTab.hpp"

class DBMappingsTab final : public IDBEditorTab {
public:
  DBMappingsTab() = delete;
  explicit DBMappingsTab(DatabaseEditor* parent);

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

  [[nodiscard]] std::string tabName() const override { return tr("Data Sorting"); }
  [[nodiscard]] constexpr std::string_view tabId() const override { return "##DBMappingsTab"sv; };
  bool isExperimental() const override { return true; }
  
  bool isReady() const override { return !!Database::instance()->system; }

  bool isInitialized() const override { return m_system; }

  void initialize() override {
    if (!isReady()) {
      return;
    }

    m_system = &Database::instance()->system.value();
    m_switch_string = m_system->switche(m_selectedSwitch);
    m_variable_string = m_system->variable(m_selectedVariable);
    m_unicodes = getUnicodeFormatters();
  }

private:
  int m_categoryStart;
  int m_categoryEnd;
  std::vector<int> m_headers;
  System* m_system{};
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