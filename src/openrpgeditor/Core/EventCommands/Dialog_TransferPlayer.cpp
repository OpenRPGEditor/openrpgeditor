#include "Core/EventCommands/Dialog_TransferPlayer.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Database/Database.hpp"

std::tuple<bool, bool> Dialog_TransferPlayer::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{380, 248} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open,
                             ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_AlwaysAutoResize)) {

    if (picker) {
      auto [closed, confirmed] = picker->draw();
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
        }

        picker.reset();
      }
    }

    ImGui::SeparatorText("Location");
    ImGui::RadioButton("Direct designation", &m_mode, 0);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
    ImGui::BeginDisabled(m_mode != 0);
    ImGui::PushID("#transfer_coord_selection");
    if (ImGui::Button(m_mode == 0 ? Database::Instance->mapNameOrId(m_mapId).c_str() : "",
                      ImVec2{(App::DPIHandler::scale_value(300)), 0})) {
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
      if (ImGui::Button(m_mode == 1 ? Database::Instance->variableNameAndId(m_mapId_var).c_str() : "",
                        ImVec2{(App::DPIHandler::scale_value(280)), 0})) {
        m_var_selection = 0;
        picker.emplace("Variables", Database::Instance->system.variables);
      }
      ImGui::PopID();

      ImGui::PushID("##transfer_var_x");
      if (ImGui::Button(m_mode == 1 ? Database::Instance->variableNameAndId(m_x_var).c_str() : "",
                        ImVec2{(App::DPIHandler::scale_value(280)), 0})) {
        m_var_selection = 1;
        picker.emplace("Variables", Database::Instance->system.variables);
      }
      ImGui::PopID();

      ImGui::PushID("##transfer_var_y");
      if (ImGui::Button(m_mode == 1 ? Database::Instance->variableNameAndId(m_y_var).c_str() : "",
                        ImVec2{(App::DPIHandler::scale_value(280)), 0})) {
        m_var_selection = 2;
        picker.emplace("Variables", Database::Instance->system.variables);
      }
      ImGui::PopID();
      ImGui::EndDisabled();
    }
    ImGui::EndGroup();

    ImGui::Separator();
    ImGui::BeginGroup();
    {
      ImGui::Text("Direction:");
      ImGui::PushItemWidth((App::DPIHandler::scale_value(180)));
      if (ImGui::BeginCombo("##direction_selection",
                            DecodeEnumName(magic_enum::enum_value<Direction>(m_direction)).c_str())) {
        for (auto& dir : magic_enum::enum_values<Direction>()) {
          bool is_selected = m_direction == magic_enum::enum_index(dir).value();
          if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(dir)).c_str(), is_selected)) {
            m_direction = magic_enum::enum_index(dir).value();
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
      ImGui::PushItemWidth((App::DPIHandler::scale_value(180)));
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
      command->direction = static_cast<Direction>((m_direction * 2) + 2);
      command->fade = static_cast<Fade>(m_fade);

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
