#include "Dialog_ShowPicture.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/Log.hpp"
#include "Core/Project.hpp"
#include "Database/Database.hpp"

std::tuple<bool, bool> Dialog_ShowPicture::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{400, 305} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize)) {

    if (picker) {
      auto [closed, confirmed] = picker->draw();
      if (confirmed) {
        if (xOrY)
          m_value2 = picker->selection();
        else
          m_value1 = picker->selection();

        picker.reset();
      }
    }

    ImGui::SeparatorText("Picture");
    ImGui::BeginGroup();
    {
      ImGui::Text("Number:");
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
      if (ImGui::InputInt("##showpicture_id", &m_number, 1, 100)) {
        if (m_number < 1)
          m_number = 1;
        if (m_number > 999)
          m_number = 999;
      }
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3.f);
    ImGui::BeginGroup();
    {
      // Actor Button
      ImGui::Text("Image:");
      ImGui::PushID("##showpicture_image_selection");
      if (ImGui::Button("", ImVec2{(App::DPIHandler::scale_value(300)), 0})) {
        // TODO: Image Picker
      }
      ImGui::PopID();
      ImGui::EndGroup();
    }
    ImVec2 cursorPos = ImGui::GetCursorPos();
    ImGui::SeparatorText("Position and Scale");
    ImGui::Text("Origin:");
    ImGui::PushItemWidth((App::DPIHandler::scale_value(160)));
    if (ImGui::BeginCombo("##showpicture_origin",
                          DecodeEnumName(magic_enum::enum_value<PictureOrigin>(m_origin)).c_str())) {
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
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
      ImGui::InputInt("##showpicture_directdesig_x", &m_constant1, 1, 100);
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
      ImGui::InputInt("##showpicture_directdesig_y", &m_constant2, 1, 100);
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
      ImGui::PushID("##showpicture_vardesig_x");
      if (ImGui::Button(
              m_type == 1 ? Database::Instance->variableNameOrId(m_value1).c_str() : "",
              ImVec2{((ImGui::GetWindowContentRegionMax().x / 2)) - (15 * App::DPIHandler::get_ui_scale()), 0})) {
        xOrY = false;
        picker.emplace("Variables", m_project->system().variables);
      }
      ImGui::PopID();
      ImGui::PushID("##showpicture_vardesig_y");
      if (ImGui::Button(
              m_type == 1 ? Database::Instance->variableNameOrId(m_value2).c_str() : "",
              ImVec2{((ImGui::GetWindowContentRegionMax().x / 2)) - (15 * App::DPIHandler::get_ui_scale()), 0})) {
        xOrY = true;
        picker.emplace("Variables", m_project->system().variables);
      }
      ImGui::PopID();
      ImGui::EndDisabled();
      if (ImGui::Button("OK")) {
        m_confirmed = true;
        command->number = m_number;
        command->imageName = m_imageName;
        command->origin = static_cast<PictureOrigin>(m_origin);
        command->type = static_cast<PictureDesignationSource>(m_type);
        if (command->type == PictureDesignationSource::Direct_designation) {
          command->value1 = m_constant1;
          command->value2 = m_constant2;
        }
        else {
          command->value1 = m_value1;
          command->value2 = m_value2;
        }
        command->zoomX = m_zoomX;
        command->zoomY = m_zoomY;
        command->opacityValue = m_opacityValue;
        command->blendMode = static_cast<Blend>(m_blendMode);
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
    // Set Cursor
    ImGui::SetCursorPos(ImVec2(cursorPos.x + 235, cursorPos.y));
    ImGui::BeginGroup();
    {
      ImGui::SeparatorText("");
      ImGui::Text("Width %");
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
      if (ImGui::InputInt("##showpicture_width", &m_zoomX, 1, 100)) {
        if (m_zoomX < -2000)
          m_zoomX = -2000;
        if (m_zoomX > 2000)
          m_zoomX = 2000;
      }
      ImGui::Text("Height %");
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
      if (ImGui::InputInt("##showpicture_height", &m_zoomY, 1, 100)) {
        if (m_zoomY < -2000)
          m_zoomY = -2000;
        if (m_zoomY > 2000)
          m_zoomY = 2000;
      }
      ImGui::SeparatorText("Blend");
      ImGui::Text("Opacity:");
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(75));
      if (ImGui::InputInt("##showpicture_opacity", &m_opacityValue, 1, 100)) {
        if (m_opacityValue < 0)
          m_opacityValue = -0;
        if (m_opacityValue > 255)
          m_opacityValue = 255;
      }
      ImGui::PushItemWidth((App::DPIHandler::scale_value(75)));
      if (ImGui::BeginCombo("##showpicture_blendmode",
                            DecodeEnumName(magic_enum::enum_value<Blend>(m_blendMode)).c_str())) {
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
    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
