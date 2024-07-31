#include "Dialog_Wait.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Database/Database.hpp"

std::tuple<bool, bool> Dialog_Wait::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{241, 92} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize)) {

    ImGui::SeparatorText("Duration");
    ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
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
