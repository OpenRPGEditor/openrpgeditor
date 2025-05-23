#include "Core/TemplateEditor/Dialog/TemplatesEvent.hpp"
#include "Core/Application.hpp"
#include "Database/Templates.hpp"

#include "imgui.h"

#include "IconsFontAwesome6.h"
void TemplatesEvent::draw() {
  static constexpr std::array<std::string_view, 4> kDirectionArrows = {{
      ICON_FA_ARROW_DOWN,
      ICON_FA_ARROW_LEFT,
      ICON_FA_ARROW_RIGHT,
      ICON_FA_ARROW_UP,
  }};
  ImGui::SetNextWindowSize(ImVec2{500, 400}, ImGuiCond_Once);
  if (ImGui::Begin("Template Properties", &m_open,
                   ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize)) {
    ImGui::Text("ID:");
    ImGui::SetNextItemWidth(200);

    if (ImGui::InputInt("##orpg_templates_event_properties_id", &m_id)) {
      if (m_id == 0) {
        m_id = 1; // Cannot pick index 0
      }
    }
    ImGui::Text("Name:");
    ImGui::InputText("##orpg_templates_event_properties_name", &m_eventName);
    ImGui::Text("Note:");
    ImGui::InputText("##orpg_templates_event_properties_note", &m_eventNote);
    ImGui::BeginChild("ArrowButtons", ImVec2{150, 150}, 0, ImGuiWindowFlags_NoBackground);
    ImGui::Text("Initial Direction:");
    ImGui::Columns(3, nullptr, false);
    const auto size = ImGui::CalcTextSize(ICON_FA_ARROW_LEFT);
    const auto width = size.x + (ImGui::GetStyle().FramePadding.x * 2) + (ImGui::GetStyle().ItemSpacing.x);
    ImGui::SetColumnWidth(0, width);
    ImGui::SetColumnWidth(1, width);
    ImGui::SetColumnWidth(2, width);
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
        ImGui::SetCursorPos(ImVec2{ImGui::GetCursorPosX() + size.x / 2, ImGui::GetCursorPosY() + size.y / 2} - (ImGui::GetStyle().ItemSpacing / 2));
        ImGui::Text("%s", kDirectionArrows[(m_direction - 2) / 2].data());
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
        m_event->setName(m_eventName);
        m_event->setNote(m_eventNote);

        m_event->setX(m_tileX);
        m_event->setY(m_tileY);
        for (auto& page : m_event->pages()) {
          page.image().setDirection(static_cast<Direction>(m_direction));
        }
        if (m_id != m_event->id()) {
          m_event->setId(m_id);
        }
        m_map->createEventFromTemplate(*m_event);
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