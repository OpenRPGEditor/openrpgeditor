#pragma once

#include "Core/Graphics/Texture.hpp"
#include "Database/Globals.hpp"

struct Image {
  Image() = delete;

  explicit Image(std::string_view imageName, int pickType, bool secondImage);
  int imageWidth() const;
  int imageHeight() const;

  explicit operator bool() const { return m_imageTexture.operator bool(); }
  const Texture& texture() const { return m_imageTexture; }

private:
  Texture m_imageTexture;
};
