#include "Dialog_GameData.hpp"
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/Project.hpp"
using namespace std::string_view_literals;
void Dialog_GameData::draw() {

  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
    // SetOpen(false);
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{300, 200} * App::DPIHandler::get_scale());
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {

    ImGui::SeparatorText("Game Data");

    // Item Selection
    ImGui::RadioButton("Item", &d_source, 0);
    ImGui::SameLine();
    std::string text =
        d_source != 0
            ? "##gamedata_item_empty"
            : (d_raw_source == 0 ? "" : std::format("{:04} ", d_raw_source) + m_project->item(d_raw_source)->name);
    ImGui::PushID("##gamedata_item_id");
    ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x + 50) - (16 * App::DPIHandler::get_scale()));
    ImGui::BeginDisabled(d_source != 0);
    if (ImGui::Button(text.c_str(),
                      ImVec2{((ImGui::GetWindowContentRegionMax().x / 2)) - (15 * App::DPIHandler::get_scale()), 0})) {
      i_picker = ObjectPicker<Item>("Items"sv, m_project->items().itemList(), 0);
    }
    ImGui::PopID();
    ImGui::EndDisabled();
    ImGui::SameLine();
    ImGui::Text("(Possession Count)");

    // Weapon Selection
    ImGui::RadioButton("Weapon", &d_source, 0);
    ImGui::SameLine();
    text = d_source != 1 ? "##gamedata_weapon_empty"
                         : std::format("{:04} ", d_raw_source) + m_project->weapon(d_raw_source)->name;
    ImGui::PushID("##gamedata_weapon_id");
    ImGui::BeginDisabled(d_source != 1);
    if (ImGui::Button(text.c_str(),
                      ImVec2{((ImGui::GetWindowContentRegionMax().x / 2)) - (15 * App::DPIHandler::get_scale()), 0})) {
      w_picker = ObjectPicker<Weapon>("Weapons"sv, m_project->weapons().weaponList(), 0);
    }
    ImGui::PopID();
    ImGui::EndDisabled();
    ImGui::SameLine();
    ImGui::Text("(Possession Count)");

    // Armor Selection
    ImGui::RadioButton("Armor", &d_source, 0);
    ImGui::SameLine();
    text = d_source != 2 ? "##gamedata_armor_empty"
                         : std::format("{:04} ", d_raw_source) + m_project->armor(d_raw_source)->name;
    ImGui::PushID("##gamedata_armor_id");
    ImGui::BeginDisabled(d_source != 2);
    if (ImGui::Button(text.c_str(),
                      ImVec2{((ImGui::GetWindowContentRegionMax().x / 2)) - (15 * App::DPIHandler::get_scale()), 0})) {
      ar_picker = ObjectPicker<Armor>("Armors"sv, m_project->armors().armorList(), 0);
    }
    ImGui::PopID();
    ImGui::EndDisabled();
    ImGui::SameLine();
    ImGui::Text("(Possession Count)");

    // Actor Selection
    ImGui::RadioButton("Actor", &d_source, 0);
    ImGui::SameLine();
    text = d_source != 3 ? "##gamedata_actor_empty"
                         : std::format("{:04} ", d_raw_source) + m_project->actor(d_raw_source)->name;
    ImGui::PushID("##gamedata_actor_id");
    ImGui::BeginDisabled(d_source != 3);
    if (ImGui::Button(text.c_str(),
                      ImVec2{((ImGui::GetWindowContentRegionMax().x / 2)) - (15 * App::DPIHandler::get_scale()), 0})) {
      a_picker = ObjectPicker<Actor>("Actors"sv, m_project->actors().actorList(), 0);
    }
    ImGui::PopID();
    ImGui::SameLine();
    if (ImGui::BeginCombo("##gamedata_actor_combo", "")) {
      for (auto dataSource : ActorSourceNames) {
        bool is_selected = (current_actorSource == dataSource);
        if (ImGui::Selectable(dataSource.data(), is_selected))
          current_actorSource = dataSource;
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
    ImGui::EndDisabled();

    // Enemy Selection
    ImGui::RadioButton("Enemy", &d_source, 0);
    ImGui::SameLine();
    ImGui::PushID("##gamedata_enemy_id");
    ImGui::BeginDisabled(d_source != 4);
#if 0 // This needs to be re-written
    if (ImGui::BeginCombo("##gamedata_enemy", "")) {
      for (auto dataSource : m_project->troops().troopList()) {
        bool is_selected = (current_enemySource == dataSource.id);
        if (ImGui::Selectable(("#" + std::to_string(dataSource.id) + dataSource.name).c_str(), is_selected))
          current_enemySource = dataSource.id;
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
#endif

    ImGui::PopID();
    ImGui::SameLine();
    if (ImGui::BeginCombo("##gamedata_enemy_combo", "")) {
      for (auto dataSource : EnemySourceNames) {
        bool is_selected = (current_enemySource == dataSource);
        if (ImGui::Selectable(dataSource.data(), is_selected))
          current_enemySource = dataSource;
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
    ImGui::EndDisabled();

    // Character Selection
    ImGui::RadioButton("Character", &d_source, 0);
    ImGui::SameLine();
    ImGui::PushID("##gamedata_character_id");
    ImGui::BeginDisabled(d_source != 5);

    if (ImGui::BeginCombo("##gamedata_character", "")) {
      for (auto& dataSource : m_project->events()) {
        bool is_selected = (current_characterSource == dataSource->id);
        if (ImGui::Selectable(("EV" + std::format("{:03} ", dataSource->id)).c_str(), is_selected))
          current_characterSource = dataSource->id;
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();

    ImGui::PopID();
    ImGui::SameLine();
    /* TODO: Rethink this */
    if (ImGui::BeginCombo("##gamedata_character_combo", "")) {
      for (auto dataSource : CharacterSourceNames) {
        bool is_selected =
            (static_cast<CharacterDataSource>(current_characterSource) ==
             magic_enum::enum_cast<CharacterDataSource>(dataSource.data(), magic_enum::case_insensitive).value());
        if (ImGui::Selectable(dataSource.data(), is_selected))
          current_characterSource = static_cast<int>(
              magic_enum::enum_cast<CharacterDataSource>(dataSource.data(), magic_enum::case_insensitive).value());
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
    ImGui::EndDisabled();

    // Party Selection
    ImGui::RadioButton("Party", &d_source, 0);
    ImGui::SameLine();
    ImGui::PushID("##gamedata_party_id");
    ImGui::BeginDisabled(d_source != 6);

    if (ImGui::BeginCombo("##gamedata_party", "")) {
      for (int n = 0; n < 9; n++) {
        bool is_selected = (current_partySource == n);
        if (ImGui::Selectable(("Member #" + std::to_string(n)).c_str(), is_selected))
          current_partySource = n;
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::PopID();
    ImGui::EndCombo();
    ImGui::EndDisabled();
    ImGui::SameLine();
    ImGui::Text("(Actor ID)");

    // Other Selection
    ImGui::RadioButton("Other", &d_source, 0);
    ImGui::SameLine();
    ImGui::PushID("##gamedata_other_id");
    ImGui::BeginDisabled(d_source != 7);
#if 0 // Rewrite this
    if (ImGui::BeginCombo("##gamedata_other", "")) {
      for (auto dataSource : OtherSourceNames) {
        bool is_selected = (current_enemySource == dataSource);
        if (ImGui::Selectable(dataSource.data(), is_selected))
          current_otherSource = dataSource;
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
#endif
    ImGui::EndDisabled();
    ImGui::PopID();

    ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionMax().x - 75, ImGui::GetCursorPosY()));
    if (ImGui::Button("OK")) {
      ImGui::CloseCurrentPopup();
      SetOpen(false);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      SetOpen(false);
    }
    ImGui::EndPopup();
  }
}
