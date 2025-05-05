#include "Core/EventCommands/Dialog_ChangeWindowColor.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "imgui.h"
#include "imgui_internal.h"

#include <tuple>

std::tuple<bool, bool> Dialog_ChangeWindowColor::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 60) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 10) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar)) {
    ImGui::BeginVertical("##change_window_color_main_layout", ImGui::GetContentRegionAvail());
    {
      m_colorPicker.draw({ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, (ImGui::GetContentRegionAvail().y * .8f)});
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5));
      ImGui::BeginHorizontal("##change_window_color_dialog_buttons", {-1, -1});
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##change_window_color_dialog_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          command->r = m_colorPicker.r();
          command->g = m_colorPicker.g();
          command->b = m_colorPicker.b();
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
