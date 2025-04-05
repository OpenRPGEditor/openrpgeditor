#include "Core/EventCommands/Dialog_TimerControl.hpp"

#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_TimerControl::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{248, 163}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    ImGui::SeparatorText("Operation");

    ImGui::RadioButton("Start", &m_operation, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Stop", &m_operation, 1);

    ImGui::SeparatorText("Time");
    ImGui::SetNextItemWidth(100);
    ImGui::InputInt("minute(s)", &m_minute);
    ImGui::SetNextItemWidth(100);
    ImGui::InputInt("second(s)", &m_second);

    if (ImGui::Button("OK")) {

      m_confirmed = true;
      command->control = static_cast<TimerControl>(m_operation);
      command->seconds = (m_minute * 60) + m_second;
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
