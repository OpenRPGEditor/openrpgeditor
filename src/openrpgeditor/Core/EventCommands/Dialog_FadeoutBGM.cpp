#include "Core/EventCommands/Dialog_FadeoutBGM.hpp"

#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_FadeoutBGM::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{241, 92}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    ImGui::SeparatorText("Duration");
    ImGui::SetNextItemWidth(100);
    ImGui::InputInt("##fadeoutbgm_input", &m_duration);
    ImGui::SameLine();
    ImGui::Text("seconds");

    if (ImGui::Button("OK")) {
      if (m_duration < 1)
        m_duration = 1;
      if (m_duration > 999)
        m_duration = 999;

      m_confirmed = true;
      command->duration = m_duration;
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
