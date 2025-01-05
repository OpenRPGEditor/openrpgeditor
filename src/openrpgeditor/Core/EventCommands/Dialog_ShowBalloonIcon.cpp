#include "Core/EventCommands/Dialog_ShowBalloonIcon.hpp"


#include "Database/Database.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_ShowBalloonIcon::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{183, 162}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    ImGui::SeparatorText("Character");
    ImGui::PushItemWidth((160));
    if (ImGui::BeginCombo("##showanim_character", Database::instance()->eventNameOrId(m_character).c_str())) {

      if (ImGui::Selectable("Player", m_character == -1)) {
        m_character = -1;
        ImGui::SetItemDefaultFocus();
      }
      if (ImGui::Selectable("This Event", m_character == 0)) {
        m_character = 0;
        ImGui::SetItemDefaultFocus();
      }

      for (auto& dataSource : Database::instance()->mapInfos.currentMap()->map()->events()) {
        if (!dataSource.has_value())
          continue;

        bool is_selected = (m_character == dataSource->id());
        if (ImGui::Selectable(("EV" + std::format("{:03} ", dataSource->id())).c_str(), is_selected)) {
          m_character = dataSource->id();
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    ImGui::SeparatorText("Balloon Icon");
    ImGui::PushItemWidth((160));
    if (ImGui::BeginCombo("##balloon_selection", DecodeEnumName(magic_enum::enum_value<BalloonIcon>(m_balloonIndex)).c_str())) {
      for (auto& balloons : magic_enum::enum_values<BalloonIcon>()) {
        bool is_selected = m_balloonIndex == magic_enum::enum_index(balloons).value();
        if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(balloons)).c_str(), is_selected)) {
          m_balloonIndex = magic_enum::enum_index(balloons).value();
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    // Wait for completion
    ImGui::Checkbox("Wait for Completion", &m_waitCompletion);

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->id = m_character;
      command->index = static_cast<BalloonIcon>(m_balloonIndex);
      command->waitForCompletion = m_waitCompletion;
      ImGui::CloseCurrentPopup();
      setOpen(false);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
      setOpen(false);
    }
    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
