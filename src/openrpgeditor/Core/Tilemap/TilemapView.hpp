
#pragma once

#include "Database/Globals.hpp"

#include "Core/Tilemap/ITileView.hpp"
#include "Core/Tilemap/TileRenderHelper.hpp"
#include <array>
#include <string>
#include <vector>

class Tileset;
class Map;
class TilemapView : public ITileView {
public:
  TilemapView(const int tileWidth, const int tileHeight)
  : ITileView(tileWidth, tileHeight)
  , m_renderHelper(tileWidth, tileHeight) {
    m_lowerTiles.setFilterMode(RenderImage::FilterMode::Nearest);
    m_belowCharacterEvents.setFilterMode(RenderImage::FilterMode::Nearest);
    m_sameAsCharacterEvents.setFilterMode(RenderImage::FilterMode::Nearest);
    m_belowCharacterEvents.setFilterMode(RenderImage::FilterMode::Nearest);
    m_upperTiles.setFilterMode(RenderImage::FilterMode::Nearest);
  }

  static bool tilesetExists(const std::array<std::string, 9>& tilesetNames, const int idx) { return !tilesetNames[idx].empty(); }

  const Map* map() const { return m_map; }
  void setMap(const Map* map);
  const Tileset* tileset() const { return m_tileset; }
  void setTileset(const Tileset* tileset);
  void renderCharacter(EventPriority priority, const RectF& src, const RectF& dst, const Texture& tex);
  void render();

  [[nodiscard]] const RenderImage& lowerTiles() const { return m_lowerTiles; }
  [[nodiscard]] const RenderImage& belowCharacterEvents() const { return m_belowCharacterEvents; }
  [[nodiscard]] const RenderImage& sameAsCharacterEvents() const { return m_sameAsCharacterEvents; }
  [[nodiscard]] const RenderImage& aboveCharacterEvents() const { return m_belowCharacterEvents; }
  [[nodiscard]] const RenderImage& upperTiles() const { return m_upperTiles; }

  [[nodiscard]] bool isTileDirty(int x, int y) const { return m_dirtyRect.contains({x, y}); }
  void setTileDirty(const int x, const int y) { m_dirtyRect |= Rect(x, y, 1, 1); }
  void clearDirtyRect() { m_dirtyRect = Rect(); }
  void setDirtyRect(const int x, const int y, const int cols, const int rows) { m_dirtyRect |= Rect(x, y, cols, rows); }

  [[nodiscard]] std::vector<std::optional<int>> tilesetFlags() const;
  [[nodiscard]] int tileId(int x, int y, int z) const;
  [[nodiscard]] std::vector<int> layeredTiles(int x, int y) const;
  [[nodiscard]] std::vector<int> allTiles(int x, int y) const;
  [[nodiscard]] bool isHigherTile(int tileId) const;
  [[nodiscard]] bool isTableTile(int tileId) const;
  [[nodiscard]] bool isOverpassPosition(int mx, int my) const;
  [[nodiscard]] bool checkPassage(int x, int y, int bit) const;
  [[nodiscard]] bool isPassable(const int x, const int y, const int d) const { return checkPassage(x, y, (1 << (d / 2 - 1)) & 0x0f); }

private:
  struct TileInfo {
    int x;
    int y;
    int id;
  };

  void paintTiles(int startX, int startY, int x, int y);
  void createRenderImages();

  TileRenderHelper m_renderHelper;
  const Map* m_map = nullptr;
  const Tileset* m_tileset = nullptr;
  std::array<Texture, 9> m_tilesetTextures;
  RenderImage m_lowerTiles;
  RenderImage m_upperTiles;
  RenderImage m_belowCharacterEvents;
  RenderImage m_sameAsCharacterEvents;
  RenderImage m_aboveCharacterEvents;
  Rect m_dirtyRect;
  std::vector<TileInfo> m_lowerTileIds;
  std::vector<TileInfo> m_upperTileIds;
  int m_frameDelay;
};
