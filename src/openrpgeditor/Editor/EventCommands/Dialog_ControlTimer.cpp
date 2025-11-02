#include "Editor/EventCommands/Dialog_ControlTimer.hpp"

#include "Editor/CommonUI/GroupBox.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <tuple>

std::tuple<bool, bool> Dialog_ControlTimer::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ControlTimer");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 24) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 8) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###ControlTimer", m_name).c_str(), &m_open, ImGuiWindowFlags_NoResize)) {
    ImGui::BeginVertical("##control_timer_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      GroupBox operationGroup(trNOOP("Operation"), "##control_timer_operation_group", {-1, 0});
      if (operationGroup.begin()) {
        ImGui::BeginHorizontal("##control_timer_operation_group_layout", {ImGui::GetContentRegionAvail().x, 0});
        {
          ImGui::Spring(0.5);
          ImGui::RadioButton(trNOOP("Start"), &m_operation, 0);
          ImGui::Spring(0.33f);
          ImGui::RadioButton(trNOOP("Stop"), &m_operation, 1);
          ImGui::Spring(0.5);
        }
        ImGui::EndHorizontal();
      }
      operationGroup.end();
      ImGui::Spring();
      GroupBox timeGroup(trNOOP("Time"), "##control_timer_time_group", {-1, 0});
      if (timeGroup.begin()) {
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * .5f);
        if (ImGui::SpinInt("##control_timer_time_group_minutes", &m_minute, 1, 5, trNOOP("%2d min"))) {
          m_minute = std::clamp(m_minute, 0, 99);
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        if (ImGui::SpinInt("##control_timer_time_group_seconds", &m_second, 1, 5, trNOOP("%2d sec"))) {
          m_second = std::clamp(m_second, 0, 59);
        }
      }
      timeGroup.end();

      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##control_timer_button_layout");
      {
        ImGui::Spring();

        if (const auto ret = ImGui::ButtonGroup("##control_timer_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->control = static_cast<TimerControl>(m_operation);
          m_command->seconds = (m_minute * 60) + m_second;
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
