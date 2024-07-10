#include "Dialog_MovementChangeSpeed.hpp"
#include <tuple>
#include "imgui.h"
#include "Core/Application.hpp"
#include "Core/DPIHandler.hpp"

std::tuple<bool, bool> Dialog_MovementChangeSpeed::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{140, 97} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize)) {

    ImGui::SeparatorText("Speed");
    ImGui::PushItemWidth((App::DPIHandler::scale_value(100)));
    if (ImGui::BeginCombo("##movement_frequency_selection", DecodeEnumName(magic_enum::enum_value<MovementSpeed>(m_speed)).c_str())) {
      for (auto& speed : magic_enum::enum_values<MovementSpeed>()) {
        bool is_selected = m_speed == magic_enum::enum_index(speed).value();
        if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(speed)).c_str(), is_selected)) {
          m_speed = magic_enum::enum_index(speed).value();
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }


    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->speed = m_speed;
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
