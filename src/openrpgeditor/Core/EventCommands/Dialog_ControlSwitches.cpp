#include "Core/EventCommands/Dialog_ControlSwitches.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Core/Log.hpp"
#include "Database/Database.hpp"
#include <imgui.h>
#include <imgui_internal.h>

std::tuple<bool, bool> Dialog_ControlSwitches::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ControlSwitches");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 30) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 8) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###ControlSwitches", m_name).c_str(), &m_open, ImGuiWindowFlags_NoResize)) {
    drawPickers();
    ImGui::BeginVertical("##control_switches_main_layout", ImGui::GetContentRegionAvail());
    {
      GroupBox switchGroup(trNOOP("Switch"), "##control_switches_switch_group", {-1, 0});
      if (switchGroup.begin()) {
        ImGui::BeginHorizontal("##control_switches_switch_group_layout", {-1, 0}, 0);
        {
          ImGui::BeginVertical("##control_switches_switch_group_layout_left");
          {
            ImGui::RadioButton(trNOOP("Single"), &m_operation, 0);
            ImGui::RadioButton(trNOOP("Range"), &m_operation, 1);
          }
          ImGui::EndVertical();
          ImGui::BeginVertical("##control_switches_switch_group_layout_right");
          {
            ImGui::BeginDisabled(m_operation != 0);
            {
              if (ImGui::EllipsesButton(m_operation != 0 ? "##control_switches_switch_empty" : (m_start == 0 ? "" : Database::instance()->switchNameAndId(m_start)).c_str(), ImVec2{-1, 0})) {
                picker.emplace(VariableSwitchPicker::Type::Switch, Database::instance()->system.switches(), m_start);
                picker->setOpen(true);
              }
            }
            ImGui::EndDisabled();
            ImGui::BeginChild("##control_switches_switch_range", {-1, 0}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_NavFlattened,
                              ImGuiWindowFlags_NoBackground);
            {
              ImGui::BeginDisabled(m_operation != 1);
              {
                ImGui::SetNextItemWidth((ImGui::GetContentRegionAvail().x * .5f) - (ImGui::CalcTextSize("~").x / 2));
                if (ImGui::SpinInt("##control_switches_switch_range_min", &m_rangeMin, 1, 100, m_operation == 1 ? nullptr : "")) {
                  m_rangeMin = std::clamp(m_rangeMin, 1, INT_MAX);
                }
                ImGui::SameLine();
                ImGui::Text("~");
                ImGui::SameLine();
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                if (ImGui::SpinInt("##control_switches_switch_range_max", &m_rangeMax, 1, 100, m_operation == 1 ? nullptr : "")) {
                  m_rangeMax = std::clamp(m_rangeMax, 1, INT_MAX);
                }
              }
              ImGui::EndDisabled();
            }
            ImGui::EndChild();
          }
          ImGui::EndVertical();
        }
        ImGui::EndHorizontal();
      }
      switchGroup.end();
      GroupBox operationGroup(trNOOP("Operation"), "##control_switches_operation_group", {-1, 0});
      if (operationGroup.begin()) {
        ImGui::BeginHorizontal("##control_switches_operation_group_layout", {ImGui::GetContentRegionAvail().x, -1}, 0);
        {
          ImGui::Spring(0.33);
          ImGui::RadioButton(trNOOP("ON"), &m_isDisabled, 0);
          ImGui::Spring(0.33);
          ImGui::RadioButton(trNOOP("OFF"), &m_isDisabled, 1);
          ImGui::Spring(0.33);
        }
        ImGui::EndHorizontal();
      }
      operationGroup.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##control_switches_buttons_layout", {-1, 0}, 0);
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##control_switches_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          if (m_operation == 0) {
            command->start = m_start;
            command->end = m_end;
          } else {
            command->start = m_rangeMin;
            command->end = m_rangeMax;
          }
          command->turnOff = static_cast<ValueControl>(m_isDisabled);
          ImGui::CloseCurrentPopup();
          setOpen(false);
        } else if (ret == 1) {
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

void Dialog_ControlSwitches::drawPickers() {
  if (picker) {
    if (const auto [closed, confirmed] = picker->draw(); closed) {
      if (confirmed) {
        m_start = picker->selection();
        m_end = picker->selection();
      }
      picker.reset();
    }
  }
}