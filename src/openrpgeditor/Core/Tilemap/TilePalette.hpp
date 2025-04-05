#pragma once

#include "Core/Graphics/CheckerboardTexture.hpp"
#include "Core/Tilemap/ITileView.hpp"
#include "Database/Tileset.hpp"
#include "TileRenderHelper.hpp"

#include <array>
#include <string>
#include <vector>

class TilePalette : public ITileView {
public:
  static constexpr int kMaxColumns = 8;
  static std::array<int, 4> paletteTiles(int x, int y, int page, const std::array<std::string, 9>& tilesetNames, Tileset::Mode mode, bool checkSpecial);
  static std::array<int, 4> regionTiles(const Point& point);

  explicit TilePalette(const int tileWidth = 48, const int tileHeight = 48)
  : ITileView(tileWidth, tileHeight)
  , m_renderHelper(tileWidth, tileHeight)
  , m_checkerboardTexture(32, 32) {
    updateCursorPixelRect();
    updateMaxRows();
    updateRenderTexture();
  }
  void setMode(const Tileset::Mode mode) { m_tilesetMode = mode; }
  Tileset::Mode mode() const { return m_tilesetMode; }

  void setRegionMode() { m_isRegionMode = true; }
  void setMapMode() { m_isRegionMode = false; }
  bool isMapMode() const { return !m_isRegionMode; }
  bool isRegionMode() const { return m_isRegionMode; }

  int pageIndex() const { return m_pageIndex; }
  void setPageIndex(int page) {
    if (page == m_pageIndex) {
      return;
    }

    if (page <= -1) {
      setRegionMode();
    } else {
      setMapMode();
    }
    m_pageIndex = page;
    updateMaxRows();
    updateRenderTexture();
  }

  static bool tilesetExists(const std::array<std::string, 9>& tilesetNames, const int idx) { return !tilesetNames[idx].empty(); }
  bool isPageValid(const int page) const {
    if (page == 0) {
      bool ret = false;
      for (int i = 0; i < 5; ++i) {
        ret |= tilesetExists(m_tilesetNames, i);
      }
      return ret;
    }

    return tilesetExists(m_tilesetNames, 4 + page);
  }
  void setTilesetNames(const std::array<std::string, 9>& tilesetNames);

  void paintTiles();

  int maxRows() const { return m_maxRows; }

  Size paletteSize() const { return {kMaxColumns, m_maxRows}; }
  Rect paletteRect() const { return {{}, paletteSize()}; }
  bool isPointContained(const Point& point) const { return paletteRect().contains(point); }

  Point imageSize() const { return {static_cast<int>(kMaxColumns * realTileWidth()), static_cast<int>(m_maxRows * realTileHeight())}; }

  explicit operator ImTextureID() const { return m_finalResult.operator ImTextureID(); }

  void setPenData(const Size& size, const std::vector<std::array<int, 4>>& data);
  const std::vector<std::array<int, 4>>& penData() const { return m_penData; }
  const Size& penSize() const { return m_penSize; }

  static int getRegionNumber(const Point& point) { return point.x() + point.y() * 8; }

  void eraseCursor();
  void setCursorRect(const Rect& rect);
  Rect cursorRect() const { return m_cursorRect; }
  Rect cursorPixelRect() const { return m_cursorPixelRect; }
  void setCursorPage(const int cursorPage) { m_cursorPage = cursorPage; }
  void startPick(const Point& point) {
    m_pickPoint = point;
    updatePick(point);
    m_isPicking = true;
  }
  void endPick();

  void onCursorDrag(const PointF& point);
  void onCursorClicked(const PointF& point);
  void onCursorReleased();
  void invalidateCursor() { setCursorRect({}); }

private:
  void updateMaxRows();
  void updateRenderTexture();
  void updateCursorPixelRect();
  void updatePick(const Point& point);
  void paintTile(RenderImage& image, const Point& point);

  static std::array<int, 4> makeTileIdList(int tileId1, int tileId2, int tileId3);
  TileRenderHelper m_renderHelper;
  bool m_isRegionMode = false;
  int m_pageIndex = 0;
  int m_cursorPage = 0;
  int m_maxRows = 0;
  Tileset::Mode m_tilesetMode{};
  std::array<std::string, 9> m_tilesetNames;
  std::array<Texture, 9> m_textures;
  RenderImage m_painter;
  RenderImage m_finalResult;
  CheckerboardTexture m_checkerboardTexture;
  Rect m_cursorRect;
  Rect m_cursorPixelRect;
  std::vector<std::array<int, 4>> m_penData;
  Size m_penSize;
  bool m_isPicking = false;
  Point m_pickPoint;
};
