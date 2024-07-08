#include "Dialog_ChangeParallax.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/Project.hpp"
#include "Database/Database.hpp"

std::tuple<bool, bool> Dialog_ChangeParallax::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{400, 200} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize)) {


    ImGui::SeparatorText("Parallax Background");
    // Actor Button
        ImGui::Text("Image:");
        ImGui::PushID("#parallax_image_selection");
        if (ImGui::Button("", ImVec2{(App::DPIHandler::scale_value(300)), 0})) {
          // TODO: Image Picker
        }
        ImGui::PopID();
        ImGui::Checkbox("Loop Horizontally", &m_loopHorizontally);
        ImGui::Text("Scroll:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
        ImGui::BeginDisabled(!m_loopHorizontally);
        if (ImGui::InputInt("##parallax_x", &m_scrollX, 1, 100)) {
          if (m_scrollX < 1)
            m_scrollX = 1;
          if (m_scrollX > 999)
            m_scrollX = 999;
        }
        ImGui::EndDisabled();
        ImGui::Checkbox("Loop Vertically", &m_loopVertically);
        ImGui::Text("Scroll:");
        ImGui::SameLine();
        ImGui::BeginDisabled(!m_loopVertically);
        ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
        if (ImGui::InputInt("##parallax_y", &m_scrollY, 1, 100)) {
          if (m_scrollY < 1)
            m_scrollY = 1;
          if (m_scrollY > 999)
            m_scrollY = 999;
        }
        ImGui::EndDisabled();
        if (ImGui::Button("OK")) {
          m_confirmed = true;
          command->image = m_image;
          command->loopVertically = m_loopVertically;
          command->loopHorizontally = m_loopHorizontally;
          command->scrollX = m_scrollX;
          command->scrollY = m_scrollY;
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
