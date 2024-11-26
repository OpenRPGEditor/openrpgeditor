#include "Core/EventCommands/Dialog_ChangeItems.hpp"

#include "Core/DPIHandler.hpp"
#include "Database/Database.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_ChangeItems::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{254, 205} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    if (picker) {
      auto [closed, confirmed] = picker->draw();
      if (closed) {
        if (confirmed) {
          m_quantity_var = picker->selection();
        }
        picker.reset();
      }
    }

    if (item_picker) {
      auto [closed, confirmed] = item_picker->draw();
      if (closed) {
        if (confirmed) {
          m_item = item_picker->selection();
        }
        item_picker.reset();
      }
    }

    // Section 1 Armor
    ImGui::SeparatorText("Item");
    ImGui::PushID("##changeitems_item");
    if (ImGui::Button(Database::instance()->itemNameOrId(m_item).c_str(), ImVec2{200 - (15 * App::DPIHandler::get_ui_scale()), 0})) {
      item_picker = ObjectPicker<Item>("Items"sv, Database::instance()->items.items(), m_item);
      item_picker->setOpen(true);
    }
    ImGui::PopID();

    // Section 2 (Operation: Increase/Decrease)
    ImGui::SeparatorText("Operation");
    ImGui::RadioButton("Increase", &m_operation, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Decrease", &m_operation, 1);

    // Section 3 (Operand: Constant/Variable)
    ImGui::SeparatorText("Operand");
    ImGui::BeginGroup();
    {
      ImGui::RadioButton("Constant", &m_operandSource, 0);
      ImGui::RadioButton("Variable", &m_operandSource, 1);
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      ImGui::BeginDisabled(m_operandSource != 0);
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
      if (ImGui::InputInt("##changeenemyhp_constant", &m_quantity)) {
        if (m_quantity > 9999)
          m_quantity = 9999;
        if (m_quantity < 0)
          m_quantity = 0;
      }
      ImGui::EndDisabled();

      ImGui::BeginDisabled(m_operandSource != 1);
      ImGui::PushID("##changeenemyhp_quant_var");
      if (ImGui::Button(m_operandSource == 1 ? Database::instance()->variableNameAndId(m_quantity_var).c_str() : "", ImVec2{200 - (15 * App::DPIHandler::get_ui_scale()), 0})) {
        picker.emplace("Variables", Database::instance()->system.variables, m_quantity_var);
        picker->setOpen(true);
      }
      ImGui::PopID();
      ImGui::EndDisabled();

      ImGui::EndGroup();
    }
    if (ImGui::Button("OK")) {
      m_confirmed = true;

      command->item = m_item;
      command->operation = static_cast<QuantityChangeOp>(m_operation);
      command->operandSource = static_cast<QuantityChangeSource>(m_operandSource);

      if (command->operandSource == QuantityChangeSource::Variable)
        command->operand = m_quantity_var;
      else
        command->operand = m_quantity;

      ImGui::CloseCurrentPopup();
      setOpen(false);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
      setOpen(false);
    }

    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
