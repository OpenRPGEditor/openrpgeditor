
#pragma once

#include "Core/Tilemap/ITileView.hpp"
#include "Core/Tilemap/TileRenderHelper.hpp"
#include <array>

#include <string>

class TilemapView : public ITileView {
public:
  TilemapView(const int tileWidth, const int tileHeight) : ITileView(tileWidth, tileHeight), m_renderHelper(tileWidth, tileHeight) {}

  static bool tilesetExists(const std::array<std::string, 9>& tilesetNames, const int idx) { return !tilesetNames[idx].empty(); }

private:
  TileRenderHelper m_renderHelper;
  RenderImage m_lowerTiles;
  RenderImage m_upperTiles;
};
