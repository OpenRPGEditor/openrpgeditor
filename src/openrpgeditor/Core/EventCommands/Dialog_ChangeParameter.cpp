#include "Core/EventCommands/Dialog_ChangeParameter.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Database.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>

std::tuple<bool, bool> Dialog_ChangeParameter::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ChangeParameter");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize =
      ImVec2{ImGui::CalcTextSize("#############################").x + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 13) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});
  if (ImGui::BeginPopupModal(std::format("{}###ChangeParameter", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    drawPickers();
    ImGui::BeginVertical("##change_parameter_main_layout", ImGui::GetContentRegionAvail());
    {
      GroupBox actorGroupBox(trNOOP("Actor"), "##change_parameter_actor_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (actorGroupBox.begin()) {
        ImGui::BeginHorizontal("##change_parameter_actor_group_main_layout", {-1, -1});
        {
          ImGui::BeginVertical("##change_parameter_actor_group_comparison_layout", {-1, -1});
          {
            ImGui::RadioButton("Fixed", &m_comparison, 0);
            ImGui::RadioButton("Variable", &m_comparison, 1);
          }
          ImGui::EndVertical();
          ImGui::BeginVertical("##change_parameter_actor_group_object_layout", {-1, -1});
          {
            ImGui::BeginDisabled(m_comparison != 0);
            ImGui::PushID("##change_parameter_actor");
            if (ImGui::EllipsesButton(m_comparison == 0 ? Database::instance()->actorNameAndId(m_value).c_str() : "", ImVec2{-1, 0})) {
              m_actorPicker = ActorPicker(Database::instance()->actors->actorList(), m_value);
              m_actorPicker->setOpen(true);
            }
            ImGui::PopID();
            ImGui::EndDisabled();

            ImGui::BeginDisabled(m_comparison != 1);
            ImGui::PushID("##change_parameter_var");
            if (ImGui::EllipsesButton(m_comparison == 1 ? Database::instance()->variableNameAndId(m_value_var).c_str() : "", ImVec2{-1, 0})) {
              m_isOperand = false;
              m_variablePicker.emplace(VariableSwitchPicker::Type::Variable, Database::instance()->system->variables(), m_value_var);
              m_variablePicker->setOpen(true);
            }
            ImGui::PopID();
            ImGui::EndDisabled();
          }
          ImGui::EndVertical();
        }
        ImGui::EndHorizontal();
      }
      actorGroupBox.end();
      GroupBox parameterGroupBox(trNOOP("Parameter"), "##change_parameter_parameter_group", {-1, 0}, nullptr,
                                 ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (parameterGroupBox.begin()) {
        ImGui::SetNextItemWidth(-1);
        if (ImGui::BeginCombo("##parameter_selection", DecodeEnumName(magic_enum::enum_value<ParameterSource>(m_parameterSource)).c_str())) {
          for (auto& parameter : magic_enum::enum_values<ParameterSource>()) {
            bool isSelected = m_parameterSource == static_cast<int>(parameter);
            if ( ImGui::Selectable(DecodeEnumName(parameter).c_str(), isSelected)) {
              m_parameterSource = static_cast<int>(parameter);
            }
            if (isSelected)
              ImGui::SetItemDefaultFocus();
          }
          ImGui::EndCombo();
        }
      }
      parameterGroupBox.end();

      GroupBox operationGroupBox(trNOOP("Operation"), "##change_parameter_operation_group", {-1, 0}, nullptr,
                                 ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (operationGroupBox.begin()) {
        ImGui::BeginHorizontal("##change_parameter_operation_group_operation_layout", {ImGui::GetContentRegionAvail().x, -1});
        {
          ImGui::Spring(.33f);
          ImGui::RadioButton("Increase", &m_quantityOp, 0);
          ImGui::Spring(.33f);
          ImGui::RadioButton("Decrease", &m_quantityOp, 1);
          ImGui::Spring(.33f);
        }
        ImGui::EndHorizontal();
      }
      operationGroupBox.end();

      GroupBox operandGroupBox(trNOOP("Operand"), "##change_parameter_operand_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (operandGroupBox.begin()) {
        ImGui::BeginHorizontal("##change_parameter_operand_main_layout", {-1, -1});
        {
          ImGui::BeginVertical("##change_parameter_operand_source_layout", {-1, -1});
          {
            ImGui::RadioButton("Constant", &m_quantitySource, 0);
            ImGui::RadioButton("Variable##2", &m_quantitySource, 1);
          }
          ImGui::EndVertical();
          ImGui::BeginVertical("##change_parameter_operand_value_layout", {-1, -1});
          {
            ImGui::BeginDisabled(m_quantitySource != 0);
            ImGui::SetNextItemWidth(-1);
            if (ImGui::SpinInt("##change_parameter_constant", &m_quantity, 1, 100, m_quantitySource == 0 ? nullptr : "")) {
              m_quantity = std::clamp(m_quantity, 1, 9999);
            }
            ImGui::EndDisabled();

            ImGui::BeginDisabled(m_quantitySource != 1);
            ImGui::PushID("##change_parameter_quant_var");
            if (ImGui::EllipsesButton(m_quantitySource == 1 ? Database::instance()->variableNameAndId(m_quantityVar).c_str() : "", ImVec2{-1, 0})) {
              m_isOperand = true;
              m_variablePicker.emplace(VariableSwitchPicker::Type::Variable, Database::instance()->system->variables(), m_quantityVar);
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
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##change_parameter_buttons");
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##change_parameter_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;

          m_command->comparison = static_cast<ActorComparisonSource>(m_comparison);
          m_command->quantityOp = static_cast<QuantityChangeOp>(m_quantityOp);
          m_command->quantitySource = static_cast<QuantityChangeSource>(m_quantitySource);
          m_command->param = static_cast<ParameterSource>(m_parameterSource);

          if (m_command->comparison == ActorComparisonSource::Variable)
            m_command->value = m_value_var;
          else
            m_command->value = m_value;

          if (m_command->quantitySource == QuantityChangeSource::Variable)
            m_command->quantity = m_quantityVar;
          else
            m_command->quantity = m_quantity;

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

void Dialog_ChangeParameter::drawPickers() {
  if (m_actorPicker) {
    if (const auto [closed, confirmed] = m_actorPicker->draw(); closed) {
      if (confirmed) {
        m_value = m_actorPicker->selection();
      }
      m_actorPicker.reset();
    }
  }
  if (m_variablePicker) {
    if (const auto [closed, confirmed] = m_variablePicker->draw(); closed) {
      if (confirmed) {
        if (m_isOperand)
          m_quantityVar = m_variablePicker->selection();
        else
          m_value_var = m_variablePicker->selection();
      }
      m_variablePicker.reset();
    }
  }
}