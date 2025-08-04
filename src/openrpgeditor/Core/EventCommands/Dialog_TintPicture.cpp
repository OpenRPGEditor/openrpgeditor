#include "Core/EventCommands/Dialog_TintPicture.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Database.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <tuple>

std::tuple<bool, bool> Dialog_TintPicture::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###TintPicture");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 60) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 19) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###TintPicture", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::BeginVertical("##tint_picture_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      GroupBox pictureGroupBox(trNOOP("Picture"), "##tint_picture_picture_group", {-1, 0});
      if (pictureGroupBox.begin()) {
        ImGui::SetNextItemWidth(-1);
        if (ImGui::SpinInt("##tint_picture_picture_id", &m_picture, 1, 100)) {
          m_picture = std::clamp(m_picture, 1, 999);
        }
      }
      pictureGroupBox.end();
      GroupBox colorToneGroupBox(trNOOP("Color Tone"), "##tint_picture_color_tone_group", {-1, 0});
      if (colorToneGroupBox.begin()) {
        m_colorPicker.draw();
      }
      colorToneGroupBox.end();
      GroupBox durationGroupBox(trNOOP("Duration"), "##tint_picture_duration_group", {-1, 0});
      if (durationGroupBox.begin()) {
        ImGui::BeginHorizontal("##move_picture_duration_layout", {-1, 0});
        {
          // We need to calculate the spinbox size based on the text size for the following checkbox, this allows us to dynamically accommodate languages
          const auto checkboxLabel = trNOOP("Wait for Completion");
          const auto width = ImGui::GetContentRegionAvail().x - (ImGui::CalcTextSize(checkboxLabel).x + ImGui::GetFrameHeightWithSpacing() + (ImGui::GetStyle().FramePadding.x * 2));
          ImGui::BeginChild("##tint_picture_duration", {0, 0}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize,
                            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NavFlattened);
          {
            ImGui::SetNextItemWidth(width);
            if (ImGui::SpinInt("##tint_picture_duration", &m_duration, 1, 100, trNOOP("%d frames (1/60 sec)"))) {
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
      ImGui::BeginHorizontal("##tint_picture_button_layout", {-1, 0}, 0);
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##tint_picture_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_command->color.r = m_colorPicker.r();
          m_command->color.g = m_colorPicker.g();
          m_command->color.b = m_colorPicker.b();
          m_command->color.gray = m_colorPicker.gray();
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
