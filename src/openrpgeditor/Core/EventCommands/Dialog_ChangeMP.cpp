#include "Core/EventCommands/Dialog_ChangeMP.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Database.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>

std::tuple<bool, bool> Dialog_ChangeMP::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ChangeMP");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize =
      ImVec2{ImGui::CalcTextSize("#############################").x + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 11) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});
  if (ImGui::BeginPopupModal(std::format("{}###ChangeMP", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    drawPickers();
    ImGui::BeginVertical("##change_mp_main_layout", ImGui::GetContentRegionAvail());
    {
      GroupBox actorGroupBox(trNOOP("Actor"), "##change_mp_actor_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (actorGroupBox.begin()) {
        ImGui::BeginHorizontal("##change_mp_actor_group_horizontal", {-1, -1});
        {
          ImGui::BeginVertical("##change_mp_actor_group_radios", {-1, -1});
          {
            ImGui::RadioButton(trNOOP("Fixed"), &m_comparison, 0);
            ImGui::RadioButton(trNOOP("Variable"), &m_comparison, 1);
          }
          ImGui::EndVertical();
          ImGui::Spring();
          ImGui::BeginVertical("##change_mp_actor_group_buttons", {-1, -1});
          {
            ImGui::BeginDisabled(m_comparison != 0);
            ImGui::PushID("##change_mp_actor_group_fixed_button");
            if (ImGui::EllipsesButton(m_comparison == 0 ? Database::instance()->actorNameAndId(m_value).c_str() : "", {-1, 0})) {
              m_actorPicker = ObjectPicker(trNOOP("Actor"), Database::instance()->actors.actorList(), m_value);
              m_actorPicker->setOpen(true);
            }
            ImGui::PopID();
            ImGui::EndDisabled();
            ImGui::BeginDisabled(m_comparison != 1);
            ImGui::PushID("##change_mp_actor_group_var");
            if (ImGui::EllipsesButton(m_comparison == 1 ? Database::instance()->variableNameAndId(m_value).c_str() : "", {-1, 0})) {
              m_isOperand = false;
              m_variablePicker.emplace(trNOOP("Variables"), Database::instance()->system.variables(), m_value);
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
      GroupBox operationGroupBox(trNOOP("Operation"), "##change_mp_operation_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (operationGroupBox.begin()) {
        ImGui::BeginHorizontal("##change_mp_operation_group_horizontal", {ImGui::GetContentRegionAvail().x, -1});
        {
          ImGui::Spring(0.33f);
          ImGui::RadioButton(trNOOP("Increase"), &m_quantityOp, 0);
          ImGui::Spring(0.33f);
          ImGui::RadioButton(trNOOP("Decrease"), &m_quantityOp, 1);
          ImGui::Spring(0.33f);
        }
        ImGui::EndHorizontal();
      }
      operationGroupBox.end();
      GroupBox operandGroupBox(trNOOP("Operand"), "##change_mp_operand", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (operandGroupBox.begin()) {
        ImGui::BeginHorizontal("##change_mp_operand_horizontal", {-1, -1});
        {
          ImGui::BeginVertical("##change_mp_operand_radios", {-1, -1});
          {
            ImGui::RadioButton(trNOOP("Constant"), &m_quantitySource, 0);
            ImGui::RadioButton(std::format("{}##operand", tr("Variable")).c_str(), &m_quantitySource, 1);
          }
          ImGui::EndVertical();
          ImGui::Spring();
          ImGui::BeginVertical("##change_mp_operand_buttons", {-1, -1});
          {
            ImGui::BeginDisabled(m_quantitySource != 0);
            ImGui::SetNextItemWidth(-1);
            if (ImGui::SpinInt("##change_mp_operand_constant", &m_quantity, 1, 100, m_quantitySource == 0 ? nullptr : "")) {
              m_quantity = std::clamp(m_quantity, 1, 9999);
            }
            ImGui::EndDisabled();

            ImGui::BeginDisabled(m_quantitySource != 1);
            ImGui::PushID("##change_mp_operand_quantity_variable");
            if (ImGui::EllipsesButton(m_quantitySource == 1 ? Database::instance()->variableNameAndId(m_quantityVar).c_str() : "", ImVec2{-1, 0})) {
              m_isOperand = true;
              m_variablePicker.emplace("Variables", Database::instance()->system.variables(), m_quantityVar);
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
      ImGui::BeginHorizontal("##change_mp_dialog_buttons", {-1, -1});
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##change_mp_dialog_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->comparison = static_cast<ActorComparisonSource>(m_comparison);
          m_command->quantityOp = static_cast<QuantityChangeOp>(m_quantityOp);
          m_command->quantitySource = static_cast<QuantityChangeSource>(m_quantitySource);

          if (m_command->comparison == ActorComparisonSource::Variable)
            m_command->value = m_valueVar;
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

void Dialog_ChangeMP::drawPickers() {
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
        if (m_isOperand) {
          m_quantityVar = m_variablePicker->selection();
        } else {
          m_valueVar = m_variablePicker->selection();
        }
      }
      m_variablePicker.reset();
    }
  }
}
