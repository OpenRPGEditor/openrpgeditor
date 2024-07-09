#include "Dialog_SetMovementRoute.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/Project.hpp"
#include "Database/Database.hpp"
#include "Core/ImGuiUtils.hpp"

std::tuple<bool, bool> Dialog_SetMovementRoute::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{1134, 532} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize)) {
    // Character Selection
    ImVec2 cursorPos = ImGui::GetCursorPos();
    ImGui::PushItemWidth((App::DPIHandler::scale_value(160)));
    if (ImGui::BeginCombo("##showroute_character", Database::Instance->eventNameOrId(m_character).c_str())) {

      if (ImGui::Selectable("Player", m_character == -1)) {
        m_character = -1;
        ImGui::SetItemDefaultFocus();
      }
      if (ImGui::Selectable("This Event", m_character == 0)) {
        m_character = 0;
        ImGui::SetItemDefaultFocus();
      }

      for (auto& dataSource : m_project->events()) {
        if (!dataSource.has_value())
          continue;

        bool is_selected = (m_character == dataSource->id);
        if (ImGui::Selectable(("EV" + std::format("{:03} ", dataSource->id)).c_str(), is_selected)) {
          m_character = dataSource->id;
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    // Step Table
    if (ImGui::BeginTable("##bgm_audio_contents", 1,
                          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX |
                              ImGuiTableFlags_ScrollY,
                          ImVec2{App::DPIHandler::scale_value(160),
                                 ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(100)})) {

      ImGui::TableSetupScrollFreeze(1, 0);
      ImGui::TableSetupColumn("File");
      ImGui::TableNextRow();
      ImGui::TableNextColumn();

      if (ImGui::SelectableWithBorder("None", m_selected == 0,
                                      ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns |
                                          ImGuiSelectableFlags_AllowDoubleClick)) {
        if (m_selected == 0)
          ImGui::SetItemDefaultFocus();
      }
      for (int n = 0; n < m_editNodes.size(); n++) {
        ImGui::TableNextColumn();
        const bool isSelected = (m_selected == n + 1);
        if (ImGui::SelectableWithBorder(m_editNodes.at(n)->step->stringRep(m_project->database()).c_str(), isSelected,
                                        ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns |
                                            ImGuiSelectableFlags_AllowDoubleClick)) {
          if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
            // Edit node
          }
          m_selected = n + 1;
          // m_audio.name = m_audios.at(m_selected - 1);
          if (isSelected)
            ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndTable();
    }
    ImGui::Text("Options");
    ImGui::Checkbox("Repeat Movements", &m_route.repeat);
    ImGui::Checkbox("Skip If Cannot Move", &m_route.skippable);
    ImGui::Checkbox("Wait for Completion", &m_route.wait);
    ImGui::SetCursorPos(ImVec2{cursorPos.x+180, cursorPos.y});
    ImGui::SeparatorText("Movement Commands");
    ImGui::SetCursorPos(ImVec2{cursorPos.x+180, cursorPos.y+40});
    ImGui::BeginGroup(); {
      if (ImGui::Button("Move Down")) {
      }
      if (ImGui::Button("Move Left")) {
      }
      if (ImGui::Button("Move Right")) {
      }
      if (ImGui::Button("Move Up")) {
      }
    }
    //if (m_editNodes.empty())
   //   m_editNodes.push_back(std::make_shared<MovementRouteStepCommand>());
    ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 300);
    if (ImGui::Button("OK")) {
      m_confirmed = true;
      ImGui::CloseCurrentPopup();
      SetOpen(false);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
      SetOpen(false);
    }

    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
