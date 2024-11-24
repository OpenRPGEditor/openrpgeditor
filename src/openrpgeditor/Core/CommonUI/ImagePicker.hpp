#pragma once

#include "Core/CommonUI/IDialogController.hpp"
#include "Core/Graphics/CheckerboardTexture.hpp"
#include "Core/Graphics/Image.hpp"
#include "Core/Graphics/Texture.hpp"
#include "Core/Log.hpp"

#include "Database/Globals.hpp"

struct Image;
struct ImagePicker : IDialogController {
  enum class PickerMode { Battleback, Parallax, Picture };

  explicit ImagePicker(PickerMode mode, std::string_view imageName = {}, std::string_view image2Name = {});
  std::tuple<bool, bool> draw() override;
  [[nodiscard]] std::string selectedImage() const { return m_selectedImage >= 0 ? m_images[m_selectedImage] : ""; }
  [[nodiscard]] std::string selectedImage2() const {
    APP_INFO(std::to_string(m_selectedImage2));
    return m_selectedImage2 >= 0 ? m_images_2[m_selectedImage2] : "";
  }
  void setImageInfo(std::string_view imageName, std::string_view image2Name);

private:
  std::optional<Image> m_image;
  std::optional<Image> m_image2;
  std::vector<std::string> m_images;
  std::vector<std::string> m_images_2;
  PickerMode m_pickType;
  CheckerboardTexture m_checkerboardTexture{};
  int m_selectedImage{-1};
  int m_selectedImage2{-1};
};
