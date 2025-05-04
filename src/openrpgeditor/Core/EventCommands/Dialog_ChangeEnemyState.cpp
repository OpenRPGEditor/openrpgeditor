#include "Core/EventCommands/Dialog_ChangeEnemyState.hpp"

#include "Database/Database.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <tuple>

std::tuple<bool, bool> Dialog_ChangeEnemyState::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ChangeEnemyState");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize =
      ImVec2{ImGui::CalcTextSize("#############################").x + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 9) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###ChangeEnemyState", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    drawPickers();
    ImGui::BeginVertical("##change_enemy_state_main_layout", ImGui::GetContentRegionAvail());
    {
      GroupBox enemyGroupBox(trNOOP("Enemy"), "##change_enemy_tp_enemy_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
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

      GroupBox operationGroupBox(trNOOP("Operation"), "##change_enemy_state_operation_group", {-1, 0}, nullptr,
                                 ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (operationGroupBox.begin()) {
        ImGui::BeginHorizontal("##change_enemy_state_operation_group_layout", {ImGui::GetContentRegionAvail().x, -1});
        {
          ImGui::Spring(0.33);
          ImGui::RadioButton(trNOOP("Add"), &m_troopMemberOp, 0);
          ImGui::Spring(0.33);
          ImGui::RadioButton(trNOOP("Remove"), &m_troopMemberOp, 1);
          ImGui::Spring(0.33);
        }
        ImGui::EndHorizontal();
      }
      operationGroupBox.end();

      GroupBox stateGroupBox(trNOOP("State"), "##change_enemy_state_state_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (stateGroupBox.begin()) {
        ImGui::SetNextItemWidth(-1);
        std::string text = Database::instance()->stateNameAndId(m_state);
        ImGui::PushID("##change_enemy_state_state_value");
        if (ImGui::EllipsesButton(text.c_str(), ImVec2{-1, 0})) {
          m_statePicker = ObjectPicker(trNOOP("State"), Database::instance()->states.states(), m_state);
          m_statePicker->setOpen(true);
        }
        ImGui::PopID();
      }
      stateGroupBox.end();

      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##change_enemy_tp_buttons_layout", {-1, -1});
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##change_enemy_tp_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;

          m_command->troopMember = m_troopMemberSelection;
          m_command->troopMemberOp = static_cast<PartyMemberOperation>(m_troopMemberOp);
          m_command->state = m_state;

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

void Dialog_ChangeEnemyState::drawPickers() {
  if (m_statePicker) {
    if (const auto [closed, confirmed] = m_statePicker->draw(); closed) {
      if (confirmed) {
        m_state = m_statePicker->selection();
      }
      m_statePicker.reset();
    }
  }
}