#pragma once

#include "Core/CommonUI/IDialogController.hpp"
#include "Core/Texture.hpp"
#include "Core/CheckerboardTexture.hpp"
#include "Database/Globals.hpp"
#include "Core/Image.hpp"
struct Image;
struct ImagePicker : IDialogController {
    enum class PickerMode {
      Battleback,
      Parallax,
      Picture
    };

  explicit ImagePicker(PickerMode mode, std::string_view imageName = {}, std::string_view image2Name = {});
  std::tuple<bool, bool> draw() override;
  [[nodiscard]] std::string selectedImage() const {
    return m_selectedImage >= 0 ? m_images[m_selectedImage] : "";
  }
  void setImageInfo(std::string_view imageName, std::string_view image2Name);

private:

  std::optional<Image> m_image;
  std::vector<std::string> m_images;
  std::vector<std::string> m_images_2;
  PickerMode m_pickType;
  CheckerboardTexture m_checkerboardTexture{};
  int m_selectedImage{-1};
};
