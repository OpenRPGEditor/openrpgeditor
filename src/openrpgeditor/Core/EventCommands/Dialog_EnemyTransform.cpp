#include "Core/EventCommands/Dialog_EnemyTransform.hpp"

#include "Database/Database.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <tuple>

void Dialog_EnemyTransform::drawPickers() {
  if (m_enemyPicker) {
    if (const auto [closed, confirmed] = m_enemyPicker->draw(); closed) {
      if (confirmed) {
        m_enemy = m_enemyPicker->selection();
        m_enemyPicker.reset();
      }
    }
  }
}
std::tuple<bool, bool> Dialog_EnemyTransform::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###EnemyAppear");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 32) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 4) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});
  if (ImGui::BeginPopupModal(std::format("{}###EnemyAppear", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    drawPickers();
    ImGui::BeginVertical("##enemy_transform_main_layout");
    {
      GroupBox memberGroupBox(trNOOP("Enemy"), "##enemy_transform_member_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (memberGroupBox.begin()) {
        ImGui::SetNextItemWidth(-1);

        auto preview = Database::instance()->troopMemberName(m_troopId, m_troopMemberSelection);
        if (m_troopMemberSelection >= 0) {
          preview = std::format("#{} {}", m_troopMemberSelection + 1, preview);
        }

        if (ImGui::BeginCombo("##enemy_transform_change_list", preview.c_str())) {
          for (int i = 0; i < 8; ++i) {
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
      memberGroupBox.end();

      GroupBox enemyGroupBox(trNOOP("Transform to"), "##enemy_transform_enemy_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (enemyGroupBox.begin()) {
        ImGui::PushID("##enemy_transform_selection");
        if (ImGui::Button(Database::instance()->enemyNameAndId(m_enemy).c_str(), ImVec2{-1, 0})) {
          m_enemyPicker = EnemyPicker(Database::instance()->enemies.enemies(), m_enemy);
          m_enemyPicker->setOpen(true);
        }
        ImGui::PopID();
      }
      enemyGroupBox.end();

      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##enemy_transform_buttons_layout", {-1, -1});
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##enemy_transform_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->troopMember = m_troopMemberSelection;
          m_command->enemy = m_enemy;
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
