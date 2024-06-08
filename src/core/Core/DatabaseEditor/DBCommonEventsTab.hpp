#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"

struct CommonEvents;
struct DBCommonEventsTab : IDBEditorTab {
  DBCommonEventsTab() = delete;
  explicit DBCommonEventsTab(CommonEvents& events, DatabaseEditor* parent) : IDBEditorTab(parent), m_events(events) {}
  void draw() override;

private:
  CommonEvents& m_events;
};