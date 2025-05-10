#include "Core/EventCommands/Dialog_SetEventLocation.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Database.hpp"
#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>

std::tuple<bool, bool> Dialog_SetEventLocation::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###SetEventLocation");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 40) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 18) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###SetEventLocation", m_name).c_str(), &m_open, ImGuiWindowFlags_NoResize)) {
    drawPickers();
    ImGui::BeginVertical("##set_event_location_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      GroupBox eventGroup(trNOOP("Event"), "##set_event_location_event_group", {-1, 0});
      if (eventGroup.begin()) {
        ImGui::SetNextItemWidth(-1);
        if (ImGui::BeginCombo("##set_event_location_event_list", Database::instance()->eventNameOrId(m_event).c_str())) {
          for (int i = 0; i < Database::instance()->mapInfos.currentMap()->map()->events().size(); ++i) {
            if (i > 0 && !Database::instance()->mapInfos.currentMap()->map()->event(i)) {
              continue;
            }

            if (ImGui::Selectable(Database::instance()->eventNameOrId(i).c_str(), i == m_event)) {
              m_event = i;
            }

            if (i == m_event) {
              ImGui::SetItemDefaultFocus();
            }
          }
          ImGui::EndCombo();
        }
      }
      eventGroup.end();
      GroupBox locationGroup(trNOOP("Location"), "##set_event_location_location_group", {-1, 0});
      if (locationGroup.begin()) {
        ImGui::RadioButton(trNOOP("Direct Designation"), &m_mode, 0);
        ImGui::Dummy({ImGui::GetFrameHeight(), ImGui::GetFrameHeight()});
        ImGui::SameLine();
        ImGui::BeginDisabled(m_mode != 0);
        {
          ImGui::PushID("#transfer_coord_selection");
          if (ImGui::EllipsesButton(m_mode == 0 ? std::format("{} ({},{})", Database::instance()->mapNameOrId(Database::instance()->mapInfos.currentMap()->id()), m_x, m_y).c_str() : "",
                                    ImVec2{-1, 0})) {
            m_eventTilePicker.emplace(Database::instance()->mapInfos.currentMap()->id(), false);
            m_eventTilePicker->setOpen(true);
          }
          ImGui::PopID();
        }
        ImGui::EndDisabled();
        ImGui::RadioButton(trNOOP("Designation with variables"), &m_mode, 1);
        ImGui::BeginDisabled(m_mode != 1);
        {
          ImGui::BeginHorizontal("##set_event_location_location_variables", {-1, 0}, 0);
          {
            ImGui::Dummy({ImGui::GetFrameHeight(), ImGui::GetFrameHeight()});
            ImGui::BeginVertical("##set_event_location_location_variables_inner", {-1, 0}, 0);
            {
              GroupBox xPosGroup("X", "##set_event_location_location_variables_x", {-1, 0});
              if (xPosGroup.begin()) {
                ImGui::PushID("##set_event_location_variables_x");
                if (ImGui::EllipsesButton(m_mode == 1 ? Database::instance()->variableNameAndId(m_xVar).c_str() : "", {-1, 0})) {
                  m_variableSelection = 0;
                  m_variablePicker.emplace(trNOOP("Variables"), Database::instance()->system.variables(), m_xVar);
                  m_variablePicker->setOpen(true);
                }
                ImGui::PopID();
              }
              xPosGroup.end();
              GroupBox yPosGroup("Y", "##set_event_location_location_variables_y", {-1, 0});
              if (yPosGroup.begin()) {
                ImGui::PushID("##set_event_location_variables_y");
                if (ImGui::EllipsesButton(m_mode == 1 ? Database::instance()->variableNameAndId(m_yVar).c_str() : "", {-1, 0})) {
                  m_variableSelection = 1;
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
        ImGui::RadioButton(trNOOP("Exchange with another event"), &m_mode, 2);
        ImGui::BeginDisabled(m_mode != 2);
        {
          ImGui::BeginHorizontal("##set_event_location_location_other_event", {-1, 0}, 0);
          {
            ImGui::Dummy({ImGui::GetFrameHeight(), ImGui::GetFrameHeight()});
            ImGui::SetNextItemWidth(-1);
            if (ImGui::BeginCombo("##set_event_location_other_event_list", m_mode == 2 ? Database::instance()->eventNameOrId(m_otherEvent).c_str() : "")) {
              for (int i = 0; i < Database::instance()->mapInfos.currentMap()->map()->events().size(); ++i) {
                if (i > 0 && !Database::instance()->mapInfos.currentMap()->map()->event(i)) {
                  continue;
                }

                if (ImGui::Selectable(Database::instance()->eventNameOrId(i).c_str(), i == m_otherEvent)) {
                  m_otherEvent = i;
                }

                if (i == m_otherEvent) {
                  ImGui::SetItemDefaultFocus();
                }
              }
              ImGui::EndCombo();
            }
          }
          ImGui::EndHorizontal();
        }
        ImGui::EndDisabled();
        ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
        GroupBox directionGroup(trNOOP("Direction"), "##set_event_location_direction", {ImGui::GetContentRegionAvail().x, 0});
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
      }
      locationGroup.end();

      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##set_event_location_button_layout", {-1, 0}, 0);
      {
        ImGui::Spring();

        if (const auto ret = ImGui::ButtonGroup("##set_event_location_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;

          m_command->mode = static_cast<TransferMode>(m_mode);
          if (m_command->mode == TransferMode::Direct) {
            m_command->event = m_event;
            m_command->x = m_x;
            m_command->y = m_y;
          } else if (m_command->mode == TransferMode::Variable_Designation) {
            m_command->event = m_event;
            m_command->x = m_xVar;
            m_command->y = m_yVar;
          } else {
            m_command->event = m_event;
            m_command->x = m_otherEvent;
            m_command->y = 0;
          }
          m_command->direction = static_cast<Direction>(m_direction);

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

void Dialog_SetEventLocation::drawPickers() {
  if (m_variablePicker) {
    if (const auto [closed, confirmed] = m_variablePicker->draw(); closed) {
      if (confirmed) {
        switch (m_variableSelection) {
        case 0:
          m_xVar = m_variablePicker->selection();
          break;
        case 1:
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
        m_x = m_eventTilePicker->selectedTile().x();
        m_y = m_eventTilePicker->selectedTile().y();
      }

      m_eventTilePicker.reset();
    }
  }
}