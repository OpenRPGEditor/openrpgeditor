#include "Dialog_MovementChangeBlendMode.hpp"
#include <tuple>
#include "imgui.h"
#include "Core/Application.hpp"
#include "Core/DPIHandler.hpp"

std::tuple<bool, bool> Dialog_MovementChangeBlendMode::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{105, 95} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize)) {

    ImGui::SeparatorText("Blend");
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

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->mode = static_cast<Blend>(m_blendMode);
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
