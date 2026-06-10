#include "Editor/CommonUI/ImagePicker.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"
#include "Editor/ResourceManager.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"

#include <IconsFontAwesome6.h>

ImagePicker::ImagePicker(PickerMode mode, const std::string_view imageName, const std::string_view image2Name)
: m_checkerboardTexture(864, 768) {
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

static bool ContainsCaseInsensitive(std::string_view str, std::string_view val) {
  return std::ranges::search(str, val, [](const char ch1, const char ch2) { return std::toupper(ch1) == std::toupper(ch2); }).begin() != str.end();
}

void ImagePicker::setImageInfo(const std::string_view imageName, const std::string_view image2Name) {
  if (!imageName.empty()) {
    bool found = false;
    for (int i = 0; i < m_images.size(); ++i) {
      if (m_images[i] == imageName) {
        found = true;
        m_selectedImage = i;
        break;
      }
    }
    if (!found) {
      m_selectedImage = -1;
    }
  } else {
    m_selectedImage = -1;
  }
  if (!image2Name.empty()) {
    bool found = false;
    for (int i = 0; i < m_images_2[i].size(); ++i) {
      if (m_images_2[i] == image2Name) {
        found = true;
        m_selectedImage2 = i;
        break;
      }
    }
    if (!found) {
      m_selectedImage2 = -1;
    }
  } else {
    m_selectedImage2 = -1;
  }
}

std::tuple<bool, bool> ImagePicker::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(("###" + m_name).c_str());
  }

  if (m_sortRequest) {
    int index{0};
    m_sortedIndexes.clear();
    for (auto& str : m_images) {
      if (!m_filter.empty()) {
        if (ContainsCaseInsensitive(str, m_filter)) {
          m_sortedIndexes.insert(index);
        }
      } else {
        m_sortedIndexes.insert(index);
      }
      index++;
    }

    index = 0;
    m_sortedIndexes2.clear();
    for (auto& str : m_images_2) {
      if (!m_filter.empty()) {
        if (ContainsCaseInsensitive(str, m_filter)) {
          m_sortedIndexes2.insert(index);
        }
      } else {
        m_sortedIndexes2.insert(index);
      }
      index++;
    }
    m_sortRequest = false;
  }

  const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size / 2, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(ImGui::GetDPIScaledSize(480, 480), {FLT_MAX, FLT_MAX});
  if (ImGui::BeginPopupModal(std::format("{}###{}", trNOOP("Select an Image"), m_name).c_str(), &m_open, ImGuiWindowFlags_NoSavedSettings)) {
    ImGui::BeginVertical("##image_picker_main_layout", ImGui::GetContentRegionAvail(), 0.f);
    {
      ImGui::BeginHorizontal("##image_picker_filter_layout", {-1, 0});
      {
        ImGui::TextUnformatted(trNOOP("Filter"));
        const std::string buttonText = trNOOP("Clear");
        const float buttonWidth = ImGui::CalcItemSize(ImGui::CalcTextSize(buttonText.c_str()), 0, 0).x + ImGui::GetStyle().ItemSpacing.x * 2.f + ImGui::GetStyle().FramePadding.x * 2.f;
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - (buttonWidth + ImGui::GetStyle().ItemSpacing.x));
        if (ImGui::InputText("##filter_input", &m_filter)) {
          if (m_filter.empty()) {
            m_sortedIndexes.clear();
          } else {
            m_sortRequest = true;
          }
        }
        if (ImGui::Button(buttonText.c_str(), {-1, 0})) {
          m_filter.clear();
          m_sortedIndexes.clear();
        }
      }
      ImGui::EndHorizontal();
      ImGui::BeginHorizontal("##image_picker_inner_layout", {-1, 0});
      {
        const ImVec2 listSize{ImGui::GetDPIScaledValue(200), ImGui::GetContentRegionAvail().y - (ImGui::GetFrameHeightWithSpacing() + ImGui::GetDPIScaledValue(1.5f))};
        if (!m_images.empty()) {
          if (ImGui::BeginChild("##image_picker_list_left", listSize, ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX)) {
            if (ImGui::BeginTable("##image_picker_list_left_contents", 1,
                                  ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY)) {
              ImGui::TableSetupScrollFreeze(1, 0);
              ImGui::TableSetupColumn(trNOOP("File"));
              ImGui::TableNextRow();
              ImGui::TableNextColumn();

              ImGui::BeginDisabled(m_imageDir->isParentDirectory());
              if (ImGui::Selectable("\u21B0 ..", false, static_cast<int>(ImGuiSelectableFlags_SelectOnNav) | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_SelectOnClick)) {
                if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
                  m_imageDir->moveUp();
                  m_images = m_imageDir->getDirectoryContents();
                  m_folderDir = m_imageDir->getDirectories();
                }
              }
              ImGui::EndDisabled();

              for (int i = 0; i < m_folderDir.size(); ++i) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                const auto& folderName = std::format("{} {}##left", ICON_FA_FOLDER_OPEN, m_folderDir[i]);
                if (ImGui::Selectable(folderName.c_str(), m_selectedFolder == i, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick | ImGuiSelectableFlags_SpanAllColumns)) {
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

              ImGui::TableNextRow();
              ImGui::TableNextColumn();
              if (ImGui::SelectableWithBorder(std::format("{}##left", trNOOP("None")).c_str(), m_selectedImage < 0,
                                              ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
                m_selectedImage = -1;
                m_image.reset();
              }
              for (int i = 0; i < m_images.size(); ++i) {
                if (m_filter.empty() == false && m_sortedIndexes.contains(i) == false) {
                  continue;
                }
                const auto& sheet = m_images[i];
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                if (ImGui::Selectable(sheet.c_str(), m_selectedImage == i, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_SelectOnClick)) {
                  m_selectedImage = i;
                  m_image.emplace(m_selectedImage == -1             ? ""
                                  : m_imageDir->isParentDirectory() ? m_images.at(m_selectedImage)
                                                                    : m_imageDir->getPathPrefix() + '/' + m_images.at(m_selectedImage),
                                  m_pickType, false);
                }
                ImGui::SetItemTooltip("%s", sheet.c_str());
                if (m_selectedImage == i) {
                  ImGui::SetItemDefaultFocus();
                }
              }
              ImGui::EndTable();
            }
          }
          ImGui::EndChild();
        }
        if (!m_images_2.empty()) {
          if (ImGui::BeginChild("##image_picker_list_right", listSize, ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX)) {
            if (ImGui::BeginTable("##image_picker_list_right_contents", 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY)) {
              ImGui::TableSetupScrollFreeze(1, 0);
              ImGui::TableSetupColumn(trNOOP("File"));
              ImGui::TableNextRow();
              ImGui::TableNextColumn();

              ImGui::BeginDisabled(m_imageDir2->isParentDirectory());
              if (ImGui::Selectable("\u21B0 ..##right", false, static_cast<int>(ImGuiSelectableFlags_SelectOnNav) | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_SelectOnClick)) {
                if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
                  m_imageDir2->moveUp();
                  m_images_2 = m_imageDir2->getDirectoryContents();
                  m_folderDir_2 = m_imageDir2->getDirectories();
                }
              }
              ImGui::EndDisabled();

              for (int i = 0; i < m_folderDir_2.size(); ++i) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                const auto& folderName = std::format("{} {}##right", ICON_FA_FOLDER_OPEN, m_folderDir_2[i]);
                if (ImGui::Selectable(folderName.c_str(), m_selectedFolder2 == i, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick | ImGuiSelectableFlags_SpanAllColumns)) {
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

              if (ImGui::SelectableWithBorder(std::format("{}##right", trNOOP("None")).c_str(), m_selectedImage2 < 0,
                                              ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
                m_selectedImage2 = -1;
                m_image2.reset();
              }

              for (int i = 0; i < m_images_2.size(); ++i) {
                if (m_filter.empty() == false && m_sortedIndexes2.contains(i) == false) {
                  continue;
                }
                const auto& sheet = m_images_2[i];
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                if (ImGui::Selectable(sheet.c_str(), m_selectedImage2 == i, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_SelectOnClick)) {
                  m_selectedImage2 = i;
                  m_image2.emplace(m_selectedImage2 == -1             ? ""
                                   : m_imageDir2->isParentDirectory() ? m_images_2.at(m_selectedImage2)
                                                                      : m_imageDir2->getPathPrefix() + '/' + m_images_2.at(m_selectedImage2),
                                   m_pickType, true);
                }
                ImGui::SetItemTooltip("%s", sheet.c_str());
                if (m_selectedImage2 == i) {
                  ImGui::SetItemDefaultFocus();
                }
              }
            }
            ImGui::EndTable();
          }
          ImGui::EndChild();
        }
        if (ImGui::BeginChild("##image_picker_image_panel", {-1, ImGui::GetContentRegionAvail().y - (ImGui::GetFrameHeightWithSpacing() + ImGui::GetDPIScaledValue(1.5f))}, ImGuiChildFlags_Border,
                              ImGuiWindowFlags_HorizontalScrollbar)) {
          if (!m_image && m_selectedImage != -1) {
            m_image.emplace(m_selectedImage == -1 ? "" : m_images.at(m_selectedImage), m_pickType, false);
          }
          if (!m_image2 && m_selectedImage2 != -1) {
            m_image2.emplace(m_selectedImage2 == -1 ? "" : m_images.at(m_selectedImage2), m_pickType, true);
          }

          const auto win = ImGui::GetCurrentWindow();
          const int dummyWidth = std::max(m_image ? m_image->imageWidth() : 0, m_image2 ? m_image2->imageWidth() : 0);
          const int dummyHeight = std::max(m_image ? m_image->imageHeight() : 0, m_image2 ? m_image2->imageHeight() : 0);
          if (dummyWidth > 0 && dummyHeight > 0) {
            ImGui::Dummy({static_cast<float>(dummyWidth), static_cast<float>(dummyHeight)});
          }

          if (m_image || m_image2) {
            ImGui::GetWindowDrawList()->AddImage(static_cast<ImTextureID>(m_checkerboardTexture), win->ContentRegionRect.Min + ImVec2{0.f, 0.f},
                                                 win->ContentRegionRect.Min + (ImVec2{static_cast<float>(dummyWidth), static_cast<float>(dummyHeight)}));
          }
          if (m_image) {

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
      ImGui::EndHorizontal();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##image_picker_button_layout", {-1, 0});
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##image_picker_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_filter.clear();
          m_sortedIndexes.clear();
          m_confirmed = true;
          m_open = false;
          ImGui::CloseCurrentPopup();
        } else if (ret == 1) {
          m_filter.clear();
          m_sortedIndexes.clear();
          m_confirmed = false;
          m_open = false;
          ImGui::CloseCurrentPopup();
        }
      }
      ImGui::EndHorizontal();
    }
    ImGui::EndVertical();
    ImGui::EndPopup();
  }
  return {!m_open, m_confirmed};
}
