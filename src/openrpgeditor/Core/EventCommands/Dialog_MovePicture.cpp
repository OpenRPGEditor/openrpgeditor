#include "Core/EventCommands/Dialog_MovePicture.hpp"

#include "Database/Database.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_MovePicture::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{400, 347}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    if (picker) {
      auto [closed, confirmed] = picker->draw();
      if (closed) {
        if (confirmed) {
          if (xOrY)
            m_value2 = picker->selection();
          else
            m_value1 = picker->selection();
        }
        picker.reset();
      }
    }

    ImGui::SeparatorText("Picture");
    ImGui::BeginGroup();
    {
      ImGui::Text("Number:");
      ImGui::SetNextItemWidth(100);
      if (ImGui::InputInt("##movepicture_id", &m_number, 1, 100)) {
        if (m_number < 1)
          m_number = 1;
        if (m_number > 999)
          m_number = 999;
      }
      ImGui::EndGroup();
    }
    ImVec2 cursorPos = ImGui::GetCursorPos();
    ImGui::SeparatorText("Position and Scale");
    ImGui::Text("Origin:");
    ImGui::PushItemWidth((160));
    if (ImGui::BeginCombo("##movepicture_origin", DecodeEnumName(magic_enum::enum_value<PictureOrigin>(m_origin)).c_str())) {
      for (auto& origin : magic_enum::enum_values<PictureOrigin>()) {
        bool is_selected = m_origin == magic_enum::enum_index(origin).value();
        if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(origin)).c_str(), is_selected)) {
          m_origin = magic_enum::enum_index(origin).value();
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }
    // Direct Designation: Type 0
    ImGui::RadioButton("Direct Designation", &m_type, 0);
    ImGui::BeginGroup();
    {
      ImGui::Text("X:");
      ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 8.f);
      ImGui::Text("Y:");
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      ImGui::BeginDisabled(m_type != 0);
      ImGui::SetNextItemWidth(100);
      ImGui::InputInt("##movepicture_directdesig_x", &m_constant1, 1, 100);
      ImGui::SetNextItemWidth(100);
      ImGui::InputInt("##movepicture_directdesig_y", &m_constant2, 1, 100);
      ImGui::EndDisabled();
      ImGui::EndGroup();
    }
    // Designation with Variables Type 1
    ImGui::RadioButton("Designation with variables", &m_type, 1);
    ImGui::BeginGroup();
    {
      ImGui::Text("X:");
      ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 8.f);
      ImGui::Text("Y:");
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      ImGui::BeginDisabled(m_type != 1);
      ImGui::PushID("##movepicture_vardesig_x");
      if (ImGui::Button(m_type == 1 ? Database::instance()->variableNameOrId(m_value1).c_str() : "", ImVec2{((ImGui::GetWindowContentRegionMax().x / 2)) - 15, 0})) {
        xOrY = false;
        picker.emplace("Variables", Database::instance()->system.variables(), m_value1);
        picker->setOpen(true);
      }
      ImGui::PopID();
      ImGui::PushID("##movepicture_vardesig_y");
      if (ImGui::Button(m_type == 1 ? Database::instance()->variableNameOrId(m_value2).c_str() : "", ImVec2{((ImGui::GetWindowContentRegionMax().x / 2)) - 15, 0})) {
        xOrY = true;
        picker.emplace("Variables", Database::instance()->system.variables(), m_value2);
        picker->setOpen(true);
      }
      ImGui::PopID();
      ImGui::EndDisabled();
      ImGui::EndGroup();
    }
    ImGui::BeginGroup();
    {
      ImGui::SeparatorText("Duration");
      ImGui::SetNextItemWidth(100);
      if (ImGui::InputInt("##movepicture_duration", &m_duration)) {
        if (m_duration < 1)
          m_duration = 1;
        if (m_duration > 999)
          m_duration = 999;
      }
      ImGui::SameLine();
      ImGui::Text("frames 1/60 sec");
      ImGui::SameLine();
      // ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 80.f);
      ImGui::Checkbox("Wait for Completion", &m_waitForCompletion);

      ImGui::EndGroup();
    }
    // ImGui::SetCursorPosX(cursorPos.x);

    // Set Cursor
    ImGui::SetCursorPos(ImVec2(cursorPos.x + 235, cursorPos.y));
    ImGui::BeginGroup();
    {
      ImGui::SeparatorText("");
      ImGui::Text("Width %%");
      ImGui::SetNextItemWidth(100);
      if (ImGui::InputInt("##movepicture_width", &m_zoomX, 1, 100)) {
        if (m_zoomX < -2000)
          m_zoomX = -2000;
        if (m_zoomX > 2000)
          m_zoomX = 2000;
      }
      ImGui::Text("Height %%");
      ImGui::SetNextItemWidth(100);
      if (ImGui::InputInt("##movepicture_height", &m_zoomY, 1, 100)) {
        if (m_zoomY < -2000)
          m_zoomY = -2000;
        if (m_zoomY > 2000)
          m_zoomY = 2000;
      }
      ImGui::SeparatorText("Blend");
      ImGui::Text("Opacity:");
      ImGui::SetNextItemWidth(100);
      if (ImGui::InputInt("##movepicture_opacity", &m_opacityValue, 1, 100)) {
        if (m_opacityValue < 0)
          m_opacityValue = -0;
        if (m_opacityValue > 255)
          m_opacityValue = 255;
      }
      ImGui::PushItemWidth((75));
      if (ImGui::BeginCombo("##movepicture_blendmode", DecodeEnumName(magic_enum::enum_value<Blend>(m_blendMode)).c_str())) {
        for (auto& blend : magic_enum::enum_values<Blend>()) {
          bool is_selected = m_blendMode == magic_enum::enum_index(blend).value();
          if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(blend)).c_str(), is_selected)) {
            m_blendMode = magic_enum::enum_index(blend).value();
            if (is_selected)
              ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
    }
    ImGui::EndGroup();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 80);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 100);
    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->picture = m_number;
      command->origin = static_cast<PictureOrigin>(m_origin);
      command->pictureLocation = static_cast<PictureDesignationSource>(m_type);
      if (command->pictureLocation == PictureDesignationSource::Direct_designation) {
        command->x = m_constant1;
        command->y = m_constant2;
      } else {
        command->x = m_value1;
        command->y = m_value2;
      }
      command->width = m_zoomX;
      command->height = m_zoomY;
      command->opacity = m_opacityValue;
      command->blendMode = static_cast<Blend>(m_blendMode);
      command->duration = m_duration;
      command->waitForCompletion = m_waitForCompletion;
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
