#include "Core/EventCommands/Dialog_ShowBattleAnimation.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Database/Database.hpp"

std::tuple<bool, bool> Dialog_ShowBattleAnimation::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{183, 159} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open,
                             ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_AlwaysAutoResize)) {

    if (animation_picker) {
      auto [closed, confirmed] = animation_picker->draw();
      if (confirmed) {
        m_animation = animation_picker->selection();
        animation_picker.reset();
      }
    }
    ImGui::SeparatorText("Enemy");
    ImGui::PushItemWidth((App::DPIHandler::scale_value(160)));
    if (ImGui::BeginCombo(
            "##enemyanimation_list",
            ("#" + std::to_string(m_enemy) + " " + Database::instance()->troopMemberName(0, m_enemy)).c_str())) {
      for (int i = 1; i < 9; ++i) {
        if (ImGui::Selectable(("#" + std::to_string(i) + " " + Database::instance()->troopMemberName(0, i)).c_str(),
                              i == m_enemy)) {
          m_enemy = i;
        }
      }
      ImGui::EndCombo();
    }
    ImGui::Checkbox("Target all enemies in troop?", &m_targetAllEnemies);

    // Animation Button
    ImGui::SeparatorText("Animation");
    ImGui::PushID("##showanim_animation_select");
    if (ImGui::Button(Database::instance()->animationName(m_animation).c_str(),
                      ImVec2{200 - (15 * App::DPIHandler::get_ui_scale()), 0})) {
      animation_picker = ObjectPicker<Animation>("Animation"sv, Database::instance()->animations.animations(), 0);
    }
    ImGui::PopID();

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->targetAllEnemies = m_targetAllEnemies;

      if (m_targetAllEnemies)
        command->enemy = 0;
      else
        command->enemy = m_enemy;

      command->animation = m_animation;

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
