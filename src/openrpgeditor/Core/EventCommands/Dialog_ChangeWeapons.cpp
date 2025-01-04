#include "Core/EventCommands/Dialog_ChangeWeapons.hpp"


#include "Database/Database.hpp"
#include "Database/Weapons.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_ChangeWeapons::draw() {
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

    if (weapon_picker) {
      auto [closed, confirmed] = weapon_picker->draw();
      if (closed) {
        if (confirmed) {
          m_item = weapon_picker->selection();
        }
        weapon_picker.reset();
      }
    }

    // Section 1 Armor
    ImGui::SeparatorText("Weapon");
    ImGui::PushID("##changeweapons_item");
    if (ImGui::Button(Database::instance()->weaponNameOrId(m_item).c_str(), ImVec2{200 - 15, 0})) {
      weapon_picker = ObjectPicker<Weapon>("Weapon"sv, Database::instance()->weapons.weaponList(), m_item);
      weapon_picker->setOpen(true);
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
        picker.emplace("Variables", Database::instance()->system.variables, m_quantity_var);
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
