#include "Core/MapEditor/MapEvent.hpp"

#include "Core/MapEditor.hpp"

#include "Core/Texture.hpp"
#include "Core/ResourceManager.hpp"

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

void MapEvent::draw(float mapScale, bool isHovered, bool selected, ImGuiWindow* win) {
  constexpr ImU32 NormalOutlineCol = 0xFF000000;
  constexpr ImU32 NormalBorderCol = 0xFFFFFFFF;
  constexpr ImU32 HoveredOutlineCol = 0xFFFFFF00;
  constexpr ImU32 HoveredBorderCol = 0xFF00FFFF;

  ImU32 outlineCol = isHovered ? HoveredOutlineCol : NormalOutlineCol;
  ImU32 borderCol = isHovered ? HoveredBorderCol : NormalBorderCol;
  ImU32 bgColor = 0x7f000000;
  if (selected) {
    ImU8 r = oscillate(0.5, 1.0, 2.0, ImGui::GetTime()) * 255;
    borderCol &= 0xFFFFFF00;
    borderCol |= r;
    bgColor |= r;//(r << 16) | (r << 8) | (r << 0);
  }

  auto eventX = static_cast<float>(m_event->x * 48) * mapScale;
  auto eventY = static_cast<float>(m_event->y * 48) * mapScale;
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

    Texture tex = m_characterSheet.texture();
    CharacterRect rect = m_characterSheet.getRectForCharacter(m_event->pages[0].image.characterIndex, m_pattern,
                                                              m_event->pages[0].image.direction);

    evMin.x -= ((static_cast<float>(m_characterSheet.characterWidth()) - m_parent->tileSize()) / 2.f) * mapScale;
    evMax.x += ((static_cast<float>(m_characterSheet.characterWidth()) - m_parent->tileSize()) / 2.f) * mapScale;
    evMin.y -= (static_cast<float>(m_characterSheet.characterHeight()) - m_parent->tileSize()) * mapScale;
    win->DrawList->AddImage(tex.get(), evMin, evMax, ImVec2{rect.uv0.u, rect.uv0.v}, ImVec2{rect.uv1.u, rect.uv1.v},
                            selected ? borderCol : 0xFFFFFFFF);
  }
}
