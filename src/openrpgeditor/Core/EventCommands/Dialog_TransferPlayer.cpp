#include "Core/EventCommands/Dialog_TransferPlayer.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Database.hpp"
#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>

std::tuple<bool, bool> Dialog_TransferPlayer::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###TransferPlayer");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 40) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 16) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###TransferPlayer", m_name).c_str(), &m_open, ImGuiWindowFlags_NoResize)) {
    drawPickers();
    ImGui::BeginVertical("##transfer_player_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      GroupBox locationGroup(trNOOP("Location"), "##transfer_player_location_group", {-1, 0});
      if (locationGroup.begin()) {
        ImGui::RadioButton(trNOOP("Direct Designation"), &m_mode, 0);
        ImGui::Dummy({ImGui::GetFrameHeight(), ImGui::GetFrameHeight()});
        ImGui::SameLine();
        ImGui::BeginDisabled(m_mode != 0);
        {
          ImGui::PushID("#transfer_coord_selection");
          if (ImGui::EllipsesButton(m_mode == 0 ? std::format("{} ({},{})", Database::instance()->mapNameOrId(m_mapId), m_x, m_y).c_str() : "", ImVec2{-1, 0})) {
            m_eventTilePicker.emplace(Database::instance()->mapInfos.currentMap()->id());
            m_eventTilePicker->setOpen(true);
          }
          ImGui::PopID();
        }
        ImGui::EndDisabled();
        ImGui::RadioButton(trNOOP("Designation with variables"), &m_mode, 1);
        ImGui::BeginDisabled(m_mode != 1);
        {
          ImGui::BeginHorizontal("##transfer_player_location_variables", {-1, 0}, 0);
          {
            ImGui::Dummy({ImGui::GetFrameHeight(), ImGui::GetFrameHeight()});
            ImGui::BeginVertical("##transfer_player_location_variables_inner", {-1, 0}, 0);
            {
              GroupBox mapIdGroup(trNOOP("Map ID"), "##transfer_player_location_variables_ID", {-1, 0});
              if (mapIdGroup.begin()) {
                ImGui::PushID("##transfer_player_variables_mapId");
                if (ImGui::EllipsesButton(m_mode == 1 ? Database::instance()->variableNameAndId(m_mapIdVar).c_str() : "", {-1, 0})) {
                  m_varSelection = 0;
                  m_variablePicker.emplace("Variables", Database::instance()->system.variables(), m_mapIdVar);
                  m_variablePicker->setOpen(true);
                }
                ImGui::PopID();
              }
              mapIdGroup.end();
              GroupBox xPosGroup("X", "##transfer_player_location_variables_x", {-1, 0});
              if (xPosGroup.begin()) {
                ImGui::PushID("##transfer_player_variables_x");
                if (ImGui::EllipsesButton(m_mode == 1 ? Database::instance()->variableNameAndId(m_xVar).c_str() : "", {-1, 0})) {
                  m_varSelection = 1;
                  m_variablePicker.emplace(trNOOP("Variables"), Database::instance()->system.variables(), m_xVar);
                  m_variablePicker->setOpen(true);
                }
                ImGui::PopID();
              }
              xPosGroup.end();
              GroupBox yPosGroup("Y", "##transfer_player_location_variables_y", {-1, 0});
              if (yPosGroup.begin()) {
                ImGui::PushID("##transfer_player_variables_y");
                if (ImGui::EllipsesButton(m_mode == 1 ? Database::instance()->variableNameAndId(m_yVar).c_str() : "", {-1, 0})) {
                  m_varSelection = 2;
                  m_variablePicker.emplace(trNOOP("Variables"), Database::instance()->system.variables(), m_yVar);
                  m_variablePicker->setOpen(true);
                }
                ImGui::PopID();
              }
              yPosGroup.end();
            }
            ImGui::EndVertical();
          }
          ImGui::EndHorizontal();
        }
        ImGui::EndDisabled();
        ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
        GroupBox directionGroup(trNOOP("Direction"), "##transfer_player_direction", {ImGui::GetContentRegionAvail().x * 0.5f, 0});
        // Store the current Y position so we can restore it later to work around an ImGui bug
        const auto cursorY = ImGui::GetCursorPosY();
        if (directionGroup.begin()) {
          ImGui::PushItemWidth(-1);
          if (ImGui::BeginCombo("##direction_selection", DecodeEnumName(magic_enum::enum_cast<Direction>(m_direction).value_or(Direction::Retain)).c_str())) {
            for (auto& direction : magic_enum::enum_values<Direction>()) {
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
        ImGui::SameLine();
        // Set the cursor Y position so the fade group is in the correct spot
        ImGui::SetCursorPosY(cursorY);
        GroupBox fadeGroup(trNOOP("Fade"), "##transfer_player_fade", {ImGui::GetContentRegionAvail().x, 0});
        if (fadeGroup.begin()) {
          ImGui::PushItemWidth(-1);
          if (ImGui::BeginCombo("##fade_selection", DecodeEnumName(magic_enum::enum_cast<Fade>(m_fade).value_or(Fade::Black)).c_str())) {
            for (auto& fade : magic_enum::enum_values<Fade>()) {
              bool selected = m_fade == static_cast<int>(fade);
              if (ImGui::Selectable(DecodeEnumName(fade).c_str(), selected)) {
                m_fade = static_cast<int>(fade);
              }
              if (selected) {
                ImGui::SetItemDefaultFocus();
              }
            }
            ImGui::EndCombo();
          }
        }
        fadeGroup.end();
      }
      locationGroup.end();

      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##transfer_player_button_layout", {-1, 0}, 0);
      {
        ImGui::Spring();

        if (const auto ret = ImGui::ButtonGroup("##transfer_player_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;

          m_command->mode = static_cast<TransferMode>(m_mode);
          if (m_command->mode == TransferMode::Variable_Designation) {
            m_command->mapId = m_mapIdVar;
            m_command->x = m_xVar;
            m_command->y = m_yVar;
          } else {
            m_command->mapId = m_mapId;
            m_command->x = m_x;
            m_command->y = m_y;
          }
          m_command->direction = static_cast<Direction>(m_direction);
          m_command->fade = static_cast<Fade>(m_fade);

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

void Dialog_TransferPlayer::drawPickers() {
  if (m_variablePicker) {
    if (const auto [closed, confirmed] = m_variablePicker->draw(); closed) {
      if (confirmed) {
        switch (m_varSelection) {
        case 0:
          m_mapIdVar = m_variablePicker->selection();
          break;
        case 1:
          m_xVar = m_variablePicker->selection();
          break;
        case 2:
          m_yVar = m_variablePicker->selection();
          break;
        default:
          break;
        }
      }
      m_variablePicker.reset();
    }
  }

  if (m_eventTilePicker) {
    if (const auto [closed, confirmed] = m_eventTilePicker->draw(); closed) {
      if (confirmed) {
        // TODO: Get mapId and event X/Y
      }

      m_eventTilePicker.reset();
    }
  }
}