#pragma once
#include "Database/Event.hpp"

struct ImGuiWindow;
struct MapEditor;
struct MapEvent {
  MapEvent() = delete;
  MapEvent(MapEditor* parent, Event* event) : m_parent(parent), m_event(event) {}

  void draw(float mapScale, bool isHovered, ImGuiWindow* win);

private:
  MapEditor* m_parent;
  Event* m_event;
};
