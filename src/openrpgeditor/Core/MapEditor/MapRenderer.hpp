#pragma once

#include <vector>
#include <algorithm>

#include "Core/Texture.hpp"
#include "Database/Map.hpp"

#include "Database/Tilesets.hpp"
#include "Database/TileHelper.hpp"

class Map;
class Tileset;
struct SDL_Texture;

struct TileRect {
  int tileId;
  float u;
  float v;
  float x;
  float y;
  int tileWidth;
  int tileHeight;
  int tileSheet;
  float animX;
  float animY;
};

class MapRenderer {
public:
  struct TileLayer {
    Texture tex;
    std::vector<TileRect> rects;
  };

  struct MapLayer {
    std::vector<TileLayer> tileLayers;
    void addRect(int setId, int tileId, float u, float v, float x, float y, int tileWidth, int tileHeight,
                 float animX = 0.f, float animY = 0.f) {
      tileLayers[setId].rects.push_back({tileId, u, v, x, y, tileWidth, tileHeight, setId, animX, animY});
    }
  };

  MapRenderer();
  void setMap(const Map* map, const Tileset* tilest, int tileWidth = 48, int tileHeight = 48);

  void update();

  int tileId(int x, int y, int z) const;
  bool isOverworld() const { return m_tileset && m_tileset->mode == Tileset::Mode::World; }

  [[nodiscard]] std::vector<int> tilesetFlags() const {
    if (m_tileset) {
      return m_tileset->flags;
    }
    return {};
  }

  [[nodiscard]] std::vector<int> layeredTiles(const int x, const int y) const;
  [[nodiscard]] std::vector<int> allTiles(const int x, const int y) const;
  [[nodiscard]] int autoTileType(const int x, const int y, const int z) const;
  [[nodiscard]] bool checkPassage(const int x, const int y, const int bit) const;
  [[nodiscard]] bool isPassable(const int x, const int y, const int d) const;

  [[nodiscard]] int tileIdFromCoords(int x, int y, int z) { return (z * m_map->height + y) * m_map->width + x; }

  [[nodiscard]] bool isHigherTile(int tileId) { return m_tileset->flags[tileId] & 0x10; }

  [[nodiscard]] bool isTableTile(int tileId) { return TileHelper::isTileA2(tileId) && m_tileset->flags[tileId] & 0x80; }

  [[nodiscard]] bool isOverpassPosition(int mx, int my) { return false; }

  void paintTiles(int startX, int startY, int x, int y);

  const Tileset* tileset() const { return m_tileset; }

  std::array<MapLayer, 6> m_lowerLayers;
  std::array<MapLayer, 6> m_upperLayers;

private:
  bool m_frameUpdated = true;
  void drawTile(MapLayer& layer, int tileId, int dx, int dy);
  void drawAutoTile(MapLayer& layer, int tileId, int dx, int dy);
  void drawNormalTile(MapLayer& layer, int tileId, int dx, int dy) const;
  void drawTableEdge(MapLayer& layer, int tileId, int dx, int dy) const;
  void beginBlit(SDL_Texture* source);
  void blitImage(SDL_Texture* bitmap, int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh);
  void endBlit();
  void clearRect(SDL_Texture* clr, int x, int y, int w, int h);

  const Map* m_map = nullptr;
  const Tileset* m_tileset = nullptr;
  bool m_isValid = false;
  int m_tileWidth{48};
  int m_tileHeight{48};
};
