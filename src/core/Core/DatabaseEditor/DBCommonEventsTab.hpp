#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"

struct CommonEvents;
struct DBCommonEventsTab : IDBEditorTab {
  DBCommonEventsTab() = delete;
  explicit DBCommonEventsTab(CommonEvents& events) : m_events(events) {}
  void draw() override;

private:
  CommonEvents& m_events;
};