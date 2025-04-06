#include "Core/Tilemap/TilePalette.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/Tilemap/TileRenderHelper.hpp"

#include "Database/TileHelper.hpp"

void TilePalette::setTilesetNames(const std::array<std::string, 9> &tilesetNames) {
  m_tilesetNames = tilesetNames;
  for (int i = 0; i < 9; i++) {
    if (!m_tilesetNames[i].empty()) {
      m_textures[i] = ResourceManager::instance()->loadTilesetImage(m_tilesetNames[i]);
    } else {
      m_textures[i].invalidate();
    }
  }
  updateMaxRows();
  updateRenderTexture();
}

void TilePalette::updateRenderTexture() {
  const int width = paletteSize().width() * realTileWidth();
  const int height = paletteSize().height() * realTileHeight();

  m_painter.setSize(width, height);
  m_finalResult.setSize(width, height);
  m_checkerboardTexture.setSize(width, height);
}

std::array<int, 4> TilePalette::paletteTiles(int x, int y, const int page,
                                             const std::array<std::string, 9> &tilesetNames, const Tileset::Mode mode,
                                             const bool checkSpecial) {
  if (page == 0) {
    // Offsets in tiles
    constexpr std::array yOffs{
      2, 4, 4, 6, 16,
    };
    constexpr std::array idStarts{
      TileHelper::TILE_ID_A1, TileHelper::TILE_ID_A2, TileHelper::TILE_ID_A3, TileHelper::TILE_ID_A4,
      TileHelper::TILE_ID_A5,
    };

    for (int i = 0; i < 5; ++i) {
      if (!tilesetExists(tilesetNames, i)) {
        continue;
      }

      if (y < yOffs[i]) {
        const int col = (x + y * 8);
        const int firstId = (i == 4 ? 1 : 48) * col + idStarts[i];
        int secondId;
        if (checkSpecial) {
          if (TileHelper::isFloorTypeAutotile(firstId)) {
            secondId = firstId + 47;
          } else if (TileHelper::isWallTypeAutotile(firstId)) {
            secondId = firstId + 15;
          } else if (TileHelper::isWaterfallTypeAutotile(firstId)) {
            secondId = firstId + 3;
          } else {
            secondId = firstId;
          }
        } else {
          secondId = firstId;
        }

        if (TileHelper::isTileA1(secondId)) {
          if (col - 1 < 3) {
            return makeTileIdList(TileHelper::TILE_ID_A1, secondId, 0);
          }
        } else if (TileHelper::isTileA2(secondId)) {
          switch (mode) {
            case Tileset::Mode::World:
              if ((x & ~2) == 1) {
                return makeTileIdList(firstId - 48, secondId, 0);
              }
            case Tileset::Mode::Area:
              if (x > 3) {
                return makeTileIdList(-1, secondId, 0);
              }
              break;
            default:
              break;
          }
        }
        return makeTileIdList(secondId, 0, 0);
      }
      y -= yOffs[i];
    }
    return makeTileIdList(0, 0, 0);
  }

  const int tileId = x + ((page - 1) * 32 + y) * 8;
  return makeTileIdList(-1, -1, tileId);
}

std::array<int, 4> TilePalette::regionTiles(const Point &point) { return {getRegionNumber(point), -1, -1, -1}; }

std::array<int, 4> TilePalette::makeTileIdList(const int tileId1, const int tileId2, const int tileId3) {
  return {tileId1, tileId2, tileId3, -1};
}

void TilePalette::updateMaxRows() {
  if (!isMapMode()) {
    m_maxRows = isRegionMode() ? 32 : 0;
    return;
  }

  int maxRows = 32;

  if (m_pageIndex == 0) {
    maxRows = 0;
    static constexpr std::array yOffs{2, 4, 4, 6, 16};

    for (int i = 0; i < 5; ++i) {
      if (tilesetExists(m_tilesetNames, i)) {
        maxRows += yOffs[i];
      }
    }
  }

  if (m_maxRows == maxRows) {
    return;
  }

  m_maxRows = maxRows;
}

void TilePalette::eraseCursor() { setCursorRect({}); }

void TilePalette::setCursorRect(const Rect &rect) {
  if (m_cursorRect == rect) {
    return;
  }

  m_cursorRect = rect;

  updateCursorPixelRect();
}

void TilePalette::updateCursorPixelRect() {
  const Rect rect(static_cast<int>(realTileWidth() * m_cursorRect.left()),
                  static_cast<int>(realTileHeight() * m_cursorRect.top()),
                  static_cast<int>(m_cursorRect.width() * realTileWidth()),
                  static_cast<int>(m_cursorRect.height() * realTileHeight()));
  if (rect == m_cursorPixelRect) {
    return;
  }
  m_cursorPixelRect = rect;
}

void TilePalette::updatePick(const Point &point) {
  setCursorPage(m_pageIndex);
  setCursorRect(createRect(m_pickPoint, point) & paletteRect());
}

void TilePalette::setPenData(const Size &size, const std::vector<std::array<int, 4> > &data) {
  m_penData = data;
  m_penSize = size;
}

void TilePalette::onCursorDrag(const PointF &point) {
  if (!m_isPicking) {
    return;
  }

  updatePick(pixelPointToTilePoint(point));
}

void TilePalette::onCursorClicked(const PointF &point) {
  const Point p = pixelPointToTilePoint(point);
  if (isPointContained(p)) {
    startPick(p);
  }
}

void TilePalette::onCursorReleased() {
  if (!m_isPicking) {
    return;
  }
  endPick();
}

void TilePalette::endPick() {
  std::vector<std::array<int, 4> > penData;
  int startX = m_cursorRect.left();
  int endX = m_cursorRect.right();
  int startY = m_cursorRect.top();
  int endY = m_cursorRect.bottom();

  if (startY <= endY) {
    startX = m_cursorRect.left();
    endX = m_cursorRect.right();
    for (; startY <= endY; ++startY) {
      for (; startX <= endX; ++startX) {
        if (isRegionMode()) {
          penData.emplace_back(regionTiles({startX, startY}));
        } else {
          penData.emplace_back(paletteTiles(startX, startY, m_pageIndex, m_tilesetNames, m_tilesetMode, false));
        }
        endX = m_cursorRect.right();
      }
      startX = m_cursorRect.left();
      endY = m_cursorRect.bottom();
    }
    startY = m_cursorRect.top();
  }
  setPenData({(endX - startX) + 1, (endY - startY) + 1}, penData);
  m_isPicking = false;
}

void TilePalette::paintTiles() {
  if (!m_painter.lock()) {
    return;
  }

  for (int y = 0; y < m_maxRows; y++) {
    for (int x = 0; x < kMaxColumns; x++) {
      paintTile(m_painter, {x, y});
    }
  }

  m_painter.unlock();
  if (m_finalResult.lock()) {
    const int width = (paletteSize().width() * realTileWidth());
    const int height = (paletteSize().height() * realTileHeight());
    m_finalResult.drawImageRaw({0, 0, static_cast<float>(width), static_cast<float>(height)},
                               m_checkerboardTexture.get(), {0, 0, width, height});
    m_finalResult.drawImageRaw({0, 0, static_cast<float>(width), static_cast<float>(height)}, m_painter.get(),
                               {0, 0, width, height});
    m_finalResult.unlock();
  }
}

void TilePalette::paintTile(RenderImage &image, const Point &point) {
  const RectF rect = {(point.x() * realTileWidth()), (point.y() * realTileHeight()), realTileWidth(), realTileHeight()};
  m_renderHelper.clearRect(image, rect);
  if (!isPointContained(point)) {
    return;
  }
  if (isMapMode()) {
    for (const auto tiles = paletteTiles(point.x(), point.y(), m_pageIndex, m_tilesetNames, m_tilesetMode, true); const
         auto &tile: tiles) {
      m_renderHelper.drawTile(m_painter, rect, tile, m_textures, false);
    }
  } else {
    m_renderHelper.drawRegionTile(m_painter, rect, point.x() + point.y() * kMaxColumns);
  }
}
