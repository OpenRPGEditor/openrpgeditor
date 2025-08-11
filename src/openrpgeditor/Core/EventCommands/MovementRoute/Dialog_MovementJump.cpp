#include "Core/EventCommands/MovementRoute/Dialog_MovementJump.hpp"
#include "Core/Application.hpp"

#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "imgui.h"
#include "imgui_internal.h"

#include <tuple>

std::tuple<bool, bool> Dialog_MovementJump::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###MovementJump");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 30) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 6) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});
  if (ImGui::BeginPopupModal(std::format("{}###MovementJump", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::BeginVertical("##movejump_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      ImGui::BeginHorizontal("##movement_jump_inner_layout", {-1, 0}, 0);
      {
        GroupBox xGroupBox(trNOOP("X"), "##movejump_x_label_group", {ImGui::GetContentRegionAvail().x * 0.5f, 0});
        if (xGroupBox.begin()) {
          ImGui::SetNextItemWidth(-1);
          ImGui::InputInt("##movejump_x", &m_jumpX);
        }
        xGroupBox.end();
        GroupBox yGroupBox(trNOOP("Y"), "##movejump_y_label_group", {-1, 0});

        if (yGroupBox.begin()) {
          ImGui::SetNextItemWidth(-1);
          ImGui::InputInt("##movejump_y", &m_jumpY);
        }
        yGroupBox.end();
      }
      ImGui::EndHorizontal();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##movejump_buttons_layout", {-1, -1}, 0);
      {
        ImGui::Spring();
        ImGui::SetNextItemWidth(-1);
        if (const auto ret = ImGui::ButtonGroup("##movejump_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->x = m_jumpX;
          m_command->y = m_jumpY;
          ImGui::CloseCurrentPopup();
          setOpen(false);
        } else if (ret == 1) {
          ImGui::CloseCurrentPopup();
          setOpen(false);
        }
      }
      ImGui::EndHorizontal();
    }
    ImGui::EndVertical();
    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
