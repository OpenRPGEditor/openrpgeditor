#include "Core/MapEditor/MapCursor.hpp"

#include "imgui_internal.h"

inline int roundUp(int numToRound, int multiple) {
  if (multiple == 0)
    return numToRound;

  int remainder = numToRound % multiple;
  if (remainder == 0)
    return numToRound;

  return numToRound + multiple - remainder;
}

inline int alignCoord(int value, int size) { return roundUp(value - (value % size), size); }

void MapCursor::update(float mapScale, int mapWidth, int mapHeight, int tileSize, ImGuiWindow* win) {
  m_mapScale = mapScale;
  m_mapWidth = mapWidth;
  m_mapHeight = mapHeight;
  m_tileSize = tileSize;

  auto tileSizef = static_cast<float>(m_tileSize);

  ImVec2 cursorPos = ImGui::GetIO().MousePos;
  cursorPos -= win->ContentRegionRect.Min;

  if (!(cursorPos.x < 0 || cursorPos.y < 0 || cursorPos.x >= ((mapWidth * tileSize) * m_mapScale) ||
        cursorPos.y >= ((m_mapHeight * tileSize) * m_mapScale))) {
    if (cursorPos.x < 0) {
      cursorPos.x = 0;
    }
    if (cursorPos.x >= ((m_mapWidth * m_tileSize) * m_mapScale)) {
      cursorPos.x = ((m_mapWidth * m_tileSize) * m_mapScale);
    }

    if (cursorPos.y < 0) {
      cursorPos.y = 0;
    }

    if (cursorPos.y >= (mapHeight * tileSizef) * mapScale) {
      cursorPos.y = ((mapHeight * tileSizef) * mapScale);
    }
    cursorPos.x = static_cast<float>(alignCoord(static_cast<int>(cursorPos.x), static_cast<int>(tileSizef * mapScale)));
    cursorPos.y = static_cast<float>(alignCoord(static_cast<int>(cursorPos.y), static_cast<int>(tileSizef * mapScale)));


    m_tileX = (static_cast<int>(cursorPos.x) + 1) / static_cast<int>(tileSizef * mapScale);
    m_tileY = (static_cast<int>(cursorPos.y) + 1) / static_cast<int>(tileSizef * mapScale);

    m_cursorPos = cursorPos + win->ContentRegionRect.Min;
    m_visible = true;
  } else {
    m_visible = false;
  }
}

void MapCursor::draw(ImGuiWindow* win) {
  if (!m_visible) {
    return;
  }

  auto tileSizef = static_cast<float>(m_tileSize);

  win->DrawList->AddRect(m_cursorPos + (ImVec2{0.f, 0.f} * m_mapScale),
                         m_cursorPos + (ImVec2{tileSizef, tileSizef} * m_mapScale), 0xFF000000, 0.f, 0, 5.f);
  win->DrawList->AddRect(m_cursorPos + (ImVec2{0.f, 0.f} * m_mapScale),
                         m_cursorPos + (ImVec2{tileSizef, tileSizef} * m_mapScale), 0xFFFFFFFF, 0.f, 0, 3.f);
}
