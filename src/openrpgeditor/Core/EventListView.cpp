#include "Core/EventListView.hpp"
#include "Core/EventEditor.hpp"

#include "Database/Map.hpp"

#include "Core/Project.hpp"

#include "imgui.h"

void EventListView::draw() {
  char eventNameBuf[4096]{};
  if (ImGui::Begin("Events")) {
    Map* map = m_parent->currentMap();
    if (map) {
      for (auto& event : map->events) {
        bool selectedHere = false;
        if (event) {
          sprintf(eventNameBuf, "%s (%i, %i)", event->name.c_str(), event->x, event->y);
          if (ImGui::Selectable(eventNameBuf, m_parent->mapEditor()->selectedEvent() == &*event,
                                ImGuiSelectableFlags_AllowDoubleClick)) {
            m_parent->mapEditor()->setSelectedEvent(&*event);
            selectedHere = true;
            if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              auto ev = EventEditor(m_parent, &event.value());
              m_parent->mapEditor()->addEventEditor(ev);
            }
          }

          if (m_parent->mapEditor()->selectedEvent() == &event.value() && !selectedHere &&
              !m_parent->mapEditor()->scrolledToEvent()) {
            ImGui::SetScrollHereY();
          }
        }
      }
    }
  }
  ImGui::End();
}
