#include "Editor/CommonUI/NewMapDialog.hpp"

#include "Database/Database.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"
#include "GroupBox.hpp"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <orei18n.hpp>

void NewMapDialog::drawPickers() {
  if (m_tilesetPicker) {
    if (const auto [closed, confirmed] = m_tilesetPicker->draw(); closed) {
      if (confirmed) {
        m_tileset = m_tilesetPicker->selection();
      }
      m_tilesetPicker.reset();
    }
  }

  if (m_soundPicker) {
    if (const auto [closed, confirmed] = m_soundPicker->draw("Choose", "audio_picker"); closed) {
      if (confirmed) {
        m_soundPicker->audio().acceptChanges();
      } else {
        m_soundPicker->audio().restoreOriginal();
      }
      m_soundPicker.reset();
    }
  }

  if (m_imagePicker) {
    if (const auto [closed, confirmed] = m_imagePicker->draw(); closed) {
      if (confirmed) {
        if (m_imagePicker->mode() == ImagePicker::PickerMode::Parallax) {
          m_parallax = m_imagePicker->selectedImage();
        } else if (m_imagePicker->mode() == ImagePicker::PickerMode::Battleback) {
          m_battleBack1 = m_imagePicker->selectedImage();
          m_battleBack2 = m_imagePicker->selectedImage2();
        }
      }
    }
  }
}
std::tuple<bool, bool> NewMapDialog::draw() {
  if (!isOpen()) {
    return {false, false};
  }
  const auto size = ImGui::GetDPIScaledSize(480, 708);
  ImGui::SetNextWindowSizeConstraints(size, size);
  if (ImGui::Begin(std::format("ID:{} - {}###NewMapDialog", m_newMapId, trNOOP("New Map")).c_str(), &m_open,
                   ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse)) {
    drawPickers();
    ImGui::BeginVertical("##new_map_main_layout", {-1, -1}, 0.f);
    {
      if (ImGui::BeginChild("##new_map_inner_child", {-1, ImGui::GetContentRegionAvail().y - (ImGui::GetFrameHeightWithSpacing() + ImGui::GetStyle().FramePadding.y * 2)}, ImGuiChildFlags_NavFlattened,
                            ImGuiWindowFlags_NoBackground)) {
        ImGui::BeginVertical("##new_map_main_layout_inner_left", {-1, -1.f}, 0.f);
        {
          GroupBox generalSettingsGroup(trNOOP("General Settings"), "##new_map_general_settings_group", {-1, 0});
          if (generalSettingsGroup.begin()) {
            ImGui::BeginVertical("##new_map_general_settings_layout", {-1, -1}, 0.f);
            {
              ImGui::BeginHorizontal("##new_map_main_general_settings_layout_name_and_disp", {-1, 0}, 0.f);
              {
                GroupBox nameGroup(trNOOP("Name"), "##new_map_name_group", {ImGui::GetContentRegionAvail().x * 0.5f, 0});
                if (nameGroup.begin()) {
                  ImGui::SetNextItemWidth(-1);
                  ImGui::InputText("##new_map_name", &m_mapName);
                }
                nameGroup.end();
                GroupBox displayNameGroup(trNOOP("Display Name"), "##new_map_display_name_group", {-1, 0});
                if (displayNameGroup.begin()) {
                  ImGui::SetNextItemWidth(-1);
                  ImGui::InputText("##new_map_display_name", &m_displayName);
                }
                displayNameGroup.end();
              }
              ImGui::EndHorizontal();
              ImGui::BeginHorizontal("##new_map_tileset_and_dimensions_layout", {-1, 0}, 0.f);
              {
                GroupBox tilesetGroup(trNOOP("Tileset"), "##new_map_tileset_group", {ImGui::GetContentRegionAvail().x * 0.5f, 0});
                if (tilesetGroup.begin() && ImGui::EllipsesButton(std::format("{}##tileset_button", Database::instance()->tilesetNameAndId(m_tileset)).c_str(), {-1, 0.f})) {
                  m_tilesetPicker.emplace(Database::instance()->tilesets->tilesets(), m_tileset);
                  m_tilesetPicker->setOpen(true);
                }
                tilesetGroup.end();
                GroupBox widthGroup(trNOOP("Width"), "##new_map_width_group", {ImGui::GetContentRegionAvail().x * 0.5f, 0});
                if (widthGroup.begin()) {
                  ImGui::SetNextItemWidth(-1);
                  if (ImGui::SpinInt("##map_width", &m_width)) {
                    m_width = std::clamp(m_width, 1, 256);
                  }
                }
                widthGroup.end();

                GroupBox heightGroup(trNOOP("Height"), "##new_map_height_group", {-1, 0});
                if (heightGroup.begin()) {
                  ImGui::SetNextItemWidth(-1);
                  if (ImGui::SpinInt("##map_height", &m_height)) {
                    m_height = std::clamp(m_height, 1, 256);
                  }
                }
                heightGroup.end();
              }
              ImGui::EndHorizontal();
              ImGui::BeginHorizontal("##new_map_scroll_type_and_enc_steps", {-1, 0}, 0.f);
              {
                GroupBox scrollType(trNOOP("Scroll Type"), "##new_map_scroll_type_group", {ImGui::GetContentRegionAvail().x * 0.5f, 0});
                if (scrollType.begin()) {
                  ImGui::SetNextItemWidth(-1);
                  if (ImGui::BeginCombo("##scroll_type_combo", DecodeEnumName(m_scrollType).c_str())) {
                    for (const auto v : magic_enum::enum_values<ScrollType>()) {
                      if (ImGui::Selectable(DecodeEnumName(v).c_str(), m_scrollType == v)) {
                        m_scrollType = v;
                      }

                      if (v == m_scrollType) {
                        ImGui::SetItemDefaultFocus();
                      }
                    }
                    ImGui::EndCombo();
                  }
                }
                scrollType.end();

                GroupBox encounterStepGroup(trNOOP("Encounter Steps"), "##new_map_encounter_steps_group", {-1, 0});
                if (encounterStepGroup.begin()) {
                  ImGui::SetNextItemWidth(-1);
                  if (ImGui::SpinInt("##encounter_steps", &m_encounterSteps)) {
                    m_encounterSteps = std::clamp(m_encounterSteps, 1, 270);
                  }
                }
                encounterStepGroup.end();
              }
              ImGui::EndHorizontal();
              ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
              ImGui::BeginHorizontal("##new_map_bgm_bgs_layout", {-1, 0}, 0.f);
              {
                GroupBox bgmGroupBox(trNOOP("Autoplay BGM"), "##bgm_group", {ImGui::GetContentRegionAvail().x * 0.5f, 0}, &m_autoplayBgm);
                if (bgmGroupBox.begin() && ImGui::EllipsesButton(std::format("{}##bgm", m_autoplayBgm ? m_bgm.name() : "").c_str(), {-1, 0})) {
                  m_soundPicker.emplace(AudioEditor::AudioType::BGM, m_bgm);
                  m_soundPicker->setOpen(true);
                }
                bgmGroupBox.end();

                GroupBox bgsGroupBox(trNOOP("Autoplay BGS"), "##bgs_group", {-1, 0}, &m_autoplayBgs);
                if (bgsGroupBox.begin() && ImGui::EllipsesButton(std::format("{}##bgs", m_autoplayBgs ? m_bgs.name() : "").c_str(), {-1, 0})) {
                  m_soundPicker.emplace(AudioEditor::AudioType::BGS, m_bgs);
                  m_soundPicker->setOpen(true);
                }
                bgsGroupBox.end();
              }
              ImGui::EndHorizontal();

              ImGui::BeginHorizontal("##new_map_battle_back_dashing_layout", {-1, 0}, 0.f);
              {
                GroupBox battleBackGroupBox(trNOOP("Specify Battleback"), "##battleback_group", {ImGui::GetContentRegionAvail().x * 0.5f, 0}, &m_specifyBattleback);
                if (battleBackGroupBox.begin()) {
                  if (ImGui::EllipsesButton(std::format("{}##battleback", m_specifyBattleback ? Database::instance()->dualImageText(m_battleBack1, m_battleBack2) : "").c_str(), {-1, 0})) {
                    m_imagePicker.emplace(ImagePicker::PickerMode::Battleback, m_battleBack1, m_battleBack2);
                    m_imagePicker->setOpen(true);
                  }
                }
                battleBackGroupBox.end();
                ImGui::BeginVertical("##new_map_dashing", {-1, 0}, 0.f);
                {
                  ImGui::Spring(0.75f);
                  ImGui::SetNextItemWidth(-1);
                  ImGui::PushStyleVarY(ImGuiStyleVar_FramePadding, ImGui::GetStyle().FramePadding.y * 0.70f);
                  ImGui::SizeableCheckbox(trNOOP("Disable Dashing"), &m_disableDashing, ImGui::GetFrameHeight() * 0.65f);
                  ImGui::Spring(0.25f);
                  ImGui::PopStyleVar();
                }
                ImGui::EndVertical();
              }
              ImGui::EndHorizontal();
            }
            ImGui::EndVertical();
          }
          generalSettingsGroup.end();
          ImGui::BeginHorizontal("##new_map_parallax_note_layout", {-1, -1}, 0.f);
          {
            GroupBox parallaxGroup(trNOOP("Parallax Background"), "##new_map_parallax_background_group", {ImGui::GetContentRegionAvail().x * .5f, -1});
            if (parallaxGroup.begin()) {
              ImGui::BeginVertical("##new_map_parallax_background_layout", ImGui::GetContentRegionAvail(), 0.f);
              {
                GroupBox imageGroup(trNOOP("Image"), "##parallax_image_group", {-1, 0});
                if (imageGroup.begin() && ImGui::EllipsesButton(std::format("{}##parallax", m_parallax).c_str(), {-1, 0})) {
                  m_imagePicker.emplace(ImagePicker::PickerMode::Parallax, m_parallax);
                  m_imagePicker->setOpen(true);
                }
                imageGroup.end();
                GroupBox loopHorizGroup(trNOOP("Loop Horizontally"), "##loop_horizontally_group", {-1, 0}, &m_loopHorizontally);
                if (loopHorizGroup.begin()) {
                  ImGui::SetNextItemWidth(-1);
                  if (ImGui::SpinInt("##scroll_x_spin", &m_scrollX, 1, 100, m_loopHorizontally ? nullptr: "")) {
                    m_scrollX = std::clamp(m_scrollX, -32, 32);
                  }
                  if (m_loopHorizontally) {
                    ImGui::SetItemTooltip("%s", trNOOP("Horizontal scroll speed for parallax background, in tiles"));
                  }
                }
                loopHorizGroup.end();
                
                GroupBox loopVerGroup(trNOOP("Loop Vertically"), "##loop_vertically_group", {-1, 0}, &m_loopVertically);
                if (loopVerGroup.begin()) {
                  ImGui::SetNextItemWidth(-1);
                  if (ImGui::SpinInt("##scroll_y_spin", &m_scrollY, 1, 100, m_loopVertically ? nullptr: "")) {
                    m_scrollY = std::clamp(m_scrollY, -32, 32);
                  }
                  if (m_loopVertically) {
                    ImGui::SetItemTooltip("%s", trNOOP("Vertical scroll speed for parallax background, in tiles"));
                  }
                }
                loopVerGroup.end();
                ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
                ImGui::Spring(0.75f);
                ImGui::SetNextItemWidth(-1);
                ImGui::PushStyleVarY(ImGuiStyleVar_FramePadding, ImGui::GetStyle().FramePadding.y * 0.70f);
                ImGui::SizeableCheckbox(trNOOP("Show in the Editor"), &m_showParallaxInEditor, ImGui::GetFrameHeight() * 0.65f);
                ImGui::Spring(0.25f);
                ImGui::PopStyleVar();
              }
              ImGui::EndVertical();
            }
            parallaxGroup.end();

            GroupBox noteGroup(trNOOP("Note"), "##new_map_note_group", {-1, -1});
            if (noteGroup.begin()) {
              ImGui::InputTextMultiline("##new_map_note", &m_note, {-1, -1});
            }
            noteGroup.end();
          }
          ImGui::EndHorizontal();
        }
        ImGui::EndVertical();
      }
      ImGui::EndChild();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##new_map_button_layout", {-1, 0}, 0.f);
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##new_map_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          setOpen(false);
          ImGui::CloseCurrentPopup();
        } else if (ret == 1) {
          m_confirmed = false;
          setOpen(false);
          ImGui::CloseCurrentPopup();
        }
      }
      ImGui::EndHorizontal();
    }
    ImGui::EndVertical();
  }
  ImGui::End();

  return {!m_open, m_confirmed};
}
