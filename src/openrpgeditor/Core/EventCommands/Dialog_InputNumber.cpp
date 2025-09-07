#include "Core/EventCommands/Dialog_InputNumber.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Database.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <tuple>

std::tuple<bool, bool> Dialog_InputNumber::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###InputNumber");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 30) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 7) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###InputNumber", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    drawPickers();

    ImGui::BeginVertical("##input_number_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      GroupBox variableGroupBox(trNOOP("Variable"), "##input_number_variable_group_box", {-1, 0});
      if (variableGroupBox.begin()) {
        if (ImGui::EllipsesButton(Database::instance()->variableNameAndId(m_variable).c_str(), {-1, 0})) {
          m_variablePicker.emplace(VariableSwitchPicker::Type::Variable, Database::instance()->system.variables(), m_variable);
          m_variablePicker->setOpen(true);
        }
      }
      variableGroupBox.end();
      GroupBox digitsGroupBox(trNOOP("Digits"), "##input_number_digits_group_box", {-1, 0});
      if (digitsGroupBox.begin()) {
        ImGui::SetNextItemWidth(-1);
        if (ImGui::SpinInt("##input_number_digits", &m_digits)) {
          m_digits = std::clamp(m_digits, 1, 9);
        }
      }
      digitsGroupBox.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##input_number_button_layout");
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##input_number_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->variable = m_variable;
          m_command->digits = m_digits;
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

void Dialog_InputNumber::drawPickers() {
  if (m_variablePicker) {
    if (const auto [closed, confirmed] = m_variablePicker->draw(); closed) {
      if (confirmed) {
        m_variable = m_variablePicker->selection();
      }
      m_variablePicker.reset();
    }
  }
}