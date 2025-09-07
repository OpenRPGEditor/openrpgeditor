#include "Core/EventCommands/Dialog_ChangeArmors.hpp"

#include "Database/Armors.hpp"
#include "Database/Database.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>

std::tuple<bool, bool> Dialog_ChangeArmors::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ChangeArmors");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize =
      ImVec2{ImGui::CalcTextSize("#############################").x + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 10) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###ChangeArmors", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    drawPickers();
    ImGui::BeginVertical("##change_armors_main_layout", ImGui::GetContentRegionAvail());
    {
      GroupBox armorGroupBox(trNOOP("Armor"), "##change_armors_item_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (armorGroupBox.begin()) {
        ImGui::PushID("##change_armors_item");
        if (ImGui::EllipsesButton(Database::instance()->armorNameAndId(m_item).c_str(), ImVec2{-1, 0})) {
          m_armorPicker = ArmorPicker(Database::instance()->armors.armorList(), m_item);
          m_armorPicker->setOpen(true);
        }
        ImGui::PopID();
      }
      armorGroupBox.end();

      GroupBox operationGroupBox(trNOOP("Operation"), "##change_armors_operation_group", {-1, 0}, nullptr,
                                 ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (operationGroupBox.begin()) {
        ImGui::BeginHorizontal("##change_armors_operation_group_layout", {ImGui::GetContentRegionAvail().x, -1});
        {
          ImGui::Spring(.33);
          ImGui::RadioButton("Increase", &m_operation, 0);
          ImGui::Spring(.33);
          ImGui::RadioButton("Decrease", &m_operation, 1);
          ImGui::Spring(.33);
        }
        ImGui::EndHorizontal();
      }
      operationGroupBox.end();

      GroupBox operandGroupBox(trNOOP("Operand"), "##change_armors_operand_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (operandGroupBox.begin()) {
        ImGui::BeginHorizontal("##change_armors_operand_group_layout", {ImGui::GetContentRegionAvail().x, -1});
        {
          ImGui::BeginVertical("##change_armors_operand_source", {-1, -1});
          {
            ImGui::RadioButton(trNOOP("Constant"), &m_operandSource, 0);
            ImGui::RadioButton(trNOOP("Variable"), &m_operandSource, 1);
          }
          ImGui::EndVertical();
          ImGui::BeginVertical("##change_armors_operand_value", {-1, -1});
          {
            ImGui::BeginDisabled(m_operandSource != 0);
            ImGui::SetNextItemWidth(-1);
            if (ImGui::SpinInt("##change_armors_constant", &m_quantity, 1, 100, m_operandSource == 0 ? nullptr : "")) {
              m_quantity = std::clamp(m_quantity, 1, 9999);
            }
            ImGui::EndDisabled();

            ImGui::BeginDisabled(m_operandSource != 1);
            ImGui::PushID("##change_armors_quant_var");
            if (ImGui::EllipsesButton(m_operandSource == 1 ? Database::instance()->variableNameAndId(m_quantityVar).c_str() : "", ImVec2{-1, 0})) {
              m_variablePicker.emplace(VariableSwitchPicker::Type::Variable, Database::instance()->system.variables(), m_quantityVar);
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
      ImGui::BeginHorizontal("##change_armors_buttons", {-1, -1});
      {
        ImGui::BeginDisabled(m_operation == 0);
        ImGui::Checkbox(trNOOP("Include Equipment"), &m_includeEquipment);
        ImGui::EndDisabled();
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##change_armors_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;

          m_command->item = m_item;
          m_command->operation = static_cast<QuantityChangeOp>(m_operation);
          m_command->operandSource = static_cast<QuantityChangeSource>(m_operandSource);
          m_command->includeEquipment = m_includeEquipment;

          if (m_command->operandSource == QuantityChangeSource::Variable)
            m_command->operand = m_quantityVar;
          else
            m_command->operand = m_quantity;

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

void Dialog_ChangeArmors::drawPickers() {
  if (m_variablePicker) {
    if (const auto [closed, confirmed] = m_variablePicker->draw(); closed) {
      if (confirmed) {
        m_quantityVar = m_variablePicker->selection();
      }
      m_variablePicker.reset();
    }
  }

  if (m_armorPicker) {
    if (const auto [closed, confirmed] = m_armorPicker->draw(); closed) {
      if (confirmed) {
        m_item = m_armorPicker->selection();
      }
      m_armorPicker.reset();
    }
  }
}