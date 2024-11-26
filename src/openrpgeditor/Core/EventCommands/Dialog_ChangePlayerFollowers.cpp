#include "Core/EventCommands/Dialog_ChangePlayerFollowers.hpp"

#include "Core/DPIHandler.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_ChangePlayerFollowers::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{248, 100} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    ImGui::SeparatorText("Player Followers");

    ImGui::RadioButton("ON", &m_checkIfOn, 0);
    ImGui::SameLine();
    ImGui::RadioButton("OFF", &m_checkIfOn, 1);

    if (ImGui::Button("OK")) {
      command->followersEnabled = m_checkIfOn == 0 ? ValueControl::ON : ValueControl::OFF;
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
