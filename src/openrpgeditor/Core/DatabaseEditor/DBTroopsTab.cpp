#include "Core/DatabaseEditor/DBTroopsTab.hpp"
#include "Core/Application.hpp"
#include "Database/Troops.hpp"

#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "imgui.h"
#include "imgui_internal.h"

DBTroopsTab::DBTroopsTab(Troops& troops, DatabaseEditor* parent)
: IDBEditorTab(parent)
, m_troops(troops)
, m_enemies(Database::instance()->enemies)
, m_troopsEditor(nullptr, troops.troop(1)) {
  m_selectedTroop = m_troops.troop(1);
  if (m_selectedTroop) {
    // m_traitsEditor.setTraits(&m_selectedClass->traits);
  }
  m_maxTroops = m_troops.count();
}

void DBTroopsTab::draw() {
  if (!m_characterButtonTexture) {
    m_characterButtonTexture.emplace();
    m_characterButtonTexture->setSize(340, 200);
    if (!m_characterSheet) {
      m_characterSheet.emplace("");
    }
    const auto& [uv0, uv1] = m_characterSheet->getRectForCharacter(0);
    const Point offset{static_cast<int>(uv0.x() * m_characterSheet.value().texture().width()), static_cast<int>(uv0.y() * m_characterSheet.value().texture().height())};
    m_characterButtonTexture->setTexturesToComposite({{m_characterSheet.value().texture(), {m_characterSheet.value().characterWidth(), m_characterSheet.value().characterHeight()}, offset}});
  }
  ImGui::BeginChild("#orpg_troops_editor");
  {
    const auto calc = ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQRSTUV");
    ImGui::BeginChild("##orpg_troops_editor_troops", ImVec2{calc.x + (ImGui::GetStyle().ItemSpacing.x * 2), 0}, 0, ImGuiWindowFlags_NoScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Troops");
        ImGui::BeginChild("##orpg_tilesets_editor_tilesets_list", ImVec2{0, ImGui::GetContentRegionMax().y - ((calc.y + ImGui::GetStyle().ItemSpacing.y) * 4)});
        {
          ImGui::BeginGroup();
          {
            for (auto& troop : m_troops.troops()) {
              if (troop.id() == 0) {
                continue;
              }

              if (ImGui::Selectable(Database::instance()->troopNameAndId(troop.id()).c_str(), &troop == m_selectedTroop) || (ImGui::IsItemFocused() && m_selectedTroop != &troop)) {
                m_selectedTroop = &troop;
                m_troopsEditor.setTroop(m_selectedTroop);
              }
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        char str[4096];
        snprintf(str, 4096, "Max Troops %i", m_maxTroops);
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - 8, 0})) {
          m_changeIntDialogOpen = true;
          m_editMaxTroops = m_maxTroops;
        }
      }
      ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_troops_editor_troops_troop_properties");
    {
      if (m_selectedTroop) {
        ImGui::BeginChild("##orpg_troops_troop_panel_top", ImVec2{0.f, ImGui::GetContentRegionMax().y / 2});
        {
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("General Settings");
            // Name
            ImGui::BeginGroup();
            {
              char name[4096];
              strncpy(name, m_selectedTroop->name().c_str(), 4096);
              if (ImGui::LabelOverLineEdit("##orpg_troops_editor_name", "Name:", name, 4096, ImGui::GetContentRegionMax().x / 2, nullptr, ImGuiInputTextFlags_None)) {
                m_selectedTroop->setName(name);
              }
              ImGui::SameLine();
              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 22.f);
              // Button 1 - Auto-name
              ImGui::PushID("##orpg_database_troops_auto_name");
              if (ImGui::Button("Auto-name", ImVec2{100, 0})) {
                // TODO
              }
              ImGui::PopID();
              ImGui::SameLine();
              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 22.f);
              // Button 2 - Change BG...
              ImGui::PushID("##orpg_database_troops_change_bg");
              if (ImGui::Button("Change BG...", ImVec2{100, 0})) {
                // TODO
              }
              ImGui::PopID();
              ImGui::SameLine();
              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 22.f);
              // Button 3 - Battle Test...
              ImGui::PushID("##orpg_database_troops_battle_test");
              if (ImGui::Button("Battle Test...", ImVec2{100, 0})) {
                // TODO
              }
              ImGui::PopID();
            }
            ImGui::EndGroup();
            // Image
            ImGui::BeginGroup();
            {
              ImGui::TextUnformatted("Image:");
              if (ImGui::ImageButtonEx(ImGui::GetID("##orpg_database_troops_image"), m_characterButtonTexture->get(), ImVec2(m_characterButtonTexture->size()), {0.f, 0.f}, {1.f, 1.f}, {},
                                       {1.f, 1.f, 1.f, 1.f}, ImGuiButtonFlags_PressedOnDoubleClick)) {
                m_currentSheet = &m_characterSheet.value();
                // m_characterPicker->setCharacterInfo("", m_selectedItem->iconIndex);
                // m_characterPicker->setOpen(true);
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 60.f);
            ImGui::BeginGroup();
            {
              // Button 1 - < Add
              ImGui::PushID("##orpg_database_troops_add");
              if (ImGui::Button("< Add", {100, 0})) {
                // TODO
              }
              ImGui::PopID();
              // Button 2 - Remove >
              ImGui::PushID("##orpg_database_troops_remove");
              if (ImGui::Button("Remove >", {100, 0})) {
                // TODO
              }
              ImGui::PopID();
              // Button 3 - Clear
              ImGui::PushID("##orpg_database_troops_clear");
              if (ImGui::Button("Clear", {100, 0})) {
                // TODO
              }
              ImGui::PopID();
              // Button 4 - Align
              ImGui::PushID("##orpg_database_troops_align");
              if (ImGui::Button("Align", {100, 0})) {
                // TODO
              }
              ImGui::PopID();
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.f);
            ImGui::BeginGroup();
            {
              ImGui::BeginChild("##orpg_enemies_editor_troops_enemies_list", ImVec2{0, ImGui::GetContentRegionMax().y - 230});
              {
                for (auto& enemy : m_enemies.enemies()) {
                  if (enemy.id() == 0) {
                    continue;
                  }
                  if (ImGui::Selectable(Database::instance()->enemyNameAndId(enemy.id()).c_str(), &enemy == m_selectedEnemy) || (ImGui::IsItemFocused() && m_selectedEnemy != &enemy)) {
                    m_selectedEnemy = &enemy;
                  }
                }
                ImGui::EndChild();
              }
            }
            ImGui::EndGroup();
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 120.f);
        ImGui::BeginChild("##orpg_troops_panel_bottom");
        {
          ImGui::SeparatorText("Battle Event");
          ImGui::BeginGroup();
          { m_troopsEditor.draw(); }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
      }
      ImGui::EndChild();
    }
    ImGui::EndChild();

    if (m_changeIntDialogOpen) {
      if (ImGui::Begin("Change Max Troops", &m_changeIntDialogOpen,
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
        ImGui::Text(
            "Specify an amount to resize the troop list to\n"
            "This cannot be undone!");
        ImGui::InputInt("##value_input", &m_editMaxTroops);
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
            const int tmpId = m_selectedTroop->id();
            m_maxTroops = m_editMaxTroops;
            m_troops.resize(m_maxTroops);
            m_selectedTroop = m_troops.troop(tmpId);
            m_changeIntDialogOpen = false;
            m_changeConfirmDialogOpen = false;
          }
          ImGui::SameLine();
          if (ImGui::Button("Cancel")) {
            m_editMaxTroops = m_maxTroops;
            m_changeIntDialogOpen = false;
            m_changeConfirmDialogOpen = false;
          }
        }
        ImGui::End();
      }
    }
  }
}
