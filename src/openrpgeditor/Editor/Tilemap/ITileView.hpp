#pragma once

#include <cmath>

#include "Editor/Graphics/RenderImage.hpp"
#include "OREMath/Rect.hpp"

class ITileView {
public:
  ITileView(const int tileWidth, const int tileHeight)
  : m_tileWidth(tileWidth)
  , m_tileHeight(tileHeight) {}

  static RenderImage createRenderTexture(int width, int height);
  static void clearRect(RenderImage& painter, const RectF& rect);

  [[nodiscard]] float realTileWidth() const { return static_cast<float>(m_tileWidth) * m_scale; }
  [[nodiscard]] float realTileHeight() const { return static_cast<float>(m_tileHeight) * m_scale; }
  void setTileSize(const int width = 48, const int height = 48) {
    m_tileWidth = width;
    m_tileHeight = height;
  }

  static Rect createRect(const Point& min, const Point& max) {
    Rect ret;
    int startX = min.x();
    if (max.x() <= min.x()) {
      startX = max.x();
    }

    int startY = min.y();
    if (max.y() <= min.y()) {
      startY = max.y();
    }

    ret.setTopLeft({startX, startY});

    int endX = min.x();
    if (min.x() <= max.x()) {
      endX = max.x();
    }

    int endY = min.y();
    if (min.y() <= max.y()) {
      endY = max.y();
    }

    ret.setBottomRight({endX, endY});

    return ret;
  }

  [[nodiscard]] Point pixelPointToTilePoint(const PointF& point) const {
    return {static_cast<int>(std::floor(point.x() / realTileWidth())), static_cast<int>(std::floor(point.y() / realTileHeight()))};
  }

private:
  float m_scale = 1.f;
  int m_tileWidth{48};
  int m_tileHeight{48};
  bool m_tileSizeChanged{false};
};
