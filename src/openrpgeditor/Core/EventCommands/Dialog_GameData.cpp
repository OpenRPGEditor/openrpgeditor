#include "Dialog_GameData.hpp"
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/Project.hpp"
using namespace std::string_view_literals;
std::tuple<bool, bool> Dialog_GameData::draw() {

  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
    // SetOpen(false);
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.f));
  ImGui::SetNextWindowSize(ImVec2{610, 380} * App::DPIHandler::get_ui_scale(),ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {

    if (a_picker) {
      auto [closed, confirmed] = a_picker->draw();
      if (confirmed) {
        m_actor_source = a_picker->selection();
        a_picker.reset();
      }
    }
    if (ar_picker) {
      auto [closed, confirmed] = ar_picker->draw();
      if (confirmed) {
        m_armor_source = ar_picker->selection();
        ar_picker.reset();
      }
    }
    if (w_picker) {
      auto [closed, confirmed] = w_picker->draw();
      if (confirmed) {
        m_weapon_source = w_picker->selection();
        w_picker.reset();
      }
    }
    if (i_picker) {
      auto [closed, confirmed] = i_picker->draw();
      if (confirmed) {
        m_item_source = i_picker->selection();
        i_picker.reset();
      }
    }

    ImGui::SeparatorText("Game Data");
    ImGui::BeginGroup(); {
      ImGui::RadioButton("Item", &m_type, 0);
      ImGui::RadioButton("Weapon", &m_type, 1);
      ImGui::RadioButton("Armor", &m_type, 2);
      ImGui::RadioButton("Actor", &m_type, 3);
      ImGui::RadioButton("Enemy", &m_type, 4);
      ImGui::RadioButton("Character", &m_type, 5);
      ImGui::RadioButton("Party", &m_type, 6);
      ImGui::RadioButton("Other", &m_type, 7);
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::BeginGroup(); {
      // Item
      std::string text =
          m_type != 0 ? "##gamedata_item_empty" : std::format("{:04} ", m_item_source) + m_project->item(m_item_source)->name;
      ImGui::PushID("##gamedata_item_id");
      ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x + 50) - (16 * App::DPIHandler::get_ui_scale()));
      ImGui::BeginDisabled(m_type != 0);
      if (ImGui::Button(text.c_str(),
                        ImVec2{((ImGui::GetWindowContentRegionMax().x / 2)) - (15 * App::DPIHandler::get_ui_scale()), 0})) {
        i_picker = ObjectPicker<Item>("Items"sv, m_project->items().items(), 0);
      }
      ImGui::PopID();
      ImGui::EndDisabled();
      ImGui::SameLine();
      ImGui::Text("(Possession Count)");


      // Weapon Selection
      text = m_type != 1 ? "##gamedata_weapon_empty"
                         : std::format("{:04} ", m_weapon_source) + m_project->weapon(m_weapon_source)->name;
      ImGui::PushID("##gamedata_weapon_id");
      ImGui::BeginDisabled(m_type != 1);
      if (ImGui::Button(text.c_str(),
                        ImVec2{((ImGui::GetWindowContentRegionMax().x / 2)) - (15 * App::DPIHandler::get_ui_scale()), 0})) {
        w_picker = ObjectPicker<Weapon>("Weapons"sv, m_project->weapons().weaponList(), 0);
      }
      ImGui::PopID();
      ImGui::EndDisabled();
      ImGui::SameLine();
      ImGui::Text("(Possession Count)");

      // Armor Selection
      text = m_type != 2 ? "##gamedata_armor_empty"
                         : std::format("{:04} ", m_armor_source) + m_project->armor(m_armor_source)->name;
      ImGui::PushID("##gamedata_armor_id");
      ImGui::BeginDisabled(m_type != 2);
      if (ImGui::Button(text.c_str(),
                        ImVec2{((ImGui::GetWindowContentRegionMax().x / 2)) - (15 * App::DPIHandler::get_ui_scale()), 0})) {
        ar_picker = ObjectPicker<Armor>("Armors"sv, m_project->armors().armorList(), 0);
      }
      ImGui::PopID();
      ImGui::EndDisabled();
      ImGui::SameLine();
      ImGui::Text("(Possession Count)");

      // Actor Selection
      text = m_type != 3 ? "##gamedata_actor_empty"
                         : std::format("{:04} ", m_actor_source) + m_project->actor(m_actor_source)->name;
      ImGui::PushID("##gamedata_actor_id");
      ImGui::BeginDisabled(m_type != 3);
      if (ImGui::Button(text.c_str(),
                        ImVec2{((ImGui::GetWindowContentRegionMax().x / 4 + 50)) - (15 * App::DPIHandler::get_ui_scale()), 0})) {
        a_picker = ObjectPicker<Actor>("Actors"sv, m_project->actors().actorList(), 0);
      }
      ImGui::EndDisabled();
      ImGui::PopID();
      ImGui::SameLine();
      ImGui::BeginDisabled(m_type != 3);
      ImGui::PushItemWidth(ImGui::GetWindowContentRegionMax().x / 4 + 50);
      if (ImGui::BeginCombo("##gamedata_actor_combo", m_type != 3 ? "" : magic_enum::enum_name(static_cast<ActorDataSource>(current_actorDataSource)).data())) {
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
      ImGui::BeginDisabled(m_type != 4);

      ImGui::PushItemWidth(ImGui::GetWindowContentRegionMax().x / 4 + 50);
      if (ImGui::BeginCombo("##gamedata_enemy", m_type != 4 ? "" : ("#"
                                                                    + std::to_string(m_project->troops().troops().at(current_enemySource).id)
                                                                    + m_project->troops().troops().at(current_enemySource).name).c_str())) {
        for (auto dataSource : m_project->troops().troops()) {
          if (dataSource.m_isValid) {
            bool is_selected = (current_enemySource == dataSource.id);
            if (ImGui::Selectable(("#" + std::to_string(dataSource.id) + dataSource.name).c_str(), is_selected)) {
              current_enemySource = dataSource.id;
              if (is_selected)
                ImGui::SetItemDefaultFocus();
            }
          }
        }
        ImGui::EndCombo();
      }
      ImGui::EndDisabled();

      ImGui::SameLine();
      ImGui::BeginDisabled(m_type != 4);
      ImGui::PushItemWidth(ImGui::GetWindowContentRegionMax().x / 4 + 50);
      if (ImGui::BeginCombo("##gamedata_enemy_combo", m_type != 4 ? "" : magic_enum::enum_name(static_cast<EnemyDataSource>(current_enemyDataSource)).data())) {
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
      ImGui::BeginDisabled(m_type != 5);
      ImGui::PushItemWidth(ImGui::GetWindowContentRegionMax().x / 4 + 50);
      if (ImGui::BeginCombo("##gamedata_character",
                            m_type != 5 ? "" : current_characterSource == 0 ? "Player" : current_characterSource == 1 ? "This Event" : ("EV" + std::format("{:03} ", m_project->events().at(current_characterSource)->id)).c_str())) {

        if (ImGui::Selectable("Player", current_characterSource == 0)) {
          current_characterSource = 0;
            ImGui::SetItemDefaultFocus();
        }
        if (ImGui::Selectable("This Event", current_characterSource == 1)) {
          current_characterSource = 1;
            ImGui::SetItemDefaultFocus();
        }

        for (auto& dataSource : m_project->events()) {
          if (!dataSource.has_value())
            continue;

          bool is_selected = (current_characterSource == dataSource->id);
          if (ImGui::Selectable(("EV" + std::format("{:03} ", dataSource->id)).c_str(), is_selected)) {
            current_characterSource = dataSource->id;
            if (is_selected)
              ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
      ImGui::EndDisabled();

      ImGui::SameLine();

      ImGui::BeginDisabled(m_type != 5);
      ImGui::PushItemWidth(ImGui::GetWindowContentRegionMax().x / 4 + 50);
      if (ImGui::BeginCombo("##gamedata_character_combo", m_type != 5 ? "" : magic_enum::enum_name(static_cast<CharacterDataSource>(current_characterDataSource)).data())) {
        for (auto dataSource : CharacterData) {
          bool is_selected = current_characterDataSource == static_cast<int>(dataSource);
          if (ImGui::Selectable(magic_enum::enum_name(dataSource).data(), is_selected)) {
            current_characterDataSource = static_cast<int>(dataSource);
            if (is_selected)
              ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
      ImGui::EndDisabled();

      // Party Selection
      ImGui::BeginDisabled(m_type != 6);
      ImGui::PushItemWidth(ImGui::GetWindowContentRegionMax().x / 2);
      if (ImGui::BeginCombo("##gamedata_party", m_type != 6 ? "" : ("Member #" + std::to_string(current_partySource)).c_str())) {
        for (int n = 1; n < 9; n++) {
          bool is_selected = (current_partySource == n);
          if (ImGui::Selectable(("Member #" + std::to_string(n)).c_str(), is_selected)) {
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
      ImGui::BeginDisabled(m_type != 7);
      ImGui::PushItemWidth(ImGui::GetWindowContentRegionMax().x / 2);
      if (ImGui::BeginCombo("##gamedata_other", m_type != 7 ? "" : magic_enum::enum_name(static_cast<OtherDataSource>(current_otherSource)).data())) {
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
      ImGui::EndGroup();
    }


    ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionMax().x - App::DPIHandler::scale_value(100) - ImGui::GetStyle().FramePadding.x, ImGui::GetCursorPosY()));
    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command.gameData.type = static_cast<GameDataType>(m_type);
      switch (m_type) {
      case 0: // Item
        command.gameData.rawSource = m_item_source;
        break;
      case 1:
        command.gameData.rawSource = m_weapon_source;
        break;
      case 2:
        command.gameData.rawSource = m_armor_source;
        break;
      case 3:
        command.gameData.rawSource = m_actor_source;
        command.gameData.value = current_actorDataSource;
        break;
      case 4:
        command.gameData.rawSource = current_enemySource;
        command.gameData.value = current_enemyDataSource;
        break;
      case 5:
        command.gameData.rawSource = current_characterSource;
        command.gameData.value = current_characterDataSource;
        break;
      case 6:
        command.gameData.rawSource = current_partySource;
        break;
      case 7:
        command.gameData.otherSource = static_cast<OtherDataSource>(current_otherSource);
        break;
      default:
        command.gameData.rawSource = 0;
        command.gameData.value = 0;
        break;
      }
      ImGui::CloseCurrentPopup();
      SetOpen(false);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      SetOpen(false);
    }
    ImGui::EndPopup();
  }
  return std::make_tuple(!m_open, m_confirmed);
}
