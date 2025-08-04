#include "Core/EventCommands/Dialog_FlashScreen.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Database.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <tuple>
std::tuple<bool, bool> Dialog_FlashScreen::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###TintPicture");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 60) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 17) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###TintPicture", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::BeginVertical("##flash_screen_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      GroupBox colorToneGroupBox(trNOOP("Flash Color"), "##flash_screen_color_color_group", {-1, 0});
      if (colorToneGroupBox.begin()) {
        m_colorPicker.draw();
      }
      colorToneGroupBox.end();
      GroupBox durationGroupBox(trNOOP("Duration"), "##flash_screen_duration_group", {-1, 0});
      if (durationGroupBox.begin()) {
        ImGui::BeginHorizontal("##move_picture_duration_layout", {-1, 0});
        {
          // We need to calculate the spinbox size based on the text size for the following checkbox, this allows us to dynamically accommodate languages
          const auto checkboxLabel = trNOOP("Wait for Completion");
          const auto width = ImGui::GetContentRegionAvail().x - (ImGui::CalcTextSize(checkboxLabel).x + ImGui::GetFrameHeightWithSpacing() + (ImGui::GetStyle().FramePadding.x * 2));
          ImGui::BeginChild("##flash_screen_duration", {0, 0}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize,
                            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NavFlattened);
          {
            ImGui::SetNextItemWidth(width);
            if (ImGui::SpinInt("##flash_screen_duration", &m_duration, 1, 100, trNOOP("%d frames (1/60 sec)"))) {
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
      ImGui::BeginHorizontal("##flash_screen_button_layout", {-1, 0}, 0);
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##flash_screen_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_command->color.r = m_colorPicker.r();
          m_command->color.g = m_colorPicker.g();
          m_command->color.b = m_colorPicker.b();
          m_command->color.intensity = m_colorPicker.intensity();
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
