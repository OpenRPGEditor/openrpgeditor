#include "Editor/EventListView.hpp"
#include "Editor/EventEditor.hpp"

#include "Database/Map.hpp"

#include "Editor/MainWindow.hpp"

#include "imgui.h"
#include "imgui_internal.h"

void EventListView::draw() {
  char eventNameBuf[4096]{};
  if (ImGui::Begin(std::format("{}###events", trNOOP("Events")).c_str())) {
    Map* map = m_parent->currentMap();
    if (map) {
      ImGui::BeginChild("##event_list");
      {
        for (auto& event : map->events()) {
          if (event) {
            bool selectedHere = false;
            ImGui::BeginDisabled(event->id() == 0);
            sprintf(eventNameBuf, "%s (%i, %i)", event->id() == 0 ? trNOOP("Player Start") : Database::instance()->eventNameOrId(event->id()).c_str(), event->x(), event->y());
            if (ImGui::Selectable(eventNameBuf, m_parent->mapEditor()->selectedEvent() == &*event, ImGuiSelectableFlags_AllowDoubleClick | static_cast<int>(ImGuiSelectableFlags_SelectOnNav))) {
              m_parent->mapEditor()->setSelectedEvent(const_cast<Event*>(&*event));
              selectedHere = true;
              if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
                event->editor()->open();
              }
            }
            ImGui::EndDisabled();

            if (m_parent->mapEditor()->selectedEvent() == &event.value() && !selectedHere && !m_parent->mapEditor()->scrolledToEvent()) {
              ImGui::SetScrollHereY();
            }
          }
        }
      }
      ImGui::EndChild();
    }
  }
  ImGui::End();
}
