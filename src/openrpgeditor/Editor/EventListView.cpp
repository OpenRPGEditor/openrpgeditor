#include "Editor/EventListView.hpp"
#include "Editor/EventEditor.hpp"

#include "Database/Map.hpp"

#include "Editor/MainWindow.hpp"

#include "imgui.h"
#include "imgui_internal.h"

void EventListView::draw() {
  if (ImGui::Begin(std::format("{}###events", trNOOP("Events")).c_str())) {
    Map* map = m_parent->currentMap();
    if (map) {
      if (ImGui::BeginTable("##event_list_table", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY, ImGui::GetContentRegionAvail())) {
        ImGui::TableSetupColumn(trNOOP("Name"));
        ImGui::TableSetupColumn(trNOOP("ID"));
        ImGui::TableSetupColumn(trNOOP("Location (X,Y)"));
        ImGui::TableSetupScrollFreeze(3, 1);
        ImGui::TableHeadersRow();
        for (auto& event : map->events()) {
          if (event) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            bool selectedHere = false;
            ImGui::BeginDisabled(event->id() == 0);
            if (ImGui::Selectable(trFormat("{}##event_{}", event->id() == 0 ? trNOOP("Player Start") : Database::instance()->eventNameOrId(event->id()).c_str(), event->id()).c_str(),
                                  m_parent->mapEditor()->selectedEvent() == &*event,
                                  ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_SpanAllColumns | static_cast<int>(ImGuiSelectableFlags_SelectOnNav))) {
              m_parent->mapEditor()->setSelectedEvent(const_cast<Event*>(&*event));
              selectedHere = true;
              if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
                event->editor()->open();
              }
            }
            ImGui::TableNextColumn();
            ImGui::Text("%03i", event->id());
            ImGui::TableNextColumn();
            ImGui::Text("%i,%i", event->x(), event->y());
            ImGui::EndDisabled();

            if (m_parent->mapEditor()->selectedEvent() == &event.value() && !selectedHere && !m_parent->mapEditor()->scrolledToEvent()) {
              ImGui::SetScrollHereY();
            }
          }
        }
        ImGui::EndTable();
      }
    }
  }
  ImGui::End();
}
