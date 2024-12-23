#pragma once

#include "Core/Tilemap/ITileView.hpp"
#include <array>
#include <string>
#include <vector>

class TilePalette : public ITileView {
public:
  std::vector<int> paletteTiles(int x, int y, int mode, bool checkSpecial) const;

  int pageIndex() const { return m_pageIndex; }
  void setPageIndex(const int index) { m_pageIndex = index; }

  void setTilesetNames(const std::array<std::string, 9>& tilesetNames) { m_tilesetNames = tilesetNames; }

private:
  static std::vector<int> makeTileIdList(int tileId1, int tileId2, int tileId3);
  int m_pageIndex = 0;
  std::array<std::string, 9> m_tilesetNames;
};
