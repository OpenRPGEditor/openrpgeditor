#include "Editor/EventCommands/Dialog_PlayMovie.hpp"

#include "Editor/ImGuiExt/ImGuiUtils.hpp"
#include "imgui.h"
#include <tuple>

#include "imgui_internal.h"
#include <IconsFontAwesome6.h>

std::tuple<bool, bool> Dialog_PlayMovie::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###PlayMovie");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 32) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetTextLineHeightWithSpacing() * 19) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});
  if (ImGui::BeginPopupModal(std::format("{}###PlayMovie", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    ImGui::BeginVertical("##play_move_main_layout", ImGui::GetContentRegionAvail());
    {
      ImGui::BeginVertical("##play_move_file_list_layout", {-1, 0});
      {
        if (ImGui::BeginTable("##movie_contents", 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                              ImVec2{-1, ImGui::GetTextLineHeightWithSpacing() * 16})) {

          ImGui::TableSetupScrollFreeze(1, 0);
          ImGui::TableSetupColumn(trNOOP("File"));
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
            if (ImGui::Selectable(folderName.c_str(), m_selectedFolder == i,
                                  static_cast<int>(ImGuiSelectableFlags_SelectOnNav) | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_SelectOnClick)) {
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

          if (ImGui::SelectableWithBorder(trNOOP("None"), m_selected == 0, ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
            if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              m_confirmed = true;
              m_command->name = m_movie;
              ImGui::CloseCurrentPopup();
              setOpen(false);
            }
            m_selected = 0;
            m_movie = "";
          }
          if (m_selected == 0) {
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
      ImGui::EndVertical();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##play_movie_buttons_layout", {-1, -1});
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##play_movie_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->name = m_movie;
          ImGui::CloseCurrentPopup();
          setOpen(false);
        } else if (ret == 1) {
          ImGui::CloseCurrentPopup();
          setOpen(false);
        }
      }
      ImGui::EndHorizontal();
    }
    ImGui::EndVertical();
    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
