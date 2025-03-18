#include "Core/CommonUI/ImagePicker.hpp"
#include "Core/Log.hpp"
#include "Core/ResourceManager.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include <iostream>

ImagePicker::ImagePicker(PickerMode mode, const std::string_view imageName, const std::string_view image2Name) : IDialogController("Select an Image##image_picker"), m_checkerboardTexture(864, 768) {
  m_pickType = mode;
  switch (m_pickType) {
  case PickerMode::Parallax:
    m_images = ResourceManager::instance()->getDirectoryContents("img/parallaxes/", ".png");
    m_folderDir = ResourceManager::instance()->getDirectories("img/parallaxes/");
    break;
  case PickerMode::Picture:
    m_images = ResourceManager::instance()->getDirectoryContents("img/pictures/", ".png");
    m_folderDir = ResourceManager::instance()->getDirectories("img/pictures/");
    break;
  case PickerMode::Battleback:
    m_images = ResourceManager::instance()->getDirectoryContents("img/battlebacks1/", ".png");
    m_images_2 = ResourceManager::instance()->getDirectoryContents("img/battlebacks2/", ".png");
    m_folderDir = ResourceManager::instance()->getDirectories("img/battlebacks1/");
    m_folderDir_2 = ResourceManager::instance()->getDirectories("img/battlebacks2/");
    break;
  case PickerMode::Title:
    m_images = ResourceManager::instance()->getDirectoryContents("img/titles1/", ".png");
    m_images_2 = ResourceManager::instance()->getDirectoryContents("img/titles2/", ".png");
    m_folderDir = ResourceManager::instance()->getDirectories("img/titles1/");
    m_folderDir_2 = ResourceManager::instance()->getDirectories("img/titles2/");
    break;
  case PickerMode::Tileset:
    m_images = ResourceManager::instance()->getDirectoryContents("img/tilesets/", ".png");
    m_folderDir = ResourceManager::instance()->getDirectories("img/tilesets/");
    break;
  case PickerMode::Animation:
    m_images = ResourceManager::instance()->getDirectoryContents("img/animations/", ".png");
    m_folderDir = ResourceManager::instance()->getDirectories("img/animations/");
    break;
  case PickerMode::SVBattler:
    m_images = ResourceManager::instance()->getDirectoryContents("img/sv_actors/", ".png");
    m_folderDir = ResourceManager::instance()->getDirectories("img/sv_actors/");
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
  ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{894, 768}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings)) {
    ImGui::BeginGroup();
    {
      ImGui::BeginChild("##image_picker_list", ImVec2{200, 768}, ImGuiChildFlags_None, ImGuiWindowFlags_NoBackground);
      {
        if (ImGui::BeginTable("##image_picker.tablelist", 1)) {
          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          if (ImGui::Selectable("(None)", m_selectedImage < 0, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
            m_selectedImage = -1;
            m_image.reset();
          }
          for (int i = 0; i < m_images.size(); ++i) {
            const auto& sheet = m_images[i];
            ImGui::TableNextColumn();
            if (ImGui::Selectable(sheet.c_str(), m_selectedImage == i, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
              m_selectedImage = i;
              m_image.emplace(m_selectedImage == -1 ? "" : m_images.at(m_selectedImage), m_pickType, false);
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
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("##image_picker_list##2", ImVec2{200, 768}, ImGuiChildFlags_None, ImGuiWindowFlags_NoBackground);
        {
          if (m_pickType == PickerMode::Battleback || m_pickType == PickerMode::Title) {
            if (ImGui::BeginTable("##image_picker.tablelist##2", 1)) {
              ImGui::TableNextRow();
              ImGui::TableNextColumn();
              if (ImGui::Selectable("(None)##2", m_selectedImage2 < 0, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
                m_selectedImage2 = -1;
                m_image2.reset();
              }
              for (int i = 0; i < m_images_2.size(); ++i) {
                const auto& sheet2 = m_images_2[i];
                ImGui::TableNextColumn();
                if (ImGui::Selectable(sheet2.c_str(), m_selectedImage2 == i, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
                  m_selectedImage2 = i;
                  m_image2.emplace(m_selectedImage2 == -1 ? "" : m_images_2.at(m_selectedImage2), m_pickType, true);
                  if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("%s", sheet2.c_str());
                  }
                }
              }
              ImGui::EndTable();
            }
          }

          ImGui::EndChild();
        }
        ImGui::SameLine();
        ImGui::BeginChild("##image_picker_image_panel", ImVec2{894, 784}, ImGuiChildFlags_Border, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_HorizontalScrollbar);
        {
          if (!m_image && m_selectedImage != -1) {
            m_image.emplace(m_selectedImage == -1 ? "" : m_images.at(m_selectedImage), m_pickType, false);
          }
          if (!m_image2 && m_selectedImage2 != -1) {
            m_image2.emplace(m_selectedImage2 == -1 ? "" : m_images.at(m_selectedImage2), m_pickType, false);
          }

          auto win = ImGui::GetCurrentWindow();
          if (m_image) {
            const auto imageRect = ImVec2{static_cast<float>(m_image->imageWidth()), static_cast<float>(m_image->imageHeight())};
            ImGui::GetWindowDrawList()->AddImage(static_cast<ImTextureID>(m_checkerboardTexture), win->ContentRegionRect.Min + ImVec2{0.f, 0.f},
                                                 win->ContentRegionRect.Min + (ImVec2{static_cast<float>(m_image->texture().width()), static_cast<float>(m_image->texture().height())}));

            ImGui::Image(m_image->texture(), imageRect);
          }
          if (m_image2) {
            ImGui::GetWindowDrawList()->AddImage(m_image2->texture(), win->ContentRegionRect.Min + ImVec2{0.f, 0.f},
                                                 win->ContentRegionRect.Min + (ImVec2{static_cast<float>(m_image2->texture().width()), static_cast<float>(m_image2->texture().height())}));
          }
        }
        ImGui::EndChild();
      }
      ImGui::EndGroup();
      ImGui::BeginGroup();
      {
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
      ImGui::EndGroup();
      ImGui::EndPopup();
    }
  }
  return {!m_open, m_confirmed};
}
