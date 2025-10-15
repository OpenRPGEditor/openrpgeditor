#include "Core/EventCommands/Dialog_BattleProcessing.hpp"
#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"

#include <Database/Database.hpp>

#include <imgui.h>
#include <imgui_internal.h>
#include <tuple>

std::tuple<bool, bool> Dialog_BattleProcessing::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###BattleProcessing");
  }
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 40) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetTextLineHeightWithSpacing() * 12) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});
  if (ImGui::BeginPopupModal(std::format("{}###BattleProcessing", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    drawPickers();
    ImGui::BeginVertical("##battle_processing_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      GroupBox troopGroupBox(trNOOP("Troop"), "##battle_processing_troop_group_box", {-1, 0});
      if (troopGroupBox.begin()) {
        ImGui::RadioButton(trNOOP("Direct designation"), &m_type, 0);
        ImGui::Dummy({ImGui::GetFrameHeightWithSpacing(), ImGui::GetFrameHeightWithSpacing()});
        ImGui::SameLine();
        ImGui::BeginDisabled(m_type != 0);
        {
          if (ImGui::EllipsesButton(m_type == 0 ? Database::instance()->troopNameOrId(m_id).c_str() : "##battle_processing_troop_direct", {-1, 0})) {
            m_enemyPicker = TroopPicker(Database::instance()->troops->troops(), m_id);
            m_enemyPicker->setOpen(true);
          }
        }
        ImGui::EndDisabled();
        ImGui::RadioButton(trNOOP("Designation with variable"), &m_type, 1);
        ImGui::Dummy({ImGui::GetFrameHeightWithSpacing(), ImGui::GetFrameHeightWithSpacing()});
        ImGui::SameLine();
        ImGui::BeginDisabled(m_type != 1);
        {
          if (ImGui::EllipsesButton(m_type == 1 ? Database::instance()->variableNameAndId(m_varSelection).c_str() : "##battle_processing_troop_variable", {-1, 0})) {
            m_variablePicker = VariableSwitchPicker(VariableSwitchPicker::Type::Variable, Database::instance()->system->variables(), m_varSelection);
            m_variablePicker->setOpen(true);
          }
        }
        ImGui::EndDisabled();
        ImGui::RadioButton(trNOOP("Same as Random Encounter"), &m_type, 2);
      }
      troopGroupBox.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##battle_processing_button_layout", {-1, 0}, 0);
      {
        ImGui::Checkbox(trNOOP("Can Escape"), &m_canEscape);
        ImGui::Spring();
        ImGui::Checkbox(trNOOP("Can Lose"), &m_canLose);
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##battle_processing_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->type = static_cast<BattleProcessType>(m_type);
          m_command->id = m_id;
          m_command->canEscape = m_canEscape;
          m_command->canLose = m_canLose;
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

void Dialog_BattleProcessing::drawPickers() {
  if (m_enemyPicker) {
    if (const auto [closed, confirmed] = m_enemyPicker->draw(); closed) {
      if (confirmed) {
        m_id = m_enemyPicker->selection();
      }
      m_enemyPicker.reset();
    }
  }

  if (m_variablePicker) {
    if (const auto [closed, confirmed] = m_variablePicker->draw(); closed) {
      if (confirmed) {
        m_varSelection = m_variablePicker->selection();
      }
      m_variablePicker.reset();
    }
  }
}