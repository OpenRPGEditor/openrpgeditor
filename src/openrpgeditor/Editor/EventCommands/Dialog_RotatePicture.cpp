#include "Editor/EventCommands/Dialog_RotatePicture.hpp"

#include "Database/Database.hpp"
#include "Editor/CommonUI/GroupBox.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <tuple>

std::tuple<bool, bool> Dialog_RotatePicture::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###RotatePicture");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 32) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 7) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###RotatePicture", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::BeginVertical("##rotate_picture_main_layout", ImGui::GetContentRegionAvail(), 0.f);
    {
      GroupBox pictureGroup(trNOOP("Picture Number"), "##rotate_picture_picture_number_group", {-1, 0});
      if (pictureGroup.begin()) {
        ImGui::SetNextItemWidth(-1);
        if (ImGui::SpinInt("##rotate_picture_picture_number", &m_picture)) {
          m_picture = std::clamp(m_picture, 1, 999);
        }
      }
      pictureGroup.end();
      GroupBox rotationGroup(trNOOP("Rotation"), "##rotate_picture_rotation_group", {-1, 0});
      if (rotationGroup.begin()) {
        ImGui::SetNextItemWidth(-1);
        if (ImGui::SpinInt("##rotate_picture_rotation", &m_rotation)) {
          m_rotation = std::clamp(m_rotation, 0, 90);
        }
      }
      rotationGroup.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##rotate_picture_button_layout");
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##rotate_picture_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;

          m_command->picture = m_picture;
          m_command->rotation = m_rotation;

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
