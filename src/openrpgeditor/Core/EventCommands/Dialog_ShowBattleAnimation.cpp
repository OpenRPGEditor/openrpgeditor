#include "Core/EventCommands/Dialog_ShowBattleAnimation.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Database.hpp"
#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>
std::tuple<bool, bool> Dialog_ShowBattleAnimation::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ShowBattleAnimation");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 36) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 8) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});
  if (ImGui::BeginPopupModal(std::format("{}###ShowBattleAnimation", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    drawPickers();
    ImGui::BeginVertical("##show_battle_animation_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      GroupBox enemyGroupBox(trNOOP("Enemy"), "##show_battle_animation_enemy_group_box", {-1, 0});
      if (enemyGroupBox.begin()) {
        ImGui::BeginDisabled(m_targetAllEnemies);
        {
          ImGui::SetNextItemWidth(-1);
          if (ImGui::BeginCombo("##show_battle_animation_enemy_combo", m_targetAllEnemies ? "" : std::format("#{} {}", m_enemy + 1, Database::instance()->troopMemberName(m_troop, m_enemy)).c_str())) {
            for (int i = 0; i < 8; ++i) {
              const auto selected = i == m_enemy;
              if (ImGui::Selectable(std::format("#{} {}", i + 1, Database::instance()->troopMemberName(m_troop, m_enemy)).c_str(), selected)) {
                m_enemy = i;
              }
              if (selected) {
                ImGui::SetItemDefaultFocus();
              }
            }
            ImGui::EndCombo();
          }
        }
        ImGui::EndDisabled();
        ImGui::Checkbox(trNOOP("Target all enemies in troop?"), &m_targetAllEnemies);
      }
      enemyGroupBox.end();
      GroupBox animationGroupBox(trNOOP("Animation"), "##show_battle_animation_animation_group_box", {-1, 0});
      if (animationGroupBox.begin()) {
        if (ImGui::EllipsesButton(Database::instance()->animationNameAndId(m_animation).c_str(), {-1, 0})) {
          m_animationPicker.emplace( Database::instance()->animations->animations(), m_animation);
          m_animationPicker->setOpen(true);
        }
      }
      animationGroupBox.end();

      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##show_battle_animation_button_layout");
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##show_battle_animation_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->targetAllEnemies = m_targetAllEnemies;
          m_command->enemy = m_enemy;
          m_command->animation = m_animation;
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

void Dialog_ShowBattleAnimation::drawPickers() {
  if (m_animationPicker) {
    if (const auto [closed, confirmed] = m_animationPicker->draw(); closed) {
      if (confirmed) {
        m_animation = m_animationPicker->selection();
      }
      m_animationPicker.reset();
    }
  }
}