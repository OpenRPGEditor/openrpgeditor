#pragma once

#include "Core/Graphics/Texture.hpp"
#include "Database/Globals.hpp"

struct Image {
  enum class Mode {
    Battleback,
    Parallax,
    Picture,
    Title,
    Tileset,
    Animation,
    SVBattler,
  };

  Image() = delete;

  explicit Image(std::string_view imageName, Mode pickType, bool secondImage);
  int imageWidth() const;
  int imageHeight() const;
  std::string imageName() const;

  explicit operator bool() const { return m_imageTexture.operator bool(); }
  const Texture& texture() const { return m_imageTexture; }

private:
  std::string m_imageName;
  Texture m_imageTexture;
};
