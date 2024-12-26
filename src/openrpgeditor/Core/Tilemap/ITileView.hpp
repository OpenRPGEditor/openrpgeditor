#pragma once

#include "Core/Graphics/RenderImage.hpp"
#include "OREMath/Rect.hpp"

class ITileView {
public:
  ITileView(int tileWidth, int tileHeight, int blockWidth, int blockHeight) : m_tileWidth(tileWidth), m_tileHeight(tileHeight), m_blockWidth(blockWidth), m_blockHeight(blockHeight) {}

  static RenderImage createRenderTexture(int width, int height);
  static void clearRect(RenderImage& painter, const RectF& rect);

  float realTileWidth() const { return m_tileWidth * m_tileScale; }
  float realTileHeight() const { return m_tileHeight * m_tileScale; }

  bool isTileDirty(int x, int y) const { return m_dirtyRect.contains({x, y}); }
  void clearDirtyRect() { m_dirtyRect = Rect(); }
  void setDirtyRect(const int x, const int y, const int width, const int height) { m_dirtyRect |= Rect(x, y, width, height); }

private:
  int m_tileWidth{48};
  int m_tileHeight{48};
  bool m_tileSizeChanged{false};
  int m_blockWidth{96};
  int m_blockHeight{96};
  float m_tileScale{1.f};
  RectF m_viewport;
  Rect m_dirtyRect;
};
