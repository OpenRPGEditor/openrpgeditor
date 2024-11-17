#include "Core/EventCommands/Dialog_EnemyTransform.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Database/Database.hpp"

std::tuple<bool, bool> Dialog_EnemyTransform::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{181, 150} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_AlwaysAutoResize)) {

    if (enemy_picker) {
      auto [closed, confirmed] = enemy_picker->draw();
      if (confirmed) {
        m_enemy = enemy_picker->selection();
        enemy_picker.reset();
      }
    }

    ImGui::SeparatorText("Enemy");
    ImGui::SetNextItemWidth(160 * App::DPIHandler::get_ui_scale());

    if (ImGui::BeginCombo("##enemystate_change_list", m_troop_selection == 0 ? "Entire Troop" : ("#" + std::to_string(m_troop_selection) + " ?").c_str())) {
      if (ImGui::Selectable("Entire Troop", m_troop_selection == 0)) {
        m_troop_selection = 0;
      }
      for (int i = 1; i < 9; ++i) {
        std::string text = "#" + std::to_string(i) + " ?";
        if (ImGui::Selectable(text.c_str(), i == m_troop_selection)) {
          m_troop_selection = i;
        }
      }
      ImGui::EndCombo();
    }
    ImGui::SeparatorText("Transform to");
    std::string text = Database::instance().enemyName(m_enemy);
    ImGui::PushID("##enemytransform_selection");
    if (ImGui::Button(
            text.c_str(),
            ImVec2{App::DPIHandler::scale_value(160), 0})) {
      enemy_picker = ObjectPicker<Enemy>("Enemy"sv, Database::instance().enemies.enemies(), 0);
            }
    ImGui::PopID();

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->enemy = m_troop_selection - 1;
      command->transform = m_enemy;
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
