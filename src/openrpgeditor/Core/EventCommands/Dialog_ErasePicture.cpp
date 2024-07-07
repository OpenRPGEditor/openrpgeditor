#include "Dialog_ErasePicture.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/Project.hpp"

std::tuple<bool, bool> Dialog_ErasePicture::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{131, 93} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize)) {

    ImGui::SeparatorText("Picture Number");
    ImGui::SetNextItemWidth(App::DPIHandler::scale_value(110));
    if (ImGui::InputInt("##erasepicture_id", &m_picture)) {
      if (m_picture > 999) {
        m_picture = 999;
      } else if (m_picture < 1)
        m_picture = 1;
    }

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->picture = m_picture;
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
