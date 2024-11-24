#include "Core/DatabaseEditor/DBItemsTab.hpp"
#include "Database/Items.hpp"
#include "Core/Application.hpp"

#include "imgui.h"

DBItemsTab::DBItemsTab(Items& Items, DatabaseEditor* parent) : IDBEditorTab(parent), m_items(Items) {
  m_selectedItem = m_items.item(1);
  if (m_selectedItem) {
    // m_traitsEditor.setTraits(&m_selectedClass->traits);
  }
}

void DBItemsTab::draw() {
  ImGui::BeginChild("#orpg_items_editor");
  {
    ImGui::BeginChild("##orpg_items_editor_items", ImVec2{250.f, 0} * App::DPIHandler::get_ui_scale(), 0,
                      ImGuiWindowFlags_HorizontalScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Items");
        ImGui::BeginChild("##orpg_items_editor_items_list",
                          ImVec2{0, ImGui::GetContentRegionMax().y - (App::DPIHandler::scale_value(108))});
        {
          ImGui::BeginGroup();
          {
            for (auto& skill_ : m_items.items()) {
              if (skill_.id == 0) {
                continue;
              }

              char name[4096];
              snprintf(name, 4096, "%04i %s", skill_.id, skill_.name.c_str());
              if (ImGui::Selectable(name, &skill_ == m_selectedItem) ||
                  (ImGui::IsItemFocused() && m_selectedItem != &skill_)) {
                m_selectedItem = &skill_;
                // m_traitsEditor.setTraits(&m_selectedClass->traits);
              }
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        char str[4096];
        snprintf(str, 4096, "Max Items %i", m_items.count());
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max",
                          ImVec2{ImGui::GetContentRegionMax().x - (App::DPIHandler::scale_value(8)), 0})) {
          m_changeIntDialogOpen = true;
          m_editMaxItems = m_items.count();
        }
      }
      ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_items_editor_items_item_properties");
    { ImGui::EndChild(); }
    ImGui::EndChild();

    if (m_changeIntDialogOpen) {
      if (ImGui::Begin("Change Max Items", &m_changeIntDialogOpen,
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal |
                           ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
        ImGui::Text(
            "Specify an amount to resize the item list to\n"
            "This cannot be undone!");
        ImGui::InputInt("##value_input", &m_editMaxItems);
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
            int tmpId = m_selectedItem->id;
            m_items.resize(m_editMaxItems);
            m_selectedItem = m_items.item(tmpId);
            m_changeIntDialogOpen = false;
            m_changeConfirmDialogOpen = false;
          }
          ImGui::SameLine();
          if (ImGui::Button("Cancel")) {
            m_editMaxItems = m_items.count();
            m_changeIntDialogOpen = false;
            m_changeConfirmDialogOpen = false;
          }
        }
        ImGui::End();
      }
    }
  }
}