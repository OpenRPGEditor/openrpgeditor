#pragma once

#include "Core/Graphics/RenderImage.hpp"
#include "OREMath/Rect.hpp"

class ITileView {
public:
  ITileView() = default;

  static RenderImage createRenderTexture(int width, int height);
  static void clearRect(RenderImage& painter, const RectF& rect);

  bool isTileDirty(int x, int y) const { return m_dirtyRect.contains({x, y}); }
  void clearDirtyRect() { m_dirtyRect = Rect(); }
  void setDirtyRect(const int x, const int y, const int width, const int height) { m_dirtyRect |= Rect(x, y, width, height); }

private:
  bool m_tileSizeChanged{false};
  int m_blockWidth{96};
  int m_blockHeight{96};
  int m_tileWidth{48};
  int m_tileHeight{48};
  float m_tileScale{1.f};
  RectF m_viewport;
  Rect m_dirtyRect;
};
