#include "Core/EventCommands/MovementRoute/Dialog_MovementChangeOpacity.hpp"
#include "Core/Application.hpp"

#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_MovementChangeOpacity::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{135, 95}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    ImGui::SeparatorText("Opacity");
    ImGui::SetNextItemWidth(100);
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
