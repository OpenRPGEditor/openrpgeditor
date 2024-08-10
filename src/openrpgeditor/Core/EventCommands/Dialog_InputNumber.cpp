#include "Dialog_InputNumber.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Database/Database.hpp"

std::tuple<bool, bool> Dialog_InputNumber::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{200, 140} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_AlwaysAutoResize)) {

    if (picker) {
      auto [closed, confirmed] = picker->draw();
      if (confirmed) {
        m_variable = picker->selection();
        picker.reset();
      }
    }
    ImGui::SeparatorText("Variable");
    ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
    ImGui::PushID("##inputnumber_variable");
    if (ImGui::Button(
            Database::Instance->variableNameOrId(m_variable).c_str(),
            ImVec2{(App::DPIHandler::scale_value(180)), 0})) {
      picker.emplace("Variables", Database::Instance->system.variables);
            }
    ImGui::PopID();
    ImGui::SeparatorText("Digits");
    ImGui::SetNextItemWidth(App::DPIHandler::scale_value(80));
    if (ImGui::InputInt("##inputnumber_digits", &m_digits)) {
      if (m_digits > 9) {
        m_digits = 9;
      }
      else if (m_digits < 1)
        m_digits = 1;
    }

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->variable = m_variable;
      command->digits = m_digits;
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
