#include "Editor/DatabaseEditor/DBWeaponsTab.hpp"
#include "Database/Weapons.hpp"
#include "Editor/Application.hpp"

#include "Editor/ImGuiExt/ImGuiUtils.hpp"
#include "imgui.h"
#include "imgui_internal.h"

void DBWeaponsTab::draw() {
  if (!m_itemButtonTexture) {
    m_itemButtonTexture.emplace();
    m_itemButtonTexture->setSize(38, 38);
    if (!m_itemSheet) {
      m_itemSheet.emplace("IconSheet");
    }
    const auto& [uv0, uv1] = m_itemSheet.value().rectForId(m_selectedWeapon->iconIndex());
    const Point offset{static_cast<int>(uv0.x() * m_itemSheet.value().texture().width()), static_cast<int>(uv0.y() * m_itemSheet.value().texture().height())};
    m_itemButtonTexture->setTexturesToComposite({{m_itemSheet.value().texture(), {m_itemSheet.value().iconWidth(), m_itemSheet.value().iconHeight()}, offset}});
  }
  if (m_animationPicker) {
    if (const auto [closed, confirmed] = m_animationPicker->draw(); closed) {
      if (confirmed) {
        m_selectedWeapon->setAnimationId(m_animationPicker->selection());
      }
      m_animationPicker.reset();
    }
  }
  ImGui::BeginChild("#orpg_weapons_editor");
  {
    const auto calc = ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQRSTUV");
    ImGui::BeginChild("##orpg_weapons_editor_weapons", ImVec2{calc.x + (ImGui::GetStyle().ItemSpacing.x * 2), 0}, 0, ImGuiWindowFlags_NoScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Weapons");
        ImGui::BeginChild("##orpg_weapons_editor_weapons_list", ImVec2{0, ImGui::GetContentRegionMax().y - ((calc.y + ImGui::GetStyle().ItemSpacing.y) * 4)});
        {
          ImGui::BeginGroup();
          {
            for (auto& weapon : m_weapons->weapons()) {
              if (weapon.id() == 0) {
                continue;
              }

              if (ImGui::Selectable(Database::instance()->weaponNameAndId(weapon.id()).c_str(), &weapon == m_selectedWeapon) || (ImGui::IsItemFocused() && m_selectedWeapon != &weapon)) {
                m_selectedWeapon = &weapon;
                m_traitsEditor.setTraits(&m_selectedWeapon->traits());
              }
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        char str[4096];
        snprintf(str, 4096, "Max Weapons %i", m_maxWeapons);
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - 8, 0})) {
          m_changeIntDialogOpen = true;
          m_editMaxWeapons = m_maxWeapons;
        }
      }
      ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_weapons_editor_weapons_weapon_properties");
    {
      if (m_selectedWeapon) {
        ImGui::BeginChild("##orpg_weapons_weapon_panel_left", ImVec2{ImGui::GetContentRegionMax().x / 2, 0.f});
        {
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("General Settings");
            // Name
            ImGui::BeginGroup();
            {
              char name[4096];
              strncpy(name, m_selectedWeapon->name().c_str(), 4096);
              if (ImGui::LabelOverLineEdit("##orpg_weapons_editor_name", "Name:", name, 4096, ImGui::GetContentRegionMax().x / 2 - 16)) {
                m_selectedWeapon->setName(name);
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            // Icon
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::TextUnformatted("Icon:");
              if (ImGui::ImageButtonEx(ImGui::GetID("##orpg_database_weapons_image"), m_itemButtonTexture->get(), ImVec2(m_itemButtonTexture->size()), {0.f, 0.f}, {1.f, 1.f}, {}, {1.f, 1.f, 1.f, 1.f},
                                       ImGuiButtonFlags_PressedOnDoubleClick)) {
                m_currentSheet = &m_itemSheet.value();
                // m_characterPicker->setCharacterInfo("", m_selectedItem->iconIndex);
                // m_characterPicker->setOpen(true);
              }
            }
            ImGui::EndGroup();
            // Description
            ImGui::BeginGroup();
            {
              char description[4096];
              strncpy(description, m_selectedWeapon->description().c_str(), 4096);
              ImGui::Text("Description:");
              ImGui::InputTextMultiline("##orpg_database_weapons_description", description, 4096, ImVec2{360, 60});
            }
            ImGui::EndGroup();
            // Weapon Type
            ImGui::BeginGroup();
            {
              ImGui::Text("Weapon Type:");
              ImGui::SetNextItemWidth(170);
              if (ImGui::BeginCombo("##orpg_database_weapons_wtype", Database::instance()->system->weaponType(m_selectedWeapon->wtypeId()).c_str())) {
                int index{0};
                for (auto v : Database::instance()->system->weaponTypes()) {
                  bool selected = index == m_selectedWeapon->wtypeId();
                  if (index == 0) {
                    if (ImGui::Selectable(std::format("None##_{}", index).c_str(), selected)) {
                      m_selectedWeapon->setWtypeId(index);
                    }
                  } else {
                    if (ImGui::Selectable(Database::instance()->system->weaponType(index) == "" ? std::format("#{:02}", index).c_str() : std::format("{}##_#{}", v, index).c_str(), selected)) {
                      m_selectedWeapon->setWtypeId(index);
                    }
                  }
                  index++;
                }
                ImGui::EndCombo();
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            // Price
            ImGui::BeginGroup();
            {
              ImGui::Text("Price:");
              ImGui::SetNextItemWidth(170);
              int price = m_selectedWeapon->price();
              if (ImGui::InputInt("##orpg_database_weapons_price", &price, 1, 100)) {
                m_selectedWeapon->setPrice(std::clamp(price, 0, 999999));
              }
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("Animation:");
              ImGui::SetNextItemWidth(160);
              // Animation Button
              ImGui::PushID("##orpg_database_weapon_animation");
              if (ImGui::Button(m_selectedWeapon->animationId() == -1  ? "Normal Attack"
                                : m_selectedWeapon->animationId() == 0 ? "None"
                                                                       : Database::instance()->animationName(m_selectedWeapon->animationId()).c_str(),
                                ImVec2{200 - 15, 0})) {
                m_animationPicker = AnimationPicker(Database::instance()->animations->animations(), m_selectedWeapon->animationId());
                m_animationPicker->setOpen(true);
              }
              ImGui::PopID();
            }
            ImGui::EndGroup();
          }
          ImGui::EndGroup();
          ImGui::BeginGroup();
          {
            ImGui::EndGroup();
            ImGui::SeparatorText("Parameter Changes");
            ImGui::BeginGroup();
            {
              ImGui::Text("Attack:");
              ImGui::SetNextItemWidth(170);
              int param = m_selectedWeapon->param(0);
              if (ImGui::InputInt("##orpg_database_weapon_attack", &param, 1, 100)) {
                m_selectedWeapon->setParam(0, std::clamp(param, -999, 999));
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Defense:");
              ImGui::SetNextItemWidth(170);
              int param = m_selectedWeapon->param(1);
              if (ImGui::InputInt("##orpg_database_weapon_defense", &param, 1, 100)) {
                m_selectedWeapon->setParam(1, std::clamp(param, -999, 999));
              }
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("M.Attack:");
              ImGui::SetNextItemWidth(170);
              int param = m_selectedWeapon->param(2);
              if (ImGui::InputInt("##orpg_database_weapon_mattack", &param, 1, 100)) {
                m_selectedWeapon->setParam(2, std::clamp(param, -999, 999));
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("M.Defense:");
              ImGui::SetNextItemWidth(170);
              int param = m_selectedWeapon->param(3);
              if (ImGui::InputInt("##orpg_database_weapon_mdefense", &param, 1, 100)) {
                m_selectedWeapon->setParam(3, std::clamp(param, -999, 999));
              }
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("Agility:");
              ImGui::SetNextItemWidth(170);
              int param = m_selectedWeapon->param(4);
              if (ImGui::InputInt("##orpg_database_weapon_agility", &param, 1, 100)) {
                m_selectedWeapon->setParam(4, std::clamp(param, -999, 999));
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Luck:");
              ImGui::SetNextItemWidth(170);
              int param = m_selectedWeapon->param(5);
              if (ImGui::InputInt("##orpg_database_weapon_luck", &param, 1, 100)) {
                m_selectedWeapon->setParam(5, std::clamp(param, -999, 999));
              }
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("Max HP:");
              ImGui::SetNextItemWidth(170);
              int param = m_selectedWeapon->param(6);
              if (ImGui::InputInt("##orpg_database_weapon_maxhp", &param, 1, 100)) {
                m_selectedWeapon->setParam(6, std::clamp(param, -9999, 9999));
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Max MP:");
              ImGui::SetNextItemWidth(170);
              int param = m_selectedWeapon->param(7);
              if (ImGui::InputInt("##orpg_database_weapon_maxmp", &param, 1, 100)) {
                m_selectedWeapon->setParam(7, std::clamp(param, -9999, 9999));
              }
            }
            ImGui::EndGroup();
          }
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("##orpg_weapons_weapon_panel_right");
        { m_traitsEditor.draw(); }
        ImGui::EndChild();
        ImGui::BeginChild("##orpg_weapons_weapon_panel_bottomright");
        {
          ImGui::SeparatorText("Note:");
          char note[8192];
          strncpy(note, m_selectedWeapon->note().c_str(), IM_ARRAYSIZE(note));
          if (ImGui::InputTextMultiline(std::format("##orpg_database_weapon_note{}", m_selectedWeapon->id()).c_str(), note, IM_ARRAYSIZE(note), ImVec2{ImGui::GetContentRegionMax().x - 16, 400})) {
            m_selectedWeapon->setNote(note);
          }
        }
        ImGui::EndChild();
      }

      ImGui::EndChild();
    }
    ImGui::EndChild();

    if (m_changeIntDialogOpen) {
      if (ImGui::Begin("Change Max Weapons", &m_changeIntDialogOpen,
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
        ImGui::Text(
            "Specify an amount to resize the weapon list to\n"
            "This cannot be undone!");
        ImGui::InputInt("##value_input", &m_editMaxWeapons);
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
            const int tmpId = m_selectedWeapon->id();
            m_maxWeapons = m_editMaxWeapons;
            m_weapons->resize(m_maxWeapons);
            m_selectedWeapon = m_weapons->weapon(tmpId);
            m_changeIntDialogOpen = false;
            m_changeConfirmDialogOpen = false;
          }
          ImGui::SameLine();
          if (ImGui::Button("Cancel")) {
            m_editMaxWeapons = m_maxWeapons;
            m_changeIntDialogOpen = false;
            m_changeConfirmDialogOpen = false;
          }
        }
        ImGui::End();
      }
    }
  }
}