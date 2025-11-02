#include "Editor/EventCommands/Dialog_ShowBalloonIcon.hpp"

#include "Database/Database.hpp"
#include "Editor/CommonUI/GroupBox.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"
#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>
std::tuple<bool, bool> Dialog_ShowBalloonIcon::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ShowBalloonIcon");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 36) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 7) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});
  if (ImGui::BeginPopupModal(std::format("{}###ShowBalloonIcon", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::BeginVertical("##show_balloon_icon_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      GroupBox characterGroupBox(trNOOP("Character"), "##show_balloon_icon_character_group_box", {-1, 0});
      if (characterGroupBox.begin()) {
        ImGui::SetNextItemWidth(-1);
        if (ImGui::BeginCombo("##show_balloon_icon_character_combo", Database::instance()->eventNameOrId(m_character).c_str())) {
          const auto currentMap = Database::instance()->mapInfos->currentMap()->map();
          for (int i = -1; i < static_cast<int>(currentMap->events().size()); ++i) {
            const auto selected = i == m_character;
            if (i > 0 && !currentMap->event(i)) {
              continue;
            }

            if (ImGui::Selectable(Database::instance()->eventNameOrId(i).c_str(), selected)) {
              m_character = i;
            }
            if (selected) {
              ImGui::SetItemDefaultFocus();
            }
          }
          ImGui::EndCombo();
        }
      }
      characterGroupBox.end();
      GroupBox balloonGroupBox(trNOOP("Balloon Icon"), "##show_balloon_icon_balloon_group_box", {-1, 0});
      if (balloonGroupBox.begin()) {
        ImGui::SetNextItemWidth(-1);
        if (ImGui::BeginCombo("##show_balloon_icon_balloon_combo", DecodeEnumName(magic_enum::enum_cast<BalloonIcon>(m_balloonIndex).value_or(BalloonIcon::Exclamation)).c_str())) {
          for (const auto balloon : magic_enum::enum_values<BalloonIcon>()) {
            const auto selected = static_cast<int>(balloon) == m_balloonIndex;
            if (ImGui::Selectable(DecodeEnumName(balloon).c_str(), selected)) {
              m_balloonIndex = static_cast<int>(balloon);
            }
            if (selected) {
              ImGui::SetItemDefaultFocus();
            }
          }
          ImGui::EndCombo();
        }
      }
      balloonGroupBox.end();

      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##show_balloon_icon_button_layout", {-1, 0}, 0);
      {
        ImGui::Checkbox(trNOOP("Wait for Completion"), &m_waitCompletion);
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##show_balloon_icon_button_layout", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->id = m_character;
          m_command->index = static_cast<BalloonIcon>(m_balloonIndex);
          m_command->waitForCompletion = m_waitCompletion;
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
