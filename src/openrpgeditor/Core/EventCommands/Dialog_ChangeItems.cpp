#include "Core/EventCommands/Dialog_ChangeItems.hpp"

#include "Database/Database.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>

std::tuple<bool, bool> Dialog_ChangeItems::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ChangeItems");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize =
      ImVec2{ImGui::CalcTextSize("#############################").x + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 10) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###ChangeItems", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    drawPickers();
    ImGui::BeginVertical("##change_items_main_layout", ImGui::GetContentRegionAvail());
    {
      GroupBox itemGroupBox(trNOOP("Item"), "##change_items_item_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (itemGroupBox.begin()) {
        ImGui::PushID("##change_items_item");
        if (ImGui::EllipsesButton(Database::instance()->itemNameAndId(m_item).c_str(), ImVec2{-1, 0})) {
          m_itemPicker = ObjectPicker(trNOOP("Items"), Database::instance()->items.items(), m_item);
          m_itemPicker->setOpen(true);
        }
        ImGui::PopID();
      }
      itemGroupBox.end();

      GroupBox operationGroupBox(trNOOP("Operation"), "##change_items_operation_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (operationGroupBox.begin()) {
        ImGui::BeginHorizontal("##change_items_operation_layout", {ImGui::GetContentRegionAvail().x, -1});
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

      GroupBox operandroupBox(trNOOP("Operand"), "##change_items_operand_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (operandroupBox.begin()) {
        ImGui::BeginHorizontal("##change_items_operand_layout");
        {
          ImGui::BeginVertical("##change_items_operand_source");
          {
            ImGui::RadioButton(trNOOP("Constant"), &m_operandSource, 0);
            ImGui::RadioButton(trNOOP("Variable"), &m_operandSource, 1);
          }
          ImGui::EndVertical();
          ImGui::BeginVertical("##change_items_operand_value");
          {
            ImGui::BeginDisabled(m_operandSource != 0);
            ImGui::SetNextItemWidth(-1);
            if (ImGui::SpinInt("###change_items_constant", &m_quantity, 1, 100, m_operandSource == 0 ? nullptr : "")) {
              m_quantity = std::clamp(m_quantity, 1, 9999);
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(m_operandSource != 1);
            ImGui::PushID("###change_items_quant_var");
            if (ImGui::EllipsesButton(m_operandSource == 1 ? Database::instance()->variableNameAndId(m_quantityVar).c_str() : "", ImVec2{-1, 0})) {
              m_variablePicker.emplace(trNOOP("Variables"), Database::instance()->system.variables(), m_quantityVar);
              m_variablePicker->setOpen(true);
            }
            ImGui::PopID();
            ImGui::EndDisabled();
          }
          ImGui::EndVertical();
        }
        ImGui::EndHorizontal();
      }
      operandroupBox.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5));
      ImGui::BeginHorizontal("##change_items_buttons");
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##recover_all_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;

          command->item = m_item;
          command->operation = static_cast<QuantityChangeOp>(m_operation);
          command->operandSource = static_cast<QuantityChangeSource>(m_operandSource);

          if (command->operandSource == QuantityChangeSource::Variable)
            command->operand = m_quantityVar;
          else
            command->operand = m_quantity;

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

void Dialog_ChangeItems::drawPickers() {
  if (m_variablePicker) {
    if (const auto [closed, confirmed] = m_variablePicker->draw(); closed) {
      if (confirmed) {
        m_quantityVar = m_variablePicker->selection();
      }
      m_variablePicker.reset();
    }
  }

  if (m_itemPicker) {
    if (const auto [closed, confirmed] = m_itemPicker->draw(); closed) {
      if (confirmed) {
        m_item = m_itemPicker->selection();
      }
      m_itemPicker.reset();
    }
  }
}