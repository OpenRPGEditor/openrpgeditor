#pragma once
#include "Core/CommonUI/EventCommandEditor.hpp"
#include "Core/CommonUI/Preview.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/CommonEvents.hpp"

struct DBCommonEventsTab : IDBEditorTab {
  DBCommonEventsTab() = delete;
  explicit DBCommonEventsTab(CommonEvents& commonEvents, DatabaseEditor* parent);
  void draw() override;

  [[nodiscard]] std::vector<std::optional<CommonEvent>>& events() { return m_events.events(); }
  [[nodiscard]] const std::vector<std::optional<CommonEvent>>& events() const { return m_events.events(); }

  [[nodiscard]] CommonEvent* event(int id) { return m_events.event(id); }
  [[nodiscard]] const CommonEvent* event(int id) const { return m_events.event(id); }
  std::vector<int>& getHeaders() override { return m_headers; }
  int getHeader(int index) override { return m_headers.at(index); }
  bool hasHeader() override { return !m_headers.empty(); }
  void setHeaderRange(int start, int end) override {
    m_categoryStart = start;
    m_categoryEnd = end;
  }
  std::string getName(int index) override { return m_events.event(index)->name(); }
  int getCount() override { return m_events.count(); }
  int getSelectedIndex() { return m_selectedCommonEvent ? m_selectedCommonEvent->id() : 0; }

  [[nodiscard]] std::string tabName() const override { return tr("Common Events"); }
  [[nodiscard]] constexpr std::string_view tabId() const override { return "##DBCommonEventsTab"sv; };

private:
  CommonEvents& m_events;
  CommonEvent* m_selectedCommonEvent{};
  EventCommandEditor m_commandEditor;
  std::optional<VariableSwitchPicker> picker;
  std::optional<Preview> preview;

  std::vector<int> m_headers;
  int m_categoryStart{0};
  int m_categoryEnd{0};

  int m_editMaxCommonEvents{};
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
};