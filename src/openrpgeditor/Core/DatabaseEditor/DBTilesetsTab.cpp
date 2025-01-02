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
  }
  m_maxTilesets = m_tilesets.count();
}

void DBTilesetsTab::draw() {
  if (const auto [closed, confirmed] = m_imagePicker->draw(); closed) {
    if (closed) {
      if (confirmed) {
        m_imagePicker->accept();
        m_selectedTileset->setTilesetName(m_pickerSelection, m_imagePicker->selectedImage());

        m_checkerBoardHeight = 0;
        m_updateCheckerboard = true;
      }
    }
  }

  ImGui::BeginChild("#orpg_tilesets_editor", ImVec2{0, 0}, 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
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
    ImGui::BeginChild("##orpg_tilesets_editor_tilesets_tileset_properties", ImVec2{0, 0}, 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
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
              m_pickerSelection = 0;
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
              m_pickerSelection = 1;
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
              m_pickerSelection = 2;
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
              m_pickerSelection = 3;
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
              m_pickerSelection = 4;
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
              m_pickerSelection = 5;
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
              m_pickerSelection = 6;
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
              m_pickerSelection = 7;
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
              m_pickerSelection = 8;
              m_imagePicker->setOpen(true);
            }
            ImGui::PopID();
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::BeginChild("##orpg_tilesets_tileset_panel_middle", ImVec2{App::DPIHandler::scale_value(410), App::DPIHandler::scale_value(860)});
        {
          ImGui::BeginChild("##orpg_database_tilesets_viewer", ImVec2{App::DPIHandler::scale_value(400), 1542}, ImGuiChildFlags_Border,
                            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
          {
            // for (auto& tilesetF : m_selectedTileset->flags()) {
            //   int test = 1;
            // }
            if (m_updateCheckerboard || !m_checkerboardTexture || !m_checkerboardTexture2) {
              m_checkerboardTexture.setSize(384, 768);
              m_checkerboardTexture2.setSize(384, 1536);
              m_updateCheckerboard = false;
            }

            auto win = ImGui::GetCurrentWindow();
            ImGui::GetWindowDrawList()->AddImage(static_cast<ImTextureID>(m_checkerboardTexture), win->ContentRegionRect.Min,
                                                 win->ContentRegionRect.Min + (ImVec2{384.f, 768.f} * App::DPIHandler::get_ui_scale()));

            ImGui::GetWindowDrawList()->AddImage(static_cast<ImTextureID>(m_checkerboardTexture2), win->ContentRegionRect.Min + (ImVec2{0.f, 768.f} * App::DPIHandler::get_ui_scale()),
                                                 win->ContentRegionRect.Min + (ImVec2{384.f, 1536.f} * App::DPIHandler::get_ui_scale()));
            if (m_selectedTileTab == 0) {
              drawA1();
              if (m_selectedTileset->tilesetName(1) != "") {
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4.f);
              }
              drawA2();
              drawA3();
              if (m_selectedTileset->tilesetName(3) != "") {
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.f);
              }
              drawA4();
              if (m_selectedTileset->tilesetName(4) != "") {
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 1.f);
              }
              drawA5();
            } else if (m_selectedTileTab > 0) {
              drawTileset(m_selectedTileTab + 4);
            }
            // ImGui::Image(m_image->texture(), imageRect);
          }
          ImGui::EndChild();
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.f);
        ImGui::BeginChild("##orpg_tilesets_tileset_panel_right");
        {
          static ImVec4 currentColor = {0.f, 0.f, 0.f, 0.f};
          static ImVec4 normalColor = {0.f, 0.f, 0.f, 0.f};
          if (m_flagSelection == 0) {
            currentColor = {0.5f, 0.5f, 0.5f, 1.0f};
          }
          ImGui::PushStyleColor(ImGuiCol_Button, currentColor);
          currentColor = normalColor;
          ImGui::BeginGroup();
          {
            ImGui::PushID("##orpg_tileset_flags_passage");
            if (ImGui::Button("Passage", ImVec2{(App::DPIHandler::scale_value(110)), 0})) {
              m_flagSelection = 0;
            }
            ImGui::PopID();
          }
          ImGui::PopStyleColor(1);
          ImGui::EndGroup();
          if (m_flagSelection == 1) {
            currentColor = {0.5f, 0.5f, 0.5f, 1.0f};
          }
          ImGui::PushStyleColor(ImGuiCol_Button, currentColor);
          currentColor = normalColor;
          ImGui::BeginGroup();
          {
            ImGui::PushID("##orpg_tileset_flags_passage_4dir");
            if (ImGui::Button("Passage (4 dir)", ImVec2{(App::DPIHandler::scale_value(110)), 0})) {
              m_flagSelection = 1;
            }
            ImGui::PopID();
          }
          ImGui::PopStyleColor(1);
          ImGui::EndGroup();
          if (m_flagSelection == 2) {
            currentColor = {0.5f, 0.5f, 0.5f, 1.0f};
          }
          ImGui::PushStyleColor(ImGuiCol_Button, currentColor);
          currentColor = normalColor;
          ImGui::BeginGroup();
          {
            ImGui::PushID("##orpg_tileset_flags_ladder");
            if (ImGui::Button("Ladder", ImVec2{(App::DPIHandler::scale_value(110)), 0})) {
              m_flagSelection = 2;
            }
            ImGui::PopID();
          }
          ImGui::PopStyleColor(1);
          ImGui::EndGroup();
          if (m_flagSelection == 3) {
            currentColor = {0.5f, 0.5f, 0.5f, 1.0f};
          }
          ImGui::PushStyleColor(ImGuiCol_Button, currentColor);
          currentColor = normalColor;
          ImGui::BeginGroup();
          {
            ImGui::PushID("##orpg_tileset_flags_bush");
            if (ImGui::Button("Bush", ImVec2{(App::DPIHandler::scale_value(110)), 0})) {
              m_flagSelection = 3;
            }
            ImGui::PopID();
          }
          ImGui::PopStyleColor(1);
          ImGui::EndGroup();
          if (m_flagSelection == 4) {
            currentColor = {0.5f, 0.5f, 0.5f, 1.0f};
          }
          ImGui::PushStyleColor(ImGuiCol_Button, currentColor);
          currentColor = normalColor;
          ImGui::BeginGroup();
          {
            ImGui::PushID("##orpg_tileset_flags_counter");
            if (ImGui::Button("Counter", ImVec2{(App::DPIHandler::scale_value(110)), 0})) {
              m_flagSelection = 4;
            }
            ImGui::PopID();
          }
          ImGui::PopStyleColor(1);
          ImGui::EndGroup();
          if (m_flagSelection == 5) {
            currentColor = {0.5f, 0.5f, 0.5f, 1.0f};
          }
          ImGui::PushStyleColor(ImGuiCol_Button, currentColor);
          currentColor = normalColor;
          ImGui::BeginGroup();
          {
            ImGui::PushID("##orpg_tileset_flags_damagefloor");
            if (ImGui::Button("Damage Floor", ImVec2{(App::DPIHandler::scale_value(110)), 0})) {
              m_flagSelection = 5;
            }
            ImGui::PopID();
          }
          ImGui::PopStyleColor(1);
          ImGui::EndGroup();
          if (m_flagSelection == 6) {
            currentColor = {0.5f, 0.5f, 0.5f, 1.0f};
          }
          ImGui::PushStyleColor(ImGuiCol_Button, currentColor);
          currentColor = normalColor;
          ImGui::BeginGroup();
          {
            ImGui::PushID("##orpg_tileset_flags_terraintag");
            if (ImGui::Button("Terrain Tag", ImVec2{(App::DPIHandler::scale_value(110)), 0})) {
              m_flagSelection = 6;
            }
            ImGui::PopID();
          }
          ImGui::PopStyleColor(1);
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - 54.f);
        ImGui::BeginChild("##orpg_database_tilesets_selection_tabs", ImVec2{App::DPIHandler::scale_value(894), App::DPIHandler::scale_value(50)}, ImGuiChildFlags_Border,
                          ImGuiWindowFlags_NoBackground);
        {

          static ImVec4 currentColor = {0.f, 0.f, 0.f, 0.f};
          static ImVec4 normalColor = {0.f, 0.f, 0.f, 0.f};
          if (m_selectedTileTab == 0) {
            currentColor = {0.5f, 0.5f, 0.5f, 1.0f};
          }
          ImGui::PushStyleColor(ImGuiCol_Button, currentColor);
          ImGui::PushID("##orpg_tileset_viewer_a_selection");
          if (ImGui::Button("A", ImVec2{(App::DPIHandler::scale_value(30)), 0})) {
            m_updateCheckerboard = true;
            m_selectedTileTab = 0;
          }
          ImGui::PopID();
          ImGui::PopStyleColor(1);
          currentColor = normalColor;
          ImGui::SameLine();
          if (m_selectedTileset->tilesetName(5) != "") {
            if (m_selectedTileTab == 1) {
              currentColor = {0.5f, 0.5f, 0.5f, 1.0f};
            }
            ImGui::PushStyleColor(ImGuiCol_Button, currentColor);
            ImGui::PushID("##orpg_tileset_viewer_b_selection");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 9.f);
            if (ImGui::Button("B", ImVec2{(App::DPIHandler::scale_value(30)), 0})) {
              m_updateCheckerboard = true;
              m_selectedTileTab = 1;
            }
            ImGui::PopID();
            ImGui::PopStyleColor(1);
            currentColor = normalColor;
            ImGui::SameLine();
          }
          if (m_selectedTileset->tilesetName(6) != "") {
            if (m_selectedTileTab == 2) {
              currentColor = {0.5f, 0.5f, 0.5f, 1.0f};
            }
            ImGui::PushStyleColor(ImGuiCol_Button, currentColor);
            ImGui::PushID("##orpg_tileset_viewer_c_selection");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 9.f);
            if (ImGui::Button("C", ImVec2{(App::DPIHandler::scale_value(30)), 0})) {
              m_updateCheckerboard = true;
              m_selectedTileTab = 2;
            }
            ImGui::PopID();
            ImGui::PopStyleColor(1);
            currentColor = normalColor;
            ImGui::SameLine();
          }
          if (m_selectedTileset->tilesetName(7) != "") {
            if (m_selectedTileTab == 3) {
              currentColor = {0.5f, 0.5f, 0.5f, 1.0f};
            }
            ImGui::PushStyleColor(ImGuiCol_Button, currentColor);
            ImGui::PushID("##orpg_tileset_viewer_d_selection");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 9.f);
            if (ImGui::Button("D", ImVec2{(App::DPIHandler::scale_value(30)), 0})) {
              m_updateCheckerboard = true;
              m_selectedTileTab = 3;
            }
            ImGui::PopID();
            ImGui::PopStyleColor(1);
            currentColor = normalColor;
            ImGui::SameLine();
          }
          if (m_selectedTileset->tilesetName(8) != "") {
            if (m_selectedTileTab == 4) {
              currentColor = {0.5f, 0.5f, 0.5f, 1.0f};
            }
            ImGui::PushStyleColor(ImGuiCol_Button, currentColor);
            ImGui::PushID("##orpg_tileset_viewer_e_selection");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 9.f);
            if (ImGui::Button("E", ImVec2{(App::DPIHandler::scale_value(30)), 0})) {
              m_updateCheckerboard = true;
              m_selectedTileTab = 4;
            }
            ImGui::PopID();
            ImGui::PopStyleColor(1);
          }
          currentColor = normalColor;
        }
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
void DBTilesetsTab::drawA1() {
  if (m_selectedTileset->tilesetName(0) == "")
    return;

  m_image.emplace(m_selectedTileset->tilesetNames().at(0), static_cast<int>(ImagePicker::PickerMode::Tileset), false);
  const int tilesetWidth = m_image->imageWidth();
  const int tilesetHeight = m_image->imageHeight();
  if (m_updateCheckerboard) {
    m_checkerBoardHeight += 2;
  }

  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));
  ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.f);
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {0, 0});

  ImVec2 uvTileSize = ImVec2(1.0f / (tilesetWidth / m_tileSize), 1.0f / (tilesetHeight / m_tileSize));
  ImVec2 uvTileSizeTest = ImVec2(1.0f / (tilesetWidth / 24), 1.0f / (tilesetHeight / 72));
  ImVec2 tileRect = ImVec2(m_tileSize, m_tileSize) * m_scale;

  ImVec2 uv0_1 = ImVec2(0 * uvTileSize.x, 0 * uvTileSize.y);             // Top-left UV
  ImVec2 uv1_1 = ImVec2((0 + 1) * uvTileSize.x, (0 + 1) * uvTileSize.y); // Bottom-right UV

  ImGui::Image(m_image->texture(), tileRect, uv0_1, uv1_1);
  ImGui::SameLine();

  ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
  ImVec2 uv0_2 = ImVec2(0 * uvTileSize.x, 3 * uvTileSize.y);             // Top-left UV
  ImVec2 uv1_2 = ImVec2((0 + 1) * uvTileSize.x, (3 + 1) * uvTileSize.y); // Bottom-right UV

  ImGui::Image(m_image->texture(), tileRect, uv0_2, uv1_2);
  ImGui::SameLine();
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
  ImVec2 a1_5Cursor = ImGui::GetCursorPos();
  ImVec2 uv0_3 = ImVec2((0 * uvTileSize.x) + uvTileSizeTest.x, (0 * uvTileSize.y) + uvTileSizeTest.y);             // Top-left UV
  ImVec2 uv1_3 = ImVec2(((0 + 1) * uvTileSize.x) + uvTileSizeTest.x, ((0 + 1) * uvTileSize.y) + uvTileSizeTest.y); // Bottom-right UV

  ImGui::Image(m_image->texture(), tileRect, uv0_3, uv1_3);
  ImGui::SameLine();
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
  // ImVec2 a1_6Cursor = ImGui::GetCursorPos();
  ImVec2 uv0_4 = ImVec2((0 * uvTileSize.x) + uvTileSizeTest.x, (0 * uvTileSize.y) + uvTileSizeTest.y);             // Top-left UV
  ImVec2 uv1_4 = ImVec2(((0 + 1) * uvTileSize.x) + uvTileSizeTest.x, ((0 + 1) * uvTileSize.y) + uvTileSizeTest.y); // Bottom-right UV

  ImGui::Image(m_image->texture(), tileRect, uv0_4, uv1_4);
  // Masks
  ImGui::SetCursorPos(a1_5Cursor);
  ImVec2 uv0_5 = ImVec2(6 * uvTileSize.x, 0 * uvTileSize.y);             // Top-left UV
  ImVec2 uv1_5 = ImVec2((6 + 1) * uvTileSize.x, (0 + 1) * uvTileSize.y); // Bottom-right UV

  ImGui::Image(m_image->texture(), tileRect, uv0_5, uv1_5);
  ImGui::SameLine();

  ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
  ImVec2 uv0_6 = ImVec2(6 * uvTileSize.x, 3 * uvTileSize.y);             // Top-left UV
  ImVec2 uv1_6 = ImVec2((6 + 1) * uvTileSize.x, (3 + 1) * uvTileSize.y); // Bottom-right UV

  ImGui::Image(m_image->texture(), tileRect, uv0_6, uv1_6);
  ImGui::SameLine();

  ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
  ImVec2 uv0_7 = ImVec2(8 * uvTileSize.x, 0 * uvTileSize.y);             // Top-left UV
  ImVec2 uv1_7 = ImVec2((8 + 1) * uvTileSize.x, (0 + 1) * uvTileSize.y); // Bottom-right UV

  ImGui::Image(m_image->texture(), tileRect, uv0_7, uv1_7);
  ImGui::SameLine();

  ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
  ImVec2 uv0_8 = ImVec2(14 * uvTileSize.x, 0 * uvTileSize.y);             // Top-left UV
  ImVec2 uv1_8 = ImVec2((14 + 1) * uvTileSize.x, (0 + 1) * uvTileSize.y); // Bottom-right UV

  ImGui::Image(m_image->texture(), tileRect, uv0_8, uv1_8);
  ImGui::SameLine();

  ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
  ImVec2 uv0_9 = ImVec2(8 * uvTileSize.x, 3 * uvTileSize.y);             // Top-left UV
  ImVec2 uv1_9 = ImVec2((8 + 1) * uvTileSize.x, (3 + 1) * uvTileSize.y); // Bottom-right UV

  ImGui::Image(m_image->texture(), tileRect, uv0_9, uv1_9);
  ImGui::SameLine();

  ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
  ImVec2 uv0_10 = ImVec2(14 * uvTileSize.x, 3 * uvTileSize.y);             // Top-left UV
  ImVec2 uv1_10 = ImVec2((14 + 1) * uvTileSize.x, (3 + 1) * uvTileSize.y); // Bottom-right UV

  ImGui::Image(m_image->texture(), tileRect, uv0_10, uv1_10);
  // ImGui::NewLine();
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4.f);

  // ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.f);
  ImVec2 uv0_11 = ImVec2(0 * uvTileSize.x, 6 * uvTileSize.y);             // Top-left UV
  ImVec2 uv1_11 = ImVec2((0 + 1) * uvTileSize.x, (6 + 1) * uvTileSize.y); // Bottom-right UV

  ImGui::Image(m_image->texture(), tileRect, uv0_11, uv1_11);
  ImGui::SameLine();

  ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
  ImVec2 uv0_12 = ImVec2(8 * uvTileSize.x, 6 * uvTileSize.y);             // Top-left UV
  ImVec2 uv1_12 = ImVec2((8 + 1) * uvTileSize.x, (6 + 1) * uvTileSize.y); // Bottom-right UV

  ImGui::Image(m_image->texture(), tileRect, uv0_12, uv1_12);
  ImGui::SameLine();
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
  ImVec2 uv0_13 = ImVec2(0 * uvTileSize.x, 9 * uvTileSize.y);             // Top-left UV
  ImVec2 uv1_13 = ImVec2((0 + 1) * uvTileSize.x, (9 + 1) * uvTileSize.y); // Bottom-right UV

  ImGui::Image(m_image->texture(), tileRect, uv0_13, uv1_13);
  ImGui::SameLine();

  ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
  ImVec2 uv0_14 = ImVec2(8 * uvTileSize.x, 9 * uvTileSize.y);             // Top-left UV
  ImVec2 uv1_14 = ImVec2((8 + 1) * uvTileSize.x, (9 + 1) * uvTileSize.y); // Bottom-right UV

  ImGui::Image(m_image->texture(), tileRect, uv0_14, uv1_14);
  ImGui::SameLine();

  ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
  ImVec2 uv0_15 = ImVec2(8 * uvTileSize.x, 6 * uvTileSize.y);             // Top-left UV
  ImVec2 uv1_15 = ImVec2((8 + 1) * uvTileSize.x, (6 + 1) * uvTileSize.y); // Bottom-right UV

  ImGui::Image(m_image->texture(), tileRect, uv0_15, uv1_15);
  ImGui::SameLine();

  ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
  ImVec2 uv0_16 = ImVec2(14 * uvTileSize.x, 6 * uvTileSize.y);             // Top-left UV
  ImVec2 uv1_16 = ImVec2((14 + 1) * uvTileSize.x, (6 + 1) * uvTileSize.y); // Bottom-right UV

  ImGui::Image(m_image->texture(), tileRect, uv0_16, uv1_16);
  ImGui::SameLine();
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
  ImVec2 uv0_17 = ImVec2(8 * uvTileSize.x, 9 * uvTileSize.y);             // Top-left UV
  ImVec2 uv1_17 = ImVec2((8 + 1) * uvTileSize.x, (9 + 1) * uvTileSize.y); // Bottom-right UV

  ImGui::Image(m_image->texture(), tileRect, uv0_17, uv1_17);
  ImGui::SameLine();

  ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
  ImVec2 uv0_18 = ImVec2(14 * uvTileSize.x, 9 * uvTileSize.y);             // Top-left UV
  ImVec2 uv1_18 = ImVec2((14 + 1) * uvTileSize.x, (9 + 1) * uvTileSize.y); // Bottom-right UV

  ImGui::Image(m_image->texture(), tileRect, uv0_18, uv1_18);
  ImGui::PopStyleColor(3);
  ImGui::PopStyleVar(2);
  m_image.reset();
}
void DBTilesetsTab::drawA2() {
  if (m_selectedTileset->tilesetName(1) == "")
    return;

  m_image.emplace(m_selectedTileset->tilesetNames().at(1), static_cast<int>(ImagePicker::PickerMode::Tileset), false);
  const int tilesetWidth = m_image->imageWidth();
  const int tilesetHeight = m_image->imageHeight();
  if (m_updateCheckerboard) {
    m_checkerBoardHeight += 4;
  }
  ImVec2 uvTileSize = ImVec2(1.0f / (tilesetWidth / m_tileSize), 1.0f / (tilesetHeight / m_tileSize));

  for (int y = 0; y < tilesetHeight / m_tileSize; ++y) {
    for (int x = 0; x < m_gridCols; ++x) {
      if (x == 0) {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.f);
      }
      ImVec2 uv0 = ImVec2(x * uvTileSize.x, y * uvTileSize.y);             // Top-left UV
      ImVec2 uv1 = ImVec2((x + 1) * uvTileSize.x, (y + 1) * uvTileSize.y); // Bottom-right UV
      ImVec2 tileRect = ImVec2(m_tileSize, m_tileSize) * m_scale;

      ImGui::PushID(std::format("##orpg_database_tileset_a2_{} ", y * m_gridCols + x).c_str());

      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));
      ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.f);
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {0, 0});

      ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
      ImGui::Image(m_image->texture(), tileRect, uv0, uv1);
      ImGui::PopStyleColor(3);
      ImGui::PopStyleVar(2);
      x += 1;
      ImGui::PopID();
      ImGui::SameLine();
    }
    ImGui::NewLine(); // Move to the next row
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4.f);
    y += 2;
  }
  m_image.reset();
}
void DBTilesetsTab::drawA3() {
  if (m_selectedTileset->tilesetName(2) == "")
    return;

  m_image.emplace(m_selectedTileset->tilesetNames().at(2), static_cast<int>(ImagePicker::PickerMode::Tileset), false);
  const int tilesetWidth = m_image->imageWidth();
  const int tilesetHeight = m_image->imageHeight();
  if (m_updateCheckerboard) {
    m_checkerBoardHeight += 4;
  }
  ImVec2 uvTileSize = ImVec2(1.0f / (tilesetWidth / 96), 1.0f / (tilesetHeight / 96));

  for (int y = 0; y < tilesetHeight / 96; ++y) {
    for (int x = 0; x < (m_gridCols / 2); ++x) {
      if (x == 0) {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.f);
      }
      ImVec2 uv0 = ImVec2(x * uvTileSize.x, y * uvTileSize.y);             // Top-left UV
      ImVec2 uv1 = ImVec2((x + 1) * uvTileSize.x, (y + 1) * uvTileSize.y); // Bottom-right UV
      ImVec2 tileRect = ImVec2(m_tileSize, m_tileSize) * m_scale;

      ImGui::PushID(std::format("##orpg_database_tileset_a3_{} ", y * m_gridCols + x).c_str());

      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));
      ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.f);
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {0, 0});

      ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);

      ImGui::Image(m_image->texture(), tileRect, uv0, uv1);
      ImGui::PopStyleColor(3);
      ImGui::PopStyleVar(2);
      ImGui::PopID();
      ImGui::SameLine();
    }
    ImGui::NewLine(); // Move to the next row
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5.f);
  }
  m_image.reset();
}
void DBTilesetsTab::drawA4() {
  if (m_selectedTileset->tilesetName(3) == "")
    return;

  m_image.emplace(m_selectedTileset->tilesetNames().at(3), static_cast<int>(ImagePicker::PickerMode::Tileset), false);
  const int tilesetWidth = m_image->imageWidth();
  const int tilesetHeight = m_image->imageHeight();
  if (m_updateCheckerboard) {
    m_checkerBoardHeight += 5;
  }
  ImVec2 uvTileSize = ImVec2(1.0f / (tilesetWidth / m_tileSize), 1.0f / (tilesetHeight / m_tileSize));

  bool yBool{false};
  for (int y = 0; y < tilesetHeight / m_tileSize; ++y) {
    for (int x = 0; x < m_gridCols; ++x) {
      if (x == 0) {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.f);
      }
      ImVec2 uv0 = ImVec2(x * uvTileSize.x, y * uvTileSize.y);             // Top-left UV
      ImVec2 uv1 = ImVec2((x + 1) * uvTileSize.x, (y + 1) * uvTileSize.y); // Bottom-right UV
      ImVec2 tileRect = ImVec2(m_tileSize, m_tileSize) * m_scale;

      ImGui::PushID(std::format("##orpg_database_tileset_a4_{} ", y * m_gridCols + x).c_str());

      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));
      ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.f);
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {0, 0});

      ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);

      ImGui::Image(m_image->texture(), tileRect, uv0, uv1);
      ImGui::PopStyleColor(3);
      ImGui::PopStyleVar(2);
      x += 1;
      ImGui::PopID();
      ImGui::SameLine();
    }
    ImGui::NewLine(); // Move to the next row
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4.f);
    if (yBool) {
      yBool = false;
    } else {
      yBool = true;
      y += 3;
    }
  }
  m_image.reset();
}
void DBTilesetsTab::drawA5() {
  if (m_selectedTileset->tilesetName(4) == "")
    return;

  m_image.emplace(m_selectedTileset->tilesetNames().at(4), static_cast<int>(ImagePicker::PickerMode::Tileset), false);
  const int tilesetWidth = m_image->imageWidth();
  const int tilesetHeight = m_image->imageHeight();
  if (m_updateCheckerboard) {
    // checkerBoardHeight += 18;
  }
  ImVec2 uvTileSize = ImVec2(1.0f / (tilesetWidth / m_tileSize), 1.0f / (tilesetHeight / m_tileSize));

  for (int y = 0; y < tilesetHeight / m_tileSize; ++y) {
    for (int x = 0; x < m_gridCols / 2; ++x) {
      if (x == 0) {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.f);
      }
      ImVec2 uv0 = ImVec2(x * uvTileSize.x, y * uvTileSize.y);             // Top-left UV
      ImVec2 uv1 = ImVec2((x + 1) * uvTileSize.x, (y + 1) * uvTileSize.y); // Bottom-right UV
      ImVec2 tileRect = ImVec2(m_tileSize, m_tileSize) * m_scale;

      ImGui::PushID(std::format("##orpg_database_tileset_a5_{} ", y * m_gridCols + x).c_str());

      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));
      ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.f);
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {0, 0});

      ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);

      ImGui::Image(m_image->texture(), tileRect, uv0, uv1);
      ImGui::PopStyleColor(3);
      ImGui::PopStyleVar(2);
      ImGui::PopID();
      ImGui::SameLine();
    }
    ImGui::NewLine(); // Move to the next row
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4.f);
  }
  m_image.reset();
}
void DBTilesetsTab::drawTileset(int type) {

  if (m_selectedTileset->tilesetName(type) == "")
    return;

  m_image.emplace(m_selectedTileset->tilesetNames().at(type), static_cast<int>(ImagePicker::PickerMode::Tileset), false);
  m_tileMarker.emplace(TileFlags::None, 1, 256, 320);
  const int tilesetWidth = m_image->imageWidth();
  const int tilesetHeight = m_image->imageHeight();
  ImVec2 uvTileSize = ImVec2(1.0f / (tilesetWidth / m_tileSize), 1.0f / (tilesetHeight / m_tileSize));
  m_checkerBoardHeight = 32;
  int tileIndex{0};
  if (type > 5) {
    tileIndex += (256 * (type - 5));
  }
  for (int y = 0; y < tilesetHeight / m_tileSize; ++y) {
    for (int x = 0; x < m_gridCols / 2; ++x) {
      if (x == 0) {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.f);
      }
      ImVec2 uv0 = ImVec2(x * uvTileSize.x, y * uvTileSize.y);             // Top-left UV
      ImVec2 uv1 = ImVec2((x + 1) * uvTileSize.x, (y + 1) * uvTileSize.y); // Bottom-right UV
      ImVec2 tileRect = ImVec2(m_tileSize, m_tileSize) * m_scale;

      ImGui::PushID(std::format("##orpg_database_tileset_{}_t_{} ", type, y * m_gridCols + x).c_str());

      ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.f);
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {0, 0});
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
      ImVec2 cursorPos = ImGui::GetCursorPos();
      ImGui::Image(m_image->texture(), tileRect, uv0, uv1);
      ImGui::PopID();
      ImGui::SetCursorPos(cursorPos);
      ImGui::PushID(std::format("##orpg_database_tileset_{}_flag_{}", type, y * m_gridCols + x).c_str());
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));
      drawTileMarker(m_flagSelection, tileRect, tileIndex);
      ImGui::PopID();
      ImGui::PopStyleColor(3);
      ImGui::PopStyleVar(2);
      ImGui::SameLine();
      tileIndex++;
    }
    ImGui::NewLine(); // Move to the next row
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4.f);
  }
  auto win = ImGui::GetCurrentWindow();

  // Second group
  for (int y = 0; y < tilesetHeight / m_tileSize; ++y) {
    for (int x = 0; x < m_gridCols / 2; ++x) {
      if (x == 0) {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.f);
      }
      ImVec2 uv0 = ImVec2((x + 8) * uvTileSize.x, y * uvTileSize.y);       // Top-left UV
      ImVec2 uv1 = ImVec2((x + 9) * uvTileSize.x, (y + 1) * uvTileSize.y); // Bottom-right UV
      ImVec2 tileRect = ImVec2(m_tileSize, m_tileSize) * m_scale;

      ImGui::PushID(std::format("##orpg_database_tileset_{}_t2_{} ", type, y * m_gridCols + x).c_str());

      ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.f);
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {0, 0});
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
      ImVec2 cursorPos = ImGui::GetCursorPos();
      ImGui::Image(m_image->texture(), tileRect, uv0, uv1);
      ImGui::PopID();
      ImGui::SetCursorPos(cursorPos);
      ImGui::PushID(std::format("##orpg_database_tileset_{}_flag2_{} ", type, y * m_gridCols + x).c_str());
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));
      drawTileMarker(m_flagSelection, tileRect, tileIndex);
      ImGui::PopID();
      ImGui::PopStyleColor(3);
      ImGui::PopStyleVar(2);
      ImGui::SameLine();
      tileIndex++;
    }
    ImGui::NewLine(); // Move to the next row
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4.f);
  }
  m_image.reset();
}
void DBTilesetsTab::drawTileMarker(int flagType, ImVec2 tileRect, int tileIndex) {
  if (flagType == 0) {
    // Passage
    bool isPassable = TileHelper::isTilePassable(m_selectedTileset->flags().at(tileIndex));
    bool hasHigherTile = TileHelper::hasHigherTile(m_selectedTileset->flags().at(tileIndex));

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
      toggleTileState(tileIndex, true);
    }
    ImVec4 tint = ImGui::IsItemHovered() ? kHoveredTint : kDefaultTint;
    if (ImGui::ImageButton("##orpg_database_tilesets_tileset_button", m_tileMarker->texture(), tileRect, m_tileMarker->uv0(hasHigherTile ? 8 : (isPassable ? 0 : 1)),
                           m_tileMarker->uv1(hasHigherTile ? 8 : (isPassable ? 0 : 1)), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tint)) {
      toggleTileState(tileIndex, false);
    }
  } else if (flagType == 1) {
    // Passage (4-dir)
    // Texture and grid details
    const int iconSize = 16; // Size of each icon in pixels

    // Grid configuration
    const int gridCols = 3;                    // Number of columns
    const int gridRows = 3;                    // Number of rows
    const ImVec2 iconSpacing = ImVec2(14, 14); // Spacing between icons

    ImVec2 startPos = ImGui::GetCursorScreenPos();
    startPos.x += 3.f;
    startPos.y += 3.f;
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Loop through rows and columns
    for (int y = 0; y < gridRows; y++) {
      for (int x = 0; x < gridCols; x++) {
        bool isDot = y == 1 && x == 1;
        bool isUpArrow = y == 0 && x == 1;
        bool isDownArrow = y == 2 && x == 1;
        bool isLeftArrow = y == 1 && x == 0;
        bool isRightArrow = y == 1 && x == 2;

        ImVec2 minPos = startPos + ImVec2(x * iconSpacing.x, y * iconSpacing.y);
        ImVec2 maxPos = minPos + ImVec2(iconSize, iconSize);

        ImVec4 tintColor = kDefaultTint;
        if (ImGui::IsWindowFocused() || ImGui::IsWindowHovered()) {
          if (ImGui::IsMouseHoveringRect(minPos, maxPos)) {
            tintColor.w = kHoveredTint.w; // Reduce alpha for transparency

            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
              toggleTileState(tileIndex, false,
                              isUpArrow      ? TileFlags::PassageNorth
                              : isDownArrow  ? TileFlags::PassageSouth
                              : isLeftArrow  ? TileFlags::PassageWest
                              : isRightArrow ? TileFlags::PassageEast
                                             : TileFlags::None);
            } else if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
              toggleTileState(tileIndex, true,
                              isUpArrow      ? TileFlags::PassageNorth
                              : isDownArrow  ? TileFlags::PassageSouth
                              : isLeftArrow  ? TileFlags::PassageWest
                              : isRightArrow ? TileFlags::PassageEast
                                             : TileFlags::None);
            }
          }
        }

        if (isDot) {
          // Dot
          drawList->AddImage(m_tileMarker->texture(), minPos, maxPos, m_tileMarker->uv0(3), m_tileMarker->uv1(3), ImGui::ColorConvertFloat4ToU32(tintColor));
        }
        if (isUpArrow) {
          // Up Arrow
          if (TileHelper::isNorthPassage(m_selectedTileset->flags().at(tileIndex))) {
            drawList->AddImage(m_tileMarker->texture(), minPos, maxPos, m_tileMarker->uv0(7), m_tileMarker->uv1(7), ImGui::ColorConvertFloat4ToU32(tintColor));
          } else {
            drawList->AddImage(m_tileMarker->texture(), minPos, maxPos, m_tileMarker->uv0(3), m_tileMarker->uv1(3), ImGui::ColorConvertFloat4ToU32(tintColor));
          }
        }
        if (isLeftArrow) {
          // Left arrow
          if (TileHelper::isWestPassage(m_selectedTileset->flags().at(tileIndex))) {
            drawList->AddImage(m_tileMarker->texture(), minPos, maxPos, m_tileMarker->uv0(5), m_tileMarker->uv1(5), ImGui::ColorConvertFloat4ToU32(tintColor));
          } else {
            drawList->AddImage(m_tileMarker->texture(), minPos, maxPos, m_tileMarker->uv0(3), m_tileMarker->uv1(3), ImGui::ColorConvertFloat4ToU32(tintColor));
          }
        }
        if (isRightArrow) {
          // Right arrow
          if (TileHelper::isEastPassage(m_selectedTileset->flags().at(tileIndex))) {
            drawList->AddImage(m_tileMarker->texture(), minPos, maxPos, m_tileMarker->uv0(6), m_tileMarker->uv1(6), ImGui::ColorConvertFloat4ToU32(tintColor));
          } else {
            drawList->AddImage(m_tileMarker->texture(), minPos, maxPos, m_tileMarker->uv0(3), m_tileMarker->uv1(3), ImGui::ColorConvertFloat4ToU32(tintColor));
          }
        }
        if (isDownArrow) {
          // Down arrow
          if (TileHelper::isSouthPassage(m_selectedTileset->flags().at(tileIndex))) {
            drawList->AddImage(m_tileMarker->texture(), minPos, maxPos, m_tileMarker->uv0(4), m_tileMarker->uv1(4), ImGui::ColorConvertFloat4ToU32(tintColor));
          } else {
            drawList->AddImage(m_tileMarker->texture(), minPos, maxPos, m_tileMarker->uv0(3), m_tileMarker->uv1(3), ImGui::ColorConvertFloat4ToU32(tintColor));
          }
        }
      }
    }
  } else if (flagType == 2) {
    // Ladder
    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
      toggleTileState(tileIndex, true);
    }
    ImVec4 tint = ImGui::IsItemHovered() ? kHoveredTint : kDefaultTint;
    if (ImGui::ImageButton("##orpg_database_tilesets_tileset_button", m_tileMarker->texture(), tileRect, m_tileMarker->uv0(TileHelper::isLadder(m_selectedTileset->flags().at(tileIndex)) ? 2 : 3),
                           m_tileMarker->uv1(TileHelper::isLadder(m_selectedTileset->flags().at(tileIndex)) ? 2 : 3), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tint)) {
      toggleTileState(tileIndex, false);
    }
  } else if (flagType == 3) {
    // Bush
    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
      toggleTileState(tileIndex, true);
    }
    ImVec4 tint = ImGui::IsItemHovered() ? kHoveredTint : kDefaultTint;
    if (ImGui::ImageButton("##orpg_database_tilesets_tileset_button", m_tileMarker->texture(), tileRect, m_tileMarker->uv0(TileHelper::isBush(m_selectedTileset->flags().at(tileIndex)) ? 9 : 3),
                           m_tileMarker->uv1(TileHelper::isBush(m_selectedTileset->flags().at(tileIndex)) ? 9 : 3), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tint)) {
      toggleTileState(tileIndex, false);
    }
  } else if (flagType == 4) {
    // Counter
    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
      toggleTileState(tileIndex, true);
    }
    ImVec4 tint = ImGui::IsItemHovered() ? kHoveredTint : kDefaultTint;
    if (ImGui::ImageButton("##orpg_database_tilesets_tileset_button", m_tileMarker->texture(), tileRect, m_tileMarker->uv0(TileHelper::isCounter(m_selectedTileset->flags().at(tileIndex)) ? 10 : 3),
                           m_tileMarker->uv1(TileHelper::isCounter(m_selectedTileset->flags().at(tileIndex)) ? 10 : 3), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tint)) {
      toggleTileState(tileIndex, false);
    }
  } else if (flagType == 5) {
    // Damage Floor
    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
      toggleTileState(tileIndex, true);
    }
    ImVec4 tint = ImGui::IsItemHovered() ? kHoveredTint : kDefaultTint;
    if (ImGui::ImageButton("##orpg_database_tilesets_tileset_button", m_tileMarker->texture(), tileRect,
                           m_tileMarker->uv0(TileHelper::isDamageFloor(m_selectedTileset->flags().at(tileIndex)) ? 11 : 3),
                           m_tileMarker->uv1(TileHelper::isDamageFloor(m_selectedTileset->flags().at(tileIndex)) ? 11 : 3), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tint)) {
      toggleTileState(tileIndex, false);
    }
  } else if (flagType == 6) {
    // Terrain Tag
  }
}

void DBTilesetsTab::toggleTileState(int tileIndex, bool reverse, TileFlags subTileFlag) {

  int currentFlags = m_selectedTileset->flags().at(tileIndex);

  if (m_flagSelection == 0) {
    // Passage
    // Determine the current state
    bool hasHigherTile = TileHelper::hasHigherTile(currentFlags);
    bool isPassable = TileHelper::isTilePassable(currentFlags) && (hasHigherTile == false);
    bool isImpassable = TileHelper::isTilePassable(currentFlags) == false && hasHigherTile == false;

    if (reverse) {
      if (isPassable) {
        // Transition to Higher Tile
        m_selectedTileset->setFlag(tileIndex, static_cast<int>(TileFlags::Impassable), false);
        m_selectedTileset->setFlag(tileIndex, static_cast<int>(TileFlags::PassageHigherTile), true);
      } else if (isImpassable) {
        // Transition to Passable
        m_selectedTileset->setFlag(tileIndex, static_cast<int>(TileFlags::Impassable), false);
        m_selectedTileset->setFlag(tileIndex, static_cast<int>(TileFlags::PassageHigherTile), false);
      } else if (hasHigherTile) {
        // Transition to Impassable
        m_selectedTileset->setFlag(tileIndex, static_cast<int>(TileFlags::Impassable), true);
        m_selectedTileset->setFlag(tileIndex, static_cast<int>(TileFlags::PassageHigherTile), false);
      }
    } else {
      if (isPassable) {
        // Transition to Impassable
        m_selectedTileset->setFlag(tileIndex, static_cast<int>(TileFlags::Impassable), true);
      } else if (isImpassable) {
        // Transition to Higher Tile
        m_selectedTileset->setFlag(tileIndex, static_cast<int>(TileFlags::Impassable), false);
        m_selectedTileset->setFlag(tileIndex, static_cast<int>(TileFlags::PassageHigherTile), true);
      } else if (hasHigherTile) {
        // Transition to Passable
        m_selectedTileset->setFlag(tileIndex, static_cast<int>(TileFlags::Impassable), false);
        m_selectedTileset->setFlag(tileIndex, static_cast<int>(TileFlags::PassageHigherTile), false);
      }
    }
  } else if (m_flagSelection == 1) {
    // Passage (4-dir)
    if (subTileFlag == TileFlags::PassageNorth || subTileFlag == TileFlags::PassageSouth || subTileFlag == TileFlags::PassageWest || subTileFlag == TileFlags::PassageEast) {
      if (static_cast<int>(subTileFlag) & m_selectedTileset->flags().at(tileIndex)) {
        m_selectedTileset->setFlag(tileIndex, static_cast<int>(subTileFlag), false);
      } else {
        m_selectedTileset->setFlag(tileIndex, static_cast<int>(subTileFlag), true);
      }
    } else {
      // Dot behavior
      if (TileHelper::isNorthPassage(m_selectedTileset->flags().at(tileIndex)) || TileHelper::isSouthPassage(m_selectedTileset->flags().at(tileIndex)) ||
          TileHelper::isWestPassage(m_selectedTileset->flags().at(tileIndex)) || TileHelper::isEastPassage(m_selectedTileset->flags().at(tileIndex))) {
        m_selectedTileset->setFlag(tileIndex, static_cast<int>(TileFlags::PassageNorth), true);
        m_selectedTileset->setFlag(tileIndex, static_cast<int>(TileFlags::PassageSouth), true);
        m_selectedTileset->setFlag(tileIndex, static_cast<int>(TileFlags::PassageWest), true);
        m_selectedTileset->setFlag(tileIndex, static_cast<int>(TileFlags::PassageEast), true);
      } else {
        m_selectedTileset->setFlag(tileIndex, static_cast<int>(TileFlags::PassageNorth), false);
        m_selectedTileset->setFlag(tileIndex, static_cast<int>(TileFlags::PassageSouth), false);
        m_selectedTileset->setFlag(tileIndex, static_cast<int>(TileFlags::PassageWest), false);
        m_selectedTileset->setFlag(tileIndex, static_cast<int>(TileFlags::PassageEast), false);
      }
    }
  } else if (m_flagSelection == 2) {
    // Ladder
    if (TileHelper::isLadder) {
      m_selectedTileset->setFlag(tileIndex, static_cast<int>(TileFlags::Ladder), true);
    } else {
      m_selectedTileset->setFlag(tileIndex, static_cast<int>(TileFlags::Ladder), false);
    }
  } else if (m_flagSelection == 3) {
    // Bush
    if (TileHelper::isBush) {
      m_selectedTileset->setFlag(tileIndex, static_cast<int>(TileFlags::Bush), true);
    } else {
      m_selectedTileset->setFlag(tileIndex, static_cast<int>(TileFlags::Bush), false);
    }
  } else if (m_flagSelection == 4) {
    // Counter
    if (TileHelper::isCounter) {
      m_selectedTileset->setFlag(tileIndex, static_cast<int>(TileFlags::Counter), true);
    } else {
      m_selectedTileset->setFlag(tileIndex, static_cast<int>(TileFlags::Counter), false);
    }
  } else if (m_flagSelection == 5) {
    // Damage Floor
    if (TileHelper::isDamageFloor) {
      m_selectedTileset->setFlag(tileIndex, static_cast<int>(TileFlags::Damage), true);
    } else {
      m_selectedTileset->setFlag(tileIndex, static_cast<int>(TileFlags::Damage), false);
    }
  } else if (m_flagSelection == 6) {
    // Terrain Tag
  }
}
