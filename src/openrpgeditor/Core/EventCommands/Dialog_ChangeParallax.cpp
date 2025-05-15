#include "Core/EventCommands/Dialog_ChangeParallax.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Database.hpp"
#include "imgui.h"
#include "imgui_internal.h"

#include <tuple>

std::tuple<bool, bool> Dialog_ChangeParallax::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  const auto maxSize =
      ImVec2{ImGui::CalcTextSize("#############################").x + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 10) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    if (m_imagePicker) {
      if (const auto [closed, confirmed] = m_imagePicker->draw(); closed) {
        if (confirmed) {
          m_imagePicker->accept();
          m_image = m_imagePicker->selectedImage();
        }
      }
    }

    ImGui::BeginVertical("##change_parallax_main_layout", ImGui::GetContentRegionAvail());
    {
      // Parallax Background Image Group
      GroupBox operationGroupBox(trNOOP("Parallax Background Image"), "##change_parallax_layout", {-1, 0}, nullptr,
                                 ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (operationGroupBox.begin()) {
        ImGui::BeginHorizontal("##change_parallax_group_horizontal", {ImGui::GetContentRegionAvail().x, -1});
        {
          ImGui::Spring(.5);
          ImGui::PushID("#parallax_image_selection");
          if (ImGui::EllipsesButton(m_image.c_str(), ImVec2{-1, 0})) {
            if (!m_imagePicker) {
              m_imagePicker.emplace(ImagePicker::PickerMode::Parallax, m_image);
            }
            m_imagePicker->setImageInfo(m_image);
            m_imagePicker->setOpen(true);
          }
          ImGui::PopID();
          ImGui::Spring(.5);
        }
        ImGui::EndHorizontal();
      }
      operationGroupBox.end();
      // Loop Horizontally Group
      GroupBox loopHorzGroup(trNOOP("Loop Horizontally"), "##loop_horz_layout", {-1, 0}, &m_loopHorizontally,
                             ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (loopHorzGroup.begin()) {
        ImGui::BeginHorizontal("##loop_horizontal", {ImGui::GetContentRegionAvail().x, -1});
        {
          ImGui::Spring(.5);
          ImGui::BeginChild("##loop_horz_child", {-1, 0}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_NavFlattened,
                            ImGuiWindowFlags_NoBackground);
          {
            ImGui::SetNextItemWidth(-1);
            ImGui::BeginDisabled(!m_loopHorizontally);
            if (ImGui::SpinInt("##parallax_x", &m_scrollX, 1, 100, m_loopHorizontally ? nullptr : "")) {
              m_scrollX = std::clamp(m_scrollX, -32, 32);
            }
            ImGui::EndDisabled();
          }
          ImGui::EndChild();
          ImGui::Spring(.5);
        }
        ImGui::EndHorizontal();
      }
      loopHorzGroup.end();
      // Loop Vertically Group
      GroupBox loopVertGroup(trNOOP("Loop Vertically"), "##loop_vert_layout", {-1, 0}, &m_loopVertically, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (loopVertGroup.begin()) {
        ImGui::BeginHorizontal("##loop_vertical", {ImGui::GetContentRegionAvail().x, -1});
        {
          ImGui::Spring(.5);
          ImGui::BeginChild("##loop_vert_child", {-1, 0}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_NavFlattened,
                            ImGuiWindowFlags_NoBackground);
          {
            ImGui::BeginDisabled(!m_loopVertically);
            ImGui::SetNextItemWidth(-1);
            if (ImGui::SpinInt("##parallax_y", &m_scrollY, 1, 100, m_loopVertically ? nullptr : "")) {
              m_scrollY = std::clamp(m_scrollY, -32, 32);
            }
            ImGui::EndDisabled();
          }
          ImGui::EndChild();
          ImGui::Spring(.5);
        }
        ImGui::EndHorizontal();
      }
      loopVertGroup.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5));
      ImGui::BeginHorizontal("##change_tileset_dialog_buttons", {-1, -1});
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##change_tileset_dialog_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {

          m_confirmed = true;
          command->image = m_image;
          command->loopVertically = m_loopVertically;
          command->loopHorizontally = m_loopHorizontally;
          command->scrollX = m_scrollX;
          command->scrollY = m_scrollY;
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
