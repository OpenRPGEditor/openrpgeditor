#include "Core/EventListView.hpp"

#include "Database/Map.hpp"

#include "Core/Project.hpp"

#include "imgui.h"

void EventListView::draw() {
  char eventNameBuf[4096]{};
  if (ImGui::Begin("Events")) {
    Map* map = m_parent->currentMap();
    if (map) {
      for (auto& event : map->events) {
        if (event) {
          sprintf(eventNameBuf, "%s (%i, %i)", event->name.c_str(), event->x, event->y);
          if (ImGui::Selectable(eventNameBuf, m_parent->mapEditor()->selectedEvent() == &*event)) {
            m_parent->mapEditor()->setSelectedEvent(&*event);
          }
          if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemActivated()) {
            m_parent->mapEditor()->addEventEditor(EventEditor(m_parent, &event.value()));
          }
        }
      }
    }
  }
  ImGui::End();
}
