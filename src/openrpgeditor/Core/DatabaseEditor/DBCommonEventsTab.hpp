#pragma once
#include "Core/CommonUI/EventCommandEditor.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"

struct CommonEvents;
struct CommonEvent;
struct System;

struct DBCommonEventsTab : IDBEditorTab {
  DBCommonEventsTab() = delete;
  explicit DBCommonEventsTab(CommonEvents& events, DatabaseEditor* parent);
  void draw() override;
private:
  CommonEvents& m_events;
  CommonEvent* m_selectedCommonEvent{};
  EventCommandEditor m_commandEditor;
  int m_maxCommonEvents{};
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
};