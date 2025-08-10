#include "Core/EventCommands/MovementRoute/Dialog_MovementChangeFrequency.hpp"
#include "Core/Application.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <tuple>

std::tuple<bool, bool> Dialog_MovementChangeFrequency::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###MovementChangeFrequency");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 20) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 5) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###MovementChangeFrequency", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::BeginVertical("##movement_change_frequency_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      GroupBox speedGroup(trNOOP("Frequency"), "##movement_change_frequency_speed_group", {-1, 0});
      if (speedGroup.begin()) {
        ImGui::SetNextItemWidth(-1);

        if (ImGui::BeginCombo("##movement_frequency_selection", DecodeEnumName(static_cast<MovementFrequency>(m_frequency)).c_str())) {
          for (auto& frequency : magic_enum::enum_values<MovementFrequency>()) {
            const auto selected = (m_frequency == static_cast<int>(frequency));
            if (ImGui::Selectable(DecodeEnumName(frequency).c_str(), selected)) {
              m_frequency = static_cast<int>(frequency);
            }
            if (selected) {
              ImGui::SetItemDefaultFocus();
            }
          }
          ImGui::EndCombo();
        }
      }
      speedGroup.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##movement_change_frequency_button_layout", {-1, 0}, 0);
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##movement_change_frequency_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->frequency = m_frequency;
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
