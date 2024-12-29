#include "Core/Tilemap/TilePalette.hpp"

#include "Database/TileHelper.hpp"
#include "TileRenderHelper.hpp"

void TilePalette::setTilesetNames(const std::array<std::string, 9>& tilesetNames) {
  m_tilesetNames = tilesetNames;
  for (int i = 0; i < 9; i++) {
    if (!m_tilesetNames[i].empty()) {
      m_textures[i] = ResourceManager::instance()->loadTilesetImage(m_tilesetNames[i]);
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
}

std::vector<int> TilePalette::paletteTiles(int x, int y, const Tileset::Mode mode, const bool checkSpecial) const {
  if (m_pageIndex == 0) {

    // Offsets in tiles
    constexpr std::array yOffs{
        2, 4, 4, 6, 16,
    };
    constexpr std::array idStarts{
        TileHelper::TILE_ID_A1, TileHelper::TILE_ID_A2, TileHelper::TILE_ID_A3, TileHelper::TILE_ID_A4, TileHelper::TILE_ID_A5,
    };

    for (int i = 0; i < 5; ++i) {
      if (!tilesetExists(i)) {
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

  const int tileId = x + ((m_pageIndex - 1) * 32 + y) * 8;
  printf("%i\n", tileId);
  return makeTileIdList(-1, -1, tileId);
}

std::vector<int> TilePalette::makeTileIdList(int tileId1, int tileId2, int tileId3) { return {tileId1, tileId2, tileId3, -1}; }

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
      if (tilesetExists(i)) {
        maxRows += yOffs[i];
      }
    }
  }

  if (m_maxRows == maxRows) {
    return;
  }

  m_maxRows = maxRows;
  paintTiles();
}

void TilePalette::paintTiles() {
  if (!m_painter.bind()) {
    return;
  }

  for (int y = 0; y < m_maxRows; y++) {
    for (int x = 0; x < kMaxColumns; x++) {
      paintTile(m_painter, {x, y});
    }
  }

  m_painter.unbind();

  if (m_finalResult.bind()) {
    const int width = paletteSize().width() * realTileWidth();
    const int height = paletteSize().height() * realTileHeight();
    const int cols = std::round(static_cast<float>(width) / static_cast<float>(m_checkerboardTexture.width()));
    const int rows = std::round(static_cast<float>(height) / static_cast<float>(m_checkerboardTexture.height()));
    const Rect srcRect{0, 0, m_checkerboardTexture.width(), m_checkerboardTexture.height()};
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        const RectF destRect{static_cast<float>(j * m_checkerboardTexture.width()), static_cast<float>(i * m_checkerboardTexture.height()), static_cast<float>(m_checkerboardTexture.width()),
                             static_cast<float>(m_checkerboardTexture.height())};
        m_finalResult.drawImageRaw(destRect, m_checkerboardTexture.get(), srcRect);
      }
    }

    m_finalResult.drawImageRaw({0, 0, static_cast<float>(width), static_cast<float>(height)}, m_painter.get(), {0, 0, width, height});
    m_finalResult.unbind();
  }
}

void TilePalette::paintTile(RenderImage& image, const Point& point) {
  const RectF rect = {point.x() * realTileWidth(), point.y() * realTileHeight(), realTileWidth(), realTileHeight()};
  m_renderHelper.clearRect(image, rect);
  if (!isPointContained(point)) {
    return;
  }
  if (isMapMode()) {
    for (const auto tiles = paletteTiles(point.x(), point.y(), m_tilesetMode, true); const auto& tile : tiles) {
      m_renderHelper.drawTile(m_painter, rect, tile, m_textures, false);
    }
  } else {
    m_renderHelper.drawRegionTile(m_painter, rect, point.x() + point.y() * kMaxColumns);
  }
}
