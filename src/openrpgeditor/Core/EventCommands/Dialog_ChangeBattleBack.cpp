#include "Core/EventCommands/Dialog_ChangeBattleBack.hpp"

#include "Core/DPIHandler.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_ChangeBattleBack::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{248, 100} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    if (const auto [closed, confirmed] = m_imagePicker->draw(); closed) {
      if (confirmed) {
        m_imagePicker->Accept();
        m_image = m_imagePicker->selectedImage();
        m_image_2 = m_imagePicker->selectedImage2();
      }
    }
    ImGui::Text("Battle Background:");
    ImGui::PushID("#battleback_image_selection");
    if (ImGui::Button(m_image_2.empty() ? m_image.c_str() : (m_image + " & " + m_image_2).c_str(), ImVec2{(App::DPIHandler::scale_value(300)), 0})) {
      m_imagePicker->SetOpen(true);
    }
    ImGui::PopID();

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->battleBack1Name = m_image;
      command->battleBack2Name = m_image_2;
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
