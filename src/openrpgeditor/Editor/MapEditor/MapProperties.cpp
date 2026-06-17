#include "Editor/MapEditor/MapProperties.hpp"

#include "Database/Database.hpp"
#include "Editor/CommonUI/GroupBox.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"
#include "Editor/MainWindow.hpp"
#include "Editor/MapEditor.hpp"

#include <Database/MapInfo.hpp>

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

Map* MapProperties::map() const { return m_mapInfo->map(); }

void MapProperties::setMapInfo(MapInfo* mapInfo) {
  m_mapInfo = mapInfo;
  if (!m_mapInfo || !m_mapInfo->map()) {
    return;
  }

  const Map* map = m_mapInfo->map();
  m_mapName = m_mapInfo->name();
  m_displayName = map->displayName();
  m_note = map->note();
  m_tileset = map->tilesetId();
  m_width = map->width();
  m_height = map->height();
  m_scrollType = map->scrollType();
  m_encounterSteps = map->encounterStep();
  m_autoplayBgm = map->autoplayBgm();
  m_bgm = map->bgm();
  m_autoplayBgs = map->autoplayBgs();
  m_bgs = map->bgs();
  m_specifyBattleback = map->specifyBattleback();
  m_battleBack1 = map->battleback1Name();
  m_battleBack2 = map->battleback2Name();
  m_disableDashing = map->disableDashing();
  m_parallaxLoopX = map->parallaxLoopX();
  m_parallaxSx = map->parallaxSx();
  m_parallaxLoopY = map->parallaxLoopY();
  m_parallaxSy = map->parallaxSy();
  m_parallaxShow = map->parallaxShow();
  m_encounters = map->encounterList();
}

void MapProperties::drawPickers() {
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
void MapProperties::applyChanges() {
  m_mapInfo->setName(m_name);
  Map* map = m_mapInfo->map();
  map->setDisplayName(m_displayName);
  map->setNote(m_note);
  map->setTilesetId(m_tileset);
  map->setScrollType(m_scrollType);
  map->setEncounterStep(m_encounterSteps);
  map->setAutoplayBgm(m_autoplayBgm);
  map->setBgm(m_bgm);
  map->setAutoplayBgs(m_autoplayBgs);
  map->setBgs(m_bgs);
  map->setSpecifyBattleback(m_specifyBattleback);
  map->setBattleback1Name(m_battleBack1);
  map->setBattleback2Name(m_battleBack2);
  map->setDisableDashing(m_disableDashing);
  map->setParallaxLoopX(m_parallaxLoopX);
  map->setParallaxSx(m_parallaxSx);
  map->setParallaxLoopY(m_parallaxLoopY);
  map->setParallaxSy(m_parallaxSy);
  map->setParallaxShow(m_parallaxShow);
  map->resize(m_width, m_height);
  map->setEncounterList(m_encounters);
}
std::tuple<bool, bool> MapProperties::draw() {
  if (!m_open || !m_mapInfo || !map()) {
    return {!m_open, m_confirmed};
  }

  const auto separatorHeight = ImGui::GetDPIScaledValue(1.5f);
  ImGui::SetNextWindowSizeConstraints(ImGui::GetDPIScaledSize(840, 708), {FLT_MAX, FLT_MAX});
  if (ImGui::Begin(std::format("{}###map_properties", trNOOP("Map Properties")).c_str(), &m_open, ImGuiWindowFlags_HorizontalScrollbar)) {
    drawPickers();
    ImGui::BeginVertical("##map_properties_main_layout", ImGui::GetContentRegionAvail(), 0.f);
    {
      ImGui::BeginHorizontal("##map_properties_inner_layout", {-1, 0}, 0.f);
      {
        const auto innerHeight = ImGui::GetContentRegionAvail().y - (ImGui::GetFrameHeightWithSpacing() + separatorHeight);
        if (ImGui::BeginChild("##map_properties_inner_left_layout", {ImGui::GetContentRegionAvail().x * 0.75f, innerHeight}, ImGuiChildFlags_ResizeX | ImGuiChildFlags_Borders,
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
                  GroupBox loopHorizGroup(trNOOP("Loop Horizontally"), "##loop_horizontally_group", {-1, 0}, &m_parallaxLoopX);
                  if (loopHorizGroup.begin()) {
                    ImGui::SetNextItemWidth(-1);
                    if (ImGui::SpinInt("##scroll_x_spin", &m_parallaxSx, 1, 100, m_parallaxLoopX ? nullptr : "")) {
                      m_parallaxSx = std::clamp(m_parallaxSx, -32, 32);
                    }
                    if (m_parallaxLoopX) {
                      ImGui::SetItemTooltip("%s", trNOOP("Horizontal scroll speed for parallax background, in tiles"));
                    }
                  }
                  loopHorizGroup.end();

                  GroupBox loopVerGroup(trNOOP("Loop Vertically"), "##loop_vertically_group", {-1, 0}, &m_parallaxLoopY);
                  if (loopVerGroup.begin()) {
                    ImGui::SetNextItemWidth(-1);
                    if (ImGui::SpinInt("##scroll_y_spin", &m_parallaxSy, 1, 100, m_parallaxLoopY ? nullptr : "")) {
                      m_parallaxSy = std::clamp(m_parallaxSy, -32, 32);
                    }
                    if (m_parallaxLoopY) {
                      ImGui::SetItemTooltip("%s", trNOOP("Vertical scroll speed for parallax background, in tiles"));
                    }
                  }
                  loopVerGroup.end();
                  ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
                  ImGui::SetNextItemWidth(-1);
                  // ImGui::PushStyleVarY(ImGuiStyleVar_FramePadding, ImGui::GetStyle().FramePadding.y * 0.70f);
                  ImGui::SizeableCheckbox(trNOOP("Show in the Editor"), &m_parallaxShow, ImGui::GetFrameHeight() * 0.65f);
                  // ImGui::PopStyleVar();
                  ImGui::Spring();
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
        if (ImGui::BeginChild("##map_properties_inner_right_layout", {-1, innerHeight}, ImGuiChildFlags_Borders, ImGuiWindowFlags_NoBackground)) {
          GroupBox encounterGroup(trNOOP("Encounters"), "##map_properties_encounters", {-1, -1});
          if (encounterGroup.begin()) {
            if (ImGui::BeginTable("##map_properties_encounter_list", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable,
                                  ImGui::GetContentRegionAvail())) {
              ImGui::TableSetupColumn(trNOOP("Troop"));
              ImGui::TableSetupColumn(trNOOP("Weight"));
              ImGui::TableSetupColumn(trNOOP("Range"), ImGuiTableFlags_SizingFixedSame);
              ImGui::TableSetupScrollFreeze(3, 1);
              ImGui::TableHeadersRow();

              for (const auto&& [index, encounter] : m_encounters | std::views::enumerate) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                if (ImGui::Selectable(std::format("{}##encounter_troop_{}", Database::instance()->troopNameAndId(encounter.troopId()), index).c_str(), /*index == selection*/ false,
                                      ImGuiSelectableFlags_SpanAllColumns)) {}
                ImGui::TableNextColumn();
                ImGui::Text("%i", encounter.weight());
                ImGui::TableNextColumn();
                std::string range;
                if (!encounter.regionSet().empty()) {
                  for (const auto region : encounter.regionSet()) {
                    range += range.empty() ? "" : ", ";
                    range += std::to_string(region);
                  }
                } else {
                  range = trNOOP("Entire Map");
                }
                ImGui::TextUnformatted(range.c_str());
              }

              ImGui::TableNextRow();
              ImGui::TableNextColumn();
              if (ImGui::Selectable("##encounters_add", false, ImGuiSelectableFlags_SpanAllColumns)) {
                // TODO: Encounter editor dialog
              }
              ImGui::EndTable();
            }
          }
          encounterGroup.end();
        }
        ImGui::EndChild();
      }
      ImGui::EndHorizontal();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, separatorHeight);
      ImGui::BeginHorizontal("##map_properties_button_layout");
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##map_properties_buttons", {trNOOP("OK"), trNOOP("Apply"), trNOOP("Cancel")}, false, {}, {}, {!ImGui::IsWindowDocked(), true, true}); ret == 0) { // OK
          applyChanges();
          if (!ImGui::IsWindowDocked()) {
            setOpen(false);
          }
        } else if (ret == 1) { // Apply
          applyChanges();
        } else if (ret == 2) { // Cancel
          setMapInfo(MainWindow::instance()->currentMapInfo());
          if (!ImGui::IsWindowDocked()) {
            setOpen(false);
          }
        }
      }
      ImGui::EndHorizontal();
    }
    ImGui::EndVertical();
  }
  ImGui::End();

  return {!m_open, m_confirmed};
}
