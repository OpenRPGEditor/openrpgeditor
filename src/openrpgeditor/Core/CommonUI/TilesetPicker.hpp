#pragma once
#include "Core/Tilemap/TilePalette.hpp"

struct Project;
struct TilesetPicker {
  explicit TilesetPicker(Project* parent) : m_parent(parent) {}

  void setPage(int page);
  void drawPageButton(std::string_view pageStr, int page);
  void draw();

private:
  Project* m_parent;
  TilePalette m_palette;
  int m_page = 0;
  Map* m_map = nullptr;
};
