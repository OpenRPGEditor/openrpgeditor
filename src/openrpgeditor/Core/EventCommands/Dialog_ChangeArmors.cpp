#include "Core/EventCommands/Dialog_ChangeArmors.hpp"

#include "Database/Armors.hpp"
#include "Database/Database.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_ChangeArmors::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{254, 225}, ImGuiCond_Appearing);
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

    if (armor_picker) {
      auto [closed, confirmed] = armor_picker->draw();
      if (closed) {
        if (confirmed) {
          m_item = armor_picker->selection();
        }
        armor_picker.reset();
      }
    }

    // Section 1 Armor
    ImGui::SeparatorText("Armor");
    ImGui::PushID("##changearmors_item");
    if (ImGui::Button(Database::instance()->armorNameOrId(m_item).c_str(), ImVec2{200 - 15, 0})) {
      armor_picker = ObjectPicker<Armor>("Armor"sv, Database::instance()->armors.armorList(), m_item);
      armor_picker->setOpen(true);
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
      ImGui::SetNextItemWidth(100);
      if (ImGui::InputInt("##changeenemyhp_constant", &m_quantity)) {
        if (m_quantity > 9999)
          m_quantity = 9999;
        if (m_quantity < 0)
          m_quantity = 0;
      }
      ImGui::EndDisabled();

      ImGui::BeginDisabled(m_operandSource != 1);
      ImGui::PushID("##changeenemyhp_quant_var");
      if (ImGui::Button(m_operandSource == 1 ? Database::instance()->variableNameAndId(m_quantity_var).c_str() : "", ImVec2{200 - 15, 0})) {
        picker.emplace("Variables", Database::instance()->system.variables(), m_quantity_var);
        picker->setOpen(true);
      }
      ImGui::PopID();
      ImGui::EndDisabled();

      ImGui::EndGroup();
    }
    ImGui::BeginDisabled(m_operation == 0);
    ImGui::Checkbox("Include Equipment", &m_includeEquipment);
    ImGui::EndDisabled();

    if (ImGui::Button("OK")) {
      m_confirmed = true;

      command->item = m_item;
      command->operation = static_cast<QuantityChangeOp>(m_operation);
      command->operandSource = static_cast<QuantityChangeSource>(m_operandSource);
      command->includeEquipment = m_includeEquipment;

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
