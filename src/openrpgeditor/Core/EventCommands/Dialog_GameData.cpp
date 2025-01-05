#include "Core/EventCommands/Dialog_GameData.hpp"

#include "imgui.h"

using namespace std::string_view_literals;
std::tuple<bool, bool> Dialog_GameData::draw() {

  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
    // SetOpen(false);
  }
  const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.f));
  ImGui::SetNextWindowSize(ImVec2{610, 380}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {

    if (m_actorPicker) {
      if (const auto [closed, confirmed] = m_actorPicker->draw(); closed) {
        if (confirmed) {
          m_actorSource = m_actorPicker->selection();
        }
        m_actorPicker.reset();
      }
    }
    if (m_armorPicker) {
      if (const auto [closed, confirmed] = m_armorPicker->draw(); closed) {
        if (confirmed) {
          m_armorSource = m_armorPicker->selection();
        }
        m_armorPicker.reset();
      }
    }
    if (m_weaponPicker) {
      if (const auto [closed, confirmed] = m_weaponPicker->draw(); closed) {
        if (confirmed) {
          m_weaponSource = m_weaponPicker->selection();
        }
        m_weaponPicker.reset();
      }
    }
    if (m_itemPicker) {
      if (const auto [closed, confirmed] = m_itemPicker->draw(); closed) {
        if (confirmed) {
          m_itemSource = m_itemPicker->selection();
        }
        m_itemPicker.reset();
      }
    }

    ImGui::SeparatorText("Game Data");
    ImGui::BeginGroup();
    {
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
    ImGui::BeginGroup();
    {
      // Item
      std::string text = m_type != 0 ? "##gamedata_item_empty" : Database::instance()->itemNameAndId(m_itemSource);
      ImGui::PushID("##gamedata_item_id");
      ImGui::SetNextItemWidth(ImGui::GetContentRegionMax().x + 50 - 16);
      ImGui::BeginDisabled(m_type != 0);
      if (ImGui::Button(text.c_str(), ImVec2{ImGui::GetWindowContentRegionMax().x / 2 - 15, 0})) {
        m_itemPicker = ObjectPicker("Items"sv, Database::instance()->items.items(), m_itemSource);
        m_itemPicker->setOpen(true);
      }
      ImGui::PopID();
      ImGui::EndDisabled();
      ImGui::SameLine();
      ImGui::Text("(Possession Count)");

      // Weapon Selection
      text = m_type != 1 ? "##gamedata_weapon_empty" : std::format("{:04} ", m_weaponSource) + Database::instance()->weaponName(m_weaponSource);
      ImGui::PushID("##gamedata_weapon_id");
      ImGui::BeginDisabled(m_type != 1);
      if (ImGui::Button(text.c_str(), ImVec2{ImGui::GetWindowContentRegionMax().x / 2 - 15, 0})) {
        m_weaponPicker = ObjectPicker("Weapons"sv, Database::instance()->weapons.weaponList(), m_weaponSource);
        m_weaponPicker->setOpen(true);
      }
      ImGui::PopID();
      ImGui::EndDisabled();
      ImGui::SameLine();
      ImGui::Text("(Possession Count)");

      // Armor Selection
      text = m_type != 2 ? "##gamedata_armor_empty" : std::format("{:04} ", m_armorSource) + Database::instance()->armorName(m_armorSource);
      ImGui::PushID("##gamedata_armor_id");
      ImGui::BeginDisabled(m_type != 2);
      if (ImGui::Button(text.c_str(), ImVec2{ImGui::GetWindowContentRegionMax().x / 2 - 15, 0})) {
        m_armorPicker = ObjectPicker("Armors"sv, Database::instance()->armors.armorList(), m_armorSource);
        m_armorPicker->setOpen(true);
      }
      ImGui::PopID();
      ImGui::EndDisabled();
      ImGui::SameLine();
      ImGui::Text("(Possession Count)");

      // Actor Selection
      text = m_type != 3 ? "##gamedata_actor_empty" : std::format("{:04} ", m_actorSource) + Database::instance()->actorName(m_actorSource);
      ImGui::PushID("##gamedata_actor_id");
      ImGui::BeginDisabled(m_type != 3);
      if (ImGui::Button(text.c_str(), ImVec2{ImGui::GetWindowContentRegionMax().x / 4 + 64 - 15, 0})) {
        m_actorPicker = ObjectPicker("Actors"sv, Database::instance()->actors.actorList(), m_actorSource);
        m_actorPicker->setOpen(true);
      }
      ImGui::EndDisabled();
      ImGui::PopID();
      ImGui::SameLine();
      ImGui::BeginDisabled(m_type != 3);
      ImGui::PushItemWidth(ImGui::GetWindowContentRegionMax().x / 4 + 50);
      if (ImGui::BeginCombo("##gamedata_actor_combo", m_type != 3 ? "" : magic_enum::enum_name(static_cast<ActorDataSource>(m_currentActorDataSource)).data())) {
        for (auto dataSource : ActorData) {
          if (const bool is_selected = m_currentActorDataSource == static_cast<int>(dataSource); ImGui::Selectable(magic_enum::enum_name(dataSource).data(), is_selected)) {
            m_currentActorDataSource = static_cast<int>(dataSource);
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
      if (ImGui::BeginCombo("##gamedata_enemy_list", (m_type != 4 ? "" : "#" + std::to_string(m_currentEnemySource) + " " + Database::instance()->troopMemberName(0, m_currentEnemySource)).c_str())) {
        for (int i = 1; i < 9; ++i) {
          if (ImGui::Selectable(("#" + std::to_string(i) + " " + Database::instance()->troopMemberName(0, i)).c_str(), i == m_currentEnemySource)) {
            m_currentEnemySource = i;
          }
        }
        ImGui::EndCombo();
      }
      ImGui::EndDisabled();

      ImGui::SameLine();
      ImGui::BeginDisabled(m_type != 4);
      ImGui::PushItemWidth(ImGui::GetWindowContentRegionMax().x / 4 + 50);
      if (ImGui::BeginCombo("##gamedata_enemy_combo", m_type != 4 ? "" : magic_enum::enum_name(static_cast<EnemyDataSource>(m_currentEnemyDataSource)).data())) {
        for (auto dataSource : EnemyData) {
          if (const bool is_selected = m_currentEnemyDataSource == static_cast<int>(dataSource); ImGui::Selectable(magic_enum::enum_name(dataSource).data(), is_selected)) {
            m_currentEnemyDataSource = static_cast<int>(dataSource);
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
      if (ImGui::BeginCombo("##gamedata_character", m_type != 5                     ? ""
                                                    : current_characterSource == -1 ? "Player"
                                                    : current_characterSource == 0
                                                        ? "This Event"
                                                        : ("EV" + std::format("{:03} ", Database::instance()->mapInfos.currentMap()->event(current_characterSource)->id())).c_str())) {

        if (ImGui::Selectable("Player", current_characterSource == -1)) {
          current_characterSource = -1;
          ImGui::SetItemDefaultFocus();
        }
        if (ImGui::Selectable("This Event", current_characterSource == 0)) {
          current_characterSource = 0;
          ImGui::SetItemDefaultFocus();
        }

        for (auto& dataSource : Database::instance()->mapInfos.currentMap()->map()->events()) {
          if (!dataSource.has_value())
            continue;

          if (const bool is_selected = current_characterSource == dataSource->id(); ImGui::Selectable(("EV" + std::format("{:03} ", dataSource->id())).c_str(), is_selected)) {
            current_characterSource = dataSource->id();
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
          if (const bool is_selected = current_characterDataSource == static_cast<int>(dataSource); ImGui::Selectable(magic_enum::enum_name(dataSource).data(), is_selected)) {
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
          if (const bool is_selected = current_partySource == n; ImGui::Selectable(("Member #" + std::to_string(n)).c_str(), is_selected)) {
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
          if (const bool is_selected = current_otherSource == static_cast<int>(dataSource); ImGui::Selectable(magic_enum::enum_name(dataSource).data(), is_selected)) {
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

    ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionMax().x - 100 - ImGui::GetStyle().FramePadding.x, ImGui::GetCursorPosY()));
    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command.gameData.type = static_cast<GameDataType>(m_type);
      switch (m_type) {
      case 0: // Item
        command.gameData.rawSource = m_itemSource;
        break;
      case 1:
        command.gameData.rawSource = m_weaponSource;
        break;
      case 2:
        command.gameData.rawSource = m_armorSource;
        break;
      case 3:
        command.gameData.rawSource = m_actorSource;
        command.gameData.value = m_currentActorDataSource;
        break;
      case 4:
        command.gameData.rawSource = m_currentEnemySource;
        command.gameData.value = m_currentEnemyDataSource;
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
      setOpen(false);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      setOpen(false);
    }
    ImGui::EndPopup();
  }
  return std::make_tuple(!m_open, m_confirmed);
}
