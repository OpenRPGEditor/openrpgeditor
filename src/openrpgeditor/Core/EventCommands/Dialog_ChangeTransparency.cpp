#include "Core/EventCommands/Dialog_ChangeTransparency.hpp"

#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_ChangeTransparency::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{170, 97}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    ImGui::SeparatorText("Transparency");

    ImGui::RadioButton("ON", &isDisabled, 0);
    ImGui::SameLine();
    ImGui::RadioButton("OFF", &isDisabled, 1);

    if (ImGui::Button("OK")) {
      command->transparency = isDisabled == 0 ? ValueControl::ON : ValueControl::OFF;
      ImGui::CloseCurrentPopup();
      setOpen(false);
      m_confirmed = true;
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
