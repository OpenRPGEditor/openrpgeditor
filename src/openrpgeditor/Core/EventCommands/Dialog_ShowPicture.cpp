#include "Dialog_ShowPicture.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/Log.hpp"

std::tuple<bool, bool> Dialog_ShowPicture::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{400, 290} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize)) {

    ImGui::SeparatorText("Picture");
    ImGui::BeginGroup();
    {
      ImGui::Text("Number:");
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(75));
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
      if (ImGui::Button("",
              ImVec2{(App::DPIHandler::scale_value(300)), 0})) {
        // TODO: Image Picker
      }
      ImGui::PopID();
      ImGui::EndGroup();
    }

    ImGui::SeparatorText("Position");
    ImGui::PushItemWidth((App::DPIHandler::scale_value(160)));
    if (ImGui::BeginCombo("##showpicture_origin", DecodeEnumName(magic_enum::enum_value<PictureOrigin>(m_origin)).c_str())) {
      int index = 1;
      for (auto& origin : magic_enum::enum_values<PictureOrigin>()) {
        APP_INFO(std::to_string(index));
        bool is_selected = m_origin == magic_enum::enum_index(origin).value();
        if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(origin)).c_str(), is_selected)) {
          m_origin = magic_enum::enum_index(origin).value();
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
        index++;
      }
      ImGui::EndCombo();
    }

    if (ImGui::Button("OK")) {
      m_confirmed = true;
       command->number = m_number;
       command->imageName = m_imageName;
       command->origin = static_cast<PictureOrigin>(m_origin);
       command->type = static_cast<PictureDesignationSource>(m_type);
       command->value1 = m_value1;
       command->value2 = m_value2;
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
    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
