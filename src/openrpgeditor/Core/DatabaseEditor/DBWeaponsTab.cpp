#include "Core/DatabaseEditor/DBWeaponsTab.hpp"
#include "Core/Application.hpp"
#include "Database/Weapons.hpp"

#include "imgui.h"

DBWeaponsTab::DBWeaponsTab(Weapons& Weapons, DatabaseEditor* parent) : IDBEditorTab(parent), m_weapons(Weapons) {
  m_selectedWeapon = m_weapons.weapon(1);
  if (m_selectedWeapon) {
    // m_traitsEditor.setTraits(&m_selectedClass->traits);
  }
  m_maxWeapons = m_weapons.count();
}

void DBWeaponsTab::draw() {
  ImGui::BeginChild("#orpg_weapons_editor");
  {
    ImGui::BeginChild("##orpg_weapons_editor_weapons", ImVec2{250.f, 0} * App::DPIHandler::get_ui_scale(), 0, ImGuiWindowFlags_HorizontalScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Weapons");
        ImGui::BeginChild("##orpg_weapons_editor_weapons_list", ImVec2{0, ImGui::GetContentRegionMax().y - (App::DPIHandler::scale_value(108))});
        {
          ImGui::BeginGroup();
          {
            for (auto& skill_ : m_weapons.weapons()) {
              if (skill_.id == 0) {
                continue;
              }

              char name[4096];
              snprintf(name, 4096, "%04i %s", skill_.id, skill_.name.c_str());
              if (ImGui::Selectable(name, &skill_ == m_selectedWeapon) || (ImGui::IsItemFocused() && m_selectedWeapon != &skill_)) {
                m_selectedWeapon = &skill_;
                // m_traitsEditor.setTraits(&m_selectedClass->traits);
              }
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        char str[4096];
        snprintf(str, 4096, "Max Weapons %i", m_maxWeapons);
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - (App::DPIHandler::scale_value(8)), 0})) {
          m_changeIntDialogOpen = true;
          m_editMaxWeapons = m_maxWeapons;
        }
      }
      ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_weapons_editor_weapons_weapon_properties");
    { ImGui::EndChild(); }
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
            int tmpId = m_selectedWeapon->id;
            m_maxWeapons = m_editMaxWeapons;
            m_weapons.resize(m_maxWeapons);
            m_selectedWeapon = m_weapons.weapon(tmpId);
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