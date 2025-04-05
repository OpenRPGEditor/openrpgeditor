#pragma once

#include "Core/Graphics/Texture.hpp"
#include <array>

class RectF;
class RenderImage;
class TileRenderHelper {
public:
  TileRenderHelper(const int tileWidth, const int tileHeight)
  : m_tileWidth(tileWidth)
  , m_tileHeight(tileHeight) {}
  void drawTile(RenderImage& painter, const RectF& rect, int tileId, const std::array<Texture, 9>& images, bool isTable) const;
  void drawNormalTile(RenderImage& painter, const RectF& rect, int tileId, const std::array<Texture, 9>& images) const;
  void drawAutoTile(RenderImage& painter, const RectF& rect, int tileId, const std::array<Texture, 9>& images, bool isTable) const;
  void drawRegionTile(RenderImage& painter, const RectF& rect, int region);
  void clearRect(RenderImage& painter, const RectF& rect) const;

private:
  int m_tileWidth;
  int m_tileHeight;
};
