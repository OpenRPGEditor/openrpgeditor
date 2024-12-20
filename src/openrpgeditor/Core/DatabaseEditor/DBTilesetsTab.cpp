#include "Core/DatabaseEditor/DBTilesetsTab.hpp"
#include "Core/Application.hpp"
#include "Database/Tilesets.hpp"

#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "imgui.h"
#include "imgui_internal.h"

DBTilesetsTab::DBTilesetsTab(Tilesets& tilesets, DatabaseEditor* parent) : IDBEditorTab(parent), m_tilesets(tilesets) {
  m_selectedTileset = m_tilesets.tileset(1);
  if (m_selectedTileset) {
    m_imagePicker.emplace(ImagePicker::PickerMode::Tileset, "", "");
    m_image.emplace(m_selectedTileset->tilesetNames().at(1), static_cast<int>(ImagePicker::PickerMode::Tileset), false);
    // m_traitsEditor.setTraits(&m_selectedClass->traits);
  }
  m_maxTilesets = m_tilesets.count();
}

void DBTilesetsTab::draw() {
  if (const auto [closed, confirmed] = m_imagePicker->draw(); closed) {
    if (closed) {
      if (confirmed) {
        m_imagePicker->accept();
        // m_imageName = m_imagePicker->selectedImage();
      }
    }
  }

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
    {
      if (m_selectedTileset) {
        ImGui::BeginChild("##orpg_tilesets_tileset_panel_left", ImVec2{ImGui::GetContentRegionMax().x / 4, 0.f});
        {
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("General Settings");
            // Name
            ImGui::BeginGroup();
            {
              char name[4096];
              strncpy(name, m_selectedTileset->name().c_str(), 4096);
              if (ImGui::LabelOverLineEdit("##orpg_tilesets_tileset_name", "Name:", name, 4096, ImGui::GetContentRegionMax().x)) {
                m_selectedTileset->setName(name);
              }
            }
            ImGui::EndGroup();
            // Mode
            ImGui::BeginGroup();
            {
              ImGui::Text("Mode:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::BeginCombo("##orpg_tilesets_tileset_mode", DecodeEnumName(m_selectedTileset->mode()).c_str())) {
                for (const auto& type : magic_enum::enum_values<Tileset::Mode>()) {
                  if (ImGui::Selectable(DecodeEnumName(type).c_str(), m_selectedTileset->mode() == type)) {
                    m_selectedTileset->setMode(type);
                  }
                }
                ImGui::EndCombo();
              }
            }
            ImGui::EndGroup();
          }
          ImGui::EndGroup();
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("Images");
            ImGui::Text("A1 (Animation):");
            ImGui::PushID("##orpg_tileset_a1_selection");
            if (ImGui::Button(m_selectedTileset->tilesetName(0).c_str(), ImVec2{(App::DPIHandler::scale_value(170)), 0})) {
              m_imagePicker->setOpen(true);
            }
            ImGui::PopID();
          }
          ImGui::EndGroup();
          ImGui::BeginGroup();
          {
            ImGui::Text("A2 (Ground):");
            ImGui::PushID("##orpg_tileset_a2_selection");
            if (ImGui::Button(m_selectedTileset->tilesetName(1).c_str(), ImVec2{(App::DPIHandler::scale_value(170)), 0})) {
              m_imagePicker->setOpen(true);
            }
            ImGui::PopID();
          }
          ImGui::EndGroup();
          // A3
          ImGui::BeginGroup();
          {
            ImGui::Text("A3 (Buildings):");
            ImGui::PushID("##orpg_tileset_a3_selection");
            if (ImGui::Button(m_selectedTileset->tilesetName(2).c_str(), ImVec2{(App::DPIHandler::scale_value(170)), 0})) {
              m_imagePicker->setOpen(true);
            }
            ImGui::PopID();
          }
          ImGui::EndGroup();
          // A4
          ImGui::BeginGroup();
          {
            ImGui::Text("A4 (Walls):");
            ImGui::PushID("##orpg_tileset_a4_selection");
            if (ImGui::Button(m_selectedTileset->tilesetName(3).c_str(), ImVec2{(App::DPIHandler::scale_value(170)), 0})) {
              m_imagePicker->setOpen(true);
            }
            ImGui::PopID();
          }
          ImGui::EndGroup();
          // A5
          ImGui::BeginGroup();
          {
            ImGui::Text("A5 (Normal):");
            ImGui::PushID("##orpg_tileset_a5_selection");
            if (ImGui::Button(m_selectedTileset->tilesetName(4).c_str(), ImVec2{(App::DPIHandler::scale_value(170)), 0})) {
              m_imagePicker->setOpen(true);
            }
            ImGui::PopID();
          }
          ImGui::EndGroup();
          // B
          ImGui::BeginGroup();
          {
            ImGui::Text("B:");
            ImGui::PushID("##orpg_tileset_b1_selection");
            if (ImGui::Button(m_selectedTileset->tilesetName(5).c_str(), ImVec2{(App::DPIHandler::scale_value(170)), 0})) {
              m_imagePicker->setOpen(true);
            }
            ImGui::PopID();
          }
          ImGui::EndGroup();
          // C
          ImGui::BeginGroup();
          {
            ImGui::Text("C:");
            ImGui::PushID("##orpg_tileset_c1_selection");
            if (ImGui::Button(m_selectedTileset->tilesetName(6).c_str(), ImVec2{(App::DPIHandler::scale_value(170)), 0})) {
              m_imagePicker->setOpen(true);
            }
            ImGui::PopID();
          }
          ImGui::EndGroup();
          // D
          ImGui::BeginGroup();
          {
            ImGui::Text("D:");
            ImGui::PushID("##orpg_tileset_d1_selection");
            if (ImGui::Button(m_selectedTileset->tilesetName(7).c_str(), ImVec2{(App::DPIHandler::scale_value(170)), 0})) {
              m_imagePicker->setOpen(true);
            }
            ImGui::PopID();
          }
          ImGui::EndGroup();
          // E
          ImGui::BeginGroup();
          {
            ImGui::Text("E:");
            ImGui::PushID("##orpg_tileset_e1_selection");
            if (ImGui::Button(m_selectedTileset->tilesetName(8).c_str(), ImVec2{(App::DPIHandler::scale_value(170)), 0})) {
              m_imagePicker->setOpen(true);
            }
            ImGui::PopID();
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::BeginChild("##orpg_tilesets_tileset_panel_middle", ImVec2{ImGui::GetContentRegionMax().x / 2, 0.f});
        {
          ImGui::BeginChild("##orpg_database_tilesets_viewer", ImVec2{ImGui::GetContentRegionMax().x, App::DPIHandler::scale_value(800)}, ImGuiChildFlags_Border,
                            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_HorizontalScrollbar);
          {
            auto win = ImGui::GetCurrentWindow();
            const auto imageRect = ImVec2{static_cast<float>(m_image->imageWidth()), static_cast<float>(m_image->imageHeight())} * App::DPIHandler::get_ui_scale();
            ImGui::GetWindowDrawList()->AddImage(m_checkerboardTexture, win->ContentRegionRect.Min + ImVec2{0.f, 0.f},
                                                 win->ContentRegionRect.Min +
                                                     (ImVec2{static_cast<float>(m_image->texture().width()), static_cast<float>(m_image->texture().height())} * App::DPIHandler::get_ui_scale()));

            ImGui::Image(m_image->texture(), imageRect);
          }
          ImGui::EndChild();

          ImGui::BeginChild("##orpg_database_tilesets_selection_tabs", ImVec2{App::DPIHandler::scale_value(894), App::DPIHandler::scale_value(784)}, ImGuiChildFlags_Border,
                            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_HorizontalScrollbar);
          {

            static ImVec4 currentColor = {0.f, 0.f, 0.f, 0.f};
            static ImVec4 normalColor = {0.f, 0.f, 0.f, 0.f};
            if (m_selectedTileTab == 0) {
              currentColor = {0.5f, 0.5f, 0.5f, 1.0f};
            }
            ImGui::PushStyleColor(ImGuiCol_Button, currentColor);
            ImGui::PushID("##orpg_tileset_viewer_a_selection");
            if (ImGui::Button("A", ImVec2{(App::DPIHandler::scale_value(30)), 0})) {
              m_selectedTileTab = 0;
            }
            ImGui::PopID();
            ImGui::PopStyleColor(1);
            currentColor = normalColor;
            ImGui::SameLine();
            if (m_selectedTileTab == 1) {
              currentColor = {0.5f, 0.5f, 0.5f, 1.0f};
            }
            ImGui::PushStyleColor(ImGuiCol_Button, currentColor);
            ImGui::PushID("##orpg_tileset_viewer_b_selection");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 9.f);
            if (ImGui::Button("B", ImVec2{(App::DPIHandler::scale_value(30)), 0})) {
              m_selectedTileTab = 1;
            }
            ImGui::PopID();
            ImGui::PopStyleColor(1);
            currentColor = normalColor;
            ImGui::SameLine();
            if (m_selectedTileTab == 2) {
              currentColor = {0.5f, 0.5f, 0.5f, 1.0f};
            }
            ImGui::PushStyleColor(ImGuiCol_Button, currentColor);
            ImGui::PushID("##orpg_tileset_viewer_c_selection");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 9.f);
            if (ImGui::Button("C", ImVec2{(App::DPIHandler::scale_value(30)), 0})) {
              m_selectedTileTab = 2;
            }
            ImGui::PopID();
            ImGui::PopStyleColor(1);
            currentColor = normalColor;
            ImGui::SameLine();
            if (m_selectedTileTab == 3) {
              currentColor = {0.5f, 0.5f, 0.5f, 1.0f};
            }
            ImGui::PushStyleColor(ImGuiCol_Button, currentColor);
            ImGui::PushID("##orpg_tileset_viewer_d_selection");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 9.f);
            if (ImGui::Button("D", ImVec2{(App::DPIHandler::scale_value(30)), 0})) {
              m_selectedTileTab = 3;
            }
            ImGui::PopID();
            ImGui::PopStyleColor(1);
            currentColor = normalColor;
            ImGui::SameLine();
            if (m_selectedTileTab == 4) {
              currentColor = {0.5f, 0.5f, 0.5f, 1.0f};
            }
            ImGui::PushStyleColor(ImGuiCol_Button, currentColor);
            ImGui::PushID("##orpg_tileset_viewer_e_selection");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 9.f);
            if (ImGui::Button("E", ImVec2{(App::DPIHandler::scale_value(30)), 0})) {
              m_selectedTileTab = 4;
            }
            ImGui::PopID();
            ImGui::PopStyleColor(1);
            currentColor = normalColor;
          }
          ImGui::EndChild();
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("##orpg_tilesets_tileset_panel_right");
        {}
        ImGui::EndChild();
      }
      ImGui::EndChild();
    }
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