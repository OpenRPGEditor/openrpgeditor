#include "Editor/EventCommands/Dialog_ScrollMap.hpp"

#include "Database/Database.hpp"
#include "Editor/CommonUI/GroupBox.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"
#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>

std::tuple<bool, bool> Dialog_ScrollMap::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ScrollMap");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 60) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 5) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});
  if (ImGui::BeginPopupModal(std::format("{}###ScrollMap", m_name).c_str(), &m_open, ImGuiWindowFlags_NoResize)) {
    ImGui::BeginVertical("##scroll_map_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      ImGui::BeginHorizontal("##scroll_map_inner_layout", {-1, 0}, 0);
      {
        GroupBox directionGroup(trNOOP("Direction"), "##scroll_map_direction_group", {ImGui::GetContentRegionAvail().x * .33f, 0.f});
        if (directionGroup.begin()) {
          ImGui::SetNextItemWidth(-1);
          if (ImGui::BeginCombo("##scroll_map_direction", DecodeEnumName(static_cast<Direction>(m_direction)).c_str())) {
            for (auto& direction : magic_enum::enum_values<Direction>()) {
              if (direction == Direction::Retain) {
                continue;
              }

              bool selected = m_direction == static_cast<int>(direction);
              if (ImGui::Selectable(DecodeEnumName(direction).c_str(), selected)) {
                m_direction = static_cast<int>(direction);
              }
              if (selected) {
                ImGui::SetItemDefaultFocus();
              }
            }
            ImGui::EndCombo();
          }
        }
        directionGroup.end();
        GroupBox distanceGroup(trNOOP("Distance"), "##scroll_map_distance_group", {ImGui::GetContentRegionAvail().x * 0.5f, 0.f});
        if (distanceGroup.begin()) {
          ImGui::SetNextItemWidth(-1);
          if (ImGui::SpinInt("##scroll_map_distance", &m_distance)) {
            m_distance = std::clamp(m_distance, 1, 100);
          }
        }
        distanceGroup.end();
        GroupBox speedGroup(trNOOP("Speed"), "##scroll_map_speed_group", {-1.f, 0.f});
        if (speedGroup.begin()) {
          ImGui::SetNextItemWidth(-1);

          if (ImGui::BeginCombo("##scrollmap_speed", DecodeEnumName(static_cast<MovementSpeed>(m_speed)).c_str())) {
            for (auto& speed : magic_enum::enum_values<MovementSpeed>()) {
              const auto selected = m_speed == magic_enum::enum_integer(speed);
              if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(speed)).c_str(), selected)) {
                m_speed = magic_enum::enum_integer(speed);
              }
              if (selected)
                ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
          }
        }
        speedGroup.end();
      }
      ImGui::EndHorizontal();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##scroll_map_button_layout", {-1, 0}, 0);
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##scroll_map_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->direction = static_cast<Direction>(m_direction);
          m_command->distance = m_distance;
          m_command->speed = static_cast<MovementSpeed>(m_speed);

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
