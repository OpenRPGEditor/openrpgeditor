#include "Core/DatabaseEditor/DBTilesetsTab.hpp"
#include "Core/Application.hpp"
#include "Database/Tilesets.hpp"

#include "imgui.h"

DBTilesetsTab::DBTilesetsTab(Tilesets& tilesets, DatabaseEditor* parent) : IDBEditorTab(parent), m_tilesets(tilesets) {
  m_selectedTileset = m_tilesets.tileset(1);
  if (m_selectedTileset) {
    // m_traitsEditor.setTraits(&m_selectedClass->traits);
  }
  m_maxTilesets = m_tilesets.count();
}

void DBTilesetsTab::draw() {
  ImGui::BeginChild("#orpg_tilesets_editor");
  {
    ImGui::BeginChild("##orpg_tilesets_editor_tilesets", ImVec2{250.f, 0} * App::DPIHandler::get_ui_scale(), 0, ImGuiWindowFlags_HorizontalScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Tilesets");
        ImGui::BeginChild("##orpg_tilesets_editor_tilesets_list", ImVec2{0, ImGui::GetContentRegionMax().y - App::DPIHandler::scale_value(108)});
        {
          ImGui::BeginGroup();
          {
            for (auto& tileset : m_tilesets.tilesets()) {
              if (tileset.id() == 0) {
                continue;
              }

              if (ImGui::Selectable(Database::instance()->tilesetNameAndId(tileset.id()).c_str(), &tileset == m_selectedTileset) || (ImGui::IsItemFocused() && m_selectedTileset != &tileset)) {
                m_selectedTileset = &tileset;
                // m_traitsEditor.setTraits(&m_selectedClass->traits);
              }
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        char str[4096];
        snprintf(str, 4096, "Max Tilesets %i", m_maxTilesets);
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - App::DPIHandler::scale_value(8), 0})) {
          m_changeIntDialogOpen = true;
          m_editMaxTilesets = m_maxTilesets;
        }
      }
      ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_tilesets_editor_tilesets_tileset_properties");
    { ImGui::EndChild(); }
    ImGui::EndChild();

    if (m_changeIntDialogOpen) {
      if (ImGui::Begin("Change Max Tilesets", &m_changeIntDialogOpen,
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
        ImGui::Text(
            "Specify an amount to resize the tileset list to\n"
            "This cannot be undone!");
        ImGui::InputInt("##value_input", &m_editMaxTilesets);
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
            const int tmpId = m_selectedTileset->id();
            m_maxTilesets = m_editMaxTilesets;
            m_tilesets.resize(m_maxTilesets);
            m_selectedTileset = m_tilesets.tileset(tmpId);
            m_changeIntDialogOpen = false;
            m_changeConfirmDialogOpen = false;
          }
          ImGui::SameLine();
          if (ImGui::Button("Cancel")) {
            m_editMaxTilesets = m_maxTilesets;
            m_changeIntDialogOpen = false;
            m_changeConfirmDialogOpen = false;
          }
        }
        ImGui::End();
      }
    }
  }
}