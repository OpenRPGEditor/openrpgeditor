#include "Editor/EventCommands/Dialog_ControlSelfSwitch.hpp"

#include "Editor/ImGuiExt/ImGuiUtils.hpp"
#include "Editor/Log.hpp"

#include "Database/Database.hpp"

#include "Editor/CommonUI/GroupBox.hpp"
#include "imgui.h"

#include <imgui_internal.h>

std::tuple<bool, bool> Dialog_ControlSelfSwitch::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ControlSelfSwitch");
  }
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 20) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetTextLineHeightWithSpacing() * 10) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});
  if (ImGui::BeginPopupModal(std::format("{}###ControlSelfSwitch", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::BeginVertical("##control_self_switch_main_layout", ImGui::GetContentRegionAvail(), 0.f);
    {
      GroupBox selfSwitchGroup(trNOOP("Self Switch"), "##control_self_switch_self_switch_group", {-1, 0});
      if (selfSwitchGroup.begin()) {
        ImGui::SetNextItemWidth(-1);
        if (ImGui::BeginCombo("###control_self_switch_self_switch_list", m_selfSw.c_str())) {
          for (const auto self : {"A", "B", "C", "D"}) {
            const bool selected = self == m_selfSw;
            if (ImGui::Selectable(self, self == m_selfSw)) {
              m_selfSw = self;
            }
            if (selected) {
              ImGui::SetItemDefaultFocus();
            }
          }
          ImGui::EndCombo();
        }
      }
      selfSwitchGroup.end();

      GroupBox operationGroup(trNOOP("Operation"), "##control_self_switch_self_operation_group", {-1, 0});
      if (operationGroup.begin()) {
        ImGui::BeginHorizontal("##control_self_switch_self_operation_group_layout", ImGui::GetContentRegionAvail(), 0.f);
        {
          ImGui::Spring(0.5f);
          ImGui::RadioButton(trNOOP("ON"), &m_turnOff, 0);
          ImGui::Spring(0.33f);
          ImGui::RadioButton(trNOOP("OFF"), &m_turnOff, 1);
          ImGui::Spring(0.5f);
        }
        ImGui::EndHorizontal();
      }
      operationGroup.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##control_self_switch_button_layout", {-1, -1}, 0.f);
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##control_self_switch_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->selfSw = m_selfSw;
          m_command->turnOff = static_cast<ValueControl>(m_turnOff);
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
