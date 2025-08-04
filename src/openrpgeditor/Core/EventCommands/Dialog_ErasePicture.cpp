#include "Core/EventCommands/Dialog_ErasePicture.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Database.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <tuple>

std::tuple<bool, bool> Dialog_ErasePicture::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ErasePicture");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 32) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 5) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###ErasePicture", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::BeginVertical("##erase_picture_main_layout", ImGui::GetContentRegionAvail(), 0.f);
    {
      GroupBox pictureGroup(trNOOP("Picture Number"), "##erase_picture_picture_number_group", {-1, 0});
      if (pictureGroup.begin()) {
        ImGui::SetNextItemWidth(-1);
        if (ImGui::SpinInt("##erase_picture_picture_number", &m_picture)) {
          m_picture = std::clamp(m_picture, 1, 999);
        }
      }
      pictureGroup.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##erase_picture_button_layout");
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##erase_picture_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;

          m_command->picture = m_picture;

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
