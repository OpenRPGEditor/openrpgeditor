#include "Dialog_PlayMovie.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/ImGuiUtils.hpp"

std::tuple<bool, bool> Dialog_PlayMovie::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{270, 250} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize)) {

    if (ImGui::BeginTable("##movie_contents", 1,
                          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX |
                              ImGuiTableFlags_ScrollY,
                          ImVec2{(ImGui::GetContentRegionMax().x) -  App::DPIHandler::scale_value(10),
                                 ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(16)})) {

      ImGui::TableSetupScrollFreeze(1, 0);
      ImGui::TableSetupColumn("File");
      ImGui::TableNextRow();
      ImGui::TableNextColumn();

      if (ImGui::SelectableWithBorder("(None)", m_selected == 0,
                                      ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns |
                                      ImGuiSelectableFlags_AllowDoubleClick)) {
        if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
          m_confirmed = true;
          command->name = m_movie;
          ImGui::CloseCurrentPopup();
          SetOpen(false);
        }
        m_selected = 0;
        m_movie = "";
        if (m_selected == 0)
          ImGui::SetItemDefaultFocus();
                                          }
      for (int n = 0; n < m_movies.size(); n++) {
        ImGui::TableNextColumn();
        const bool isSelected = (m_selected == n + 1);
        if (ImGui::SelectableWithBorder(m_movies.at(n).c_str(), isSelected,
                                        ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns |
                                            ImGuiSelectableFlags_AllowDoubleClick)) {
          if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
            m_confirmed = true;
            command->name = m_movie;
            ImGui::CloseCurrentPopup();
            SetOpen(false);
          }
          m_selected = n + 1;
          m_movie = m_movies.at(m_selected - 1);
          if (isSelected)
            ImGui::SetItemDefaultFocus();
                                            }
      }
      ImGui::EndTable();
                                 }

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->name = m_movie;
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
