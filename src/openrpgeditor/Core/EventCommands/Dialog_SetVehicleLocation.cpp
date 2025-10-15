#include "Core/EventCommands/Dialog_SetVehicleLocation.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Database.hpp"
#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>

std::tuple<bool, bool> Dialog_SetVehicleLocation::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###SetVehicleLocation");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 40) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 16) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###SetVehicleLocation", m_name).c_str(), &m_open, ImGuiWindowFlags_NoResize)) {
    drawPickers();
    ImGui::BeginVertical("##set_vehicle_location_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      GroupBox vehicleGroupBox(trNOOP("Vehicle"), "##set_vehicle_location_vehicle_group", {-1, 0});
      if (vehicleGroupBox.begin()) {
        ImGui::PushItemWidth(-1);
        if (ImGui::BeginCombo("##set_vehicle_location_vehicle_selection", DecodeEnumName(magic_enum::enum_cast<VehicleType>(m_vehicle).value_or(VehicleType::Boat)).c_str())) {
          for (auto& vehicle : magic_enum::enum_values<VehicleType>()) {
            bool selected = m_vehicle == static_cast<int>(vehicle);
            if (ImGui::Selectable(DecodeEnumName(vehicle).c_str(), selected)) {
              m_vehicle = static_cast<int>(vehicle);
            }
            if (selected) {
              ImGui::SetItemDefaultFocus();
            }
          }
          ImGui::EndCombo();
        }
      }
      vehicleGroupBox.end();
      GroupBox locationGroupBox(trNOOP("Location"), "##set_vehicle_location_location_group", {-1, 0});
      if (locationGroupBox.begin()) {
        ImGui::RadioButton(trNOOP("Direct Designation"), &m_mode, 0);
        ImGui::Dummy({ImGui::GetFrameHeight(), ImGui::GetFrameHeight()});
        ImGui::SameLine();
        ImGui::BeginDisabled(m_mode != 0);
        {
          ImGui::PushID("#transfer_coord_selection");
          if (ImGui::EllipsesButton(m_mode == 0 ? std::format("{} ({},{})", Database::instance()->mapNameOrId(Database::instance()->mapInfos->currentMap()->id()), m_x, m_y).c_str() : "",
                                    ImVec2{-1, 0})) {
            m_eventTilePicker.emplace(Database::instance()->mapInfos->currentMap()->id());
            m_eventTilePicker->setOpen(true);
          }
          ImGui::PopID();
        }
        ImGui::EndDisabled();
        ImGui::RadioButton(trNOOP("Designation with variables"), &m_mode, 1);
        ImGui::BeginDisabled(m_mode != 1);
        {
          ImGui::BeginHorizontal("##set_vehicle_location_location_variables", {-1, 0}, 0);
          {
            ImGui::Dummy({ImGui::GetFrameHeight(), ImGui::GetFrameHeight()});
            ImGui::BeginVertical("##set_vehicle_location_location_variables_inner", {-1, 0}, 0);
            {

              GroupBox idPosGroup("ID", "##set_vehicle_location_location_variables_id", {-1, 0});
              if (idPosGroup.begin()) {
                ImGui::PushID("##set_vehicle_location_variables_id");
                if (ImGui::EllipsesButton(m_mode == 1 ? Database::instance()->variableNameAndId(m_mapIdVar).c_str() : "", {-1, 0})) {
                  m_variableSelection = 0;
                  m_variablePicker.emplace(VariableSwitchPicker::Type::Variable, Database::instance()->system->variables(), m_mapIdVar);
                  m_variablePicker->setOpen(true);
                }
                ImGui::PopID();
              }
              idPosGroup.end();

              GroupBox xPosGroup("X", "##set_vehicle_location_location_variables_x", {-1, 0});
              if (xPosGroup.begin()) {
                ImGui::PushID("##set_vehicle_location_variables_x");
                if (ImGui::EllipsesButton(m_mode == 1 ? Database::instance()->variableNameAndId(m_xVar).c_str() : "", {-1, 0})) {
                  m_variableSelection = 1;
                  m_variablePicker.emplace(VariableSwitchPicker::Type::Variable, Database::instance()->system->variables(), m_xVar);
                  m_variablePicker->setOpen(true);
                }
                ImGui::PopID();
              }
              xPosGroup.end();
              GroupBox yPosGroup("Y", "##set_vehicle_location_location_variables_y", {-1, 0});
              if (yPosGroup.begin()) {
                ImGui::PushID("##set_vehicle_location_variables_y");
                if (ImGui::EllipsesButton(m_mode == 1 ? Database::instance()->variableNameAndId(m_yVar).c_str() : "", {-1, 0})) {
                  m_variableSelection = 2;
                  m_variablePicker.emplace(VariableSwitchPicker::Type::Variable, Database::instance()->system->variables(), m_yVar);
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
      }
      locationGroupBox.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##set_vehicle_location_button_layout", {-1, 0}, 0);
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##set_vehicle_location_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->vehicle = static_cast<VehicleType>(m_vehicle);
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

void Dialog_SetVehicleLocation::drawPickers() {
  if (m_variablePicker) {
    if (const auto [closed, confirmed] = m_variablePicker->draw(); closed) {
      if (confirmed) {
        switch (m_variableSelection) {
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
        m_x = m_eventTilePicker->selectedTile().x();
        m_y = m_eventTilePicker->selectedTile().y();
      }

      m_eventTilePicker.reset();
    }
  }
}