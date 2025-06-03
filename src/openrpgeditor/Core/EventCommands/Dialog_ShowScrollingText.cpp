#include "Core/EventCommands/Dialog_ShowScrollingText.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include <tuple>

std::tuple<bool, bool> Dialog_ShowScrollingText::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ShowScrollingText");
  }
  const auto maxSize = ImVec2{ImGui::CalcTextSize("#").x * 64 + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 12) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});
  if (ImGui::BeginPopupModal(std::format("{}###ShowScrollingText", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::BeginVertical("##show_scrolling_text_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      GroupBox textGroupBox(trNOOP("Text"), "##text_group", {-1, 0});
      if (textGroupBox.begin()) {
        ImGui::InputTextMultiline("##show_scrolling_text_input", &m_textLine, {-1, ImGui::GetTextLineHeightWithSpacing() * 7 + ImGui::GetStyle().FramePadding.y});
      }
      textGroupBox.end();
      ImGui::Spring();
      GroupBox speedGroupBox(trNOOP("Speed"), "##speed_group", {-1, 0});
      if (speedGroupBox.begin()) {
        ImGui::SetNextItemWidth(-1);
        if (ImGui::SpinInt("##show_scrolling_text_speed", &m_speed)) {
          m_speed = std::clamp(m_speed, 1, 8);
        }
      }
      speedGroupBox.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##show_scrolling_text_button_layout", {-1, 0}, 0);
      {
        ImGui::Checkbox(trNOOP("No Fast Forward"), &m_noFast);
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##show_text_buttons", {trNOOP("Preview..."), trNOOP("OK"), trNOOP("Cancel")}, false, {trNOOP("UNIMPLEMENTED")}); ret == 0) {

        } else if (ret == 1) {
          m_confirmed = true;
          m_command->speed = m_speed;
          m_command->noFast = m_noFast;
          for (const auto split = splitString(m_textLine, '\n'); const auto& str : split) {
            m_command->text.emplace_back();
            m_command->text.back()->text = str;
          }
          setOpen(false);
        } else if (ret == 2) {
          setOpen(false);
        }
      }
      ImGui::EndHorizontal();
    }
    ImGui::EndVertical();
    ImGui::EndPopup();
  }

  return {!m_open, m_confirmed};
}
