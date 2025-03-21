#include "Core/CommonUI/FacePicker.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <IconsFontAwesome6.h>

FacePicker::FacePicker(const std::string_view sheetName, const int faceIndex)
: IDialogController("Face"), m_faceSheet(sheetName), m_faceDirectory("img/faces/", ".png", std::string(sheetName)), m_faceIndex(faceIndex) {
  m_faceSheets = m_faceDirectory.getDirectoryContents();
  m_folderDir = m_faceDirectory.getDirectories();
}

void FacePicker::setSelectedSheet() {}

std::tuple<bool, bool> FacePicker::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }

  const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size / 2, ImGuiCond_Appearing);

  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoSavedSettings)) {
    const auto calc = ImGui::CalcTextSize("OKCANCEL");
    ImGui::BeginChild("##top_child", {0, ImGui::GetContentRegionAvail().y - (calc.y + (ImGui::GetStyle().ItemSpacing.y * 3) + ImGui::GetStyle().FramePadding.y)});
    {
      ImGui::BeginChild("##image_picker_list", ImVec2{ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQRS").x, ImGui::GetContentRegionAvail().y - ImGui::GetStyle().FramePadding.y},
                        ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
      {
        if (ImGui::BeginTable("##image_picker.tablelist", 1)) {
          ImGui::TableNextRow();
          ImGui::TableNextColumn();

          ImGui::BeginDisabled(m_faceDirectory.isParentDirectory());
          if (ImGui::Selectable("\u21B0 ..", false, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
            if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              m_faceDirectory.moveUp();
              m_faceSheets = m_faceDirectory.getDirectoryContents();
              m_folderDir = m_faceDirectory.getDirectories();
            }
          }
          ImGui::EndDisabled();

          for (int i = 0; i < m_folderDir.size(); ++i) {
            const auto& folderName = std::format("{} {}", ICON_FA_FOLDER_OPEN, m_folderDir[i]);
            if (ImGui::Selectable(folderName.c_str(), m_selectedFolder == i, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
              m_selectedFolder = i;
            }
            if (m_selectedFolder == i && ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              m_faceDirectory.setDirectory(i);
              m_folderDir = m_faceDirectory.getDirectories();
              m_faceSheets = m_faceDirectory.getDirectoryContents();
              m_selectedFolder = -1;
              m_selectedSheet = -1;
              m_faceSheet.reset();
            }
          }

          if (ImGui::Selectable("(None)", m_selectedSheet < 0, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
            m_selectedSheet = -1;
            m_faceSheet.reset();
          }

          for (int i = 0; i < m_faceSheets.size(); ++i) {
            const auto& sheet = m_faceSheets[i];
            ImGui::TableNextColumn();
            if (ImGui::Selectable(sheet.c_str(), m_selectedSheet == i, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
              m_selectedSheet = i;
              m_faceSheet.emplace(m_selectedSheet == -1                 ? ""
                                  : m_faceDirectory.isParentDirectory() ? m_faceSheets.at(m_selectedSheet)
                                                                        : m_faceDirectory.getPathPrefix() + '/' + m_faceSheets.at(m_selectedSheet));
              if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("%s", sheet.c_str());
              }
            }
            if (m_selectedSheet == i && ImGui::IsWindowAppearing()) {
              ImGui::SetScrollHereY();
            }
          }
          ImGui::EndTable();
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("##image_picker_image_panel");
        {
          auto win = ImGui::GetCurrentWindow();
          if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered()) {
            auto mouseCursor = ImGui::GetMousePos();
            mouseCursor -= win->ContentRegionRect.Min;
            int x = alignValue(mouseCursor.x, FaceSheet::faceWidth());
            int y = alignValue(mouseCursor.y, FaceSheet::faceHeight());
          }
        }
        ImGui::EndChild();
      }
      ImGui::EndChild();
      ImGui::BeginChild("##bottom_child");
      {
        ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - (calc.x + (ImGui::GetStyle().FramePadding.x * 2) + ImGui::GetStyle().ItemSpacing.x));
        if (ImGui::Button("OK")) {
          m_confirmed = true;
          m_open = false;
          ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
          m_confirmed = false;
          m_open = false;
          ImGui::CloseCurrentPopup();
        }
      }
      ImGui::EndChild();
      ImGui::EndPopup();
    }
  }
  return {!m_open, m_confirmed};
}
