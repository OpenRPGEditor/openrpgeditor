#pragma once

#include "Directory.hpp"
#include "Editor/CommonUI/IDialogController.hpp"
#include "Editor/Graphics/CheckerboardTexture.hpp"
#include "Editor/Graphics/Image.hpp"
#include "Editor/Graphics/Texture.hpp"
#include "Editor/Log.hpp"

#include "Database/Globals.hpp"
#include <set>

namespace fs = std::filesystem;
struct Image;
struct ImagePicker : ITypedDialogController<ImagePicker> {
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

  PickerMode mode() const { return m_pickType; }

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
  std::string m_filter2;
  std::set<int> m_sortedIndexes;
  std::set<int> m_sortedIndexes2;
  bool m_sortRequest{false};
  bool m_sortRequest2{false};
};
