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
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.f));
  ImGui::SetNextWindowSize(ImVec2{610, 380} * App::DPIHandler::get_ui_scale(),ImGuiCond_Appearing);
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
    ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x + 50) - (16 * App::DPIHandler::get_ui_scale()));
    ImGui::BeginDisabled(d_source != 0);
    if (ImGui::Button(text.c_str(),
                      ImVec2{((ImGui::GetWindowContentRegionMax().x / 2)) - (15 * App::DPIHandler::get_ui_scale()), 0})) {
      i_picker = ObjectPicker<Item>("Items"sv, m_project->items().itemList(), 0);
    }
    ImGui::PopID();
    ImGui::EndDisabled();
    ImGui::SameLine();
    ImGui::Text("(Possession Count)");

    // Weapon Selection
    ImGui::RadioButton("Weapon", &d_source, 1);
    ImGui::SameLine();
    text = d_source != 1 ? "##gamedata_weapon_empty"
                         : std::format("{:04} ", d_raw_source) + m_project->weapon(d_raw_source)->name;
    ImGui::PushID("##gamedata_weapon_id");
    ImGui::BeginDisabled(d_source != 1);
    if (ImGui::Button(text.c_str(),
                      ImVec2{((ImGui::GetWindowContentRegionMax().x / 2)) - (15 * App::DPIHandler::get_ui_scale()), 0})) {
      w_picker = ObjectPicker<Weapon>("Weapons"sv, m_project->weapons().weaponList(), 0);
    }
    ImGui::PopID();
    ImGui::EndDisabled();
    ImGui::SameLine();
    ImGui::Text("(Possession Count)");

    // Armor Selection
    ImGui::RadioButton("Armor", &d_source, 2);
    ImGui::SameLine();
    text = d_source != 2 ? "##gamedata_armor_empty"
                         : std::format("{:04} ", d_raw_source) + m_project->armor(d_raw_source)->name;
    ImGui::PushID("##gamedata_armor_id");
    ImGui::BeginDisabled(d_source != 2);
    if (ImGui::Button(text.c_str(),
                      ImVec2{((ImGui::GetWindowContentRegionMax().x / 2)) - (15 * App::DPIHandler::get_ui_scale()), 0})) {
      ar_picker = ObjectPicker<Armor>("Armors"sv, m_project->armors().armorList(), 0);
    }
    ImGui::PopID();
    ImGui::EndDisabled();
    ImGui::SameLine();
    ImGui::Text("(Possession Count)");

    // Actor Selection
    ImGui::RadioButton("Actor", &d_source, 3);
    ImGui::SameLine();
    text = d_source != 3 ? "##gamedata_actor_empty"
                         : std::format("{:04} ", d_raw_source) + m_project->actor(d_raw_source)->name;
    ImGui::PushID("##gamedata_actor_id");
    ImGui::BeginDisabled(d_source != 3);
    if (ImGui::Button(text.c_str(),
                      ImVec2{((ImGui::GetWindowContentRegionMax().x / 4 + 50)) - (15 * App::DPIHandler::get_ui_scale()), 0})) {
      a_picker = ObjectPicker<Actor>("Actors"sv, m_project->actors().actorList(), 0);
    }
    ImGui::EndDisabled();
    ImGui::PopID();
    ImGui::SameLine();
    ImGui::BeginDisabled(d_source != 3);
    ImGui::PushItemWidth(ImGui::GetWindowContentRegionMax().x / 4 + 50);
    if (ImGui::BeginCombo("##gamedata_actor_combo", d_source != 3 ? "" : magic_enum::enum_name(static_cast<ActorDataSource>(current_actorDataSource)).data())) {
      for (auto dataSource : ActorData) {
        bool is_selected = (current_actorDataSource == static_cast<int>(dataSource));

        if (ImGui::Selectable(magic_enum::enum_name(dataSource).data(), is_selected)) {
          current_actorDataSource = static_cast<int>(dataSource);
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
      }
    ImGui::EndCombo();
    }
    ImGui::EndDisabled();

    // Enemy Selection
    ImGui::RadioButton("Enemy", &d_source, 4);
    ImGui::SameLine();
    ImGui::BeginDisabled(d_source != 4);

    ImGui::PushItemWidth(ImGui::GetWindowContentRegionMax().x / 4 + 50);
    if (ImGui::BeginCombo("##gamedata_enemy", d_source != 4 ? "" : ("#"
      + std::to_string(m_project->troops().troopList().at(current_enemySource).id + 1)
      + m_project->troops().troopList().at(current_enemySource).name).c_str())) {
      for (auto dataSource : m_project->troops().troopList()) {
        bool is_selected = (current_enemySource == dataSource.id);
        if (ImGui::Selectable(("#" + std::to_string(dataSource.id + 1) + dataSource.name).c_str(), is_selected)) {
          current_enemySource = dataSource.id;
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
      }
    ImGui::EndCombo();
    }
    ImGui::EndDisabled();

    ImGui::SameLine();
    ImGui::BeginDisabled(d_source != 4);
    ImGui::PushItemWidth(ImGui::GetWindowContentRegionMax().x / 4 + 50);
    if (ImGui::BeginCombo("##gamedata_enemy_combo", d_source != 4 ? "" : magic_enum::enum_name(static_cast<EnemyDataSource>(current_enemyDataSource)).data())) {
      for (auto dataSource : EnemyData) {
        bool is_selected = (current_enemyDataSource == static_cast<int>(dataSource));
        if (ImGui::Selectable(magic_enum::enum_name(dataSource).data(), is_selected)) {
          current_enemyDataSource = static_cast<int>(dataSource);
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
      }
    ImGui::EndCombo();
    }
    ImGui::EndDisabled();

    // Character Selection
    ImGui::RadioButton("Character", &d_source, 5);
    ImGui::SameLine();

    ImGui::BeginDisabled(d_source != 5);
    ImGui::PushItemWidth(ImGui::GetWindowContentRegionMax().x / 4 + 50);
    if (ImGui::BeginCombo("##gamedata_character", d_source != 5 ? "" : ("EV" + std::format("{:03} ", m_project->events().at(current_characterSource)->id)).c_str())) {

      if (ImGui::Selectable("Player", current_characterSource == 0)) {
        current_characterSource = 0;
        if (current_characterSource == 0)
          ImGui::SetItemDefaultFocus();
      }
      if (ImGui::Selectable("This Event", current_characterSource == 1)) {
        current_characterSource = 1;
        if (current_characterSource == 1)
          ImGui::SetItemDefaultFocus();
      }

      for (auto& dataSource : m_project->events()) {
        if (!dataSource.has_value())
          continue;

        bool is_selected = (current_characterSource == dataSource->id + 1);
        if (ImGui::Selectable(("EV" + std::format("{:03} ", dataSource->id)).c_str(), is_selected)) {
          current_characterSource = dataSource->id + 1;
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
      }
    ImGui::EndCombo();
    }
    ImGui::EndDisabled();

    ImGui::SameLine();

    ImGui::BeginDisabled(d_source != 5);
    ImGui::PushItemWidth(ImGui::GetWindowContentRegionMax().x / 4 + 50);
    if (ImGui::BeginCombo("##gamedata_character_combo", d_source != 5 ? "" : magic_enum::enum_name(static_cast<CharacterDataSource>(current_characterDataSource)).data())) {
      for (auto dataSource : CharacterData) {
        bool is_selected = current_characterDataSource == static_cast<int>(dataSource);
        if (ImGui::Selectable(magic_enum::enum_name(dataSource).data(), is_selected)) {
          current_characterSource = static_cast<int>(dataSource);
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
      }
    ImGui::EndCombo();
    }
    ImGui::EndDisabled();

    // Party Selection
    ImGui::RadioButton("Party", &d_source, 6);
    ImGui::SameLine();

    ImGui::BeginDisabled(d_source != 6);
    ImGui::PushItemWidth(ImGui::GetWindowContentRegionMax().x / 2);
    if (ImGui::BeginCombo("##gamedata_party", d_source != 6 ? "" : ("Member #" + std::to_string(current_partySource + 1)).c_str())) {
      for (int n = 1; n < 10; n++) {
        bool is_selected = (current_partySource == n);
        if (ImGui::Selectable(("Member #" + std::to_string(n + 1)).c_str(), is_selected)) {
          current_partySource = n;
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
      }
    ImGui::EndCombo();
    }
    ImGui::EndDisabled();
    ImGui::SameLine();
    ImGui::Text("(Actor ID)");

    // Other Selection
    ImGui::RadioButton("Other", &d_source, 7);
    ImGui::SameLine();

    ImGui::BeginDisabled(d_source != 7);
    ImGui::PushItemWidth(ImGui::GetWindowContentRegionMax().x / 2);
    if (ImGui::BeginCombo("##gamedata_other", d_source != 7 ? "" : magic_enum::enum_name(static_cast<OtherDataSource>(current_otherSource)).data())) {
      for (auto dataSource : OtherSource) {
        bool is_selected = (current_otherSource == static_cast<int>(dataSource));
        if (ImGui::Selectable(magic_enum::enum_name(dataSource).data(), is_selected)) {
          current_otherSource = static_cast<int>(dataSource);
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
      }
    ImGui::EndCombo();
    }
    ImGui::EndDisabled();

    ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionMax().x - App::DPIHandler::scale_value(100) - ImGui::GetStyle().FramePadding.x, ImGui::GetCursorPosY()));
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
