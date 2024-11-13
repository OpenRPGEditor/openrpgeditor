#include "Core/EventCommands/MovementRoute/Dialog_MovementJump.hpp"
#include <tuple>
#include "imgui.h"
#include "Core/Application.hpp"
#include "Core/DPIHandler.hpp"

std::tuple<bool, bool> Dialog_MovementJump::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{235, 110} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open,
                             ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::SeparatorText("Offset");
    ImGui::BeginGroup(); {
      ImGui::Text("X:");
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
      if (ImGui::InputInt("##moveroute_jump_x", &m_jump_x)) {
        if (m_jump_x < -99)
          m_jump_x = -99;
        if (m_jump_x > 999)
          m_jump_x = 999;
      }
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPos().y - 3.f);
    ImGui::BeginGroup(); {
      ImGui::Text("Y:");
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
      if (ImGui::InputInt("##moveroute_jump_y", &m_jump_y)) {
        if (m_jump_x < -99)
          m_jump_x = -99;
        if (m_jump_x > 999)
          m_jump_x = 999;
      }
      ImGui::EndGroup();
    }
    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->x = m_jump_x;
      command->y = m_jump_y;
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
