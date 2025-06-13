#include "Core/EventCommands/Dialog_ShowAnimation.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Database.hpp"
#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>

std::tuple<bool, bool> Dialog_ShowAnimation::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ShowAnimation");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 36) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 7) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});
  if (ImGui::BeginPopupModal(std::format("{}###ShowAnimation", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    drawPickers();

    ImGui::BeginVertical("##show_animation_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      GroupBox characterGroupBox(trNOOP("Character"), "##show_animation_character_group", {-1, 0});
      if (characterGroupBox.begin()) {
        ImGui::SetNextItemWidth(-1);
        if (ImGui::BeginCombo("##show_animation_character_combo", Database::instance()->eventNameOrId(m_character).c_str())) {
          const auto currentMap = Database::instance()->mapInfos.currentMap();
          for (int i = -1; i < static_cast<int>(currentMap->map()->events().size()); ++i) {
            if (i > 0 && !currentMap->event(i)) {
              continue;
            }
            const auto name = Database::instance()->eventNameOrId(i);
            const auto selected = m_character == i;
            if (ImGui::Selectable(name.c_str(), selected)) {
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
      GroupBox animationGroupBox(trNOOP("Animation"), "##show_animation_animation_group", {-1, 0});
      if (animationGroupBox.begin()) {
        if (ImGui::EllipsesButton(Database::instance()->animationNameAndId(m_animation).c_str(), {-1, 0})) {
          m_animationPicker.emplace(trNOOP("Animations"), Database::instance()->animations.animations(), m_animation);
          m_animationPicker->setOpen(true);
        }
      }
      animationGroupBox.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##show_animation_button_layout", {-1, 0}, 0);
      {
        ImGui::Checkbox(trNOOP("Wait for Completion"), &m_waitCompletion);
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##show_animation_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->character = m_character;
          m_command->animation = m_animation;
          m_command->waitForCompletion = m_waitCompletion;
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

void Dialog_ShowAnimation::drawPickers() {
  if (m_animationPicker) {
    if (const auto [closed, confirmed] = m_animationPicker->draw(); closed) {
      if (confirmed) {
        m_animation = m_animationPicker->selection();
      }
      m_animationPicker.reset();
    }
  }
}