#include "TemplatesEvent.hpp"
#include "Core/Application.hpp"
#include "Database/Templates.hpp"

#include "imgui.h"

void TemplatesEvent::draw() {
  ImGui::SetNextWindowSize(ImVec2{500, 400} * App::DPIHandler::get_ui_scale(), ImGuiCond_Once);
  if (ImGui::Begin("Template Properties", &m_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize)) {
    ImGui::Text("ID:");
    ImGui::SetNextItemWidth(200 * App::DPIHandler::get_ui_scale());
    ImGui::InputInt("##orpg_templates_event_properties_id", &m_id);
    ImGui::Text("Name:");
    ImGui::InputText("##orpg_templates_event_properties_name", &m_eventName);
    ImGui::Text("Note:");
    ImGui::InputText("##orpg_templates_event_properties_note", &m_eventNote);
    ImGui::BeginChild("ArrowButtons", ImVec2{150, 150} * App::DPIHandler::get_ui_scale(), 0, ImGuiWindowFlags_NoBackground);
    ImGui::Text("Initial Direction:");
    ImGui::Columns(3, nullptr, false);
    ImGui::SetColumnWidth(0, 40.f);
    ImGui::SetColumnWidth(1, 40.f);
    ImGui::SetColumnWidth(2, 40.f);
    for (int i = 0; i < 9; i++) {
      switch (i) {
      case 0:
        // if (ImGui::ArrowButton("##UpLeft", ImGuiDir_UpLeft)){}
        break;
      case 1:
        if (ImGui::ArrowButton("##Up", ImGuiDir_Up)) {
          m_direction = 8;
        }
        break;
      case 2:
        // if (ImGui::ArrowButton("##UpRight", ImGuiDir_UpRight)){}
        break;
      case 3:
        if (ImGui::ArrowButton("##Left", ImGuiDir_Left)) {
          m_direction = 4;
        }
        break;
      case 4:
        ImGui::SetCursorPos(ImVec2{ImGui::GetCursorPosX() + 10, ImGui::GetCursorPosY() + 5} * App::DPIHandler::get_ui_scale());
        ImGui::Text("%s", std::to_string(m_direction).c_str());
        break;
      case 5:
        if (ImGui::ArrowButton("##Right", ImGuiDir_Right)) {
          m_direction = 6;
        }
        break;
      case 6:
        // if (ImGui::ArrowButton("##DownLeft", ImGuiDir_DownLeft)){}
        break;
      case 7:
        if (ImGui::ArrowButton("##Down", ImGuiDir_Down)) {
          m_direction = 2;
        }
        break;
      case 8:
        // if (ImGui::ArrowButton("##DownRight", ImGuiDir_DownRight)){}
        break;
      }
      ImGui::NextColumn();
    }
    ImGui::EndChild();

    ImGui::BeginGroup();
    {
      if (ImGui::Button("OK")) {
        // if (m_id < m_map->events.size()) {
        //   if (m_id != m_event->id) {
        //     // User requested that the event index be changed
        //     int oldIndex = m_event->id;
        //     int newIndex = m_map->events.at(m_id)->id;
        //     std::swap(m_map->events.at(oldIndex), m_map->events.at(newIndex));
        //     m_event->id = m_id;
        //   }
        // }
        m_event->name = m_eventName;
        m_event->note = m_eventNote;

        m_event->x = tileX;
        m_event->y = tileY;
        for (auto& page : m_event->pages) {
          page.image.direction = static_cast<Direction>(m_direction);
        }
        if (m_id != m_event->id) {
          m_event->id = m_id;
        }
        m_map->createEventFromTemplate(*m_event, m_event->id);
        m_hasChanges = true;
        ImGui::CloseCurrentPopup();
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        m_hasChanges = true;
        ImGui::CloseCurrentPopup();
      }
    }
    ImGui::EndGroup();
    // ImGui::PopButtonRepeat();
    // ImGui::EndChild();
  }
  ImGui::End();
};