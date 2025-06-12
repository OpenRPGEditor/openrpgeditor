#include "Core/EventCommands/MovementRoute/Dialog_MovementChangeSpeed.hpp"
#include "Core/Application.hpp"

#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_MovementChangeSpeed::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{140, 97}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    ImGui::SeparatorText("Speed");
    ImGui::PushItemWidth((100));

    if (ImGui::BeginCombo("##movement_frequency_selection", DecodeEnumName(static_cast<MovementSpeed>(m_speed)).c_str())) {
      for (auto& speed : magic_enum::enum_values<MovementSpeed>()) {
        bool is_selected = (m_speed == static_cast<int>(speed));
        if (ImGui::Selectable(DecodeEnumName(speed).c_str(), is_selected)) {
          m_speed = static_cast<int>(speed);
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
      setOpen(false);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
      setOpen(false);
    }

    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
