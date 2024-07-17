#pragma once

namespace TileHelper {
constexpr int TILE_ID_B = 0;
constexpr int TILE_ID_C = 256;
constexpr int TILE_ID_D = 512;
constexpr int TILE_ID_E = 768;
constexpr int TILE_ID_A5 = 1356;
constexpr int TILE_ID_A1 = 2048;
constexpr int TILE_ID_A2 = 2816;
constexpr int TILE_ID_A3 = 4352;
constexpr int TILE_ID_A4 = 5888;
constexpr int TILE_ID_MAX = 8192;

[[nodiscard]] bool isVisibleTile(int tileId);
[[nodiscard]] bool isAutoTile(int tileId);
[[nodiscard]] int getAutoTileKind(int tileId);
[[nodiscard]] int getAutoTileShape(int tileId);
[[nodiscard]] int makeAutoTileId(int kind, int shape);
[[nodiscard]] bool isSameKindTile(int tileId1, int tileId2);
[[nodiscard]] bool isTileA1(int tileId);
[[nodiscard]] bool isTileA2(int tileId);
[[nodiscard]] bool isTileA3(int tileId);
[[nodiscard]] bool isTileA4(int tileId);
[[nodiscard]] bool isTileA5(int tileId);
[[nodiscard]] bool isTileB(int tileId);
[[nodiscard]] bool isTileC(int tileId);
[[nodiscard]] bool isTileD(int tileId);
[[nodiscard]] bool isTileE(int tileId);
[[nodiscard]] bool isWaterTile(int tileId);
[[nodiscard]] bool isWaterfallTile(int tileId);
[[nodiscard]] bool isGroundTile(int tileId);
[[nodiscard]] bool isShadowingTile(int tileId);
[[nodiscard]] bool isRoofTile(int tileId);
[[nodiscard]] bool isWallTopTile(int tileId);
[[nodiscard]] bool isWallSideTile(int tileId);
[[nodiscard]] bool isWallTile(int tileId);
[[nodiscard]] bool isFloorTypeAutotile(int tileId);
[[nodiscard]] bool isWallTypeAutotile(int tileId);
[[nodiscard]] bool isWaterfallTypeAutotile(int tileId);
[[nodiscard]] bool shouldCreateBorder(int currentTileId, int adjacentTileId);

[[nodiscard]] int floorDirToShape(int dir);
[[nodiscard]]int floorShapeToDir(int shape);
} // namespace TileHelper
