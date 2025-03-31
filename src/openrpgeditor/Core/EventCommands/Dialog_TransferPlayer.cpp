#include "Core/EventCommands/Dialog_TransferPlayer.hpp"

#include "Database/Database.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_TransferPlayer::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{380, 248}, ImGuiCond_Appearing);
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
        picker.emplace("Variables", Database::instance()->system.variables(), m_mapId_var);
        picker->setOpen(true);
      }
      ImGui::PopID();

      ImGui::PushID("##transfer_var_x");
      if (ImGui::Button(m_mode == 1 ? Database::instance()->variableNameAndId(m_x_var).c_str() : "", ImVec2{(280), 0})) {
        m_var_selection = 1;
        picker.emplace("Variables", Database::instance()->system.variables(), m_x_var);
        picker->setOpen(true);
      }
      ImGui::PopID();

      ImGui::PushID("##transfer_var_y");
      if (ImGui::Button(m_mode == 1 ? Database::instance()->variableNameAndId(m_y_var).c_str() : "", ImVec2{(280), 0})) {
        m_var_selection = 2;
        picker.emplace("Variables", Database::instance()->system.variables(), m_y_var);
        picker->setOpen(true);
      }
      ImGui::PopID();
      ImGui::EndDisabled();
    }
    ImGui::EndGroup();

    ImGui::Separator();
    ImGui::BeginGroup();
    {
      ImGui::Text("Direction:");
      ImGui::PushItemWidth(180);
      auto dir = magic_enum::enum_cast<Direction>(m_direction);
      std::string test = DecodeEnumName(magic_enum::enum_name(dir.value()));
      if (ImGui::BeginCombo("##direction_selection", DecodeEnumName(magic_enum::enum_name(dir.value())).c_str())) {
        for (auto& direction : magic_enum::enum_values<Direction>()) {
          bool is_selected = m_direction == magic_enum::enum_integer(direction);
          if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(direction)).c_str(), is_selected)) {
            m_direction = magic_enum::enum_integer(direction);
            if (is_selected)
              ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPos().y - 3.f);
    ImGui::BeginGroup();
    {
      ImGui::Text("Fade:");
      ImGui::PushItemWidth(180);
      if (ImGui::BeginCombo("##fade_selection", DecodeEnumName(magic_enum::enum_value<Fade>(m_fade)).c_str())) {
        for (auto& fade : magic_enum::enum_values<Fade>()) {
          bool is_selected = m_fade == magic_enum::enum_index(fade).value();
          if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(fade)).c_str(), is_selected)) {
            m_fade = magic_enum::enum_index(fade).value();
            if (is_selected)
              ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
    }
    ImGui::EndGroup();
    if (ImGui::Button("OK")) {
      m_confirmed = true;

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
      command->direction = static_cast<Direction>(m_direction);
      command->fade = static_cast<Fade>(m_fade);

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
