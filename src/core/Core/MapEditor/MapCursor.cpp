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

int MapCursor::alignCoord(int value) {
  return roundUp(value - (value % (static_cast<int>(m_tileSize * m_mapScale))),
                 static_cast<int>(m_tileSize * m_mapScale));
}

void MapCursor::update(float mapScale, int mapWidth, int mapHeight, int tileSize, ImGuiWindow* win) {
  m_mapScale = mapScale;
  m_mapWidth = mapWidth;
  m_mapHeight = mapHeight;
  m_tileSize = tileSize;


  /* Keyboard mode is disabled until scrolling can be figured out */
  if ((ImGui::IsKeyPressed(ImGuiKey_LeftArrow) || ImGui::IsKeyPressed(ImGuiKey_RightArrow) ||
      ImGui::IsKeyPressed(ImGuiKey_UpArrow) || ImGui::IsKeyPressed(ImGuiKey_DownArrow)) && false) {
    m_mode = MapCursorMode::Keyboard;

    if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow)) {
      printf("LEFT\n");
      m_tileX -= 1;
      if (m_tileX < 0) {
        m_tileX = 0;
      }
    }

    if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) {
      printf("RIGHT\n");
      m_tileX += 1;
      if (m_tileX >= mapWidth) {
        m_tileX = mapWidth - 1;
      }
    }

    if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) {
      printf("UP\n");
      m_tileY -= 1;
      if (m_tileY < 0) {
        m_tileY = 0;
      }
    }

    if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {
      printf("DOWN\n");
      m_tileY += 1;
      if (m_tileY >= mapHeight) {
        m_tileY = mapHeight - 1;
      }
    }
    m_cursorPos.x = static_cast<float>(absoluteX());
    m_cursorPos.y = static_cast<float>(absoluteY());
    m_cursorPos += win->ContentRegionRect.Min;
    m_visible = true;
  } else if (m_mode == MapCursorMode::Mouse) {
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
      cursorPos.x = static_cast<float>(alignCoord(static_cast<int>(cursorPos.x)));
      cursorPos.y = static_cast<float>(alignCoord(static_cast<int>(cursorPos.y)));

      m_tileX = (static_cast<int>(cursorPos.x) + 1) / static_cast<int>(tileSizef * mapScale);
      m_tileY = (static_cast<int>(cursorPos.y) + 1) / static_cast<int>(tileSizef * mapScale);

      m_cursorPos = cursorPos + win->ContentRegionRect.Min;
      m_visible = true;
    } else {
      m_visible = false;
    }
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

void MapCursor::setFromAbsolute(float x, float y, ImGuiWindow* win) {
  m_cursorPos.x = x;
  m_cursorPos.y = y;
  m_cursorPos -= win->ContentRegionRect.Min;
  m_cursorPos.x = static_cast<float>(alignCoord(static_cast<int>(m_cursorPos.x)));
  m_cursorPos.y = static_cast<float>(alignCoord(static_cast<int>(m_cursorPos.y)));

  m_tileX = (static_cast<int>(m_cursorPos.x) + 1) / static_cast<int>(m_tileSize * m_mapScale);
  m_tileY = (static_cast<int>(m_cursorPos.y) + 1) / static_cast<int>(m_tileSize * m_mapScale);
  m_cursorPos += win->ContentRegionRect.Min;
}
