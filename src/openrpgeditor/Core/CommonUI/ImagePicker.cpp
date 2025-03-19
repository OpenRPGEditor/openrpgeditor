#include "Core/CommonUI/ImagePicker.hpp"
#include "Core/ResourceManager.hpp"
#include "imgui.h"
#include "imgui_internal.h"

#include <IconsFontAwesome6.h>

ImagePicker::ImagePicker(PickerMode mode, const std::string_view imageName, const std::string_view image2Name) : IDialogController("Select an Image##image_picker"), m_checkerboardTexture(864, 768) {
  m_pickType = mode;
  switch (m_pickType) {
  case PickerMode::Parallax:
    m_imageDir.emplace("img/parallaxes/", ".png", std::string(imageName));

    m_images = m_imageDir.value().getDirectoryContents();
    m_folderDir = m_imageDir.value().getDirectories();
    break;
  case PickerMode::Picture:
    m_imageDir.emplace("img/pictures/", ".png", std::string(imageName));

    m_images = m_imageDir.value().getDirectoryContents();
    m_folderDir = m_imageDir.value().getDirectories();
    break;
  case PickerMode::Battleback:
    m_imageDir.emplace("img/battlebacks1/", ".png", std::string(imageName));
    m_imageDir2.emplace("img/battlebacks2/", ".png", std::string(image2Name));

    m_images = m_imageDir.value().getDirectoryContents();
    m_images_2 = m_imageDir2.value().getDirectoryContents();
    m_folderDir = m_imageDir.value().getDirectories();
    m_folderDir_2 = m_imageDir2.value().getDirectories();
    break;
  case PickerMode::Title:
    m_imageDir.emplace("img/titles1/", ".png", std::string(imageName));
    m_imageDir2.emplace("img/titles2/", ".png", std::string(image2Name));

    m_images = m_imageDir.value().getDirectoryContents();
    m_images_2 = m_imageDir2.value().getDirectoryContents();
    m_folderDir = m_imageDir.value().getDirectories();
    m_folderDir_2 = m_imageDir2.value().getDirectories();
    break;
  case PickerMode::Tileset:
    m_imageDir.emplace("img/tilesets/", ".png", std::string(imageName));

    m_images = m_imageDir.value().getDirectoryContents();
    m_folderDir = m_imageDir.value().getDirectories();
    break;
  case PickerMode::Animation:
    m_imageDir.emplace("img/animations/", ".png", std::string(imageName));

    m_images = m_imageDir.value().getDirectoryContents();
    m_folderDir = m_imageDir.value().getDirectories();
    break;
  case PickerMode::SVBattler:
    m_imageDir.emplace("img/sv_actors/", ".png", std::string(imageName));

    m_images = m_imageDir.value().getDirectoryContents();
    m_folderDir = m_imageDir.value().getDirectories();
    break;
  default:
    break;
  }
  setImageInfo(imageName, image2Name);
}

void ImagePicker::setImageInfo(std::string_view imageName, std::string_view image2Name) {
  if (!imageName.empty()) {
    bool found = false;
    for (int i = 0; i < m_images.size(); ++i) {
      if (!m_images[i].compare(imageName)) {
        found = true;
        m_selectedImage = i;
        break;
      }
    }
    if (!found) {
      m_selectedImage = -1;
      return;
    }

  } else {
    m_selectedImage = -1;
  }
  if (!image2Name.empty()) {
    bool found = false;
    for (int i = 0; i < m_images_2[i].size(); ++i) {
      if (!m_images_2[i].compare(image2Name)) {
        found = true;
        m_selectedImage2 = i;
        break;
      }
    }
    if (!found) {
      m_selectedImage2 = -1;
      return;
    }

  } else {
    m_selectedImage2 = -1;
  }
}

std::tuple<bool, bool> ImagePicker::draw() {
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
      ImGui::BeginChild("##image_picker_list##1", ImVec2{200, ImGui::GetContentRegionAvail().y - ImGui::GetStyle().FramePadding.y}, ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
      {
        if (ImGui::BeginTable("##image_picker.tablelist", 1)) {
          ImGui::TableNextRow();
          ImGui::TableNextColumn();

          ImGui::BeginDisabled(m_imageDir.value().isParentDirectory());
          if (ImGui::Selectable("\u21B0 ..", false, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
            if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              m_imageDir.value().moveUp();
              m_images = m_imageDir->getDirectoryContents();
              m_folderDir = m_imageDir.value().getDirectories();
              m_selectedImage = -1;
              m_selectedFolder = -1;
              m_image.reset();
            }
          }
          ImGui::EndDisabled();
          for (int i = 0; i < m_folderDir.size(); ++i) {
            const auto& folderName = std::format("{} {}", ICON_FA_FOLDER_OPEN, m_folderDir[i]);
            if (ImGui::Selectable(folderName.c_str(), m_selectedFolder == i, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
              m_selectedFolder = i;
            }
            if (m_selectedFolder == i && ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              m_imageDir->setDirectory(i);
              m_folderDir = m_imageDir.value().getDirectories();
              m_images = m_imageDir.value().getDirectoryContents();
              m_selectedImage = -1;
              m_selectedFolder = -1;
              m_image.reset();
            }
          }
          if (ImGui::Selectable("(None)", m_selectedImage < 0, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
            m_selectedImage = -1;
            m_image.reset();
          }

          for (int i = 0; i < m_images.size(); ++i) {
            const auto& sheet = m_images[i];
            ImGui::TableNextColumn();
            if (ImGui::Selectable(sheet.c_str(), m_selectedImage == i, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
              m_selectedImage = i;
              m_image.emplace(m_selectedImage == -1                    ? ""
                              : m_imageDir.value().isParentDirectory() ? m_images.at(m_selectedImage)
                                                                       : m_imageDir.value().pathPrefix + '/' + m_images.at(m_selectedImage),
                              m_pickType, false);
              if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("%s", sheet.c_str());
              }
            }
            if (m_selectedImage == i && ImGui::IsWindowAppearing()) {
              ImGui::SetScrollHereY();
            }
          }
          ImGui::EndTable();
        }
      }
      ImGui::EndChild();
      ImGui::SameLine();
      ImGui::BeginChild("##image_picker_list##2", ImVec2{200, ImGui::GetContentRegionAvail().y - ImGui::GetStyle().FramePadding.y}, ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
      {
        if (m_pickType == PickerMode::Battleback || m_pickType == PickerMode::Title) {
          if (ImGui::BeginTable("##image_picker.tablelist##2", 1)) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            ImGui::BeginDisabled(m_imageDir2.value().isParentDirectory());
            if (ImGui::Selectable("\u21B0 ..##2", false, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
              if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
                m_imageDir2.value().moveUp();
                m_images_2 = m_imageDir2->getDirectoryContents();
                m_folderDir_2 = m_imageDir2.value().getDirectories();
              }
            }
            ImGui::EndDisabled();
            for (int i = 0; i < m_folderDir_2.size(); ++i) {
              const auto& folderName = std::format("{} {}##2", ICON_FA_FOLDER_OPEN, m_folderDir_2[i]);
              if (ImGui::Selectable(folderName.c_str(), m_selectedFolder2 == i, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
                m_selectedFolder2 = i;
              }
              if (m_selectedFolder2 == i && ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
                m_imageDir2->setDirectory(i);
                m_folderDir_2 = m_imageDir2.value().getDirectories();
                m_images_2 = m_imageDir2.value().getDirectoryContents();
                m_selectedImage2 = -1;
                m_selectedFolder2 = -1;
                m_image2.reset();
              }
            }

            if (ImGui::Selectable("(None)##2", m_selectedImage2 < 0, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
              m_selectedImage2 = -1;
              m_image2.reset();
            }
            for (int i = 0; i < m_images_2.size(); ++i) {
              const auto& sheet2 = m_images_2[i];
              ImGui::TableNextColumn();
              if (ImGui::Selectable(sheet2.c_str(), m_selectedImage2 == i, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
                m_selectedImage2 = i;
                m_image2.emplace(m_selectedImage2 == -1                    ? ""
                                 : m_imageDir2.value().isParentDirectory() ? m_images_2.at(m_selectedImage2)
                                                                           : m_imageDir2.value().pathPrefix + '/' + m_images_2.at(m_selectedImage2),
                                 m_pickType, true);
                if (ImGui::IsItemHovered()) {
                  ImGui::SetTooltip("%s", sheet2.c_str());
                }
              }
            }
            ImGui::EndTable();
          }
        }
      }
      ImGui::EndChild();
      ImGui::SameLine();
      ImGui::BeginChild("##image_picker_image_panel", ImVec2{ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, ImGui::GetContentRegionAvail().y - ImGui::GetStyle().FramePadding.y},
                        ImGuiChildFlags_Border, ImGuiWindowFlags_HorizontalScrollbar);
      {
        if (!m_image && m_selectedImage != -1) {
          m_image.emplace(m_selectedImage == -1 ? "" : m_images.at(m_selectedImage), m_pickType, false);
        }
        if (!m_image2 && m_selectedImage2 != -1) {
          m_image2.emplace(m_selectedImage2 == -1 ? "" : m_images.at(m_selectedImage2), m_pickType, false);
        }

        const auto win = ImGui::GetCurrentWindow();
        const int dummyWidth = std::max(m_image ? m_image->imageWidth() : 0, m_image2 ? m_image2->imageWidth() : 0);
        const int dummyHeight = std::max(m_image ? m_image->imageHeight() : 0, m_image2 ? m_image2->imageHeight() : 0);
        if (dummyWidth > 0 && dummyHeight > 0) {
          ImGui::Dummy({static_cast<float>(dummyWidth), static_cast<float>(dummyHeight)});
        }

        if (m_image) {
          ImGui::GetWindowDrawList()->AddImage(static_cast<ImTextureID>(m_checkerboardTexture), win->ContentRegionRect.Min + ImVec2{0.f, 0.f},
                                               win->ContentRegionRect.Min + (ImVec2{static_cast<float>(m_image->texture().width()), static_cast<float>(m_image->texture().height())}));

          ImGui::GetWindowDrawList()->AddImage(m_image->texture(), win->ContentRegionRect.Min + ImVec2{0.f, 0.f},
                                               win->ContentRegionRect.Min + (ImVec2{static_cast<float>(m_image->texture().width()), static_cast<float>(m_image->texture().height())}));
        }
        if (m_image2) {
          ImGui::GetWindowDrawList()->AddImage(m_image2->texture(), win->ContentRegionRect.Min + ImVec2{0.f, 0.f},
                                               win->ContentRegionRect.Min + (ImVec2{static_cast<float>(m_image2->texture().width()), static_cast<float>(m_image2->texture().height())}));
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
  return {!m_open, m_confirmed};
}
