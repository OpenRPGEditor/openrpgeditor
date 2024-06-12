#include "Core/MapEditor/MapEvent.hpp"

#include "Core/MapEditor.hpp"

#include "Core/Texture.hpp"
#include "Core/ResourceManager.hpp"

#include "imgui.h"
#include "imgui_internal.h"

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

  if (!m_event->pages[0].image.characterName.empty() && m_event->pages[0].image.tileId == 0) {
    if (m_event->pages[0].stepAnime) {
      m_event->pages[0].image.pattern = std::clamp<int>(std::abs(std::remainder(ImGui::GetTime() * 8, 3 * 2)), 0, 3);
    }

    // TODO: This is still wrong
    Texture tex = ResourceManager::instance()->loadCharacterImage(m_event->pages[0].image.characterName);
    const int CharacterSpriteWidth = tex.width() / 12;
    const int CharacterSpriteHeight = tex.height() / 8;
    const int CharacterAtlasWidth = tex.width() / 4;
    const int CharacterAtlasHeight = tex.height() / 2;

    const float charX = static_cast<float>(
        (m_event->pages[0].image.characterIndex % (tex.width() / CharacterAtlasWidth)) * CharacterAtlasWidth);
    const float charY = static_cast<float>(
        (m_event->pages[0].image.characterIndex / (tex.width() / CharacterAtlasWidth)) * CharacterAtlasHeight);
    const float patternOffset = static_cast<float>(m_event->pages[0].image.pattern * CharacterSpriteWidth);
    const float directionOffset =
        ((static_cast<float>(m_event->pages[0].image.direction - 2) / 2) * CharacterSpriteHeight);

    float x1 = ((charX + patternOffset) /*+ (CharacterSpriteWidth / 5)*/);
    float y1 = ((charY + directionOffset) /*+ (CharacterSpriteHeight / 4) */);
    float x2 = (((charX + patternOffset) + CharacterSpriteWidth) /*- (CharacterSpriteWidth / 5) */);
    float y2 = (((charY + directionOffset) + CharacterSpriteHeight) /*- (CharacterSpriteHeight / 4) */);

    evMin.x -= ((static_cast<float>(CharacterSpriteWidth) - 48.f) / 2.f) * mapScale;
    evMax.x += ((static_cast<float>(CharacterSpriteWidth) - 48.f) / 2.f) * mapScale;
    evMin.y -= (static_cast<float>(CharacterSpriteHeight) - 48.f) * mapScale;
    win->DrawList->AddImage(tex.get(), evMin, evMax,
                            ImVec2{x1 / static_cast<float>(tex.width()), y1 / static_cast<float>(tex.height())},
                            ImVec2{x2 / static_cast<float>(tex.width()), y2 / static_cast<float>(tex.height())});
  }
}
