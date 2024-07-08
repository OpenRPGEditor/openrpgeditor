#include "Dialog_BattleProcessing.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/Project.hpp"
#include "Database/Database.hpp"

std::tuple<bool, bool> Dialog_BattleProcessing::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{194, 230} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize)) {

        if (enemy_picker) {
          auto [closed, confirmed] = enemy_picker->draw();
          if (confirmed) {
            m_id = enemy_picker->selection();
            enemy_picker.reset();
          }
        }

        if (picker) {
          auto [closed, confirmed] = picker->draw();
          if (confirmed) {
            m_var_selection = picker->selection();
            picker.reset();
          }
        }
        ImGui::SeparatorText("Troop");
        // Direct Designation
        ImGui::RadioButton("Direct Designation", &m_type, 0);
        ImGui::BeginDisabled(m_type != 0);
        ImGui::PushID("##battleprocess_direct");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
        if (ImGui::Button(
                m_type == 0 ? Database::Instance->troopNameOrId(m_id).c_str() : "",
                {(App::DPIHandler::scale_value(160)), 0})) {

          enemy_picker = ObjectPicker<Troop>("Troop"sv, Database::Instance->troops.troops(), 0);
        }
        ImGui::PopID();
        ImGui::EndDisabled();

        // Designation with variable
        ImGui::RadioButton("Designation with variable", &m_type, 1);
        ImGui::BeginDisabled(m_type != 1);
        ImGui::PushID("##battleprocess_var");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
        if (ImGui::Button(
                m_type == 1 ? Database::Instance->variableNameOrId(m_var_selection).c_str() : "",
                ImVec2{(App::DPIHandler::scale_value(160)), 0})) {
          picker.emplace("Variables", m_project->system().variables);
        }
        ImGui::PopID();
        ImGui::EndDisabled();
        ImGui::RadioButton("Same as Random Encounter", &m_type, 2);
        ImGui::Checkbox("Can Escape", &m_canEscape);
        ImGui::Checkbox("Can Lose", &m_canLose);


    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->type = static_cast<BattleProcessType>(m_type);
      command->id = m_id;
      command->canEscape = m_canEscape;
      command->canLose = m_canLose;
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
