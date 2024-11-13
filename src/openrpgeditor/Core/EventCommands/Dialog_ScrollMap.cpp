#include "Core/EventCommands/Dialog_ScrollMap.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"

std::tuple<bool, bool> Dialog_ScrollMap::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{200, 140} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open,
                             ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_AlwaysAutoResize)) {

    ImGui::SeparatorText("Direction                Distance");

    ImGui::PushItemWidth(App::DPIHandler::scale_value(ImGui::GetContentRegionMax().x / 2));

    auto dir = magic_enum::enum_cast<Direction>(m_direction);
    if (ImGui::BeginCombo("##scrollmap_direction", DecodeEnumName(magic_enum::enum_name(dir.value())).c_str())) {
      for (auto& direction : magic_enum::enum_values<Direction>()) {
        if (magic_enum::enum_integer(direction) == 0)
          continue;

        bool is_selected = m_direction == magic_enum::enum_integer(direction);
        if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(direction)).c_str(), is_selected)) {
          m_direction = magic_enum::enum_integer(direction);
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }
    ImGui::SameLine();
    ImGui::SetNextItemWidth(App::DPIHandler::scale_value(80));
    if (ImGui::InputInt("##scrollmap_distance", &m_distance)) {
      if (m_distance > 100) {
        m_distance = 100;
      } else if (m_distance < 1)
        m_distance = 1;
    }

    ImGui::SeparatorText("Speed");
    ImGui::PushItemWidth(App::DPIHandler::scale_value(180));

    if (ImGui::BeginCombo("##scrollmap_speed", DecodeEnumName(static_cast<MovementSpeed>(m_speed)).c_str())) {
      for (auto& speed : magic_enum::enum_values<MovementSpeed>()) {

        bool is_selected = m_speed == magic_enum::enum_integer(speed);
        if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(speed)).c_str(), is_selected)) {
          m_speed = magic_enum::enum_integer(speed);
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->direction = static_cast<Direction>(m_direction);
      command->distance = m_distance;
      command->speed = static_cast<MovementSpeed>(m_speed);

      ImGui::CloseCurrentPopup();
      SetOpen(false);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
      SetOpen(false);
    }
    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
