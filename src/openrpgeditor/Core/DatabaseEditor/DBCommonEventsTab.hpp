#pragma once
#include "Core/CommonUI/EventCommandEditor.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/CommonEvents.hpp"

struct DBCommonEventsTab : IDBEditorTab {
  DBCommonEventsTab() = delete;
  explicit DBCommonEventsTab(CommonEvents& events, DatabaseEditor* parent);
  void draw() override;

  [[nodiscard]] std::vector<std::optional<CommonEvent>>& events() { return m_events.events(); }
  [[nodiscard]] const std::vector<std::optional<CommonEvent>>& events() const { return m_events.events(); }

  [[nodiscard]] CommonEvent* event(int id) { return m_events.event(id); }
  [[nodiscard]] const CommonEvent* event(int id) const { return m_events.event(id); }
private:
  CommonEvents& m_events;
  CommonEvent* m_selectedCommonEvent{};
  EventCommandEditor m_commandEditor;
  int m_editMaxCommonEvents{};
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
};