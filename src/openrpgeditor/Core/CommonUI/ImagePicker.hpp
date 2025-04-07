#pragma once

#include "Core/CommonUI/IDialogController.hpp"
#include "Core/Graphics/CheckerboardTexture.hpp"
#include "Core/Graphics/Image.hpp"
#include "Core/Graphics/Texture.hpp"
#include "Core/Log.hpp"
#include "Directory.hpp"

#include "Database/Globals.hpp"

namespace fs = std::filesystem;
struct Image;
struct ImagePicker : IDialogController {
  using PickerMode = Image::Mode;

  explicit ImagePicker(PickerMode mode, std::string_view imageName = {}, std::string_view image2Name = {});
  std::tuple<bool, bool> draw() override;
  [[nodiscard]] std::string selectedImage() const {
    return m_selectedImage >= 0 ? m_imageDir.value().isParentDirectory() ? m_images[m_selectedImage] : m_imageDir.value().getPathPrefix() + '/' + m_images[m_selectedImage] : "";
  }

  [[nodiscard]] std::string selectedImage2() const {
    return m_selectedImage2 >= 0 ? m_imageDir2.value().isParentDirectory() ? m_images_2[m_selectedImage2] : m_imageDir2.value().getPathPrefix() + '/' + m_images_2[m_selectedImage2] : "";
  }
  void setImageInfo(std::string_view imageName, std::string_view image2Name = {});

private:
  std::optional<Image> m_image;
  std::optional<Image> m_image2;
  std::vector<std::string> m_images;
  std::vector<std::string> m_images_2;
  std::vector<std::string> m_folderDir;
  std::vector<std::string> m_folderDir_2;
  PickerMode m_pickType;
  CheckerboardTexture m_checkerboardTexture{};
  int m_selectedImage{-1};
  int m_selectedImage2{-1};
  int m_selectedFolder{-1};
  int m_selectedFolder2{-1};

  std::optional<Directory> m_imageDir;
  std::optional<Directory> m_imageDir2;

  std::string m_filter;
  std::map<int, std::string> m_sortedIndexes;
  bool m_sortRequest{false};
};
