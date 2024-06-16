#include "Core/DatabaseEditor/DBClassesTab.hpp"

#include "Core/DPIHandler.hpp"
#include "Database/Classes.hpp"

#include <imgui.h>

DBClassesTab::DBClassesTab(Classes& classes, DatabaseEditor* parent) : IDBEditorTab(parent), m_classes(classes) {
  m_selectedClass = m_classes.classType(1);
  m_maxClasses = m_classes.count();
}

void DBClassesTab::draw() {
  ImGui::BeginChild("#orpg_classes_editor");
  {
    ImGui::BeginChild("##orpg_classes_editor_classes");
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Classes");
        ImGui::BeginChild("##orpg_classes_editor_class_list",
                          ImVec2{0, ImGui::GetContentRegionMax().y - (App::DPIHandler::scale_value(108))});
        {
          ImGui::BeginGroup();
          {
            for (auto& class_ : m_classes.classes()) {
              if (class_.id == 0) {
                continue;
              }

              char name[4096];
              snprintf(name, 4096, "%04i %s", class_.id, class_.name.c_str());
              if (ImGui::Selectable(name, &class_ == m_selectedClass) ||
                  (ImGui::IsItemFocused() && m_selectedClass != &class_)) {
                m_selectedClass = &class_;
              }
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        char str[4096];
        snprintf(str, 4096, "Max Classes %i", m_maxClasses);
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max",
                          ImVec2{ImGui::GetContentRegionMax().x - (App::DPIHandler::scale_value(8)), 0})) {
          m_changeIntDialogOpen = true;
          m_editMaxClasses = m_maxClasses;
        }
      }
      ImGui::EndGroup();
    }
    ImGui::EndChild();
  }
  ImGui::EndChild();

  if (m_changeIntDialogOpen) {
    if (ImGui::Begin("Change Max Classes", &m_changeIntDialogOpen,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal |
                         ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
      ImGui::Text(
          "Specify an amount to resize the class list to\n"
          "This cannot be undone!");
      ImGui::InputInt("##value_input", &m_editMaxClasses);
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
          int tmpId = m_selectedClass->id;
          m_maxClasses = m_editMaxClasses;
          m_classes.resize(m_maxClasses);
          m_selectedClass = m_classes.classType(tmpId);
          m_changeIntDialogOpen = false;
          m_changeConfirmDialogOpen = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
          m_editMaxClasses = m_maxClasses;
          m_changeIntDialogOpen = false;
          m_changeConfirmDialogOpen = false;
        }
      }
      ImGui::End();
    }
  }
}
