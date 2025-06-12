#include "Core/EventCommands/MovementRoute/Dialog_MovementChangeFrequency.hpp"
#include "Core/Application.hpp"

#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_MovementChangeFrequency::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{140, 97}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    ImGui::SeparatorText("Frequency");
    ImGui::PushItemWidth((100));
    if (ImGui::BeginCombo("##movement_frequency_selection", DecodeEnumName(static_cast<MovementFrequency>(m_frequency)).c_str())) {
      for (auto& freq : magic_enum::enum_values<MovementFrequency>()) {
        bool is_selected = (m_frequency == static_cast<int>(freq));
        if (ImGui::Selectable(DecodeEnumName(freq).c_str(), is_selected)) {
          m_frequency = static_cast<int>(freq);
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->frequency = m_frequency;
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
