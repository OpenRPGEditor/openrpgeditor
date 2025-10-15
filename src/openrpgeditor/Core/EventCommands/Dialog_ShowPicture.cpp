#include "Core/EventCommands/Dialog_ShowPicture.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Database.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <tuple>

std::tuple<bool, bool> Dialog_ShowPicture::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ShowPicture");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 60) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 17) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###ShowPicture", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    drawPickers();
    ImGui::BeginVertical("##show_picture_main_layout", ImGui::GetContentRegionAvail(), 0);
    {

      GroupBox pictureGroup(trNOOP("Picture"), "##show_picture_image_group", {-1, 0});
      if (pictureGroup.begin()) {
        GroupBox numberGroupBox(trNOOP("Number"), "##show_picture_number", {ImGui::GetContentRegionAvail().x * 0.25f, 0});
        if (numberGroupBox.begin()) {
          ImGui::SetNextItemWidth(-1.f);
          if (ImGui::SpinInt("##show_picture_number_input", &m_number, 1, 100)) {
            m_number = std::clamp(m_number, 1, 999);
          }
        }
        numberGroupBox.end();
        ImGui::SameLine();
        GroupBox imageGroupBox(trNOOP("Image"), "##show_picture_image", {-1, 0});
        if (imageGroupBox.begin()) {
          if (ImGui::EllipsesButton(std::format("{}##show_picture_image_button", Database::imageText(m_imageName)).c_str(), {-1, 0})) {
            m_imagePicker->setImageInfo(m_imageName);
            m_imagePicker->setOpen(true);
          }
        }
        imageGroupBox.end();
      }
      pictureGroup.end();
      ImGui::BeginHorizontal("##show_picture_inner_layout", {-1, 0}, 0);
      {
        GroupBox positionGroup(trNOOP("Position"), "##show_picture_position", {ImGui::GetContentRegionAvail().x * .75f, 0});
        if (positionGroup.begin()) {
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
                      m_varPicker.emplace(VariableSwitchPicker::Type::Variable, Database::instance()->system->variables(), m_xVariable);
                      m_varPicker->setOpen(true);
                    }
                  }
                  varDesigXGroup.end();
                  GroupBox varDesigYGroup(trNOOP("Y"), "##show_picture_var_desig_y_group", {ImGui::GetContentRegionAvail().x, 0});
                  if (varDesigYGroup.begin()) {
                    ImGui::SetNextItemWidth(-1.f);
                    if (ImGui::EllipsesButton(std::format("{}##show_picture_direct_var_y", m_type == 1 ? Database::instance()->variableNameAndId(m_yVariable) : "").c_str(), {-1, 0})) {
                      m_isPickingY = true;
                      m_varPicker.emplace(VariableSwitchPicker::Type::Variable, Database::instance()->system->variables(), m_yVariable);
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
        positionGroup.end();
        ImGui::BeginVertical("##show_picture_inner_inner", {-1, 0});
        {
          GroupBox scaleGroupBox(trNOOP("Scale"), "##show_picture_scale_group", {-1, 0});
          if (scaleGroupBox.begin()) {
            GroupBox widthGroupBox(trNOOP("Width"), "##show_picture_scale_width_group", {-1, 0});
            if (widthGroupBox.begin()) {
              ImGui::SetNextItemWidth(-1.f);
              if (ImGui::SpinInt("##show_picture_scale_width", &m_zoomX, 1, 100, "%d %%")) {
                m_zoomX = std::clamp(m_zoomX, -2000, 2000);
              }
            }
            widthGroupBox.end();
            GroupBox heightGroupBox(trNOOP("Height"), "##show_picture_scale_height_group", {-1, 0});
            if (heightGroupBox.begin()) {
              ImGui::SetNextItemWidth(-1.f);
              if (ImGui::SpinInt("##show_picture_scale_height", &m_zoomY, 1, 100, "%d %%")) {
                m_zoomY = std::clamp(m_zoomY, -2000, 2000);
              }
            }
            heightGroupBox.end();
          }
          scaleGroupBox.end();
          GroupBox blendGroupBox(trNOOP("Blend"), "##show_picture_blend_group", {-1, 0});
          if (blendGroupBox.begin()) {
            GroupBox opacityGroup(trNOOP("Opacity"), "##show_picture_blend_opacity_group", {-1, 0});
            if (opacityGroup.begin()) {
              ImGui::SetNextItemWidth(-1);
              if (ImGui::SpinInt("##show_picture_blend_opacity", &m_opacityValue, 1, 100)) {
                m_opacityValue = std::clamp(m_opacityValue, 0, 255);
              }
            }
            opacityGroup.end();
            GroupBox modeGroup(trNOOP("Mode"), "##show_picture_blend_mode_group", {-1, 0});
            if (modeGroup.begin()) {
              ImGui::SetNextItemWidth(-1);
              if (ImGui::BeginCombo("##show_picture_blend_mode", DecodeEnumName(static_cast<Blend>(m_blendMode)).c_str())) {
                for (auto& blend : magic_enum::enum_values<Blend>()) {
                  bool selected = m_blendMode == static_cast<int>(blend);
                  if (ImGui::Selectable(DecodeEnumName(blend).c_str(), selected)) {
                    m_blendMode = static_cast<int>(blend);
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

      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##show_picture_button_layout", {-1, 0}, 0);
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##show_picture_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->number = m_number;
          m_command->imageName = m_imageName;
          m_command->origin = static_cast<PictureOrigin>(m_origin);
          m_command->type = static_cast<PictureDesignationSource>(m_type);
          if (m_command->type == PictureDesignationSource::Direct_designation) {
            m_command->value1 = m_xConstant;
            m_command->value2 = m_yConstant;
          } else {
            m_command->value1 = m_xVariable;
            m_command->value2 = m_yVariable;
          }
          m_command->zoomX = m_zoomX;
          m_command->zoomY = m_zoomY;
          m_command->opacityValue = m_opacityValue;
          m_command->blendMode = static_cast<Blend>(m_blendMode);
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

void Dialog_ShowPicture::drawPickers() {
  if (m_varPicker) {
    if (const auto [closed, confirmed] = m_varPicker->draw(); closed) {
      if (confirmed) {
        if (m_isPickingY) {
          m_yVariable = m_varPicker->selection();
        } else {
          m_xVariable = m_varPicker->selection();
        }
      }
      m_varPicker.reset();
    }
  }

  if (const auto [closed, confirmed] = m_imagePicker->draw(); closed) {
    if (confirmed) {
      m_imagePicker->accept();
      m_imageName = m_imagePicker->selectedImage();
    }
  }
}