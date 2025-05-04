#include "Core/EventCommands/Dialog_PlayMovie.hpp"

#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "imgui.h"
#include <tuple>

#include "imgui_internal.h"
#include <IconsFontAwesome6.h>

std::tuple<bool, bool> Dialog_PlayMovie::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{270, 250}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    ImGui::BeginGroup();
    {

      if (ImGui::BeginTable("##movie_contents", 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY, ImVec2{500, 500})) {

        ImGui::TableSetupScrollFreeze(1, 0);
        ImGui::TableSetupColumn("File");
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        ImGui::BeginDisabled(m_movieDir.value().isParentDirectory());
        if (ImGui::Selectable("\u21B0 ..", false, static_cast<int>(ImGuiSelectableFlags_SelectOnNav) | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_SelectOnClick)) {
          if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
            m_movieDir.value().moveUp();
            m_movies = m_movieDir->getDirectoryContents();
            m_folders = m_movieDir.value().getDirectories();
            m_selected = 0;
            m_selectedFolder = -1;
          }
        }
        ImGui::EndDisabled();
        for (int i = 0; i < m_folders.size(); ++i) {
          const auto& folderName = std::format("{} {}", ICON_FA_FOLDER_OPEN, m_folders[i]);
          if (ImGui::Selectable(folderName.c_str(), m_selectedFolder == i, static_cast<int>(ImGuiSelectableFlags_SelectOnNav) | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_SelectOnClick)) {
            m_selectedFolder = i;
          }
          if (m_selectedFolder == i && ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
            m_movieDir->setDirectory(i);
            m_folders = m_movieDir.value().getDirectories();
            m_movies = m_movieDir.value().getDirectoryContents();
            m_selected = 0;
            m_selectedFolder = -1;
          }
        }

        if (ImGui::SelectableWithBorder("(None)", m_selected == 0, ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
          if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
            m_confirmed = true;
            m_command->name = m_movie;
            ImGui::CloseCurrentPopup();
            setOpen(false);
          }
          m_selected = 0;
          m_movie = "";
          if (m_selected == 0)
            ImGui::SetItemDefaultFocus();
        }
        for (int n = 0; n < m_movies.size(); n++) {
          ImGui::TableNextColumn();
          const bool isSelected = (m_selected == n + 1);
          if (ImGui::SelectableWithBorder(m_movies.at(n).c_str(), isSelected, ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
            if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              m_confirmed = true;
              m_command->name = m_movie;
              ImGui::CloseCurrentPopup();
              setOpen(false);
            }
            m_selected = n + 1;
            m_movie = m_movies.at(m_selected - 1);
            if (isSelected)
              ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndTable();
      }
    }
    ImGui::EndGroup();

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      m_command->name = m_movie;
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
