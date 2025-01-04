#include "Core/EventCommands/MovementRoute/Dialog_MovementChangeBlendMode.hpp"
#include "Core/Application.hpp"

#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_MovementChangeBlendMode::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{105, 95}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    ImGui::SeparatorText("Blend");
    ImGui::PushItemWidth((75));
    if (ImGui::BeginCombo("##showpicture_blendmode", DecodeEnumName(magic_enum::enum_value<Blend>(m_blendMode)).c_str())) {
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
