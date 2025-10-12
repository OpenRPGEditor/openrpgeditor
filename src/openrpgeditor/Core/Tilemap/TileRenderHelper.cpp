#include "Core/Tilemap/TileRenderHelper.hpp"

#include "Core/Graphics/RenderImage.hpp"
#include "Database/TileHelper.hpp"
#include "OREMath/Color.hpp"
#include <math.h>

namespace {
constexpr std::array<std::array<std::array<int, 2>, 4>, 48> FloorAutoTileTable{{
    {{{2, 4}, {1, 4}, {2, 3}, {1, 3}}}, {{{2, 0}, {1, 4}, {2, 3}, {1, 3}}}, {{{2, 4}, {3, 0}, {2, 3}, {1, 3}}}, {{{2, 0}, {3, 0}, {2, 3}, {1, 3}}}, {{{2, 4}, {1, 4}, {2, 3}, {3, 1}}},
    {{{2, 0}, {1, 4}, {2, 3}, {3, 1}}}, {{{2, 4}, {3, 0}, {2, 3}, {3, 1}}}, {{{2, 0}, {3, 0}, {2, 3}, {3, 1}}}, {{{2, 4}, {1, 4}, {2, 1}, {1, 3}}}, {{{2, 0}, {1, 4}, {2, 1}, {1, 3}}},
    {{{2, 4}, {3, 0}, {2, 1}, {1, 3}}}, {{{2, 0}, {3, 0}, {2, 1}, {1, 3}}}, {{{2, 4}, {1, 4}, {2, 1}, {3, 1}}}, {{{2, 0}, {1, 4}, {2, 1}, {3, 1}}}, {{{2, 4}, {3, 0}, {2, 1}, {3, 1}}},
    {{{2, 0}, {3, 0}, {2, 1}, {3, 1}}}, {{{0, 4}, {1, 4}, {0, 3}, {1, 3}}}, {{{0, 4}, {3, 0}, {0, 3}, {1, 3}}}, {{{0, 4}, {1, 4}, {0, 3}, {3, 1}}}, {{{0, 4}, {3, 0}, {0, 3}, {3, 1}}},
    {{{2, 2}, {1, 2}, {2, 3}, {1, 3}}}, {{{2, 2}, {1, 2}, {2, 3}, {3, 1}}}, {{{2, 2}, {1, 2}, {2, 1}, {1, 3}}}, {{{2, 2}, {1, 2}, {2, 1}, {3, 1}}}, {{{2, 4}, {3, 4}, {2, 3}, {3, 3}}},
    {{{2, 4}, {3, 4}, {2, 1}, {3, 3}}}, {{{2, 0}, {3, 4}, {2, 3}, {3, 3}}}, {{{2, 0}, {3, 4}, {2, 1}, {3, 3}}}, {{{2, 4}, {1, 4}, {2, 5}, {1, 5}}}, {{{2, 0}, {1, 4}, {2, 5}, {1, 5}}},
    {{{2, 4}, {3, 0}, {2, 5}, {1, 5}}}, {{{2, 0}, {3, 0}, {2, 5}, {1, 5}}}, {{{0, 4}, {3, 4}, {0, 3}, {3, 3}}}, {{{2, 2}, {1, 2}, {2, 5}, {1, 5}}}, {{{0, 2}, {1, 2}, {0, 3}, {1, 3}}},
    {{{0, 2}, {1, 2}, {0, 3}, {3, 1}}}, {{{2, 2}, {3, 2}, {2, 3}, {3, 3}}}, {{{2, 2}, {3, 2}, {2, 1}, {3, 3}}}, {{{2, 4}, {3, 4}, {2, 5}, {3, 5}}}, {{{2, 0}, {3, 4}, {2, 5}, {3, 5}}},
    {{{0, 4}, {1, 4}, {0, 5}, {1, 5}}}, {{{0, 4}, {3, 0}, {0, 5}, {1, 5}}}, {{{0, 2}, {3, 2}, {0, 3}, {3, 3}}}, {{{0, 2}, {1, 2}, {0, 5}, {1, 5}}}, {{{0, 4}, {3, 4}, {0, 5}, {3, 5}}},
    {{{2, 2}, {3, 2}, {2, 5}, {3, 5}}}, {{{0, 2}, {3, 2}, {0, 5}, {3, 5}}}, {{{0, 0}, {1, 0}, {0, 1}, {1, 1}}},
}};

constexpr std::array<std::array<std::array<int, 2>, 4>, 48> WallAutoTileTable{{{{{2, 2}, {1, 2}, {2, 1}, {1, 1}}},
                                                                               {{{0, 2}, {1, 2}, {0, 1}, {1, 1}}},
                                                                               {{{2, 0}, {1, 0}, {2, 1}, {1, 1}}},
                                                                               {{{0, 0}, {1, 0}, {0, 1}, {1, 1}}},
                                                                               {{{2, 2}, {3, 2}, {2, 1}, {3, 1}}},
                                                                               {{{0, 2}, {3, 2}, {0, 1}, {3, 1}}},
                                                                               {{{2, 0}, {3, 0}, {2, 1}, {3, 1}}},
                                                                               {{{0, 0}, {3, 0}, {0, 1}, {3, 1}}},
                                                                               {{{2, 2}, {1, 2}, {2, 3}, {1, 3}}},
                                                                               {{{0, 2}, {1, 2}, {0, 3}, {1, 3}}},
                                                                               {{{2, 0}, {1, 0}, {2, 3}, {1, 3}}},
                                                                               {{{0, 0}, {1, 0}, {0, 3}, {1, 3}}},
                                                                               {{{2, 2}, {3, 2}, {2, 3}, {3, 3}}},
                                                                               {{{0, 2}, {3, 2}, {0, 3}, {3, 3}}},
                                                                               {{{2, 0}, {3, 0}, {2, 3}, {3, 3}}},
                                                                               {{{0, 0}, {3, 0}, {0, 3}, {3, 3}}}}};

constexpr std::array<std::array<std::array<int, 2>, 4>, 48> WaterfallAutoTileTable{{
    {{{2, 0}, {1, 0}, {2, 1}, {1, 1}}},
    {{{0, 0}, {1, 0}, {0, 1}, {1, 1}}},
    {{{2, 0}, {3, 0}, {2, 1}, {3, 1}}},
    {{{0, 0}, {3, 0}, {0, 1}, {3, 1}}},
}};
} // namespace

void TileRenderHelper::drawShadowTile(RenderImage& painter, const RectF& rect, const int flags) const {

  if (!(flags & 0xF)) {
    return;
  }

  const float w1 = m_tileWidth / 2;
  const float h1 = m_tileHeight / 2;

  for (int i = 0; i < 4; ++i) {
    if (!(flags & (1 << i))) {
      continue;
    }

    const float dx1 = rect.x() + ((i % 2) * w1);
    const float dy1 = rect.y() + (std::floor(i / 2) * h1);
    painter.fillRect({dx1, dy1, w1, h1}, Color(0, 0, 0, 127));
  }
}

void TileRenderHelper::drawTile(RenderImage& painter, const RectF& rect, const int tileId, const std::array<Texture, 9>& images, const bool isTable) const {
  if (!TileHelper::isVisibleTile(tileId)) {
    return;
  }

  if (!TileHelper::isAutoTile(tileId)) {
    drawNormalTile(painter, rect, tileId, images);
  } else {
    drawAutoTile(painter, rect, tileId, images, isTable);
  }
}

void TileRenderHelper::drawNormalTile(RenderImage& painter, const RectF& rect, int tileId, const std::array<Texture, 9>& images) const {
  int setNumber = 0;

  if (TileHelper::isTileA5(tileId)) {
    setNumber = 4;
  } else {
    setNumber = 5 + static_cast<int>(floor(tileId / 256));
  }
  int w = m_tileWidth;
  int h = m_tileHeight;
  float sx = (fmod(floor(tileId / 128), 2) * 8 + (tileId % 8)) * w;
  float sy = fmod(floor(tileId % 256 / 8), 16) * h;
  painter.drawImage(rect, images[setNumber], {static_cast<int>(sx), static_cast<int>(sy), w, h});
}

void TileRenderHelper::drawAutoTile(RenderImage& painter, const RectF& rect, int tileId, const std::array<Texture, 9>& images, const bool isTable) const {
  const int dx = rect.left();
  const int dy = rect.top();
  auto autoTileTable = FloorAutoTileTable;
  const int kind = TileHelper::getAutoTileKind(tileId);
  const int shape = TileHelper::getAutoTileShape(tileId);
  const float tx = kind % 8;
  const float ty = floor(kind / 8);
  float bx = 0;
  float by = 0;
  int setNumber = 0;
  float animX = 0;
  float animY = 0;

  if (TileHelper::isTileA1(tileId)) {
    setNumber = 0;
    if (kind == 0) {
      animX = 2;
      bx = 0;
    } else if (kind == 1) {
      animX = 2;
      by = 3;
    } else if (kind == 2) {
      bx = 6;
      by = 0;
    } else if (kind == 3) {
      bx = 6;
      by = 3;
    } else {
      bx = floor(tx / 4) * 8;
      by = ty * 6 + fmod(floor(tx / 2), 2) * 3;
      if ((kind % 2) == 0) {
        animX = 2;
      } else {
        bx += 6;
        autoTileTable = WaterfallAutoTileTable;
        animY = 1;
      }
    }
  } else if (TileHelper::isTileA2(tileId)) {
    setNumber = 1;
    bx = tx * 2;
    by = (ty - 2) * 3;
  } else if (TileHelper::isTileA3(tileId)) {
    setNumber = 2;
    bx = tx * 2;
    by = (ty - 6) * 2;
    autoTileTable = WallAutoTileTable;
  } else if (TileHelper::isTileA4(tileId)) {
    setNumber = 3;
    bx = tx * 2;
    by = floor((ty - 10) * 2.5 + (fmod(ty, 2) == 1 ? 0.5f : 0));
    if (fmod(ty, 2) == 1) {
      autoTileTable = WallAutoTileTable;
    }
  }

  const auto& table = autoTileTable[shape];
  const int w1 = m_tileWidth / 2;
  const int h1 = m_tileHeight / 2;

  for (int i = 0; i < 4; i++) {
    const int qsx = table[i][0];
    const int qsy = table[i][1];

    const float sx1 = (bx * 2 + qsx) * w1;
    const float sy1 = (by * 2 + qsy) * h1;
    const float dx1 = dx + (i % 2) * w1;
    float dy1 = dy + static_cast<int>(floor(i / 2)) * h1;
    if (isTable && (qsy == 1 || qsy == 5)) {
      float qsx2 = qsx;
      constexpr float qsy2 = 3;
      if (qsy == 1) {
        //          static const int tbl[4] {0, 3, 2, 1};
        //          qsx2 = tbl[qsx];
        qsx2 = fmod(4 - qsx, 4);
      }

      const int sx2 = (bx * 2 + qsx2) * w1;
      const int sy2 = (by * 2 + qsy2) * h1;
      painter.drawImage({dx1, dy1, static_cast<float>(w1), static_cast<float>(h1)}, images[setNumber], {sx2, sy2, w1, h1}); //, tileId, sx2, sy2, dx1, dy1, w1, h1, animX, animY);
      dy1 += h1 / 2;
      painter.drawImage({dx1, dy1 + h1 / 2, static_cast<float>(w1), static_cast<float>(h1 / 2)}, images[setNumber],
                        {static_cast<int>(static_cast<float>(sx1)), static_cast<int>(static_cast<float>(sy1)), w1, h1 / 2}); //, tileId, sx1, sy1, dx1, dy1 + h1 / 2, w1, h1 / 2, animX, animY);
    } else {
      painter.drawImage({dx1, dy1, static_cast<float>(w1), static_cast<float>(h1)}, images[setNumber],
                        {static_cast<int>(sx1), static_cast<int>(sy1), w1, h1}); //, tileId, sx1, sy1, dx1, dy1, w1, h1, animX, animY);
    }
  }
}

void TileRenderHelper::drawRegionTile(RenderImage& painter, const RectF& rect, int region) {
  if (region == 0) {
    painter.fillRect(rect, Color(255, 255, 255, 64));
  } else {
    Color color = Color::fromHsl(((region + 11) * 30) % 360, 160, 96, 160);
    painter.fillRect(rect, color);
  }
}

void TileRenderHelper::clearRect(RenderImage& painter, const RectF& rect) const {
  const auto oldMode = painter.compositionMode();
  painter.setCompositionMode(RenderImage::CompositionMode::Clear);
  painter.fillRect(rect, Color(255, 255, 255, 255));
  painter.setCompositionMode(oldMode);
}
