#include "Core/EventCommands/Dialog_ChangeVehicleImage.hpp"
#include <tuple>
#include "imgui.h"
#include "Core/Application.hpp"
#include "Core/Log.hpp"
#include "Core/DPIHandler.hpp"

std::tuple<bool, bool> Dialog_ChangeVehicleImage::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{125, 225} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open,
                             ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_AlwaysAutoResize)) {

    if (const auto [closed, confirmed] = m_characterPicker.draw(); closed) {
      if (confirmed) {
        m_characterPicker.Accept();
        m_character = m_characterPicker.character();
        m_image = m_characterPicker.selectedSheet();
        m_characterSheet = CharacterSheet(m_image);
      }
    }
    ImGui::BeginGroup();
    {
      ImGui::Text("Vehicle:");
      ImGui::PushItemWidth((App::DPIHandler::scale_value(100)));
      if (ImGui::BeginCombo("##vehicle_location_selection",
                            DecodeEnumName(magic_enum::enum_value<VehicleType>(m_vehicle)).c_str())) {
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

    const auto buttonSize = ImVec2{144, 144} * App::DPIHandler::get_ui_scale();
    const auto buttonCenter = (buttonSize / 2);
    ImGui::SeparatorText("Image");
    ImGui::BeginGroup();
    {
      auto cursorPos = ImGui::GetCursorPos();
      if (ImGui::ImageButton("##event_image", m_buttonBack.get(),
                             ImVec2{80.f, 102.f} * App::DPIHandler::get_ui_scale())) {
        m_characterPicker.setCharacterInfo(m_image, m_character);
        m_characterPicker.SetOpen(true);
      }
      if (m_characterSheet && m_characterSheet->texture()) {
        if (m_characterSheet->characterWidth() < 72 || m_characterSheet->characterHeight() < 96) {
          ImGui::SetCursorPos(
              cursorPos + (ImVec2{m_characterSheet->characterWidth() / 2.f, m_characterSheet->characterHeight() / 2.f} *
                           App::DPIHandler::get_ui_scale()));
        } else {
          ImGui::SetCursorPos(cursorPos);
        }
        const auto [min, max] = m_characterSheet->getRectForCharacter(m_character);
        ImGui::Image(m_characterSheet->texture().get(),
                     ImVec2{static_cast<float>(m_characterSheet->characterWidth()),
                            static_cast<float>(m_characterSheet->characterHeight())} *
                         App::DPIHandler::get_ui_scale(),
                     min, max);
      }
    }
    ImGui::SameLine();
    ImGui::Dummy(ImVec2{App::DPIHandler::scale_value(20), 0});
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.f);
    ImGui::Dummy(ImVec2{0, App::DPIHandler::scale_value(10)});
    ImGui::BeginGroup();
    {
      if (ImGui::Button("OK")) {
        m_confirmed = true;
        command->vehicle = static_cast<VehicleType>(m_vehicle);
        command->picture = m_image;
        command->pictureIndex = m_character;
        ImGui::CloseCurrentPopup();
        SetOpen(false);
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        ImGui::CloseCurrentPopup();
        SetOpen(false);
      }
      ImGui::EndGroup();
    }

    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
