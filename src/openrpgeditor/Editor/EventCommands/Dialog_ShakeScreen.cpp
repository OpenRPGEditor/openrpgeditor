#include "Editor/EventCommands/Dialog_ShakeScreen.hpp"

#include "Database/Database.hpp"
#include "Editor/CommonUI/GroupBox.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <tuple>

std::tuple<bool, bool> Dialog_ShakeScreen::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ShakeScreen");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 40) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 7) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###ShakeScreen", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::BeginVertical("###shake_screen_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      ImGui::BeginHorizontal("##shake_screen_inner_layout");
      {
        GroupBox powerGroup(trNOOP("Power"), "##shake_screen_power", {ImGui::GetContentRegionAvail().x * .50f, 0});
        if (powerGroup.begin()) {
          ImGui::SetNextItemWidth(-1);
          if (ImGui::SliderInt("##shake_screen_power_int", &m_power, 1, 9)) {
            m_power = std::clamp(m_power, 1, 9);
          }
        }
        powerGroup.end();
        GroupBox speedGroup(trNOOP("Speed"), "##shake_screen_speed", {ImGui::GetContentRegionAvail().x, 0});
        if (speedGroup.begin()) {
          ImGui::SetNextItemWidth(-1);
          if (ImGui::SliderInt("##shake_screen_speed_int", &m_speed, 1, 9)) {
            m_speed = std::clamp(m_speed, 1, 9);
          }
        }
        speedGroup.end();
      }
      ImGui::EndHorizontal();
      GroupBox durationGroupBox(trNOOP("Duration"), "##shake_screen_duration_group", {-1, 0});
      if (durationGroupBox.begin()) {
        ImGui::BeginHorizontal("##move_picture_duration_layout", {-1, 0});
        {
          // We need to calculate the spinbox size based on the text size for the following checkbox, this allows us to dynamically accommodate languages
          const auto checkboxLabel = trNOOP("Wait for Completion");
          const auto width = ImGui::GetContentRegionAvail().x - (ImGui::CalcTextSize(checkboxLabel).x + ImGui::GetFrameHeightWithSpacing() + (ImGui::GetStyle().FramePadding.x * 2));
          ImGui::BeginChild("##shake_screen_duration", {0, 0}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize,
                            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NavFlattened);
          {
            ImGui::SetNextItemWidth(width);
            if (ImGui::SpinInt("##shake_screen_duration", &m_duration, 1, 100, trNOOP("%d frames (1/60 sec)"))) {
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
      ImGui::BeginHorizontal("##shake_screen_button_layout");
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##shake_screen_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->power = m_power;
          m_command->speed = m_speed;
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
