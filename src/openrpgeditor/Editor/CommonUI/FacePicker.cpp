#include "Editor/CommonUI/FacePicker.hpp"

#include <format>
#include <imgui.h>
#include <imgui_internal.h>

#include <IconsFontAwesome6.h>

FacePicker::FacePicker(const std::string_view sheetName, const int faceIndex)
: IDialogController("Face")
, m_faceDirectory("img/faces/", ".png", std::string(sheetName)) {
  if (!sheetName.empty()) {
    m_faceSheet.emplace(sheetName);
  }

  m_faceSheets = m_faceDirectory.getDirectoryContents();
  m_folderDir = m_faceDirectory.getDirectories();
  setFaceInfo(sheetName, faceIndex);
}

void FacePicker::setFaceInfo(const std::string_view sheetName, const int faceIndex) {
  m_faceSheet.emplace(sheetName);
  m_faceIndex = faceIndex;
  const std::string imageName = m_faceDirectory.getFileName(static_cast<std::string>(sheetName));

  if (!imageName.empty()) {
    bool found = false;
    for (int i = 0; i < m_faceSheets.size(); ++i) {
      if (!m_faceSheets[i].compare(sheetName)) {
        found = true;
        m_selectedSheet = i;
        break;
      }
    }

    if (!found) {
      m_faceSheet.reset();
      m_selectedSheet = -1;
      return;
    }

    const auto [min, _] = m_faceSheet->getFaceRect(m_faceIndex);
    m_selectionX = min.x() * m_faceSheet->texture().width();
    m_selectionY = min.y() * m_faceSheet->texture().height();
  }

  if (m_faceSheet) {
    m_checkerboardTexture.setSize(m_faceSheet->texture().width(), m_faceSheet->texture().height());
  }
}

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
              m_checkerboardTexture.setSize(m_faceSheet->texture().width(), m_faceSheet->texture().height());

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
            if ((x >= 0 && x < m_faceSheet->texture().width()) && (y >= 0 && y < m_faceSheet->texture().height())) {
              x /= FaceSheet::faceWidth();
              y /= FaceSheet::faceHeight();

              m_faceIndex = (y * (m_faceSheet->texture().width() / FaceSheet::faceWidth())) + x;
              const auto [min, _] = m_faceSheet->getFaceRect(m_faceIndex);
              m_selectionX = min.x() * m_faceSheet->texture().width();
              m_selectionY = min.y() * m_faceSheet->texture().height();
            }
          }

          if (m_faceSheet && m_selectedSheet >= 0) {
            ImGui::Dummy({static_cast<float>(m_faceSheet->texture().width()), static_cast<float>(m_faceSheet->texture().height())});
            win->DrawList->AddImage(static_cast<ImTextureID>(m_checkerboardTexture), win->ContentRegionRect.Min + ImVec2{0.f, 0.f},
                                    win->ContentRegionRect.Min + (ImVec2{static_cast<float>(m_checkerboardTexture.width()), static_cast<float>(m_checkerboardTexture.height())}));
            win->DrawList->AddImage(m_faceSheet->texture(), win->ContentRegionRect.Min + ImVec2{0.f, 0.f},
                                    win->ContentRegionRect.Min + (ImVec2{static_cast<float>(m_faceSheet->texture().width()), static_cast<float>(m_faceSheet->texture().height())}));
            win->DrawList->AddRect(win->ContentRegionRect.Min + (ImVec2{static_cast<float>(m_selectionX), static_cast<float>(m_selectionY)}),
                                   win->ContentRegionRect.Min + (ImVec2{static_cast<float>(m_selectionX) + static_cast<float>(FaceSheet::faceWidth()),
                                                                        static_cast<float>(m_selectionY) + static_cast<float>(FaceSheet::faceHeight())}),
                                   0xFF000000, 0.f, 0, 7.f);
            win->DrawList->AddRect(win->ContentRegionRect.Min + (ImVec2{static_cast<float>(m_selectionX), static_cast<float>(m_selectionY)}),
                                   win->ContentRegionRect.Min + (ImVec2{static_cast<float>(m_selectionX) + static_cast<float>(FaceSheet::faceWidth()),
                                                                        static_cast<float>(m_selectionY) + static_cast<float>(FaceSheet::faceHeight())}),
                                   0xFFFFFFFF, 0.f, 0, 3.f);
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
