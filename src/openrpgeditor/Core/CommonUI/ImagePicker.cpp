#include "Core/CommonUI/ImagePicker.hpp"
#include <iostream>
#include "imgui.h"
#include "imgui_internal.h"
#include "Core/DPIHandler.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/Log.hpp"

ImagePicker::ImagePicker(PickerMode mode, const std::string_view imageName, const std::string_view image2Name)
: IDialogController("Select an Image##image_picker"), m_checkerboardTexture(864, 768) {
  m_pickType = mode;
  if (m_pickType == PickerMode::Parallax) {
    m_images = ResourceManager::instance()->getDirectoryContents("img/parallaxes/", ".png");
  } else if (m_pickType == PickerMode::Picture) {
    m_images = ResourceManager::instance()->getDirectoryContents("img/pictures/", ".png");
  } else {
    m_images = ResourceManager::instance()->getDirectoryContents("img/battlebacks1/", ".png");
    m_images_2 = ResourceManager::instance()->getDirectoryContents("img/battlebacks2/", ".png");
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
      if (!m_images_2[i].compare(imageName)) {
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
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }

  const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{894, 768} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open,
                             ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings)) {
    ImGui::BeginGroup();
    {
      ImGui::BeginChild("##image_picker_list",
                        ImVec2{App::DPIHandler::scale_value(200), App::DPIHandler::scale_value(768)},
                        ImGuiChildFlags_None, ImGuiWindowFlags_NoBackground);
      {
        if (ImGui::BeginTable("##image_picker.tablelist", 1)) {
          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          if (ImGui::Selectable("(None)", m_selectedImage < 0,
                                ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
            m_selectedImage = -1;
            m_image.reset();
          }
          for (int i = 0; i < m_images.size(); ++i) {
            const auto& sheet = m_images[i];
            ImGui::TableNextColumn();
            if (ImGui::Selectable(sheet.c_str(), m_selectedImage == i,
                                  ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
              if (m_selectedImage != i) {
                m_selectedImage = i;
                m_image.emplace(m_selectedImage == -1 ? "" : m_images.at(m_selectedImage), static_cast<int>(m_pickType), false);
              }
              if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("%s", sheet.c_str());
              }
            }
          }
          ImGui::EndTable();
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("##image_picker_list##2",
                          ImVec2{App::DPIHandler::scale_value(200), App::DPIHandler::scale_value(768)},
                          ImGuiChildFlags_None, ImGuiWindowFlags_NoBackground);
        {
          if (m_pickType == PickerMode::Battleback) {
            if (ImGui::BeginTable("##image_picker.tablelist##2", 1)) {
              ImGui::TableNextRow();
              ImGui::TableNextColumn();
              if (ImGui::Selectable("(None)##2", m_selectedImage2 < 0,
                                    ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
                m_selectedImage2 = -1;
                m_image2.reset();
              }
              for (int i = 0; i < m_images_2.size(); ++i) {
                const auto& sheet2 = m_images_2[i];
                ImGui::TableNextColumn();
                if (ImGui::Selectable(sheet2.c_str(), m_selectedImage2 == i,
                                      ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
                  if (m_selectedImage2 != i) {
                    m_selectedImage2 = i;
                    m_image2.emplace(m_selectedImage2 == -1 ? "" : m_images_2.at(m_selectedImage2), static_cast<int>(m_pickType), true);
                  }
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
        ImGui::BeginChild("##image_picker_image_panel",
                          ImVec2{App::DPIHandler::scale_value(894), App::DPIHandler::scale_value(784)},
                          ImGuiChildFlags_Border, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_HorizontalScrollbar);
        {
          const auto imageRect =
              ImVec2{static_cast<float>(m_image->imageWidth()), static_cast<float>(m_image->imageHeight())} *
              App::DPIHandler::get_ui_scale();
          auto win = ImGui::GetCurrentWindow();
          if (m_image) {
            ImGui::GetWindowDrawList()->AddImage(
                m_checkerboardTexture.get(), win->ContentRegionRect.Min + ImVec2{0.f, 0.f},
                win->ContentRegionRect.Min + (ImVec2{static_cast<float>(m_image->texture().width()),
                                                     static_cast<float>(m_image->texture().height())} *
                                              App::DPIHandler::get_ui_scale()));

            ImGui::Image(m_image->texture().get(), imageRect);
          }
          if (m_image2) {
            ImGui::GetWindowDrawList()->AddImage(
                m_image2->texture().get(), win->ContentRegionRect.Min + ImVec2{0.f, 0.f},
                win->ContentRegionRect.Min + (ImVec2{static_cast<float>(m_image2->texture().width()),
                                                     static_cast<float>(m_image2->texture().height())} *
                                              App::DPIHandler::get_ui_scale()));
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
