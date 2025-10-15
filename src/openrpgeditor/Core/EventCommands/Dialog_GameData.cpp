#include "Core/EventCommands/Dialog_GameData.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Core/Log.hpp"
#include "Database/Database.hpp"
#include <imgui.h>
#include <imgui_internal.h>

std::tuple<bool, bool> Dialog_GameData::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###GameData");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 48) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 11) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###GameData", m_name).c_str(), &m_open, ImGuiWindowFlags_NoResize)) {
    drawPickers();
    ImGui::BeginVertical("##game_data_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      GroupBox gameDataGroup("", "##game_data_group_box", {-1, 0});
      if (gameDataGroup.begin()) {
        ImGui::BeginHorizontal("##game_data_inner_layout");
        {
          ImGui::BeginVertical("##game_data_inner_radios", {-1, -1}, 0);
          {
            ImGui::RadioButton(trNOOP("Item"), &m_type, 0);
            ImGui::RadioButton(trNOOP("Weapon"), &m_type, 1);
            ImGui::RadioButton(trNOOP("Armor"), &m_type, 2);
            ImGui::RadioButton(trNOOP("Actor"), &m_type, 3);
            ImGui::RadioButton(trNOOP("Enemy"), &m_type, 4);
            ImGui::RadioButton(trNOOP("Character"), &m_type, 5);
            ImGui::RadioButton(trNOOP("Party"), &m_type, 6);
            ImGui::RadioButton(trNOOP("Other"), &m_type, 7);
          }
          ImGui::EndVertical();
          ImGui::BeginVertical("##game_data_inner_values", {-1, -1}, 0);
          {
            ImGui::BeginHorizontal("##game_data_inner_values_item");
            {
              ImGui::BeginDisabled(m_type != 0);
              {
                ImGui::PushID("##game_data_item_id");
                if (ImGui::EllipsesButton(m_type != 0 ? "" : Database::instance()->itemNameAndId(m_itemValue).c_str(), ImVec2{ImGui::GetContentRegionAvail().x * .55f, 0})) {
                  m_itemPicker = ItemPicker(Database::instance()->items->items(), m_itemValue);
                  m_itemPicker->setOpen(true);
                }
                ImGui::PopID();
                ImGui::TextUnformatted(trNOOP("(Possession Count)"));
              }
              ImGui::EndDisabled();
            }
            ImGui::EndHorizontal();
            ImGui::BeginHorizontal("##game_data_inner_values_weapon");
            {
              ImGui::BeginDisabled(m_type != 1);
              {
                ImGui::PushID("##game_data_weapon_id");
                if (ImGui::EllipsesButton(m_type != 1 ? "" : Database::instance()->weaponNameAndId(m_weaponValue).c_str(), ImVec2{ImGui::GetContentRegionAvail().x * .55f, 0})) {
                  m_weaponPicker = WeaponPicker(Database::instance()->weapons->weapons(), m_weaponValue);
                  m_weaponPicker->setOpen(true);
                }
                ImGui::PopID();
                ImGui::TextUnformatted(trNOOP("(Possession Count)"));
              }
              ImGui::EndDisabled();
            }
            ImGui::EndHorizontal();
            ImGui::BeginHorizontal("##game_data_inner_values_armor");
            {
              ImGui::BeginDisabled(m_type != 2);
              {
                ImGui::PushID("##game_data_weapon_id");
                if (ImGui::EllipsesButton(m_type != 2 ? "" : Database::instance()->armorNameAndId(m_armorValue).c_str(), ImVec2{ImGui::GetContentRegionAvail().x * .55f, 0})) {
                  m_armorPicker = ArmorPicker(Database::instance()->armors->armors(), m_armorValue);
                  m_armorPicker->setOpen(true);
                }
                ImGui::PopID();
                ImGui::TextUnformatted(trNOOP("(Possession Count)"));
              }
              ImGui::EndDisabled();
            }
            ImGui::EndHorizontal();
            ImGui::BeginHorizontal("##game_data_inner_values_actor");
            {
              ImGui::BeginDisabled(m_type != 3);
              {
                ImGui::PushID("##game_data_actor_id");
                if (ImGui::EllipsesButton(m_type != 3 ? "" : Database::instance()->actorNameAndId(m_actorValue).c_str(), ImVec2{ImGui::GetContentRegionAvail().x * .55f, 0})) {
                  m_actorPicker = ActorPicker(Database::instance()->actors->actorList(), m_actorValue);
                  m_actorPicker->setOpen(true);
                }
                ImGui::PopID();
                ImGui::SetNextItemWidth(-1);
                if (ImGui::BeginCombo("##game_data_actor_combo", m_type != 3 ? "" : DecodeEnumName(static_cast<ActorDataSource>(m_actorSource)).c_str())) {
                  for (auto dataSource : ActorData) {
                    const bool isSelected = m_actorSource == static_cast<int>(dataSource);
                    if (ImGui::Selectable(DecodeEnumName(dataSource).c_str(), isSelected)) {
                      m_actorSource = static_cast<int>(dataSource);
                    }
                    if (isSelected) {
                      ImGui::SetItemDefaultFocus();
                    }
                  }
                  ImGui::EndCombo();
                }
              }
              ImGui::EndDisabled();
            }
            ImGui::EndHorizontal();
            ImGui::BeginHorizontal("##game_data_inner_values_enemy");
            {
              ImGui::BeginDisabled(m_type != 4);
              {
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * .55f);
                const auto preview = m_type != 4 ? "" : std::format("#{} {}", m_enemyValue + 1, Database::instance()->troopMemberName(m_troopId, m_enemyValue));
                if (ImGui::BeginCombo("##game_data_enemy_list", preview.c_str())) {
                  for (int i = 0; i < 8; ++i) {
                    const auto name = std::format("#{} {}", i + 1, Database::instance()->troopMemberName(m_troopId, i));
                    if (ImGui::Selectable(name.c_str(), i == m_enemyValue)) {
                      m_enemyValue = i;
                    }
                  }
                  ImGui::EndCombo();
                }
                ImGui::SetNextItemWidth(-1);
                if (ImGui::BeginCombo("##game_data_enemy_source_combo", m_type != 4 ? "" : DecodeEnumName(static_cast<EnemyDataSource>(m_enemySource)).c_str())) {
                  for (auto dataSource : EnemyData) {
                    const bool isSelected = m_enemySource == static_cast<int>(dataSource);
                    if (ImGui::Selectable(DecodeEnumName(dataSource).c_str(), isSelected)) {
                      m_enemySource = static_cast<int>(dataSource);
                    }
                    if (isSelected) {
                      ImGui::SetItemDefaultFocus();
                    }
                  }
                  ImGui::EndCombo();
                }
              }
              ImGui::EndDisabled();
            }
            ImGui::EndHorizontal();

            ImGui::BeginHorizontal("##game_data_inner_values_character");
            {
              ImGui::BeginDisabled(m_type != 5);
              {
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * .55f);
                const auto preview = m_type != 5 ? "" : Database::instance()->eventNameOrId(m_characterValue);
                if (ImGui::BeginCombo("##game_data_character_list", preview.c_str())) {
                  for (int i = -1; i < static_cast<int>(Database::instance()->mapInfos->currentMap()->map()->events().size()); ++i) {
                    if (i > 0 && !Database::instance()->mapInfos->currentMap()->map()->event(i)) {
                      continue;
                    }

                    if (ImGui::Selectable(Database::instance()->eventNameOrId(i).c_str(), i == m_characterValue)) {
                      m_characterValue = i;
                    }
                  }
                  ImGui::EndCombo();
                }
                ImGui::SetNextItemWidth(-1);
                if (ImGui::BeginCombo("##game_data_enemy_source_combo", m_type != 5 ? "" : DecodeEnumName(static_cast<CharacterDataSource>(m_characterSource)).c_str())) {
                  for (auto dataSource : CharacterData) {
                    const bool isSelected = m_characterSource == static_cast<int>(dataSource);
                    if (ImGui::Selectable(DecodeEnumName(dataSource).c_str(), isSelected)) {
                      m_characterSource = static_cast<int>(dataSource);
                    }
                    if (isSelected) {
                      ImGui::SetItemDefaultFocus();
                    }
                  }
                  ImGui::EndCombo();
                }
              }
              ImGui::EndDisabled();
            }
            ImGui::EndHorizontal();
            ImGui::BeginHorizontal("##game_data_inner_values_party");
            {
              ImGui::BeginDisabled(m_type != 6);
              {
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * .55f);
                if (ImGui::BeginCombo("##game_data_party", m_type != 6 ? "" : std::format("Member #{}", m_partyValue + 1).c_str())) {
                  for (int n = 0; n < 8; n++) {
                    const bool isSelected = m_partyValue == n;
                    if (ImGui::Selectable(std::format("Member #{}", n + 1).c_str(), isSelected)) {
                      m_partyValue = n;
                    }
                    if (isSelected)
                      ImGui::SetItemDefaultFocus();
                  }
                  ImGui::EndCombo();
                }
                ImGui::TextUnformatted(trNOOP("(Actor ID)"));
              }
              ImGui::EndDisabled();
            }
            ImGui::EndHorizontal();
            ImGui::BeginHorizontal("##game_data_inner_values_other");
            {
              ImGui::BeginDisabled(m_type != 7);
              {
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * .55f);
                if (ImGui::BeginCombo("##game_data_other", m_type != 7 ? "" : DecodeEnumName(static_cast<OtherDataSource>(m_otherValue)).c_str())) {
                  for (auto dataSource : OtherSource) {
                    const bool isSelected = m_otherValue == static_cast<int>(dataSource);
                    if (ImGui::Selectable(DecodeEnumName(dataSource).c_str(), isSelected)) {
                      m_otherValue = static_cast<int>(dataSource);
                    }
                    if (isSelected) {
                      ImGui::SetItemDefaultFocus();
                    }
                  }
                  ImGui::EndCombo();
                }
              }
              ImGui::EndDisabled();
            }
            ImGui::EndHorizontal();
          }
          ImGui::EndVertical();
        }
        ImGui::EndHorizontal();
      }
      gameDataGroup.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##game_data_button_layout", {-1, -1}, 0);
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##game_data_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command.gameData.type = static_cast<GameDataType>(m_type);
          switch (static_cast<GameDataType>(m_type)) {
          case GameDataType::Item:
            m_command.gameData.value = m_itemValue;
            break;
          case GameDataType::Weapon:
            m_command.gameData.value = m_weaponValue;
            break;
          case GameDataType::Armor:
            m_command.gameData.value = m_armorValue;
            break;
          case GameDataType::Actor:
            m_command.gameData.rawSource = m_actorSource;
            m_command.gameData.value = m_actorValue;
            break;
          case GameDataType::Enemy:
            m_command.gameData.rawSource = m_enemySource;
            m_command.gameData.value = m_enemyValue;
            break;
          case GameDataType::Character:
            m_command.gameData.rawSource = m_characterSource;
            m_command.gameData.value = m_characterValue;
            break;
          case GameDataType::Party:
            m_command.gameData.value = m_partyValue;
            break;
          case GameDataType::Other:
            m_command.gameData.otherSource = static_cast<OtherDataSource>(m_otherValue);
            break;
          default:
            m_command.gameData.rawSource = 0;
            m_command.gameData.value = 0;
            break;
          }
          ImGui::CloseCurrentPopup();
          setOpen(false);
        } else if (ret == 1) {
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

void Dialog_GameData::drawPickers() {
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
        m_armorValue = m_armorPicker->selection();
      }
      m_armorPicker.reset();
    }
  }
  if (m_weaponPicker) {
    if (const auto [closed, confirmed] = m_weaponPicker->draw(); closed) {
      if (confirmed) {
        m_weaponValue = m_weaponPicker->selection();
      }
      m_weaponPicker.reset();
    }
  }
  if (m_itemPicker) {
    if (const auto [closed, confirmed] = m_itemPicker->draw(); closed) {
      if (confirmed) {
        m_itemValue = m_itemPicker->selection();
      }
      m_itemPicker.reset();
    }
  }
}