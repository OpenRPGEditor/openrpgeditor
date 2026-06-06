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

  ImGui::SetNextWindowSizeConstraints(ImGui::GetDPIScaledSize(720, 512), {FLT_MAX, FLT_MAX});
  if (ImGui::Begin(std::format("ID:{} - {}###NewMapDialog", m_newMapId, trNOOP("Map Properties")).c_str(), &m_open,
                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse)) {
    drawPickers();
    ImGui::BeginVertical("##new_map_main_layout", {-1, -1}, 0.f);
    {
      if (ImGui::BeginChild("##new_map_inner_child", {-1, ImGui::GetContentRegionAvail().y - (ImGui::GetFrameHeightWithSpacing() + ImGui::GetStyle().FramePadding.y * 2)}, ImGuiChildFlags_NavFlattened,
                            ImGuiWindowFlags_NoBackground)) {
        ImGui::BeginHorizontal("##new_map_layout_inner", {-1, -1}, 0.f);
        {
          ImGui::BeginVertical("##new_map_main_layout_inner_left", {ImGui::GetContentRegionAvail().x * 0.6f, -1.f}, 0.f);
          {
            GroupBox generalSettingsGroup(trNOOP("General Settings"), "##new_map_general_settings", {-1, 0});
            if (generalSettingsGroup.begin()) {
              
            }
            generalSettingsGroup.end();
          }
          ImGui::EndVertical();
          ImGui::BeginVertical("##new_map_main_layout_inner_right", {-1, -1}, 0.f);
          {
            GroupBox encountersGroup(trNOOP("Encounters"), "##encounters_group", {-1, -1});
            if (encountersGroup.begin()) {
              if (ImGui::BeginTable("##event_list_table", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY, {-1, -1})) {
                ImGui::TableSetupColumn(trNOOP("Troop"));
                ImGui::TableSetupColumn(trNOOP("Weight"));
                ImGui::TableSetupColumn(trNOOP("Range"));
                ImGui::TableSetupScrollFreeze(3, 1);
                ImGui::TableHeadersRow();

                // TODO:

                ImGui::EndTable();
              }
            }
            encountersGroup.end();
          }
          ImGui::EndVertical();
        }
        ImGui::EndHorizontal();
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
