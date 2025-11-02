#include "Editor/EventCommands/Dialog_ChangeGold.hpp"

#include "Database/Database.hpp"
#include "imgui.h"
#include <tuple>

#include "Editor/CommonUI/GroupBox.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"

#include "Database/Database.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>

std::tuple<bool, bool> Dialog_ChangeGold::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ChangeGold");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize =
      ImVec2{ImGui::CalcTextSize("#############################").x + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 8) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});
  if (ImGui::BeginPopupModal(std::format("{}###ChangeGold", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    drawPickers();

    ImGui::BeginVertical("##change_gold_main_layout", ImGui::GetContentRegionAvail());
    {
      GroupBox operationGroupBox(trNOOP("Operation"), "##change_exp_operation_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (operationGroupBox.begin()) {
        ImGui::BeginHorizontal("##change_exp_operation_group_horizontal", {ImGui::GetContentRegionAvail().x, -1});
        {
          ImGui::Spring(0.33f);
          ImGui::RadioButton(trNOOP("Increase"), &m_operation, 0);
          ImGui::Spring(0.33f);
          ImGui::RadioButton(trNOOP("Decrease"), &m_operation, 1);
          ImGui::Spring(0.33f);
        }
        ImGui::EndHorizontal();
      }
      operationGroupBox.end();
      GroupBox operandGroupBox(trNOOP("Operand"), "##change_exp_operand", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (operandGroupBox.begin()) {
        ImGui::BeginHorizontal("##change_exp_operand_horizontal", {-1, -1});
        {
          ImGui::BeginVertical("##change_exp_operand_radios", {-1, -1});
          {
            ImGui::RadioButton(trNOOP("Constant"), &m_operandSource, 0);
            ImGui::RadioButton(trNOOP("Variable"), &m_operandSource, 1);
          }
          ImGui::EndVertical();
          ImGui::Spring();
          ImGui::BeginVertical("##change_gold_operand_inputs", {-1, -1});
          {
            ImGui::BeginDisabled(m_operandSource != 0);
            ImGui::SetNextItemWidth(-1);
            ImGui::SpinInt("##change_gold_constant_input", &m_constant, 1, 100, m_operandSource == 0 ? nullptr : "");
            ImGui::EndDisabled();

            ImGui::BeginDisabled(m_operandSource != 1);
            const std::string text = m_operandSource != 1 ? "##change_gold_variable_empty" : Database::instance()->variableNameOrId(m_variable);
            ImGui::PushID("##change_gold_variable_input");
            if (ImGui::EllipsesButton(text.c_str(), ImVec2{-1, 0})) {
              m_variablePicker.emplace(VariableSwitchPicker::Type::Variable, Database::instance()->system->variables(), m_variable);
              m_variablePicker->setOpen(true);
            }
            ImGui::PopID();
            ImGui::EndDisabled();
          }
          ImGui::EndVertical();
        }
        ImGui::EndHorizontal();
      }
      operandGroupBox.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5));
      ImGui::BeginHorizontal("##change_exp_dialog_buttons", {-1, -1});
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##change_exp_dialog_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->operation = static_cast<QuantityChangeOp>(m_operation);
          m_command->operandSource = static_cast<QuantityChangeSource>(m_operandSource);
          if (m_command->operandSource == QuantityChangeSource::Constant) {
            m_command->operand = m_constant;
          } else {
            m_command->operand = m_variable;
          }

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

void Dialog_ChangeGold::drawPickers() {
  if (m_variablePicker) {
    if (const auto [closed, confirmed] = m_variablePicker->draw(); closed) {
      if (confirmed) {
        m_variable = m_variablePicker->selection();
      }
      m_variablePicker.reset();
    }
  }
}