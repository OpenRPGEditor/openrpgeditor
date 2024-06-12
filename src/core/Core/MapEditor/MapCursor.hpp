#pragma once

#include "imgui.h"

struct ImGuiWindow;
struct MapEditor;
struct MapCursor {
  MapCursor() = delete;

  MapCursor(MapEditor* parent) : m_parent(parent) {}

  void update(float mapScale, int mapWidth, int mapHeight, int tileSize, ImGuiWindow* win);

  void draw(ImGuiWindow* win);

  int tileX() const { return m_tileX; }
  int tileY() const { return m_tileY; }

  int absoluteX() const { return m_tileX * m_tileSize; }
  int absoluteY() const { return m_tileX * m_tileSize; }

  int tileSize() const { return m_tileSize; }
  void setTileSize(int tileSize) { m_tileSize = tileSize; }

private:
  MapEditor* m_parent = nullptr;
  float m_mapScale{1.f};
  int m_mapHeight{0};
  int m_mapWidth{0};
  int m_tileX{0};
  int m_tileY{0};
  int m_tileSize{48};
  ImVec2 m_cursorPos;
  bool m_visible{true};
};
