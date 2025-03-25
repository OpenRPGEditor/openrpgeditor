#include "Core/DatabaseEditor/DBEnemiesTab.hpp"
#include "Core/Application.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Enemies.hpp"
#include "imgui.h"
#include "imgui_internal.h"

DBEnemiesTab::DBEnemiesTab(Enemies& Enemies, DatabaseEditor* parent) : IDBEditorTab(parent), m_enemies(Enemies) {
  m_selectedEnemy = m_enemies.enemy(1);
  if (m_selectedEnemy) {
    m_traitsEditor.setTraits(&m_selectedEnemy->traits());
    m_actionsEditor.setActions(&m_selectedEnemy->actions());
  }
}

void DBEnemiesTab::draw() {
  if (!m_characterButtonTexture) {
    m_characterButtonTexture.emplace();
    m_characterButtonTexture->setSize(340, 200);
    if (!m_characterSheet) {
      m_characterSheet.emplace(m_selectedEnemy->battlerName());
    }
    const auto& [uv0, uv1] = m_characterSheet->getRectForCharacter(m_selectedEnemy->battlerHue());
    const Point offset{static_cast<int>(uv0.x() * m_characterSheet.value().texture().width()), static_cast<int>(uv0.y() * m_characterSheet.value().texture().height())};
    m_characterButtonTexture->setTexturesToComposite({{m_characterSheet.value().texture(), {m_characterSheet.value().characterWidth(), m_characterSheet.value().characterHeight()}, offset}});
  }

  if (m_isDropConfirmed) {
    m_selectedEnemy->dropItems().at(m_dropIndex).setDataId(m_item.at(m_dropIndex));
    m_selectedEnemy->dropItems().at(m_dropIndex).setKind(m_dropSelection);
    m_selectedEnemy->dropItems().at(m_dropIndex).setDenominator(m_dropDenominator);
    m_isOpen = false;
    m_isDropConfirmed = false;
  }
  ImGui::BeginChild("#orpg_enemies_editor");
  {
    const auto calc = ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQRSTUV");
    ImGui::BeginChild("##orpg_enemies_editor_enemies", ImVec2{calc.x + (ImGui::GetStyle().ItemSpacing.x * 2), 0}, 0, ImGuiWindowFlags_NoScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Enemies");
        ImGui::BeginChild("##orpg_enemies_editor_enemies_list", ImVec2{0, ImGui::GetContentRegionMax().y - ((calc.y + ImGui::GetStyle().ItemSpacing.y) * 4)});
        {
          ImGui::BeginGroup();
          {
            for (auto& enemy : m_enemies.enemies()) {
              if (enemy.id() == 0) {
                continue;
              }
              if (ImGui::Selectable(Database::instance()->enemyNameAndId(enemy.id()).c_str(), &enemy == m_selectedEnemy) || (ImGui::IsItemFocused() && m_selectedEnemy != &enemy)) {
                ImGui::ClearActiveID();
                m_selectedEnemy = &enemy;
                m_traitsEditor.setTraits(&m_selectedEnemy->traits());
                m_actionsEditor.setActions(&m_selectedEnemy->actions());
              }
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        char str[4096];
        snprintf(str, 4096, "Max Enemies %i", m_enemies.count());
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - 8, 0})) {
          m_changeIntDialogOpen = true;
          m_editMaxEnemies = m_enemies.count();
        }
      }
      ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_enemies_editor_enemies_enemy_properties");
    {
      if (m_selectedEnemy) {
        ImGui::BeginChild("##orpg_enemies_enemy_panel_left", ImVec2{ImGui::GetContentRegionMax().x / 2, 0.f});
        {
          ImGui::BeginGroup();
          {

            ImGui::SeparatorText("General Settings");
            // Name
            ImGui::BeginGroup();
            {
              char name[4096];
              strncpy(name, m_selectedEnemy->name().c_str(), 4096);
              if (ImGui::LabelOverLineEdit("##orpg_enemies_editor_name", "Name:", name, 4096, ImGui::GetContentRegionMax().x / 2, nullptr, ImGuiInputTextFlags_None)) {
                m_selectedEnemy->setName(name);
              }
              if (ImGui::IsItemDeactivatedAfterEdit()) {
                int test = 1;
              }
            }
            ImGui::EndGroup();
            // Image
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::TextUnformatted("Image:");
              if (ImGui::ImageButtonEx(ImGui::GetID("##orpg_database_enemies_image"), m_characterButtonTexture->get(), ImVec2(m_characterButtonTexture->size()), {0.f, 0.f}, {1.f, 1.f}, {},
                                       {1.f, 1.f, 1.f, 1.f}, ImGuiButtonFlags_PressedOnDoubleClick)) {
                m_currentSheet = &m_characterSheet.value();
                // m_characterPicker->setCharacterInfo("", m_selectedItem->iconIndex);
                // m_characterPicker->setOpen(true);
              }
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("Max HP:");
              ImGui::SetNextItemWidth(170);
              int param = m_selectedEnemy->param(0);
              if (ImGui::InputInt("##orpg_database_enemy_maxhp", &param, 1, 100)) {
                m_selectedEnemy->setParam(0, std::clamp(param, -999, 999));
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Max MP:");
              ImGui::SetNextItemWidth(170);
              int param = m_selectedEnemy->param(1);
              if (ImGui::InputInt("##orpg_database_enemy_maxmp", &param, 1, 100)) {
                m_selectedEnemy->setParam(1, std::clamp(param, -999, 999));
              }
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("Attack:");
              ImGui::SetNextItemWidth(170);
              int param = m_selectedEnemy->param(2);
              if (ImGui::InputInt("##orpg_database_enemy_attack", &param, 1, 100)) {
                m_selectedEnemy->setParam(2, std::clamp(param, -999, 999));
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Defense:");
              ImGui::SetNextItemWidth(170);
              int param = m_selectedEnemy->param(3);
              if (ImGui::InputInt("##orpg_database_enemy_defense", &param, 1, 100)) {
                m_selectedEnemy->setParam(3, std::clamp(param, -999, 999));
              }
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("M.Attack:");
              ImGui::SetNextItemWidth(170);
              int param = m_selectedEnemy->param(4);
              if (ImGui::InputInt("##orpg_database_enemy_mattack", &param, 1, 100)) {
                m_selectedEnemy->setParam(4, std::clamp(param, -999, 999));
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("M.Defense:");
              ImGui::SetNextItemWidth(170);
              int param = m_selectedEnemy->param(5);
              if (ImGui::InputInt("##orpg_database_enemy_mdefense", &param, 1, 100)) {
                m_selectedEnemy->setParam(5, std::clamp(param, -999, 999));
              }
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("Agility:");
              ImGui::SetNextItemWidth(170);
              int param = m_selectedEnemy->param(6);
              if (ImGui::InputInt("##orpg_database_enemy_agility", &param, 1, 100)) {
                m_selectedEnemy->setParam(6, std::clamp(param, -9999, 9999));
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Luck:");
              ImGui::SetNextItemWidth(170);
              int param = m_selectedEnemy->param(7);
              if (ImGui::InputInt("##orpg_database_enemy_luck", &param, 1, 100)) {
                m_selectedEnemy->setParam(7, std::clamp(param, -9999, 9999));
              }
            }
            ImGui::EndGroup();
            ImGui::SeparatorText("Rewards");
            ImGui::BeginGroup();
            {
              ImGui::Text("EXP:");
              ImGui::SetNextItemWidth(170);
              int exp = m_selectedEnemy->exp();
              if (ImGui::InputInt("##orpg_database_enemy_exp", &exp, 1, 100)) {
                m_selectedEnemy->setExp(std::clamp(exp, 0, 9999999));
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Gold:");
              ImGui::SetNextItemWidth(170);
              int gold = m_selectedEnemy->gold();
              if (ImGui::InputInt("##orpg_database_enemy_gold", &gold, 1, 100)) {
                m_selectedEnemy->setGold(std::clamp(gold, 0, 9999999));
              }
            }
            ImGui::EndGroup();
            ImGui::Text("Drop Items");
            // Drop Item 1
            ImGui::BeginGroup();
            {
              ImGui::SetNextItemWidth(160);
              ImGui::PushID("##orpg_database_enemy_drops_1");
              if (ImGui::Button(m_selectedEnemy->dropItems().at(0).kind() > 0 ? getDropString(0, m_selectedEnemy->dropItems().at(0).kind()).c_str() : "", ImVec2{ImGui::GetContentRegionMax().x, 0})) {
                m_dropIndex = 0;
                m_isOpen = true;
              }
              ImGui::PopID();
            }
            ImGui::EndGroup();
            // Drop Item 2
            ImGui::BeginGroup();
            {
              ImGui::SetNextItemWidth(160);
              ImGui::PushID("##orpg_database_enemy_drops_2");
              if (ImGui::Button(m_selectedEnemy->dropItems().at(1).kind() > 0 ? getDropString(1, m_selectedEnemy->dropItems().at(1).kind()).c_str() : "", ImVec2{ImGui::GetContentRegionMax().x, 0})) {
                m_dropIndex = 1;
                m_isOpen = true;
              }
              ImGui::PopID();
            }
            ImGui::EndGroup();
            // Drop Item 3
            ImGui::BeginGroup();
            {
              ImGui::SetNextItemWidth(160);
              ImGui::PushID("##orpg_database_enemy_drops_3");
              if (ImGui::Button(m_selectedEnemy->dropItems().at(2).kind() > 0 ? getDropString(2, m_selectedEnemy->dropItems().at(2).kind()).c_str() : "", ImVec2{ImGui::GetContentRegionMax().x, 0})) {
                m_dropIndex = 2;
                m_isOpen = true;
              }
              ImGui::PopID();
            }
            ImGui::EndGroup();
            // Action Patterns
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("##orpg_weapons_weapon_panel_right");
        {
          m_actionsEditor.draw(m_parent);
          m_traitsEditor.draw(m_parent);
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("Note:");
            char note[8192];
            strncpy(note, m_selectedEnemy->note().c_str(), IM_ARRAYSIZE(note));
            if (ImGui::InputTextMultiline("##orpg_database_enemies_note", note, IM_ARRAYSIZE(note), ImVec2{ImGui::GetContentRegionMax().x - 16, ImGui::GetContentRegionAvail().y - 16})) {
              m_selectedEnemy->setNote(note);
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
      }

      ImGui::EndChild();
    }
    ImGui::EndChild();

    if (m_changeIntDialogOpen) {
      if (ImGui::Begin("Change Max Enemies", &m_changeIntDialogOpen,
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
        ImGui::Text(
            "Specify an amount to resize the enemy list to\n"
            "This cannot be undone!");
        ImGui::InputInt("##value_input", &m_editMaxEnemies);
        ImGui::SameLine();
        if (ImGui::Button("Accept")) {
          m_changeConfirmDialogOpen = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
          m_changeIntDialogOpen = false;
        }
      }
      ImGui::End();

      if (m_changeConfirmDialogOpen) {
        if (ImGui::Begin("Confirm Change", &m_changeConfirmDialogOpen,
                         ImGuiWindowFlags_NoResize | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
          ImGui::Text("Are you sure?");
          if (ImGui::Button("Yes")) {
            const int tmpId = m_selectedEnemy->id();
            m_enemies.resize(m_editMaxEnemies);
            m_selectedEnemy = m_enemies.enemy(tmpId);
            m_changeIntDialogOpen = false;
            m_changeConfirmDialogOpen = false;
          }
          ImGui::SameLine();
          if (ImGui::Button("Cancel")) {
            m_changeIntDialogOpen = false;
            m_changeConfirmDialogOpen = false;
          }
        }
        ImGui::End();
      }
    }
    drawPopup();
  }
}
void DBEnemiesTab::drawPopup() {
  if (m_isOpen) {
    ImGui::OpenPopup("Drop Item Selection");
  }

  if (ImGui::BeginPopupModal("Drop Item Selection", nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize)) {
    if (item_picker) {
      if (const auto [closed, confirmed] = item_picker->draw(); closed) {
        if (confirmed) {
          m_item.at(0) = item_picker->selection();
        }
        item_picker.reset();
      }
    }
    if (weapon_picker) {
      if (const auto [closed, confirmed] = weapon_picker->draw(); closed) {
        if (confirmed) {
          m_item.at(1) = weapon_picker->selection();
        }
        weapon_picker.reset();
      }
    }

    if (armor_picker) {
      auto [closed, confirmed] = armor_picker->draw();
      if (closed) {
        if (confirmed) {
          m_item.at(2) = armor_picker->selection();
        }
        armor_picker.reset();
      }
    }
    ImGui::BeginChild("##orpg_dropitem_left_child", ImVec2(0, 0), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize, ImGuiWindowFlags_NoBackground);
    {

      if (ImGui::RadioButton("None", m_dropSelection == 0)) {
        m_dropSelection = 0;
      }
      if (ImGui::RadioButton("Item", m_dropSelection == 1)) {
        m_dropSelection = 1;
      }
      if (ImGui::RadioButton("Weapon", m_dropSelection == 2)) {
        m_dropSelection = 2;
      }
      if (ImGui::RadioButton("Armor", m_dropSelection == 3)) {
        m_dropSelection = 3;
      }
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_dropitem_right_child", ImVec2(0, 0), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize, ImGuiWindowFlags_NoBackground);
    {
      // None
      ImGui::NewLine();
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.f);
      //  Item
      ImGui::BeginDisabled(m_dropSelection != 1);
      {
        ImGui::PushID("##orpg_dropitem_item");
        if (ImGui::Button(m_dropSelection != 1 ? "" : Database::instance()->itemNameOrId(m_item.at(0)).c_str(), ImVec2{200 - 15, 0})) {
          item_picker = ObjectPicker("Item"sv, Database::instance()->items.items(), m_item.at(0));
          item_picker->setOpen(true);
        }
        ImGui::PopID();
      }
      ImGui::EndDisabled();
      // Weapon
      ImGui::BeginDisabled(m_dropSelection != 2);
      {
        ImGui::PushID("##orpg_dropitem_weapon");
        if (ImGui::Button(m_dropSelection != 2 ? "" : Database::instance()->weaponNameOrId(m_item.at(1)).c_str(), ImVec2{200 - 15, 0})) {
          weapon_picker = ObjectPicker("Weapon"sv, Database::instance()->weapons.weaponList(), m_item.at(1));
          weapon_picker->setOpen(true);
        }
        ImGui::PopID();
      }
      ImGui::EndDisabled();
      // Armor
      ImGui::BeginDisabled(m_dropSelection != 3);
      {
        ImGui::PushID("##orpg_dropitem_armor");
        if (ImGui::Button(m_dropSelection != 3 ? "" : Database::instance()->armorNameOrId(m_item.at(2)).c_str(), ImVec2{200 - 15, 0})) {
          armor_picker = ObjectPicker("Armor"sv, Database::instance()->armors.armorList(), m_item.at(2));
          armor_picker->setOpen(true);
        }
        ImGui::PopID();
      }
      ImGui::EndDisabled();
      ImGui::BeginDisabled(m_dropSelection < 1);
      {
        ImGui::SeparatorText("Probability");
        if (ImGui::InputInt("##orpg_dropitem_denom", &m_dropDenominator)) {
          if (m_dropDenominator < 1) {
            m_dropDenominator = 1;
          }
          if (m_dropDenominator > 1000) {
            m_dropDenominator = 1000;
          }
        }
      }
      ImGui::EndDisabled();
    }
    ImGui::EndChild();
    ImGui::Separator();
    ImGui::BeginGroup();
    {
      if (ImGui::Button("OK")) {
        m_isOpen = false;
        m_isDropConfirmed = true;
        ImGui::CloseCurrentPopup();
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        m_isOpen = false;
        ImGui::CloseCurrentPopup();
      }
      ImGui::EndGroup();
    }
    ImGui::EndPopup();
  }
}
std::string DBEnemiesTab::getDropString(const int dropIndex, const int kindId) const {
  switch (kindId) {
  case 1:
    return Database::instance()->itemNameOrId(m_selectedEnemy->dropItems().at(dropIndex).dataId());
  case 2:
    return Database::instance()->weaponNameOrId(m_selectedEnemy->dropItems().at(dropIndex).dataId());
  case 3:
    return Database::instance()->armorNameOrId(m_selectedEnemy->dropItems().at(dropIndex).dataId());
  default:
    break;
  }
  return "WAT";
}