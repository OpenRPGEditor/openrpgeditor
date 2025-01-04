#include "Core/EventCommands/Dialog_SetVehicleLocation.hpp"


#include "Database/Database.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_SetVehicleLocation::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{380, 242}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    if (picker) {
      auto [closed, confirmed] = picker->draw();
      if (closed) {
        if (confirmed) {
          switch (m_var_selection) {
          case 0:
            m_mapId_var = picker->selection();
            break;
          case 1:
            m_x_var = picker->selection();
            break;
          case 2:
            m_y_var = picker->selection();
            break;
          default:
            break;
          }
        }
        picker.reset();
      }
    }
    ImGui::BeginGroup();
    {
      ImGui::Text("Vehicle:");
      ImGui::PushItemWidth(180);
      if (ImGui::BeginCombo("##vehicle_location_selection", DecodeEnumName(magic_enum::enum_value<VehicleType>(m_vehicle)).c_str())) {
        for (auto& vehicle : magic_enum::enum_values<VehicleType>()) {
          bool is_selected = m_vehicle == magic_enum::enum_index(vehicle).value();
          if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(vehicle)).c_str(), is_selected)) {
            m_vehicle = magic_enum::enum_index(vehicle).value();
            if (is_selected)
              ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
      ImGui::EndGroup();
    }

    ImGui::SeparatorText("Location");
    ImGui::RadioButton("Direct designation", &m_mode, 0);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
    ImGui::BeginDisabled(m_mode != 0);
    ImGui::PushID("#transfer_coord_selection");
    if (ImGui::Button(m_mode == 0 ? Database::instance()->mapNameOrId(m_mapId).c_str() : "", ImVec2{300, 0})) {
      // TODO: Coordinate selector
    }
    ImGui::PopID();
    ImGui::EndDisabled();

    ImGui::RadioButton("Designation with variables", &m_mode, 1);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 30);
    ImGui::BeginGroup();
    {
      ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 2.f);
      ImGui::Text("ID:");
      ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 6.f);
      ImGui::Text("X:");
      ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 6.f);
      ImGui::Text("Y:");
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      ImGui::BeginDisabled(m_mode != 1);
      ImGui::PushID("##transfer_var_mapId");
      if (ImGui::Button(m_mode == 1 ? Database::instance()->variableNameAndId(m_mapId_var).c_str() : "", ImVec2{(280), 0})) {
        m_var_selection = 0;
        picker.emplace("Variables", Database::instance()->system.variables, m_mapId_var);
        picker->setOpen(true);
      }
      ImGui::PopID();

      ImGui::PushID("##transfer_var_x");
      if (ImGui::Button(m_mode == 1 ? Database::instance()->variableNameAndId(m_x_var).c_str() : "", ImVec2{(280), 0})) {
        m_var_selection = 1;
        picker.emplace("Variables", Database::instance()->system.variables, m_x_var);
        picker->setOpen(true);
      }
      ImGui::PopID();

      ImGui::PushID("##transfer_var_y");
      if (ImGui::Button(m_mode == 1 ? Database::instance()->variableNameAndId(m_y_var).c_str() : "", ImVec2{(280), 0})) {
        m_var_selection = 2;
        picker.emplace("Variables", Database::instance()->system.variables, m_y_var);
        picker->setOpen(true);
      }
      ImGui::PopID();
      ImGui::EndDisabled();
    }
    ImGui::EndGroup();

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->vehicle = static_cast<VehicleType>(m_vehicle);
      command->mode = static_cast<TransferMode>(m_mode);
      if (command->mode == TransferMode::Variable_Designation) {
        command->mapId = m_mapId_var;
        command->x = m_x_var;
        command->y = m_y_var;
      } else {
        command->mapId = m_mapId;
        command->x = m_x;
        command->y = m_y;
      }

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
