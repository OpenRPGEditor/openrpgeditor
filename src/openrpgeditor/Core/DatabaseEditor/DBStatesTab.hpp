#pragma once
#include "Core/CommonUI/TraitsEditor.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "DBCommonEventsTab.hpp"
#include "Database/States.hpp"

struct DBStatesTab : IDBEditorTab {
  DBStatesTab() = delete;
  explicit DBStatesTab(States& system, DatabaseEditor* parent);
  void draw() override;

  [[nodiscard]] State* state(int id) { return m_states.state(id); }
  [[nodiscard]] const State* state(int id) const { return m_states.state(id); }

  [[nodiscard]] States& states() { return m_states; }
  [[nodiscard]] const States& states() const { return m_states; }
  std::vector<int>& getHeaders() override { return m_headers; }
  int getHeader(int index) override { return m_headers.at(index); }
  bool hasHeader() override { return !m_headers.empty(); }
  void setHeaderRange(int start, int end) override {
    m_categoryStart = start;
    m_categoryEnd = end;
  }

  std::string getName(int index) override { return m_states.state(index)->name(); }
  int getCount() override { return m_states.count(); }

  [[nodiscard]] std::string tabName() const override { return tr("States"); }
  [[nodiscard]] constexpr std::string_view tabId() const override { return "##DBStatesTab"sv; };

private:
  int m_categoryStart;
  int m_categoryEnd;
  std::vector<int> m_headers;
  States& m_states;
  State* m_selectedState{};
  int m_editMaxStates;
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
  TraitsEditor m_traitsEditor;
};