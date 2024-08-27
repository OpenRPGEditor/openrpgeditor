#include "Core/DatabaseEditor/DBSkillsTab.hpp"
#include "Database/Skills.hpp"
#include "Core/Application.hpp"
#include "imgui.h"
DBSkillsTab::DBSkillsTab(Skills& skills, DatabaseEditor* parent) : IDBEditorTab(parent), m_skills(skills) {
  m_selectedSkill = m_skills.skill(1);
  if (m_selectedSkill) {
    //m_traitsEditor.setTraits(&m_selectedClass->traits);
  }
  m_maxSkills = m_skills.count();
}

void DBSkillsTab::draw() {
  ImGui::BeginChild("#orpg_skills_editor");
  {
    ImGui::BeginChild("##orpg_skills_editor_skills", ImVec2{250.f, 0} * App::DPIHandler::get_ui_scale(), 0,
                      ImGuiWindowFlags_HorizontalScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Skills");
        ImGui::BeginChild("##orpg_skills_editor_skills_list",
                          ImVec2{0, ImGui::GetContentRegionMax().y - (App::DPIHandler::scale_value(108))});
        {
          ImGui::BeginGroup();
          {
            for (auto& skill_ : m_skills.skills()) {
              if (skill_.id == 0) {
                continue;
              }

              char name[4096];
              snprintf(name, 4096, "%04i %s", skill_.id, skill_.name.c_str());
              if (ImGui::Selectable(name, &skill_ == m_selectedSkill) ||
                  (ImGui::IsItemFocused() && m_selectedSkill != &skill_)) {
                m_selectedSkill = &skill_;
                //m_traitsEditor.setTraits(&m_selectedClass->traits);
                  }
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        char str[4096];
        snprintf(str, 4096, "Max Skills %i", m_maxSkills);
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max",
                          ImVec2{ImGui::GetContentRegionMax().x - (App::DPIHandler::scale_value(8)), 0})) {
          m_changeIntDialogOpen = true;
          m_editMaxSkills = m_maxSkills;
                          }
      }
      ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_skills_editor_skills_skill_properties");
    {
      ImGui::EndChild();
    }
    ImGui::EndChild();

    if (m_changeIntDialogOpen) {
      if (ImGui::Begin("Change Max Classes", &m_changeIntDialogOpen,
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal |
                           ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
        ImGui::Text(
            "Specify an amount to resize the skill list to\n"
            "This cannot be undone!");
        ImGui::InputInt("##value_input", &m_editMaxSkills);
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
            int tmpId = m_selectedSkill->id;
            m_maxSkills = m_editMaxSkills;
            m_skills.resize(m_maxSkills);
            m_selectedSkill = m_skills.skill(tmpId);
            m_changeIntDialogOpen = false;
            m_changeConfirmDialogOpen = false;
          }
          ImGui::SameLine();
          if (ImGui::Button("Cancel")) {
            m_editMaxSkills = m_maxSkills;
            m_changeIntDialogOpen = false;
            m_changeConfirmDialogOpen = false;
          }
                             }
        ImGui::End();
      }
    }
  }
}

