#include "Database/TileHelper.hpp"

#include <array>
#include <cmath>

namespace TileHelper {

bool isVisibleTile(const int tileId) { return tileId > 0 && tileId < TILE_ID_MAX; }

bool isAutoTile(const int tileId) { return tileId >= TILE_ID_A1; }

int getAutoTileKind(const int tileId) { return std::floor((tileId - TILE_ID_A1) / 48); }

int getAutoTileShape(const int tileId) { return (tileId - TILE_ID_A1) % 48; }

int makeAutoTileId(const int kind, const int shape) { return TILE_ID_A1 + kind * 48 + shape; }

bool isSameKindTile(const int tileId1, const int tileId2) {
  return (isAutoTile(tileId1) && isAutoTile(tileId2) && getAutoTileKind(tileId1) == getAutoTileKind(tileId2)) ||
         tileId1 == tileId2;
}

bool isTileA1(const int tileId) { return tileId >= TILE_ID_A1 && tileId < TILE_ID_A2; }

bool isTileA2(const int tileId) { return tileId >= TILE_ID_A2 && tileId < TILE_ID_A3; }

bool isTileA3(const int tileId) { return tileId >= TILE_ID_A3 && tileId < TILE_ID_A4; }

bool isTileA4(const int tileId) { return tileId >= TILE_ID_A4 && tileId < TILE_ID_MAX; }

bool isTileA5(const int tileId) { return tileId >= TILE_ID_A5 && tileId < TILE_ID_A1; }

bool isTileB(const int tileId) { return tileId >= TILE_ID_B && tileId < TILE_ID_C; }

bool isTileC(const int tileId) { return tileId >= TILE_ID_C && tileId < TILE_ID_D; }

bool isTileD(const int tileId) { return tileId >= TILE_ID_D && tileId < TILE_ID_E; }

bool isTileE(const int tileId) { return tileId >= TILE_ID_E && tileId < TILE_ID_A5; }

bool isWaterTile(const int tileId) {
  return isTileA1(tileId) && !(tileId >= TILE_ID_A1 + 96 && tileId < TILE_ID_A1 + 192);
}

bool isWaterfallTile(const int tileId) {
  return (tileId >= TILE_ID_A1 + 192 && tileId < TILE_ID_A2) && getAutoTileKind(tileId) % 2 == 1;
}

bool isGroundTile(const int tileId) { return isTileA1(tileId) || isTileA2(tileId) || isTileA5(tileId); }

bool isShadowingTile(const int tileId) { return isTileA3(tileId) || isTileA4(tileId); }

bool isRoofTile(const int tileId) { return isTileA3(tileId) && getAutoTileKind(tileId) % 16 < 8; }

bool isWallTopTile(const int tileId) { return isTileA4(tileId) && getAutoTileKind(tileId) % 16 < 8; }

bool isWallSideTile(const int tileId) {
  return (isTileA3(tileId) || isTileA4(tileId)) && getAutoTileKind(tileId) % 16 >= 8;
}

bool isWallTile(const int tileId) { return isWallTopTile(tileId) || isWallSideTile(tileId); }

bool isFloorTypeAutotile(const int tileId) {
  return (isTileA1(tileId) && !isWaterfallTile(tileId)) || isTileA2(tileId) || isWallTopTile(tileId);
}

bool isWallTypeAutotile(const int tileId) { return isRoofTile(tileId) || isWallSideTile(tileId); }

bool isWaterfallTypeAutotile(const int tileId) { return isWaterfallTile(tileId); }

bool shouldCreateBorder(const int currentTileId, const int adjacentTileId) {
  if (adjacentTileId < TILE_ID_B || !isAutoTile(currentTileId)) {
    return false;
  }

  if (!isAutoTile(adjacentTileId)) {
    return true;
  }

  if (!isWaterTile(currentTileId) || !isWaterTile(adjacentTileId)) {
    if (!isWaterfallTile(currentTileId) && !isWaterfallTile(adjacentTileId)) {
      return !isSameKindTile(currentTileId, adjacentTileId);
    }
  }

  return false;
}

int floorDirToShape(const int dir) {
  static constexpr std::array table{
      0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 16, 17, 17, 18, 18, 19, 19, 16, 16,
      17, 17, 18, 18, 19, 19, 20, 20, 20, 20, 21, 21, 21, 21, 22, 22, 22, 22, 23, 23, 23, 23, 34, 34, 34, 34,
      35, 35, 35, 35, 34, 34, 34, 34, 35, 35, 35, 35, 24, 26, 24, 26, 24, 26, 24, 26, 25, 27, 25, 27, 25, 27,
      25, 27, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 36, 36, 36, 36, 36, 36, 36, 36,
      37, 37, 37, 37, 37, 37, 37, 37, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 28, 29,
      30, 31, 28, 29, 30, 31, 28, 29, 30, 31, 28, 29, 30, 31, 40, 40, 41, 41, 40, 40, 41, 41, 40, 40, 41, 41,
      40, 40, 41, 41, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 43, 43, 43, 43, 43, 43,
      43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 38, 39, 38, 39, 38, 39, 38, 39, 38, 39, 38, 39, 38, 39, 38, 39,
      44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
      45, 45, 45, 45, 45, 45, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46,
  };

  return table[dir];
}

int floorShapeToDir(const int shape) {
  static constexpr std::array table{
      0,  1,  2,  3,  4,   5,   6,   7,   8,  9,   10, 11, 12,  13,  14,  15,  25,  27,  29,  31,  35,  39,  43,  47,
      70, 78, 71, 79, 140, 141, 142, 143, 95, 175, 59, 63, 103, 111, 206, 207, 157, 159, 127, 191, 223, 239, 255, 0,
  };

  return table[shape];
}

} // namespace TileHelper