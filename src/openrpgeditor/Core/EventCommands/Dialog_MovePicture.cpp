#include "Core/EventCommands/Dialog_MovePicture.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Database.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <tuple>

std::tuple<bool, bool> Dialog_MovePicture::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###MovePicture");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 60) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 18) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###MovePicture", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    drawPickers();
    ImGui::BeginVertical("##move_picture_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      GroupBox pictureGroupBox(trNOOP("Picture Number"), "##move_picture_picture_group", {-1, 0});
      if (pictureGroupBox.begin()) {
        ImGui::SetNextItemWidth(-1);
        if (ImGui::SpinInt("##move_picture_picture_id", &m_number)) {
          m_number = std::clamp(m_number, 1, 999);
        }
      }
      pictureGroupBox.end();
      ImGui::BeginHorizontal("##move_picture_inner", {-1, 0});
      {
        GroupBox positionGroupBox(trNOOP("Position And Scale"), "##move_picture_position_group", {ImGui::GetContentRegionAvail().x * .75f, 0});
        if (positionGroupBox.begin()) {
          ImGui::BeginVertical("##show_picture_position_layout", {-1, 0});
          {
            GroupBox originGroup(trNOOP("Origin"), "##show_picture_origin", {-1, 0});
            if (originGroup.begin()) {
              ImGui::SetNextItemWidth(-1.f);
              if (ImGui::BeginCombo("##show_picture_origin", DecodeEnumName(magic_enum::enum_value<PictureOrigin>(m_origin)).c_str())) {
                for (auto origin : magic_enum::enum_values<PictureOrigin>()) {
                  bool selected = m_origin == static_cast<int>(origin);
                  if (ImGui::Selectable(DecodeEnumName(origin).c_str(), selected)) {
                    m_origin = static_cast<int>(origin);
                  }
                  if (selected) {
                    ImGui::SetItemDefaultFocus();
                  }
                }
                ImGui::EndCombo();
              }
            }
            originGroup.end();
            ImGui::RadioButton(trNOOP("Direct designation"), &m_type, 0);
            ImGui::BeginHorizontal("##show_picture_position_direct_desig_layout", {-1, 0});
            {
              ImGui::Dummy({ImGui::GetFrameHeightWithSpacing(), 0});
              ImGui::BeginHorizontal("##show_picture_position_direct_desig_right_layout", {-1, 0});
              {
                ImGui::BeginDisabled(m_type != 0);
                {

                  GroupBox directDesigXGroup(trNOOP("X"), "##show_picture_direct_desig_x_group", {ImGui::GetContentRegionAvail().x * 0.5f, 0});
                  if (directDesigXGroup.begin()) {
                    ImGui::SetNextItemWidth(-1.f);
                    ImGui::SpinInt("##show_picture_direct_desig_x", &m_xConstant, 1, 100, m_type == 0 ? nullptr : "");
                  }
                  directDesigXGroup.end();
                  GroupBox directDesigYGroup(trNOOP("Y"), "##show_picture_direct_desig_y_group", {ImGui::GetContentRegionAvail().x, 0});
                  if (directDesigYGroup.begin()) {
                    ImGui::SetNextItemWidth(-1.f);
                    ImGui::SpinInt("##show_picture_direct_desig_y", &m_yConstant, 1, 100, m_type == 0 ? nullptr : "");
                  }
                  directDesigYGroup.end();
                }
                ImGui::EndDisabled();
              }
              ImGui::EndHorizontal();
            }
            ImGui::EndHorizontal();
            ImGui::RadioButton(trNOOP("Designation with variables"), &m_type, 1);
            ImGui::BeginHorizontal("##show_picture_position_var_desig_layout", {-1, 0});
            {
              ImGui::Dummy({ImGui::GetFrameHeightWithSpacing(), 0});
              ImGui::BeginHorizontal("##show_picture_position_var_desig_right_layout", {-1, 0});
              {
                ImGui::BeginDisabled(m_type != 1);
                {

                  GroupBox varDesigXGroup(trNOOP("X"), "##show_picture_var_desig_x_group", {ImGui::GetContentRegionAvail().x * 0.5f, 0});
                  if (varDesigXGroup.begin()) {
                    ImGui::SetNextItemWidth(-1.f);
                    if (ImGui::EllipsesButton(std::format("{}##show_picture_direct_var_x", m_type == 1 ? Database::instance()->variableNameAndId(m_xVariable) : "").c_str(), {-1, 0})) {
                      m_isPickingY = false;
                      m_varPicker.emplace(trNOOP("Variables"), Database::instance()->system.variables(), m_xVariable);
                      m_varPicker->setOpen(true);
                    }
                  }
                  varDesigXGroup.end();
                  GroupBox varDesigYGroup(trNOOP("Y"), "##show_picture_var_desig_y_group", {ImGui::GetContentRegionAvail().x, 0});
                  if (varDesigYGroup.begin()) {
                    ImGui::SetNextItemWidth(-1.f);
                    if (ImGui::EllipsesButton(std::format("{}##show_picture_direct_var_y", m_type == 1 ? Database::instance()->variableNameAndId(m_yVariable) : "").c_str(), {-1, 0})) {
                      m_isPickingY = true;
                      m_varPicker.emplace(trNOOP("Variables"), Database::instance()->system.variables(), m_yVariable);
                      m_varPicker->setOpen(true);
                    }
                  }
                  varDesigYGroup.end();
                }
                ImGui::EndDisabled();
              }
              ImGui::EndHorizontal();
            }
            ImGui::EndHorizontal();
          }
          ImGui::EndVertical();
        }
        positionGroupBox.end();
        ImGui::BeginVertical("##move_picture_inner_inner", {-1, 0});
        {
          GroupBox scaleGroupBox(trNOOP("Scale"), "##move_picture_scale_group", {-1, 0});
          if (scaleGroupBox.begin()) {
            GroupBox widthGroupBox(trNOOP("Width"), "##move_picture_scale_width_group", {-1, 0});
            if (widthGroupBox.begin()) {
              ImGui::SetNextItemWidth(-1.f);
              if (ImGui::SpinInt("##move_picture_scale_width", &m_zoomX, 1, 100, "%d %%")) {
                m_zoomX = std::clamp(m_zoomX, -2000, 2000);
              }
            }
            widthGroupBox.end();
            GroupBox heightGroupBox(trNOOP("Height"), "##move_picture_scale_height_group", {-1, 0});
            if (heightGroupBox.begin()) {
              ImGui::SetNextItemWidth(-1.f);
              if (ImGui::SpinInt("##move_picture_scale_height", &m_zoomY, 1, 100, "%d %%")) {
                m_zoomY = std::clamp(m_zoomY, -2000, 2000);
              }
            }
            heightGroupBox.end();
          }
          scaleGroupBox.end();
          GroupBox blendGroupBox(trNOOP("Blend"), "##move_picture_blend_group", {-1, 0});
          if (blendGroupBox.begin()) {
            GroupBox opacityGroup(trNOOP("Opacity"), "##move_picture_blend_opacity_group", {-1, 0});
            if (opacityGroup.begin()) {
              ImGui::SetNextItemWidth(-1);
              if (ImGui::SpinInt("##move_picture_blend_opacity", &m_opacityValue, 1, 100)) {
                m_opacityValue = std::clamp(m_opacityValue, 0, 255);
              }
            }
            opacityGroup.end();
            GroupBox modeGroup(trNOOP("Mode"), "##move_picture_blend_mode_group", {-1, 0});
            if (modeGroup.begin()) {
              ImGui::SetNextItemWidth(-1);
              if (ImGui::BeginCombo("##move_picture_blend_mode", DecodeEnumName(static_cast<Blend>(m_blendMode)).c_str())) {
                for (auto& blend : magic_enum::enum_values<Blend>()) {
                  bool selected = m_blendMode == static_cast<int>(blend);
                  if (ImGui::Selectable(DecodeEnumName(blend).c_str(), selected)) {
                    m_blendMode = magic_enum::enum_index(blend).value();
                  }
                  if (selected) {
                    ImGui::SetItemDefaultFocus();
                  }
                }
                ImGui::EndCombo();
              }
            }
            modeGroup.end();
          }
          blendGroupBox.end();
        }
        ImGui::EndVertical();
      }
      ImGui::EndHorizontal();
      GroupBox durationGroupBox(trNOOP("Duration"), "##move_picture_duration_group", {-1, 0});
      if (durationGroupBox.begin()) {
        ImGui::BeginHorizontal("##move_picture_duration_layout", {-1, 0});
        {
          // We need to calculate the spinbox size based on the text size for the following checkbox, this allows us to dynamically accommodate languages
          const auto checkboxLabel = trNOOP("Wait for Completion");
          const auto width = ImGui::GetContentRegionAvail().x - (ImGui::CalcTextSize(checkboxLabel).x + ImGui::GetFrameHeightWithSpacing() + (ImGui::GetStyle().FramePadding.x * 2));
          ImGui::BeginChild("##move_picture_duration", {0, 0}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize,
                            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NavFlattened);
          {
            ImGui::SetNextItemWidth(width);
            if (ImGui::SpinInt("##move_picture_duration", &m_duration, 1, 100, trNOOP("%d frames (1/60 sec)"))) {
              m_duration = std::clamp(m_duration, 1, 999);
            }
          }
          ImGui::EndChild();
          ImGui::Checkbox(checkboxLabel, &m_waitForCompletion);
        }
        ImGui::EndHorizontal();
      }
      durationGroupBox.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##move_picture_button_layout", {-1, 0}, 0);
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##move_picture_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->picture = m_number;
          m_command->origin = static_cast<PictureOrigin>(m_origin);
          m_command->pictureLocation = static_cast<PictureDesignationSource>(m_type);
          if (m_command->pictureLocation == PictureDesignationSource::Direct_designation) {
            m_command->x = m_xConstant;
            m_command->y = m_yConstant;
          } else {
            m_command->x = m_xVariable;
            m_command->y = m_yVariable;
          }
          m_command->width = m_zoomX;
          m_command->height = m_zoomY;
          m_command->opacity = m_opacityValue;
          m_command->blendMode = static_cast<Blend>(m_blendMode);
          m_command->duration = m_duration;
          m_command->waitForCompletion = m_waitForCompletion;
          ImGui::CloseCurrentPopup();
          setOpen(false);
        } else if (ret == 1) {
          ImGui::CloseCurrentPopup();
          setOpen(false);
        }
      }
      ImGui::EndHorizontal();
    }
    ImGui::EndVertical();

    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}

void Dialog_MovePicture::drawPickers() {
  if (m_varPicker) {
    if (const auto [closed, confirmed] = m_varPicker->draw(); closed) {
      if (confirmed) {
        if (m_isPickingY)
          m_yVariable = m_varPicker->selection();
        else
          m_xVariable = m_varPicker->selection();
      }
      m_varPicker.reset();
    }
  }
}