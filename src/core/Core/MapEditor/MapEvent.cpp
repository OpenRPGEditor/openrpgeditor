#include "Core/MapEditor/MapEvent.hpp"

#include "Core/MapEditor.hpp"

#include "Core/Texture.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/Project.hpp"

#include "imgui.h"
#include "imgui_internal.h"

MapEvent::MapEvent(MapEditor* parent, Event* event)
: m_parent(parent), m_event(event), m_characterSheet(event->pages[0].image.characterName) {
  m_pattern = m_event->pages[0].image.pattern;
}

double oscillate(const double minValue, const double maxValue, const double period, const double currentTime) {
  const double rangeDelta = maxValue - minValue;
  const double phase = (sin(2 * M_PI * currentTime / period) + 1) / 2;
  return minValue + phase * rangeDelta;
}

void MapEvent::draw(float mapScale, bool isHovered, bool selected, bool halfAlpha, ImGuiWindow* win) {
  constexpr ImU32 NormalOutlineCol = 0xFF000000;
  constexpr ImU32 NormalBorderCol = 0xFFFFFFFF;
  constexpr ImU32 HoveredOutlineCol = 0xFFFFFF00;
  constexpr ImU32 HoveredBorderCol = 0xFF00FFFF;

  ImU32 outlineCol = isHovered ? HoveredOutlineCol : NormalOutlineCol;
  ImU32 borderCol = isHovered ? HoveredBorderCol : NormalBorderCol;
  ImU32 bgColor = 0x7f000000;
  ImU32 imageColor = 0xFFFFFFFF;
  if (selected) {
    ImU8 r = oscillate(0.5, 1.0, 2.0, ImGui::GetTime()) * 255;
    borderCol &= 0xFFFFFF00;
    borderCol |= r;
    bgColor &= 0xFFFFFF00;
    bgColor |= r;
    imageColor = (0xFF << 24) | (r << 16) | (r << 8) | (r << 0);
  }

  if (halfAlpha) {
    outlineCol &= 0x00FFFFFF;
    outlineCol |= (127 << 24);
    borderCol &= 0x00FFFFFF;
    borderCol |= (127 << 24);
    bgColor &= 0x00FFFFFF;
    bgColor |= (63 << 24);
    imageColor &= 0x00FFFFFF;
    imageColor |= (127 << 24);
  }

  auto eventX = static_cast<float>(m_event->x * m_parent->tileSize()) * mapScale;
  auto eventY = static_cast<float>(m_event->y * m_parent->tileSize()) * mapScale;
  eventX += win->ContentRegionRect.Min.x;
  eventY += win->ContentRegionRect.Min.y;
  float eventS = m_parent->tileSize() * mapScale;
  ImVec2 evMin = ImVec2{eventX, eventY};
  ImVec2 evMax = ImVec2{(eventX + eventS), (eventY + eventS)};
  win->DrawList->AddRectFilled(evMin + ImVec2{1.f, 1.f}, evMax - ImVec2{1.f, 1.f}, bgColor);
  win->DrawList->AddRect(evMin + ImVec2{1.f, 1.f}, evMax - ImVec2{1.f, 1.f}, outlineCol, 0, 0, 5.f);
  win->DrawList->AddRect(evMin + ImVec2{1.f, 1.f}, evMax - ImVec2{1.f, 1.f}, borderCol, 0, 0, 3.f);

  if (!m_event->pages[0].image.characterName.empty() && m_event->pages[0].image.tileId == 0 && m_characterSheet) {
    if (m_event->pages[0].stepAnime) {
      m_pattern = std::clamp<int>(std::abs(std::remainder(ImGui::GetTime() * 8, 3 * 2)), 0, 3);
    }
    if (!m_parent->prisonMode()) {
      Texture tex = m_characterSheet.texture();
      CharacterRect rect = m_characterSheet.getRectForCharacter(m_event->pages[0].image.characterIndex, m_pattern,
                                                                m_event->pages[0].image.direction);

      evMin.x -= ((static_cast<float>(m_characterSheet.characterWidth()) - m_parent->tileSize()) / 2.f) * mapScale;
      evMax.x += ((static_cast<float>(m_characterSheet.characterWidth()) - m_parent->tileSize()) / 2.f) * mapScale;
      evMin.y -= (static_cast<float>(m_characterSheet.characterHeight()) - m_parent->tileSize()) * mapScale;
      win->DrawList->AddImage(tex.get(), evMin, evMax, ImVec2{rect.uv0.u, rect.uv0.v}, ImVec2{rect.uv1.u, rect.uv1.v},
                              imageColor);
    } else {
      Texture tex = m_characterSheet.texture();
      CharacterRect rect = m_characterSheet.getRectForCharacter(
          m_event->pages[0].image.characterIndex, m_event->pages[0].image.pattern, m_event->pages[0].image.direction);

      if (!m_characterSheet.isSingleCharacter()) {
        rect.uv0.u *= tex.width();
        rect.uv0.v *= tex.height();
        rect.uv1.u *= tex.width();
        rect.uv1.v *= tex.height();
        rect.uv0.u += 16;
        rect.uv1.u -= 16;
        rect.uv0.v += 24;
        rect.uv1.v -= 32;
        rect.uv0.u /= tex.width();
        rect.uv0.v /= tex.height();
        rect.uv1.u /= tex.width();
        rect.uv1.v /= tex.height();
      }
      evMin += ImVec2{3.f, 3.f};
      evMax -= ImVec2{3.f, 3.f};
      win->DrawList->AddImage(tex.get(), evMin, evMax, ImVec2{rect.uv0.u, rect.uv0.v}, ImVec2{rect.uv1.u, rect.uv1.v},
                              imageColor);
    }
  } else if (m_parent->map() && m_event->pages[0].image.tileId) {
    int tileId = m_event->pages[0].image.tileId;
    auto tileset = m_parent->project()->tileset(m_parent->map()->tilesetId);
    if (!tileset) {
      return;
    }
    int setId;
    if (MapRenderer::isTileA5(tileId)) {
      setId = 4;
    } else {
      setId = 5 + floor(tileId / 256);
    }

    auto tex = ResourceManager::instance()->loadTilesetImage(tileset->tilesetNames[setId]);
    if (!tex) {
      return;
    }

    float tileU0 = (fmod(floor(tileId / 128), 2) * 8 + (tileId % 8)) * m_parent->tileSize();
    float tileV0 = fmod(floor(tileId % 256 / 8), 16) * m_parent->tileSize();
    float tileU1 = tileU0 + static_cast<float>(m_parent->tileSize());
    float tileV1 = tileV0 + static_cast<float>(m_parent->tileSize());
    if (m_parent->prisonMode()) {
      evMin += ImVec2{3.f, 3.f};
      evMax -= ImVec2{3.f, 3.f};
    }

    tileU0 /= static_cast<float>(tex.width());
    tileV0 /= static_cast<float>(tex.height());
    tileU1 /= static_cast<float>(tex.width());
    tileV1 /= static_cast<float>(tex.height());

    win->DrawList->AddImage(tex.get(), evMin, evMax, ImVec2{tileU0, tileV0}, ImVec2{tileU1, tileV1}, imageColor);
  }
}
