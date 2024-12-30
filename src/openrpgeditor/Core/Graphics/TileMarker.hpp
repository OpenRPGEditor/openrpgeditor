#pragma once

#include "Core/Graphics/Texture.hpp"
#include "Database/Globals.hpp"

struct TileMarker {
  static constexpr int kNumColumns = 4;
  static constexpr int kNumRows = 5;
  TileMarker() = delete;

  explicit TileMarker(TileFlags flags, int pageIndex, int width, int height);
  int imageWidth() const;
  int imageHeight() const;
  ImVec2 uv0() const;
  ImVec2 uv1() const;
  ImVec2 uvTileSize() const;
  ImVec2 rectSize() const;
  ImVec2 passableOffset(bool isPassable, bool hasHigherTile) const;
  void setMarker(TileFlags flags) const;

  explicit operator bool() const { return m_imageTexture.operator bool(); }
  const Texture& texture() const { return m_imageTexture; }

private:
  Texture m_imageTexture;
  TileFlags m_flags;
  ImVec2 m_markerType{0, 0};
  int m_pageIndex{0};
  int m_imageWidth{16};
  int m_imageHeight{16};
};
