#include "Dialog_MovementChangeOpacity.hpp"
#include <tuple>
#include "imgui.h"
#include "Core/Application.hpp"
#include "Core/DPIHandler.hpp"

std::tuple<bool, bool> Dialog_MovementChangeOpacity::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{135, 95} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open,
                             ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_AlwaysAutoResize)) {

    ImGui::SeparatorText("Opacity");
    ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
    if (ImGui::InputInt("##movement_opacity_input", &m_opacity)) {
      if (m_opacity < 0)
        m_opacity = 0;
      if (m_opacity > 255)
        m_opacity = 255;
    }

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->opacity = m_opacity;
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
