#include "Editor/EventCommands/Dialog_ChangeEnemyHP.hpp"

#include "Database/Database.hpp"

#include "Editor/CommonUI/GroupBox.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>
std::tuple<bool, bool> Dialog_ChangeEnemyHP::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ChangeEnemyHP");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize =
      ImVec2{ImGui::CalcTextSize("#############################").x + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 10) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###ChangeEnemyHP", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    drawPickers();
    ImGui::BeginVertical("##change_enemy_hp_main_layout", ImGui::GetContentRegionAvail());
    {
      GroupBox enemyGroupBox(trNOOP("Enemy"), "##change_enemy_hp_enemy_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (enemyGroupBox.begin()) {
        ImGui::SetNextItemWidth(-1);

        auto preview = Database::instance()->troopMemberName(m_troopId, m_troopMemberSelection);
        if (m_troopMemberSelection >= 0) {
          preview = std::format("#{} {}", m_troopMemberSelection + 1, preview);
        }

        if (ImGui::BeginCombo("##enemy_recover_all_change_list", preview.c_str())) {
          for (int i = -1; i < 8; ++i) {
            auto name = Database::instance()->troopMemberName(m_troopId, i);
            if (i >= 0) {
              name = std::format("#{} {}", i + 1, name);
            }

            if (ImGui::Selectable(name.c_str(), i == m_troopMemberSelection)) {
              m_troopMemberSelection = i;
            }
          }
          ImGui::EndCombo();
        }
      }
      enemyGroupBox.end();

      GroupBox operationGroupBox(trNOOP("Operation"), "##change_enemy_hp_operation_group", {-1, 0}, nullptr,
                                 ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (operationGroupBox.begin()) {
        ImGui::BeginHorizontal("##change_enemy_hp_operation_group_layout", {ImGui::GetContentRegionAvail().x, -1});
        {
          ImGui::Spring(0.33);
          ImGui::RadioButton(trNOOP("Increase"), &m_troopMemberOp, 0);
          ImGui::Spring(0.33);
          ImGui::RadioButton(trNOOP("Decrease"), &m_troopMemberOp, 1);
          ImGui::Spring(0.33);
        }
        ImGui::EndHorizontal();
      }
      operationGroupBox.end();
      GroupBox operandGroupBox(trNOOP("Operand"), "##change_enemy_hp_operand_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (operandGroupBox.begin()) {
        ImGui::BeginHorizontal("##change_enemy_hp_operand_group_layout", {-1, -1});
        {
          ImGui::BeginVertical("##change_enemy_hp_operand_group_source_layout", {-1, -1});
          {
            ImGui::RadioButton("Constant", &m_quantitySource, 0);
            ImGui::RadioButton("Variable", &m_quantitySource, 1);
          }
          ImGui::EndVertical();
          ImGui::BeginVertical("##change_enemy_hp_operand_group_value_layout", {-1, -1});
          {
            ImGui::BeginDisabled(m_quantitySource != 0);
            ImGui::SetNextItemWidth(-1);
            if (ImGui::SpinInt("##change_enemy_hp_constant", &m_quantity, 1, 100, m_quantitySource == 0 ? nullptr : "")) {
              m_quantity = std::clamp(m_quantity, 1, 9999);
            }
            ImGui::EndDisabled();

            ImGui::BeginDisabled(m_quantitySource != 1);
            ImGui::PushID("##change_enemy_hp_quant_var");
            if (ImGui::EllipsesButton(m_quantitySource == 1 ? Database::instance()->variableNameAndId(m_quantityVar).c_str() : "", ImVec2{-1, 0})) {
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
      ImGui::BeginHorizontal("##change_enemy_hp_buttons_layout", {-1, -1});
      {
        ImGui::BeginDisabled(m_troopMemberOp == 0);
        ImGui::Checkbox("Allow Knockout", &m_allowKnockout);
        ImGui::EndDisabled();
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##change_enemy_hp_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;

          m_command->troopMember = m_troopMemberSelection;
          m_command->troopMemberOp = static_cast<QuantityChangeOp>(m_troopMemberOp);
          m_command->quantitySource = static_cast<QuantityChangeSource>(m_quantitySource);
          m_command->allowKnockOut = m_allowKnockout;

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

void Dialog_ChangeEnemyHP::drawPickers() {
  if (m_variablePicker) {
    if (const auto [closed, confirmed] = m_variablePicker->draw(); closed) {
      if (confirmed) {
        m_quantityVar = m_variablePicker->selection();
      }
      m_variablePicker.reset();
    }
  }
}