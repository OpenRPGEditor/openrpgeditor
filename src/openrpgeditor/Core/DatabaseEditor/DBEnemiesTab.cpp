#include "Core/DatabaseEditor/DBEnemiesTab.hpp"
#include "Database/Enemies.hpp"
#include "Core/Application.hpp"

#include "imgui.h"

DBEnemiesTab::DBEnemiesTab(Enemies& Enemies, DatabaseEditor* parent) : IDBEditorTab(parent), m_enemies(Enemies) {
  m_selectedEnemy = m_enemies.enemy(1);
  if (m_selectedEnemy) {
    // m_traitsEditor.setTraits(&m_selectedClass->traits);
  }
}

void DBEnemiesTab::draw() {
  ImGui::BeginChild("#orpg_enemies_editor");
  {
    ImGui::BeginChild("##orpg_enemies_editor_enemies", ImVec2{250.f, 0} * App::DPIHandler::get_ui_scale(), 0,
                      ImGuiWindowFlags_HorizontalScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Enemies");
        ImGui::BeginChild("##orpg_enemies_editor_enemies_list",
                          ImVec2{0, ImGui::GetContentRegionMax().y - (App::DPIHandler::scale_value(108))});
        {
          ImGui::BeginGroup();
          {
            for (auto& skill_ : m_enemies.enemies()) {
              if (skill_.id == 0) {
                continue;
              }

              char name[4096];
              snprintf(name, 4096, "%04i %s", skill_.id, skill_.name.c_str());
              if (ImGui::Selectable(name, &skill_ == m_selectedEnemy) ||
                  (ImGui::IsItemFocused() && m_selectedEnemy != &skill_)) {
                m_selectedEnemy = &skill_;
                // m_traitsEditor.setTraits(&m_selectedClass->traits);
              }
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        char str[4096];
        snprintf(str, 4096, "Max Enemies %i", m_enemies.count());
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max",
                          ImVec2{ImGui::GetContentRegionMax().x - (App::DPIHandler::scale_value(8)), 0})) {
          m_changeIntDialogOpen = true;
          m_editMaxEnemies = m_enemies.count();
        }
      }
      ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_enemies_editor_enemies_enemy_properties");
    { ImGui::EndChild(); }
    ImGui::EndChild();

    if (m_changeIntDialogOpen) {
      if (ImGui::Begin("Change Max Enemies", &m_changeIntDialogOpen,
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal |
                           ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
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
                         ImGuiWindowFlags_NoResize | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoSavedSettings |
                             ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
          ImGui::Text("Are you sure?");
          if (ImGui::Button("Yes")) {
            int tmpId = m_selectedEnemy->id;
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
  }
}
