#include "Core/EventListView.hpp"

#include "Database/Map.hpp"

#include "Core/Project.hpp"

#include "imgui.h"

void EventListView::draw() {
  char eventNameBuf[4096]{};
  if (ImGui::Begin("Events")) {
    Map* map = m_parent->currentMap();
    if (map) {
      for (const auto& event : map->events) {
        if (event) {
          sprintf(eventNameBuf, "%s (%i, %i)", event->name.c_str(), event->x, event->y);
          ImGui::Selectable(eventNameBuf);
        }
      }
    }
  }
  ImGui::End();
}
