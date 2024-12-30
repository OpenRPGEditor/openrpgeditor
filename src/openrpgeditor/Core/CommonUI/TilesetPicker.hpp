#pragma once
#include "Core/Tilemap/TilePalette.hpp"

class Map;
struct Project;
struct TilesetPicker {
  explicit TilesetPicker(Project* parent) : m_parent(parent) {}

  void setPage(int page);
  void drawPageButton(std::string_view pageStr, int page);
  void draw();

  const std::vector<int>& penData() const { return m_palette.penData(); }
  const Size& penSize() const { return m_palette.penSize(); }
  Rect cursorRect() const { return m_palette.cursorRect(); }
  bool isRegionMode() const { return m_palette.isRegionMode(); }
  bool isMapMode() const { return m_palette.isMapMode(); }

private:
  Project* m_parent;
  TilePalette m_palette;
  int m_page = 0;
  Map* m_map = nullptr;
};
