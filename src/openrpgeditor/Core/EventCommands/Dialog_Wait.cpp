#include "Core/EventCommands/Dialog_Wait.hpp"

#include "Database/Database.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_Wait::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{241, 92}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    ImGui::SeparatorText("Duration");
    ImGui::SetNextItemWidth(100);
    if (ImGui::InputInt("##wait_input", &m_waitDuration)) {
      if (m_waitDuration < 1)
        m_waitDuration = 1;
      if (m_waitDuration > 999)
        m_waitDuration = 999;
    }
    ImGui::SameLine();
    ImGui::Text("frames 1/60 sec");

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->duration = m_waitDuration;
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
