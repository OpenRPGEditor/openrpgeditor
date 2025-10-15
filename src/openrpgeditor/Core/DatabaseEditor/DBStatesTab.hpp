#pragma once
#include "Core/CommonUI/TraitsEditor.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/States.hpp"
#include "DBCommonEventsTab.hpp"

struct DBStatesTab : IDBEditorTab {
  DBStatesTab() = delete;
  explicit DBStatesTab(DatabaseEditor* parent);
  void draw() override;

  std::vector<int>& getHeaders() override { return m_headers; }
  int getHeader(const int index) override { return m_headers.at(index); }
  bool hasHeader() override { return !m_headers.empty(); }
  void setHeaderRange(const int start, const int end) override {
    m_categoryStart = start;
    m_categoryEnd = end;
  }

  std::string getName(const int index) override { return m_states->state(index)->name(); }
  int getCount() override { return m_states->count(); }

  [[nodiscard]] std::string tabName() const override { return tr("States"); }
  [[nodiscard]] constexpr std::string_view tabId() const override { return "##DBStatesTab"sv; };

  bool isReady() const override { return !!Database::instance()->states; }
  bool isInitialized() const override { return m_states; }

  void initialize() override {
    if (!isReady()) {
      return;
    }
    m_states = &Database::instance()->states.value();
    m_selectedState = m_states->state(1);
    if (m_selectedState) {
      m_traitsEditor.setTraits(&m_selectedState->traits());
    }
  }

private:
  int m_categoryStart;
  int m_categoryEnd;
  std::vector<int> m_headers;
  States* m_states{};
  State* m_selectedState{};
  int m_editMaxStates;
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
  TraitsEditor m_traitsEditor;
};