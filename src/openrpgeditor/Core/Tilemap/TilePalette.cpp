#include "Core/Tilemap/TilePalette.hpp"

#include "Database/TileHelper.hpp"

std::vector<int> TilePalette::paletteTiles(int x, int y, const int mode, const bool checkSpecial) const {
  if (m_pageIndex == 0) {

    // Offsets in tiles
    constexpr std::array yOffs{
        2, 4, 4, 6, 16,
    };
    constexpr std::array idStarts{
        TileHelper::TILE_ID_A1, TileHelper::TILE_ID_A2, TileHelper::TILE_ID_A3, TileHelper::TILE_ID_A4, TileHelper::TILE_ID_A5,
    };

    for (int i = 0; i < 5; ++i) {
      if (i >= m_tilesetNames.size()) {
        continue;
      }

      if (m_tilesetNames[i].empty()) {
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
          case 0:
            if ((x & ~2) == 1) {
              return makeTileIdList(firstId - 48, secondId, 0);
            }
          case 1:
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

  int tileId = x + ((m_pageIndex - 1) * 32 + y) * 8;
  printf("%i\n", tileId);
  return makeTileIdList(-1, -1, tileId);
}

std::vector<int> TilePalette::makeTileIdList(int tileId1, int tileId2, int tileId3) { return {tileId1, tileId2, tileId3, -1}; }