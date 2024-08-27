#include "Core/DatabaseEditor/DBTroopsTab.hpp"
#include "Database/Troops.hpp"
#include "Core/Application.hpp"

#include "imgui.h"

DBTroopsTab::DBTroopsTab(Troops& Troops, DatabaseEditor* parent) : IDBEditorTab(parent), m_troops(Troops) {
  m_selectedTroop = m_troops.troop(1);
  if (m_selectedTroop) {
    //m_traitsEditor.setTraits(&m_selectedClass->traits);
  }
  m_maxTroops = m_troops.count();
}

void DBTroopsTab::draw() {
  ImGui::BeginChild("#orpg_troops_editor");
  {
    ImGui::BeginChild("##orpg_troops_editor_troops", ImVec2{250.f, 0} * App::DPIHandler::get_ui_scale(), 0,
                      ImGuiWindowFlags_HorizontalScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Troops");
        ImGui::BeginChild("##orpg_troops_editor_troops_list",
                          ImVec2{0, ImGui::GetContentRegionMax().y - (App::DPIHandler::scale_value(108))});
        {
          ImGui::BeginGroup();
          {
            for (auto& skill_ : m_troops.troops()) {
              if (skill_.id == 0) {
                continue;
              }

              char name[4096];
              snprintf(name, 4096, "%04i %s", skill_.id, skill_.name.c_str());
              if (ImGui::Selectable(name, &skill_ == m_selectedTroop) ||
                  (ImGui::IsItemFocused() && m_selectedTroop != &skill_)) {
                m_selectedTroop = &skill_;
                //m_traitsEditor.setTraits(&m_selectedClass->traits);
                  }
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        char str[4096];
        snprintf(str, 4096, "Max Troops %i", m_maxTroops);
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max",
                          ImVec2{ImGui::GetContentRegionMax().x - (App::DPIHandler::scale_value(8)), 0})) {
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
      ImGui::EndChild();
    }
    ImGui::EndChild();

    if (m_changeIntDialogOpen) {
      if (ImGui::Begin("Change Max Troops", &m_changeIntDialogOpen,
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal |
                           ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
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
                         ImGuiWindowFlags_NoResize | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoSavedSettings |
                             ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
          ImGui::Text("Are you sure?");
          if (ImGui::Button("Yes")) {
            int tmpId = m_selectedTroop->id;
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