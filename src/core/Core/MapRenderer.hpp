#pragma once

#include <vector>
#include <algorithm>

#include "Core/Texture.hpp"

#include "Database/Tilesets.hpp"

class Map;
class Tileset;
struct SDL_Texture;

constexpr int FloorAutoTileTable[48][4][2]{
    {{2, 4}, {1, 4}, {2, 3}, {1, 3}}, {{2, 0}, {1, 4}, {2, 3}, {1, 3}}, {{2, 4}, {3, 0}, {2, 3}, {1, 3}},
    {{2, 0}, {3, 0}, {2, 3}, {1, 3}}, {{2, 4}, {1, 4}, {2, 3}, {3, 1}}, {{2, 0}, {1, 4}, {2, 3}, {3, 1}},
    {{2, 4}, {3, 0}, {2, 3}, {3, 1}}, {{2, 0}, {3, 0}, {2, 3}, {3, 1}}, {{2, 4}, {1, 4}, {2, 1}, {1, 3}},
    {{2, 0}, {1, 4}, {2, 1}, {1, 3}}, {{2, 4}, {3, 0}, {2, 1}, {1, 3}}, {{2, 0}, {3, 0}, {2, 1}, {1, 3}},
    {{2, 4}, {1, 4}, {2, 1}, {3, 1}}, {{2, 0}, {1, 4}, {2, 1}, {3, 1}}, {{2, 4}, {3, 0}, {2, 1}, {3, 1}},
    {{2, 0}, {3, 0}, {2, 1}, {3, 1}}, {{0, 4}, {1, 4}, {0, 3}, {1, 3}}, {{0, 4}, {3, 0}, {0, 3}, {1, 3}},
    {{0, 4}, {1, 4}, {0, 3}, {3, 1}}, {{0, 4}, {3, 0}, {0, 3}, {3, 1}}, {{2, 2}, {1, 2}, {2, 3}, {1, 3}},
    {{2, 2}, {1, 2}, {2, 3}, {3, 1}}, {{2, 2}, {1, 2}, {2, 1}, {1, 3}}, {{2, 2}, {1, 2}, {2, 1}, {3, 1}},
    {{2, 4}, {3, 4}, {2, 3}, {3, 3}}, {{2, 4}, {3, 4}, {2, 1}, {3, 3}}, {{2, 0}, {3, 4}, {2, 3}, {3, 3}},
    {{2, 0}, {3, 4}, {2, 1}, {3, 3}}, {{2, 4}, {1, 4}, {2, 5}, {1, 5}}, {{2, 0}, {1, 4}, {2, 5}, {1, 5}},
    {{2, 4}, {3, 0}, {2, 5}, {1, 5}}, {{2, 0}, {3, 0}, {2, 5}, {1, 5}}, {{0, 4}, {3, 4}, {0, 3}, {3, 3}},
    {{2, 2}, {1, 2}, {2, 5}, {1, 5}}, {{0, 2}, {1, 2}, {0, 3}, {1, 3}}, {{0, 2}, {1, 2}, {0, 3}, {3, 1}},
    {{2, 2}, {3, 2}, {2, 3}, {3, 3}}, {{2, 2}, {3, 2}, {2, 1}, {3, 3}}, {{2, 4}, {3, 4}, {2, 5}, {3, 5}},
    {{2, 0}, {3, 4}, {2, 5}, {3, 5}}, {{0, 4}, {1, 4}, {0, 5}, {1, 5}}, {{0, 4}, {3, 0}, {0, 5}, {1, 5}},
    {{0, 2}, {3, 2}, {0, 3}, {3, 3}}, {{0, 2}, {1, 2}, {0, 5}, {1, 5}}, {{0, 4}, {3, 4}, {0, 5}, {3, 5}},
    {{2, 2}, {3, 2}, {2, 5}, {3, 5}}, {{0, 2}, {3, 2}, {0, 5}, {3, 5}}, {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
};

constexpr int WallAutoTileTable[16][4][2]{
    {{2, 2}, {1, 2}, {2, 1}, {1, 1}}, {{0, 2}, {1, 2}, {0, 1}, {1, 1}}, {{2, 0}, {1, 0}, {2, 1}, {1, 1}},
    {{0, 0}, {1, 0}, {0, 1}, {1, 1}}, {{2, 2}, {3, 2}, {2, 1}, {3, 1}}, {{0, 2}, {3, 2}, {0, 1}, {3, 1}},
    {{2, 0}, {3, 0}, {2, 1}, {3, 1}}, {{0, 0}, {3, 0}, {0, 1}, {3, 1}}, {{2, 2}, {1, 2}, {2, 3}, {1, 3}},
    {{0, 2}, {1, 2}, {0, 3}, {1, 3}}, {{2, 0}, {1, 0}, {2, 3}, {1, 3}}, {{0, 0}, {1, 0}, {0, 3}, {1, 3}},
    {{2, 2}, {3, 2}, {2, 3}, {3, 3}}, {{0, 2}, {3, 2}, {0, 3}, {3, 3}}, {{2, 0}, {3, 0}, {2, 3}, {3, 3}},
    {{0, 0}, {3, 0}, {0, 3}, {3, 3}}};

constexpr int WaterfallAutoTileTable[4][4][2]{{{2, 0}, {1, 0}, {2, 1}, {1, 1}},
                                              {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
                                              {{2, 0}, {3, 0}, {2, 1}, {3, 1}},
                                              {{0, 0}, {3, 0}, {0, 1}, {3, 1}}};

class MapRenderer {
public:
  static constexpr int TILE_ID_B = 0;
  static constexpr int TILE_ID_C = 256;
  static constexpr int TILE_ID_D = 512;
  static constexpr int TILE_ID_E = 768;
  static constexpr int TILE_ID_A5 = 1356;
  static constexpr int TILE_ID_A1 = 2048;
  static constexpr int TILE_ID_A2 = 2816;
  static constexpr int TILE_ID_A3 = 4352;
  static constexpr int TILE_ID_A4 = 5888;
  static constexpr int TILE_ID_MAX = 8192;

  void setMap(const Map* map, const Tileset* tilest, int tileWidth = 48, int tileHeight = 48);

  void invalidate(int tileIndex) {
    if (tileIndex < 0 || tileIndex >= m_invalidTiles.size()) {
      return;
    }

    m_invalidTiles[tileIndex] = true;
  }

  bool isInvalid() {
    return std::any_of(m_invalidTiles.begin(), m_invalidTiles.end(), [](const bool v) { return v; });
  }
  void update(int mouseX, int mouseY);

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

  [[nodiscard]] static bool isVisibleTile(int tileId) { return tileId > 0 && tileId < TILE_ID_MAX; }
  [[nodiscard]] static bool isAutoTile(int tileId) { return tileId >= TILE_ID_A1; }
  [[nodiscard]] static int getAutoTileKind(int tileId) { return std::floor((tileId - TILE_ID_A1) / 48); }
  [[nodiscard]] static int getAutoTileShape(int tileId) { return (tileId - TILE_ID_A1) % 48; }
  [[nodiscard]] static int makeAutoTileId(int kind, int shape) { return TILE_ID_A1 + kind * 48 + shape; }
  [[nodiscard]] static bool isSameKindTile(int tileId1, int tileId2) {
    if (isAutoTile(tileId1) && isAutoTile(tileId2)) {
      return getAutoTileKind(tileId1) == getAutoTileKind(tileId2);
    }

    return tileId1 == tileId2;
  }
  [[nodiscard]] static bool isTileA1(int tileId) { return tileId >= TILE_ID_A1 && tileId < TILE_ID_A2; }
  [[nodiscard]] static bool isTileA2(int tileId) { return tileId >= TILE_ID_A2 && tileId < TILE_ID_A3; }
  [[nodiscard]] static bool isTileA3(int tileId) { return tileId >= TILE_ID_A3 && tileId < TILE_ID_A4; }
  [[nodiscard]] static bool isTileA4(int tileId) { return tileId >= TILE_ID_A4 && tileId < TILE_ID_MAX; }
  [[nodiscard]] static bool isTileA5(int tileId) { return tileId >= TILE_ID_A5 && tileId < TILE_ID_A1; }
  [[nodiscard]] static bool isWaterTile(int tileId) {
    if (isTileA1(tileId)) {
      return !(tileId >= TILE_ID_A1 + 96 && tileId < TILE_ID_A1 + 192);
    }
    return false;
  }
  [[nodiscard]] static bool isWaterfallTile(int tileId) {
    if (tileId >= TILE_ID_A1 + 192 && tileId < TILE_ID_A2) {
      return getAutoTileKind(tileId) % 2 == 1;
    }
    return false;
  }

  [[nodiscard]] static bool isGroundTile(int tileId) {
    return isTileA1(tileId) || isTileA2(tileId) || isTileA5(tileId);
  }
  [[nodiscard]] static bool isShadowingTile(int tileId) { return isTileA3(tileId) || isTileA4(tileId); }
  [[nodiscard]] static bool isRoofTile(int tileId) { return isTileA3(tileId) && getAutoTileKind(tileId) % 16 < 8; }
  [[nodiscard]] static bool isWallTopTile(int tileId) { return isTileA4(tileId) && getAutoTileKind(tileId) % 16 < 8; }
  [[nodiscard]] static bool isWallSideTile(int tileId) {
    return (isTileA3(tileId) || isTileA4(tileId)) && getAutoTileKind(tileId) % 16 >= 8;
  }
  [[nodiscard]] static bool isWallTile(int tileId) { return isWallTopTile(tileId) || isWallSideTile(tileId); }
  [[nodiscard]] static bool isFloorTypeAutotile(int tileId) {
    return (isTileA1(tileId) && !isWaterfallTile(tileId)) || isTileA2(tileId) || isWallTopTile(tileId);
  }
  [[nodiscard]] static bool isWallTypeAutotile(int tileId) { return isRoofTile(tileId) || isWallSideTile(tileId); }
  [[nodiscard]] static bool isWaterfallTypeAutotile(int tileId) { return isWaterfallTile(tileId); }

private:
  void draw();
  const Map* m_map = nullptr;
  const Tileset* m_tileset = nullptr;
  std::array<Texture, 9> m_tilesetTextures;
  bool m_isValid = false;
  int m_tileWidth;
  int m_tileHeight;
  SDL_Texture* m_lowerTexture = nullptr;
  SDL_Texture* m_upperTexture = nullptr;
  std::vector<bool> m_invalidTiles;
};
