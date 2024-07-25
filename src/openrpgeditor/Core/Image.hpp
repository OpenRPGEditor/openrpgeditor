#pragma once

#include "Core/Texture.hpp"
#include "Database/Globals.hpp"

struct Image {
  Image() = delete;

  explicit Image(std::string_view imageName, std::string_view imageName2, int pickType);
  int imageWidth() const;
  int imageHeight() const;

  int image2Width() const;
  int image2Height() const;

  explicit operator bool() const { return m_imageTexture.operator bool(); }
  const Texture& texture() const { return m_imageTexture; }

  const Texture& texture2() const { return m_imageTexture2; }

private:
  Texture m_imageTexture;
  Texture m_imageTexture2;
};
