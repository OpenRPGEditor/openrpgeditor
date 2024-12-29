#pragma once

#include "Core/Graphics/Texture.hpp"
#include "Database/Globals.hpp"

struct TileMarker {
  TileMarker() = delete;

  explicit TileMarker(int markerType, int width, int height);
  int imageWidth() const;
  int imageHeight() const;
  ImVec2 uv0() const;
  ImVec2 uv1() const;
  ImVec2 uvTileSize() const;
  ImVec2 rectSize() const;

  explicit operator bool() const { return m_imageTexture.operator bool(); }
  const Texture& texture() const { return m_imageTexture; }

private:
  Texture m_imageTexture;
  int m_markerType{0};
  const int m_imageWidth{5};
  const int m_imageHeight{5};
};
