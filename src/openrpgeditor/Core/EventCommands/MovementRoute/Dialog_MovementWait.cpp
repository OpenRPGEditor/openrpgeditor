#include "Core/EventCommands/MovementRoute/Dialog_MovementWait.hpp"
#include "Core/Application.hpp"
#include "Core/DPIHandler.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_MovementWait::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{221, 97} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    ImGui::SeparatorText("Duration");
    ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
    if (ImGui::InputInt("##movement_wait_input", &m_waitDuration)) {
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
