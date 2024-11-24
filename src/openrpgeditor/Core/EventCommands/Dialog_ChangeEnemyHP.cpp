#include "Core/EventCommands/Dialog_ChangeEnemyHP.hpp"

#include "Core/DPIHandler.hpp"
#include "Database/Database.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_ChangeEnemyHP::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{254, 250} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    if (picker) {
      auto [closed, confirmed] = picker->draw();
      if (confirmed) {
        m_quantity_var = picker->selection();
        picker.reset();
      }
    }
    // Section 1 Enemy
    ImGui::SeparatorText("Enemy");
    ImGui::PushItemWidth((App::DPIHandler::scale_value(200)));
    if (ImGui::BeginCombo("##changeenemyhp_list", (m_enemy > -1 ? "#" + std::to_string(m_enemy + 1) : "" + Database::instance()->troopMemberName(0, m_enemy)).c_str())) {
      if (ImGui::Selectable("Entire Troop", m_enemy == -1)) {
        m_enemy = -1;
      }
      for (int i = 0; i < 8; ++i) {
        if (ImGui::Selectable((i > -1 ? "#" + std::to_string(i + 1) : "" + Database::instance()->troopMemberName(0, i)).c_str(), i == m_enemy)) {
          m_enemy = i;
        }
      }
      ImGui::EndCombo();
    }

    // Section 2 (Operation: Increase/Decrease)
    ImGui::SeparatorText("Operation");
    ImGui::RadioButton("Increase", &m_enemyOp, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Decrease", &m_enemyOp, 1);

    // Section 3 (Operand: Constant/Variable)
    ImGui::SeparatorText("Operand");
    ImGui::BeginGroup();
    {
      ImGui::RadioButton("Constant", &m_quantitySource, 0);
      ImGui::RadioButton("Variable", &m_quantitySource, 1);
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      ImGui::BeginDisabled(m_quantitySource != 0);
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
      if (ImGui::InputInt("##changeenemyhp_constant", &m_quantity)) {
        if (m_quantity > 9999)
          m_quantity = 9999;
        if (m_quantity < 0)
          m_quantity = 0;
      }
      ImGui::EndDisabled();

      ImGui::BeginDisabled(m_quantitySource != 1);
      ImGui::PushID("##changeenemyhp_quant_var");
      if (ImGui::Button(m_quantitySource == 1 ? Database::instance()->variableNameAndId(m_quantity_var).c_str() : "", ImVec2{200 - (15 * App::DPIHandler::get_ui_scale()), 0})) {
        picker.emplace("Variables", Database::instance()->system.variables);
      }
      ImGui::PopID();
      ImGui::EndDisabled();

      ImGui::EndGroup();
    }
    ImGui::BeginDisabled(m_enemyOp == 0);
    ImGui::Checkbox("Allow Knockout", &m_allowKnockout);
    ImGui::EndDisabled();

    if (ImGui::Button("OK")) {
      m_confirmed = true;

      command->enemy = m_enemy;
      command->enemyOp = static_cast<QuantityChangeOp>(m_enemyOp);
      command->quantitySource = static_cast<QuantityChangeSource>(m_quantitySource);
      command->allowKnockOut = m_allowKnockout;

      if (command->quantitySource == QuantityChangeSource::Variable)
        command->quantity = m_quantity_var;
      else
        command->quantity = m_quantity;

      ImGui::CloseCurrentPopup();
      SetOpen(false);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
      SetOpen(false);
    }

    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
