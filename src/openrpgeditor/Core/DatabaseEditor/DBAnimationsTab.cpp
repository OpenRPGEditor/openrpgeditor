#include "Core/DatabaseEditor/DBAnimationsTab.hpp"
#include "Database/Animations.hpp"
#include "Core/Application.hpp"

#include "imgui.h"

DBAnimationsTab::DBAnimationsTab(Animations& animations, DatabaseEditor* parent)
: IDBEditorTab(parent), m_animations(animations) {
  m_selectedAnimation = m_animations.animation(1);
}

void DBAnimationsTab::draw() {
  ImGui::BeginChild("#orpg_animations_editor");
  {
    ImGui::BeginChild("##orpg_animations_editor_animations", ImVec2{250.f, 0} * App::DPIHandler::get_ui_scale(), 0,
                      ImGuiWindowFlags_HorizontalScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Animations");
        ImGui::BeginChild("##orpg_animations_editor_animations_list",
                          ImVec2{0, ImGui::GetContentRegionMax().y - (App::DPIHandler::scale_value(108))});
        {
          ImGui::BeginGroup();
          {
            for (auto& skill_ : m_animations.animations()) {
              if (skill_.id == 0) {
                continue;
              }

              char name[4096];
              snprintf(name, 4096, "%04i %s", skill_.id, skill_.name.c_str());
              if (ImGui::Selectable(name, &skill_ == m_selectedAnimation) ||
                  (ImGui::IsItemFocused() && m_selectedAnimation != &skill_)) {
                m_selectedAnimation = &skill_;
                // m_traitsEditor.setTraits(&m_selectedClass->traits);
              }
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        char str[4096];
        snprintf(str, 4096, "Max Animations %i", m_animations.count());
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max",
                          ImVec2{ImGui::GetContentRegionMax().x - (App::DPIHandler::scale_value(8)), 0})) {
          m_changeIntDialogOpen = true;
          m_editMaxAnimations = m_animations.count();
        }
      }
      ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_animations_editor_animations_animation_properties");
    { ImGui::EndChild(); }
    ImGui::EndChild();

    if (m_changeIntDialogOpen) {
      if (ImGui::Begin("Change Max Animations", &m_changeIntDialogOpen,
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal |
                           ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
        ImGui::Text(
            "Specify an amount to resize the animation list to\n"
            "This cannot be undone!");
        ImGui::InputInt("##value_input", &m_editMaxAnimations);
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
            int tmpId = m_selectedAnimation->id;
            m_animations.resize(m_editMaxAnimations);
            m_selectedAnimation = m_animations.animation(tmpId);
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
