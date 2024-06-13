#pragma once

#include "imgui.h"

struct ImGuiWindow;
struct MapEditor;

enum class MapCursorMode {
  Mouse,
  Keyboard,
};

struct MapCursor {
  MapCursor() = delete;

  MapCursor(MapEditor* parent) : m_parent(parent) {}

  void update(float mapScale, int mapWidth, int mapHeight, int tileSize, ImGuiWindow* win);

  void draw(ImGuiWindow* win);

  int tileX() const { return m_tileX; }
  int tileY() const { return m_tileY; }

  void setPosition(int x, int y) {
    m_tileX = x;
    m_tileY = y;
  }

  int alignCoord(int value);

  int absoluteX() const { return m_tileX * m_tileSize; }
  int absoluteY() const { return m_tileY * m_tileSize; }
  void setFromAbsolute(float x, float y, ImGuiWindow* win);

  int tileSize() const { return m_tileSize; }
  void setTileSize(int tileSize) { m_tileSize = tileSize; }

  MapCursorMode mode() const { return m_mode; }
  void setMouseMode() { m_mode = MapCursorMode::Mouse; }
  void setKeyboardMode() {
    // Disabled due to issues with scrolling
    // m_mode = MapCursorMode::Keyboard;
  }

  void setVisible(bool visible) { m_visible = visible; }

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
  MapCursorMode m_mode = MapCursorMode::Mouse;
};
