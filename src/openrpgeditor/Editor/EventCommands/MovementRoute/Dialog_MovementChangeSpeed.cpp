#include "Editor/EventCommands/MovementRoute/Dialog_MovementChangeSpeed.hpp"

#include "Database/Database.hpp"
#include "Editor/CommonUI/GroupBox.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <tuple>

std::tuple<bool, bool> Dialog_MovementChangeSpeed::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###MovementChangeSpeed");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 20) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 5) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###MovementChangeSpeed", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::BeginVertical("##movement_change_speed_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      GroupBox speedGroup(trNOOP("Speed"), "##movement_change_speed_speed_group", {-1, 0});
      if (speedGroup.begin()) {
        ImGui::SetNextItemWidth(-1);

        if (ImGui::BeginCombo("##movement_frequency_selection", DecodeEnumName(static_cast<MovementSpeed>(m_speed)).c_str())) {
          for (auto& speed : magic_enum::enum_values<MovementSpeed>()) {
            const auto selected = (m_speed == static_cast<int>(speed));
            if (ImGui::Selectable(DecodeEnumName(speed).c_str(), selected)) {
              m_speed = static_cast<int>(speed);
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
      ImGui::BeginHorizontal("##movement_change_speed_button_layout", {-1, 0}, 0);
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##movement_change_speed_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->speed = m_speed;
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
