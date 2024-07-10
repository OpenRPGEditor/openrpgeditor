#include "Dialog_SetMovementRoute.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/Project.hpp"
#include "Database/Database.hpp"
#include "Core/ImGuiUtils.hpp"
#include "Database/EventCommands/MovementRoute/MoveDown.hpp"

std::tuple<bool, bool> Dialog_SetMovementRoute::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{818, 395} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
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
                                 ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(110)})) {

      ImGui::TableSetupScrollFreeze(1, 0);
      ImGui::TableSetupColumn("File");
      ImGui::TableNextRow();
      ImGui::TableNextColumn();

      for (int n = 0; n < m_route.list.size(); n++) {
        ImGui::TableNextColumn();
        const bool isSelected = (m_selected == n + 1);
        if (ImGui::SelectableWithBorder(m_route.list.at(n)->stringRep(m_project->database()).c_str(), isSelected,
                                        ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns |
                                            ImGuiSelectableFlags_AllowDoubleClick)) {
          if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
            // Edit node
          }
          m_selected = n + 1;
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
    ImGui::SetCursorPos(ImVec2{cursorPos.x+App::DPIHandler::scale_value(180), cursorPos.y - App::DPIHandler::scale_value(10)});
    ImGui::SeparatorText("Movement Commands");
    ImGui::SetCursorPos(ImVec2{cursorPos.x+App::DPIHandler::scale_value(180), cursorPos.y + App::DPIHandler::scale_value(10)});
    ImGui::BeginGroup(); {
      if (ImGui::Button("Move Down", ImVec2{App::DPIHandler::scale_value(200), 0})) {
        m_route.addCommand(std::make_shared<MovementMoveDownCommand>(), m_selected);
      }
      if (ImGui::Button("Move Left", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Move Right", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Move Up", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Move Lower Left", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Move Lower Right", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Move Upper Left", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Move Upper Right", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Move at Random", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Move toward Player", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Move away from Player", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("1 Step Forward", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("1 Step Backward", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Jump...", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Wait...", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::BeginGroup(); {
      if (ImGui::Button("Turn Down", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Turn Left", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Turn Right", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Turn Up", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Turn 90° Right", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Turn 90° Left", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Turn 180°", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Turn 90° Right or Left", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Turn at Random", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Turn toward Player", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Turn away from Player", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Switch ON...", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Switch OFF...", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Change Speed...", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Change Frequency...", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::BeginGroup(); {
      if (ImGui::Button("Walking Animation ON", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Walking Animation OFF", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Stepping Animation ON", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Stepping Animation OFF", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Direction Fix ON", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Direction Fix OFF", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Through ON", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Through OFF", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Transparent ON", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Transparent OFF", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Change Image...", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Change Opacity...", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Change Blend Mode...", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Play SE...", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      if (ImGui::Button("Script...", ImVec2{App::DPIHandler::scale_value(200), 0})) {
      }
      ImGui::EndGroup();
    }
    //if (m_editNodes.empty())
   //   m_editNodes.push_back(std::make_shared<MovementRouteStepCommand>());
    ImGui::SetCursorPos(ImVec2{ImGui::GetContentRegionMax().x - App::DPIHandler::scale_value(80), ImGui::GetContentRegionMax().y - App::DPIHandler::scale_value(20)});
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
