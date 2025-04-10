#include "Core/MapEditor/MapCursor.hpp"

#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "imgui_internal.h"

inline float roundUp(const float numToRound, const float multiple) {
  if (multiple == 0)
    return numToRound;

  const float remainder = fmod(numToRound, multiple);
  if (remainder == 0)
    return numToRound;

  return numToRound + multiple - remainder;
}

float MapCursor::alignCoord(const float value) const { return roundUp(value - fmod(value, m_tileSize * m_mapScale), m_tileSize * m_mapScale); }

void MapCursor::setCursorPositionFromTilePosition() {
  m_cursorPos.x = static_cast<float>(absoluteX()) * m_mapScale;
  m_cursorPos.y = static_cast<float>(absoluteY()) * m_mapScale;
  if (!(m_cursorPos.x < 0 || m_cursorPos.y < 0 || m_cursorPos.x >= ((m_mapWidth * m_tileSize) * m_mapScale) || m_cursorPos.y >= ((m_mapHeight * m_tileSize) * m_mapScale))) {
    if (m_cursorPos.x < 0) {
      m_cursorPos.x = 0;
    }
    if (m_cursorPos.x >= ((m_mapWidth * m_tileSize) * m_mapScale)) {
      m_cursorPos.x = ((m_mapWidth * m_tileSize) * m_mapScale);
    }

    if (m_cursorPos.y < 0) {
      m_cursorPos.y = 0;
    }

    if (m_cursorPos.y >= (m_mapHeight * m_tileSize) * m_mapScale) {
      m_cursorPos.y = ((m_mapHeight * m_tileSize) * m_mapScale);
    }
    m_cursorPos.x = alignCoord(m_cursorPos.x);
    m_cursorPos.y = alignCoord(m_cursorPos.y);
  }
}

void MapCursor::update(float mapScale, int mapWidth, int mapHeight, int tileSize, ImGuiWindow* win) {
  m_mapScale = mapScale;
  m_mapWidth = mapWidth;
  m_mapHeight = mapHeight;
  m_tileSize = tileSize;

  /* Keyboard mode is disabled until scrolling can be figured out */
  if (ImGui::IsWindowHovered() && ImGui::IsWindowFocused() &&
      (ImGui::IsKeyPressed(ImGuiKey_LeftArrow) || ImGui::IsKeyPressed(ImGuiKey_RightArrow) || ImGui::IsKeyPressed(ImGuiKey_UpArrow) || ImGui::IsKeyPressed(ImGuiKey_DownArrow))) {
    m_mode = MapCursorMode::Keyboard;

    if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow)) {
      m_tileX -= 1;
      if (m_tileX < 0) {
        m_tileX = 0;
      }
    }

    if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) {
      m_tileX += 1;
      if (m_tileX >= mapWidth) {
        m_tileX = mapWidth - 1;
      }
    }

    if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) {
      m_tileY -= 1;
      if (m_tileY < 0) {
        m_tileY = 0;
      }
    }

    if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {
      m_tileY += 1;
      if (m_tileY >= mapHeight) {
        m_tileY = mapHeight - 1;
      }
    }
    setCursorPositionFromTilePosition();

    const float absX = m_cursorPos.x + win->ContentRegionRect.Min.x;
    const float absY = m_cursorPos.y + win->ContentRegionRect.Min.y;
    const auto deadZone = (m_tileSize * 3) * m_mapScale;
    ImGui::ScrollToRect(win, {absX - deadZone, absY - deadZone, absX + deadZone, absY + deadZone});
    m_visible = true;
  } else if (m_mode == MapCursorMode::Mouse) {
    // Is the user attempting to zoom in?
    if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_MouseWheelY)) {
      setFromAbsolute(ImGui::GetIO().MousePos.x - win->ContentRegionRect.Min.x, ImGui::GetIO().MousePos.y - win->ContentRegionRect.Min.y);
      setCursorPositionFromTilePosition();
    } else {
      ImVec2 cursorPos = ImGui::GetIO().MousePos - win->ContentRegionRect.Min;

      if (!(cursorPos.x < 0 || cursorPos.y < 0 || cursorPos.x >= ((mapWidth * tileSize) * m_mapScale) || cursorPos.y >= ((m_mapHeight * tileSize) * m_mapScale))) {
        if (cursorPos.x < 0) {
          cursorPos.x = 0;
        }
        if (cursorPos.x >= ((m_mapWidth * m_tileSize) * m_mapScale)) {
          cursorPos.x = ((m_mapWidth * m_tileSize) * m_mapScale);
        }

        if (cursorPos.y < 0) {
          cursorPos.y = 0;
        }

        if (cursorPos.y >= (mapHeight * m_tileSize) * m_mapScale) {
          cursorPos.y = ((mapHeight * m_tileSize) * m_mapScale);
        }
        cursorPos.x = alignCoord(cursorPos.x);
        cursorPos.y = alignCoord(cursorPos.y);

        m_tileX = (static_cast<int>(cursorPos.x) + 1) / static_cast<int>(m_tileSize * m_mapScale);
        m_tileY = (static_cast<int>(cursorPos.y) + 1) / static_cast<int>(m_tileSize * m_mapScale);

        setCursorPositionFromTilePosition();
        m_visible = true;
      } else {
        m_visible = false;
      }
    }
  }
}

void MapCursor::draw(ImGuiWindow* win) {
  if (!m_visible) {
    return;
  }

  const auto tileSizef = static_cast<float>(m_tileSize);
  const auto outerThickness = ImGui::GetDPIScaledValue(2.0f);
  const auto innerThickness = ImGui::GetDPIScaledValue(1.5f);
  /* Contract the size a bit so that the cursor appears inside the tile cell */
  const ImVec2 cursorContraction = ImGui::GetDPIScaledSize(1.5, 1.5);
  /* Actually render the cursor, two unfilled rectangles: One a bit thicker for the outline, and one thinner for the white accent */
  const auto cursorPos = m_cursorPos + win->ContentRegionRect.Min;
  win->DrawList->AddRect(cursorPos + (ImVec2{0.f, 0.f} * m_mapScale) + cursorContraction, cursorPos + (ImVec2{tileSizef, tileSizef} * m_mapScale) - cursorContraction, 0xFF000000, 0.f, 0,
                         outerThickness);
  win->DrawList->AddRect(cursorPos + (ImVec2{0.f, 0.f} * m_mapScale) + cursorContraction, cursorPos + (ImVec2{tileSizef, tileSizef} * m_mapScale) - cursorContraction, 0xFFFFFFFF, 0.f, 0,
                         innerThickness);
}

void MapCursor::setFromAbsolute(const float x, const float y) {
  m_cursorPos.x = x;
  m_cursorPos.y = y;
  m_cursorPos.x = alignCoord(m_cursorPos.x);
  m_cursorPos.y = alignCoord(m_cursorPos.y);

  m_tileX = (static_cast<int>(m_cursorPos.x) + 1) / static_cast<int>(m_tileSize * m_mapScale);
  m_tileY = (static_cast<int>(m_cursorPos.y) + 1) / static_cast<int>(m_tileSize * m_mapScale);
}
