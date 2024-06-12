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

void MapEvent::draw(float mapScale, bool isHovered, ImGuiWindow* win) {
  auto eventX = static_cast<float>(m_event->x * 48) * mapScale;
  auto eventY = static_cast<float>(m_event->y * 48) * mapScale;
  eventX += win->ContentRegionRect.Min.x;
  eventY += win->ContentRegionRect.Min.y;
  float eventS = m_parent->tileSize() * mapScale;
  ImVec2 evMin = ImVec2{eventX, eventY};
  ImVec2 evMax = ImVec2{(eventX + eventS), (eventY + eventS)};
  win->DrawList->AddRectFilled(evMin + ImVec2{1.f, 1.f}, evMax - ImVec2{1.f, 1.f}, 0x7f000000);
  win->DrawList->AddRect(evMin + ImVec2{1.f, 1.f}, evMax - ImVec2{1.f, 1.f}, isHovered ? 0xFFFFFF00 : 0xFF000000, 0, 0,
                         5.f);
  win->DrawList->AddRect(evMin + ImVec2{1.f, 1.f}, evMax - ImVec2{1.f, 1.f}, isHovered ? 0xFF00FFFF : 0xFFFFFFFF, 0, 0,
                         3.f);

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
    win->DrawList->AddImage(tex.get(), evMin, evMax, ImVec2{rect.uv0.u, rect.uv0.v}, ImVec2{rect.uv1.u, rect.uv1.v});
  }
}
