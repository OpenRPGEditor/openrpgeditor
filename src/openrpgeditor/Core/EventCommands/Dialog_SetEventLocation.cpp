#include "Core/EventCommands/Dialog_SetEventLocation.hpp"

#include "Database/Database.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_SetEventLocation::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{380, 313}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    if (picker) {
      if (auto [closed, confirmed] = picker->draw(); closed) {
        if (confirmed) {
          switch (m_var_selection) {
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
    ImGui::SeparatorText("Event");
    ImGui::PushItemWidth(160);
    if (ImGui::BeginCombo("##setevloc_character", Database::instance()->eventNameOrId(m_event).c_str())) {

      if (ImGui::Selectable("This Event", m_event == 0)) {
        m_event = 0;
        ImGui::SetItemDefaultFocus();
      }

      for (auto& dataSource : Database::instance()->mapInfos.currentMap()->map()->events()) {
        if (!dataSource.has_value())
          continue;

        if (const bool is_selected = m_event == dataSource->id(); ImGui::Selectable(("EV" + std::format("{:03} ", dataSource->id())).c_str(), is_selected)) {
          m_event = dataSource->id();
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    ImGui::SeparatorText("Location");
    ImGui::RadioButton("Direct designation", &m_mode, 0);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
    ImGui::BeginDisabled(m_mode != 0);
    ImGui::PushID("#transfer_coord_selection");
    if (ImGui::Button(m_mode == 0 ? std::format("Current Map ({}, {})", m_x, m_y).c_str() : "", ImVec2{300, 0})) {
      // TODO: Coordinate selector
    }
    ImGui::PopID();
    ImGui::EndDisabled();

    ImGui::RadioButton("Designation with variables", &m_mode, 1);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 30);
    ImGui::BeginGroup();
    {
      ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 2.f);
      ImGui::Text("X:");
      ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 6.f);
      ImGui::Text("Y:");
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::BeginGroup();
    {

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
      // FIXME: What's the intent here?
      // ImGui::EndDisabled();
    }
    ImGui::EndGroup();

    ImGui::RadioButton("Exchange with another event", &m_mode, 2);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
    ImGui::BeginDisabled(m_mode != 2);
    ImGui::PushItemWidth(160);
    if (ImGui::BeginCombo("##exchangeev_event", m_mode != 2 ? "" : Database::instance()->eventNameOrId(m_otherEvent).c_str())) {

      if (ImGui::Selectable("This Event", m_otherEvent == 0)) {
        m_otherEvent = 0;
        ImGui::SetItemDefaultFocus();
      }

      for (auto& dataSource : Database::instance()->mapInfos.currentMap()->map()->events()) {
        if (!dataSource.has_value())
          continue;

        if (const bool is_selected = m_otherEvent == dataSource->id(); ImGui::Selectable(("EV" + std::format("{:03} ", dataSource->id())).c_str(), is_selected)) {
          m_otherEvent = dataSource->id();
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }
    ImGui::EndDisabled();

    ImGui::Separator();
    ImGui::BeginGroup();
    {
      ImGui::Text("Direction:");
      ImGui::PushItemWidth(180);
      if (ImGui::BeginCombo("##direction_selection", DecodeEnumName(magic_enum::enum_value<Direction>(m_direction)).c_str())) {
        for (auto& dir : magic_enum::enum_values<Direction>()) {
          if (const bool is_selected = m_direction == magic_enum::enum_index(dir).value(); ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(dir)).c_str(), is_selected)) {
            m_direction = magic_enum::enum_index(dir).value();
            if (is_selected)
              ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
      ImGui::EndGroup();
    }
    if (ImGui::Button("OK")) {
      m_confirmed = true;

      command->event = m_event;
      command->mode = static_cast<TransferMode>(m_mode);
      if (command->mode == TransferMode::Variable_Designation) {
        command->x = m_x_var;
        command->y = m_y_var;
      } else if (command->mode == TransferMode::Exchange_With_Another_Event) {
        command->x = m_otherEvent;
        command->y = 0;
      } else {
        command->x = m_x;
        command->y = m_y;
      }
      command->direction = static_cast<Direction>(m_direction * 2 + 2);

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
