#include "Core/MapEditor.hpp"
#include "Core/Project.hpp"

#include "ImGuiUtils.hpp"

#include "imgui_internal.h"
#include "imgui.h"

#include <format>

struct EventMoveUndoCommand : IUndoCommand {
  EventMoveUndoCommand() = delete;
  EventMoveUndoCommand(Event* event, int x, int y) : m_event(event), m_x(x), m_y(y) {}
  int type() const override { return -1; }

  bool undo() override {
    if (m_event) {
      std::swap(m_event->x, m_x);
      std::swap(m_event->y, m_y);
      return true;
    }

    return false;
  }

  std::string description() override {
    if (!m_event) {
      return "Invalid move command!";
    }
    if (!m_isRedo) {
      return std::format("Move {} from {},{} to {},{}",
                         m_event->name.empty() ? std::to_string(m_event->id) : m_event->name, m_x, m_y, m_event->x,
                         m_event->y);
    }
    return std::format("Move {} from {},{} to {},{}",
                       m_event->name.empty() ? std::to_string(m_event->id) : m_event->name, m_event->x, m_event->y, m_x,
                       m_y);
  }

private:
  Event* m_event = nullptr;
  int m_x = -1;
  int m_y = -1;
};

void MapEditor::setMap(Map* map, MapInfo* info) {
  if (map == nullptr) {
    m_mapRenderer.setMap(nullptr, nullptr);
  } else {
    m_mapRenderer.setMap(map, m_parent->tileset(map->tilesetId));
  }

  m_eventEditors.clear();
  m_map = map;
  m_mapInfo = info;
  m_lowerLayer.clear();
  m_upperLayer.clear();
}

inline int roundUp(int numToRound, int multiple) {
  if (multiple == 0)
    return numToRound;

  int remainder = numToRound % multiple;
  if (remainder == 0)
    return numToRound;

  return numToRound + multiple - remainder;
}

inline int alignCoord(int value, int size) { return roundUp(value - (value % size), size); }

void MapEditor::draw() {
  std::erase_if(m_eventEditors, [](EventEditor& editor) { return !editor.draw(); });

  static float timer = 0.5f;
  if (timer <= 0.f) {
    timer = 0.5f;
  }
  timer -= (1 / 60.f);

  // Keep mapScale to a quarter step

  if (ImGui::IsMouseKey(ImGuiKey_MouseWheelY) && ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
    m_mapScale += ImGui::GetIO().MouseWheel * 0.25f;
  }
  m_mapScale = (floorf((m_mapScale * 2.f) + .25f) / 2.f);
  m_mapScale = std::clamp(m_mapScale, .25f, 4.f);

  if (ImGui::Begin("Map Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar)) {
    ImGui::BeginChild("##mapcontents", ImVec2(0, ImGui::GetContentRegionMax().y - 90.f), ImGuiChildFlags_Border,
                      ImGuiWindowFlags_HorizontalScrollbar);
    if (m_map) {
      if (m_initialScrollSet) {
        // m_mapScale = std::min((m_map->width * 48) / m_initialScrollX, (m_map->height * 48) / m_initialScrollY);
        ImGui::SetScrollX(m_initialScrollX);
        ImGui::SetScrollY(m_initialScrollY);
        m_initialScrollX = m_initialScrollY = 0.0;
        m_initialScrollSet = false;
      }
      m_mapInfo->scrollX = ImGui::GetScrollX();
      m_mapInfo->scrollY = ImGui::GetScrollY();
      auto sortedEvents = m_map->getSortedBy();
      // m_mapRenderer.update();
      const Tileset* tileset = m_parent->tileset(m_map->tilesetId);

      ImGuiWindow* win = ImGui::GetCurrentWindow();
      ImGui::Dummy(ImVec2{m_map->width * (48 * m_mapScale), m_map->height * (48 * m_mapScale)});
#if 0
      if (m_lowerLayer.empty()) {
        m_lowerLayer.clear();
        m_upperLayer.clear();
        for (int y = 0; y < m_map->height; y++) {
          for (int x = 0; x < m_map->width; x++) {
            for (int i = 0; i < 10; ++i) {
              int tileId = m_mapRenderer.tileId(x, y, i);
              if (m_mapRenderer.isHigherTile(tileId)) {
                m_mapRenderer.getTileRect(m_upperLayer, m_mapRenderer.tileId(x, y, i), x * 48, y * 48);
              } else {
                m_mapRenderer.getTileRect(m_lowerLayer, m_mapRenderer.tileId(x, y, i), x * 48, y * 48);
              }
            }
          }
        }
      }

      for (auto tile : m_lowerLayer) {
        const auto& tex = m_mapRenderer.m_tilesetTextures[tile.setNum];
        const float x0 = tile.x;
        const float x1 = tile.x + tile.tileWidth;
        const float y0 = tile.y;
        const float y1 = tile.y + tile.tileHeight;
        const float u0 = tile.u / tex.width();
        const float u1 = (tile.u + tile.tileWidth) / tex.width();
        const float v0 = tile.v / tex.height();
        const float v1 = (tile.v + tile.tileHeight) / tex.height();
        // win->DrawList->AddImage(tex.get(), ImVec2{x0, y0}, ImVec2{x1, y1}, ImVec2{u0, v0}, ImVec2{u1, v1});
        int color = 0xFFAAAAAA;
        if (MapRenderer::isGroundTile(tile.tileId)) {
          color = 0xFF00AA00;
        }
        if (MapRenderer::isWaterTile(tile.tileId)) {
          color = 0xFFAA0000;
        }
        if (MapRenderer::isShadowingTile(tile.tileId)) {
          color = 0xFF7A7A7A;
        }
        if (MapRenderer::isWallTile(tile.tileId)) {
          color = 0xFF3A3A3A;
        }
        win->DrawList->AddRect(win->ContentRegionRect.Min + (ImVec2{x0, y0} * m_mapScale),
                               win->ContentRegionRect.Min + (ImVec2{x1, y1} * m_mapScale), color);
      }
      for (auto tile : m_upperLayer) {
        const auto& tex = m_mapRenderer.m_tilesetTextures[tile.setNum];
        const float x0 = tile.x;
        const float x1 = tile.x + tile.tileWidth;
        const float y0 = tile.y;
        const float y1 = tile.y + tile.tileHeight;
        const float u0 = tile.u / tex.width();
        const float u1 = (tile.u + tile.tileWidth) / tex.width();
        const float v0 = tile.v / tex.height();
        const float v1 = (tile.v + tile.tileHeight) / tex.height();
        int color = 0xFFAA00AA;
        if (MapRenderer::isGroundTile(tile.tileId)) {
          color = 0xFF00AA00;
        }
        if (MapRenderer::isWaterTile(tile.tileId)) {
          color = 0xFFAA0000;
        }
        if (MapRenderer::isShadowingTile(tile.tileId)) {
          color = 0xFF7A7A7A;
        }
        if (MapRenderer::isWallTile(tile.tileId)) {
          color = 0xFF3A3A3A;
        }
        // win->DrawList->AddImage(tex.get(), ImVec2{x0, y0}, ImVec2{x1, y1}, ImVec2{u0, v0}, ImVec2{u1, v1});
        win->DrawList->AddRect(win->ContentRegionRect.Min + (ImVec2{x0, y0} * m_mapScale),
                               win->ContentRegionRect.Min + (ImVec2{x1, y1} * m_mapScale), color);
        // win->DrawList->AddRect(
        //     win->ContentRegionRect.Min + (ImVec2{static_cast<float>(tile.x), static_cast<float>(tile.y)} *
        //     m_mapScale), win->ContentRegionRect.Min +
        //         (ImVec2{static_cast<float>(tile.x + tile.tileWidth), static_cast<float>(tile.y + tile.tileHeight)} *
        //          m_mapScale),
        //     0xFFFFFFFF);
      }
#endif
#if 0
      for (int y = 0; y <= (m_map->height * 48) * m_mapScale; y += 48 * m_mapScale) {
        win->DrawList->AddLine(win->ContentRegionRect.Min + ImVec2{0.f, static_cast<float>(y)},
                               win->ContentRegionRect.Min +
                                   ImVec2{(m_map->width * 48) * m_mapScale, static_cast<float>(y)},
                               0x7f0a0a0a, 3.f);
      }

      for (int x = 0; x <= (m_map->width * 48) * m_mapScale; x += 48 * m_mapScale) {
        win->DrawList->AddLine(win->ContentRegionRect.Min + ImVec2{static_cast<float>(x), 0.f},
                               win->ContentRegionRect.Min +
                                   ImVec2{static_cast<float>(x), (m_map->height * 48) * m_mapScale},
                               0x7f0a0a0a, 3.f);
      }
#endif

      if (ImGui::IsWindowHovered()) {
        ImVec2 cursorPos = ImGui::GetIO().MousePos;

        cursorPos -= win->ContentRegionRect.Min;

        if (!(cursorPos.x < 0 || cursorPos.y < 0 || cursorPos.x >= ((m_map->width * 48) * m_mapScale) ||
              cursorPos.y >= ((m_map->height * 48) * m_mapScale))) {
          if (cursorPos.x < 0) {
            cursorPos.x = 0;
          }
          if (cursorPos.x >= ((m_map->width * 48) * m_mapScale)) {
            cursorPos.x = ((m_map->width * 48) * m_mapScale);
          }

          if (cursorPos.y < 0) {
            cursorPos.y = 0;
          }

          if (cursorPos.y >= ((m_map->height * 48) * m_mapScale)) {
            cursorPos.y = ((m_map->height * 48) * m_mapScale);
          }
          cursorPos.x =
              static_cast<float>(alignCoord(static_cast<int>(cursorPos.x), static_cast<int>(48.f * m_mapScale)));
          cursorPos.y =
              static_cast<float>(alignCoord(static_cast<int>(cursorPos.y), static_cast<int>(48.f * m_mapScale)));

          m_tileCellX = (static_cast<int>(cursorPos.x) + 1) / static_cast<int>(48.f * m_mapScale);
          m_tileCellY = (static_cast<int>(cursorPos.y) + 1) / static_cast<int>(48.f * m_mapScale);
          m_tileId = ((0 * m_map->height) + m_tileCellY) * m_map->width + m_tileCellX;

          cursorPos += win->ContentRegionRect.Min;

          win->DrawList->AddRect(cursorPos + (ImVec2{0.f, 0.f} * m_mapScale), cursorPos + (ImVec2{48, 48} * m_mapScale),
                                 0xFF000000, 0.f, 0, 5.f);
          win->DrawList->AddRect(cursorPos + (ImVec2{0.f, 0.f} * m_mapScale), cursorPos + (ImVec2{48, 48} * m_mapScale),
                                 0xFFFFFFFF, 0.f, 0, 3.f);
        }
      }
      // win->DrawList->AddImage(m_mapRenderer.getUpperBitmap(), win->ContentRegionRect.Min,
      //                         win->ContentRegionRect.Min +
      //                             ImVec2{(m_map->width * 48) * m_mapScale, (m_map->height * 48) * m_mapScale});

      for (auto& event : sortedEvents) {
        if (event) {
          ImGui::BeginGroup();
          {
            bool isHovered = event->x == m_tileCellX && event->y == m_tileCellY;
            auto eventX = static_cast<float>(event->x * 48) * m_mapScale;
            auto eventY = static_cast<float>(event->y * 48) * m_mapScale;
            eventX += win->ContentRegionRect.Min.x;
            eventY += win->ContentRegionRect.Min.y;
            float eventS = 48 * m_mapScale;
            ImVec2 evMin = ImVec2{eventX, eventY};
            ImVec2 evMax = ImVec2{(eventX + eventS), (eventY + eventS)};
            win->DrawList->AddRectFilled(evMin + ImVec2{1.f, 1.f}, evMax - ImVec2{1.f, 1.f}, 0x7f000000);
            win->DrawList->AddRect(evMin + ImVec2{1.f, 1.f}, evMax - ImVec2{1.f, 1.f},
                                   isHovered ? 0xFFFFFF00 : 0xFF000000, 0, 0, 5.f);
            win->DrawList->AddRect(evMin + ImVec2{1.f, 1.f}, evMax - ImVec2{1.f, 1.f},
                                   isHovered ? 0xFF00FFFF : 0xFFFFFFFF, 0, 0, 3.f);

            if (!event->pages[0].image.characterName.empty() && event->pages[0].image.tileId == 0) {
              if (event->pages[0].stepAnime) {
                event->pages[0].image.pattern =
                    std::clamp<int>(std::abs(std::remainder(ImGui::GetTime() * 8, 3 * 2)), 0, 3);
              }

              // TODO: This is still wrong
              Texture tex = ResourceManager::instance()->loadCharacterImage(event->pages[0].image.characterName);
              const int CharacterSpriteWidth = tex.width() / 12;
              const int CharacterSpriteHeight = tex.height() / 8;
              const int CharacterAtlasWidth = tex.width() / 4;
              const int CharacterAtlasHeight = tex.height() / 2;

              const float charX = static_cast<float>(
                  (event->pages[0].image.characterIndex % (tex.width() / CharacterAtlasWidth)) * CharacterAtlasWidth);
              const float charY = static_cast<float>(
                  (event->pages[0].image.characterIndex / (tex.width() / CharacterAtlasWidth)) * CharacterAtlasHeight);
              const float patternOffset = static_cast<float>(event->pages[0].image.pattern * CharacterSpriteWidth);
              const float directionOffset =
                  ((static_cast<float>(event->pages[0].image.direction - 2) / 2) * CharacterSpriteHeight);

              float x1 = ((charX + patternOffset) /*+ (CharacterSpriteWidth / 5)*/);
              float y1 = ((charY + directionOffset) /*+ (CharacterSpriteHeight / 4) */);
              float x2 = (((charX + patternOffset) + CharacterSpriteWidth) /*- (CharacterSpriteWidth / 5) */);
              float y2 = (((charY + directionOffset) + CharacterSpriteHeight) /*- (CharacterSpriteHeight / 4) */);

              evMin.x -= ((static_cast<float>(CharacterSpriteWidth) - 48.f) / 2.f) * m_mapScale;
              evMax.x += ((static_cast<float>(CharacterSpriteWidth) - 48.f) / 2.f) * m_mapScale;
              evMin.y -= (static_cast<float>(CharacterSpriteHeight) - 48.f) * m_mapScale;
              win->DrawList->AddImage(
                  tex.get(), evMin, evMax,
                  ImVec2{x1 / static_cast<float>(tex.width()), y1 / static_cast<float>(tex.height())},
                  ImVec2{x2 / static_cast<float>(tex.width()), y2 / static_cast<float>(tex.height())});
            }
            /* Check if event is selected */
            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
              if (isHovered && event) {

                auto it =
                    std::find_if(m_eventEditors.begin(), m_eventEditors.end(), [&event](const EventEditor& editor) {
                      return event && editor.event()->id == event->id;
                    });
                if (it == m_eventEditors.end()) {
                  printf("Event selected!");
                  m_eventEditors.emplace_back(m_parent, m_map->event(event->id));
                }
              }
            } else if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered()) {
              if (isHovered && m_selectedEvent == nullptr) {
                m_selectedEvent = m_map->event(event->id);
                if (m_selectedEvent != nullptr) {
                  m_selectedEventX = m_selectedEvent->x;
                  m_selectedEventY = m_selectedEvent->y;
                }
              }
              if (m_selectedEvent != nullptr) {
                /* For now we'll prevent events from occupying the same tile */
                /* TODO(phil): Implement some way to sort through events on the same tile */
                int oldX = m_selectedEvent->x;
                int oldY = m_selectedEvent->y;
                auto it = std::find_if(m_map->events.begin(), m_map->events.end(), [&](const std::optional<Event>& e) {
                  return e->x == m_tileCellX && e->y == m_tileCellY && &e.value() != m_selectedEvent;
                });

                m_selectedEvent->x = m_tileCellX;
                m_selectedEvent->y = m_tileCellY;

                if (it != m_map->events.end()) {
                  m_selectedEvent->x = oldX;
                  m_selectedEvent->y = oldY;
                }
              }
            } else if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
              /* If we have a selected actor and it's no longer in it's original location, push it onto the undo stack
               * as an operation
               */
              if (m_selectedEvent != nullptr &&
                  (m_selectedEvent->x != m_selectedEventX || m_selectedEvent->y != m_selectedEventY)) {
                m_parent->addUndo(
                    std::make_shared<EventMoveUndoCommand>(m_selectedEvent, m_selectedEventX, m_selectedEventY));
              }
              m_selectedEvent = nullptr;
            }
            ImGui::EndGroup();
          }
        }
      }
    }
    ImGui::EndChild();
    ImGui::Text("Scale:");
    ImGui::SameLine();
    ImGui::SliderFloat("##map_scale", &m_mapScale, 0.25f, 4.f);
    ImGui::SameLine();
    ImGui::Text("Tile %i, (%i, %i)", m_tileId, m_tileCellX, m_tileCellY);
  }
  ImGui::End();
  if (ImGui::Begin("Map Properties")) {
    char buf[4096]{};
    if (m_map) {
      ImGui::BeginGroupPanel();
      {
        ImGui::SeparatorText("General Settings");
        ImGui::BeginGroup();
        {
          ImGui::Text("Name - ID: %.03i", m_mapInfo->id);
          ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - 15);
          strncpy(buf, m_mapInfo->name.c_str(), 4096);
          if (ImGui::InputText("##map_name", buf, 4096)) {
            m_mapInfo->name = buf;
          }
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          ImVec2 cursorPos = ImGui::GetCursorPos();
          // Move back up a couple couple pixels
          cursorPos.y -= 4.f;
          ImGui::SetCursorPos(cursorPos);
          ImGui::Text("Display Name");
          ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - 15);
          strncpy(buf, m_map->displayName.c_str(), 4096);
          if (ImGui::InputText("##map_display_name", buf, 4096)) {
            m_map->displayName = buf;
          }
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          ImGui::Text("Tileset");
          strncpy(buf, m_parent->currentMapInfo()->name.c_str(), 4096);
          std::string text = m_parent->tileset(m_map->tilesetId)->name.empty()
                                 ? "##map_tileset_button_empty"
                                 : m_parent->tileset(m_map->tilesetId)->name;
          ImGui::PushID("##map_tileset_button");
          if (ImGui::Button(text.c_str(), ImVec2{ImGui::GetWindowContentRegionMax().x / 2 - 15, 0})) {}
          ImGui::PopID();
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          ImVec2 cursorPos = ImGui::GetCursorPos();
          // Move back up a couple couple pixels
          cursorPos.y -= 4.f;
          ImGui::SetCursorPos(cursorPos);
          ImGui::Text("Width");
          ImGui::SetNextItemWidth(((ImGui::GetContentRegionMax().x / 2) / 2) - 15);
          strncpy(buf, m_map->displayName.c_str(), 4096);
          if (ImGui::DragInt("##map_width", &m_map->width, 0, 0, 256)) {
            m_map->displayName = buf;
          }
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          ImVec2 cursorPos = ImGui::GetCursorPos();
          // Move back up a couple couple pixels
          cursorPos.y -= 4.f;
          ImGui::SetCursorPos(cursorPos);
          ImGui::Text("Height");
          ImGui::SetNextItemWidth(((ImGui::GetContentRegionMax().x / 2) / 2) - 15);
          strncpy(buf, m_map->displayName.c_str(), 4096);
          if (ImGui::DragInt("##map_height", &m_map->height, 0, 0, 256)) {
            m_map->displayName = buf;
          }
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          ImVec2 cursorPos = ImGui::GetCursorPos();
          // Move back up a couple couple pixels
          ImGui::SetCursorPos(cursorPos);
          ImGui::Text("Scroll Type");
          ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - 15);
          strncpy(buf, m_map->displayName.c_str(), 4096);
          if (ImGui::BeginCombo("##map_scroll_type",
                                DecodeEnumName(magic_enum::enum_name(m_map->scrollType)).c_str())) {
            for (const auto& e : magic_enum::enum_values<ScrollType>()) {
              if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(e)).c_str(), e == m_map->scrollType)) {
                m_map->scrollType = e;
              }
              if (e == m_map->scrollType) {
                ImGui::SetItemDefaultFocus();
              }
            }
            ImGui::EndCombo();
          }
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          ImVec2 cursorPos = ImGui::GetCursorPos();
          // Move back up a couple couple pixels
          cursorPos.y -= 4.f;
          ImGui::SetCursorPos(cursorPos);
          ImGui::Text("Enc. Steps");
          ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - 15);
          strncpy(buf, m_map->displayName.c_str(), 4096);
          if (ImGui::DragInt("##map_enc_steps", &m_map->encounterStep, 0, 0, 999)) {
            m_map->displayName = buf;
          }
        }
        ImGui::EndGroup();
      }
      ImGui::Separator();
      {
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Autoplay BGM", &m_map->autoPlayBgm)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !m_map->autoPlayBgm);
          {
            ImGui::PushID("##map_bgm_button");
            ImGui::SetNextItemWidth((ImGui::GetWindowContentRegionMax().x / 2) - 30);
            std::string text = m_map->bgm.name.empty() ? "##map_bgm_button_empty" : m_map->bgm.name;
            if (ImGui::Button(text.c_str(), ImVec2{(ImGui::GetWindowContentRegionMax().x / 2) - 15, 0})) {}
            ImGui::PopID();
          }
          ImGui::PopItemFlag();
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Autoplay BGS", &m_map->autoPlayBgs)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !m_map->autoPlayBgs);
          {
            ImGui::PushID("##map_bgs_button");
            std::string text = m_map->bgs.name.empty() ? "##map_bgs_button_empty" : m_map->bgs.name;
            if (ImGui::Button(text.c_str(), ImVec2{(ImGui::GetWindowContentRegionMax().x / 2) - 15, 0})) {}
            ImGui::PopID();
          }
          ImGui::PopItemFlag();
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Battleback", &m_map->specifyBattleBack)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !m_map->specifyBattleBack);
          {
            ImGui::PushID("##map_battleback_button");
            // TODO: Combine battleBack1Name and battleBack2Name
            std::string text = m_map->bgs.name.empty() ? "##map_battleback_button_empty" : m_map->battleBack1Name;
            if (ImGui::Button(text.c_str(), ImVec2{(ImGui::GetWindowContentRegionMax().x / 2) - 15, 0})) {}
            ImGui::PopID();
          }
          ImGui::PopItemFlag();
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Disable Dashing", &m_map->disableDashing)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !m_map->disableDashing);
          ImGui::PopItemFlag();
        }
        ImGui::EndGroup();
      }
      ImGui::Separator();
      {
        ImGui::BeginGroup();
        {
          ImGui::Text("Parallax Background");
          ImGui::PushID("##map_parallax_button");
          std::string text = m_map->bgs.name.empty() ? "##map_parallax_button_empty" : m_map->parallaxName;
          if (ImGui::Button(text.c_str(), ImVec2{(ImGui::GetWindowContentRegionMax().x / 2) - 15, 0})) {}
          ImGui::PopID();
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Loop Horizontally", &m_map->parallaxLoopX)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !m_map->parallaxLoopX);
          {
            ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - 15);
            if (ImGui::DragInt("##map_parallax_Sx", &m_map->parallaxSx, 0, 0, 999)) {}
          }
          ImGui::PopItemFlag();
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Loop Vertically", &m_map->parallaxLoopY)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !m_map->parallaxLoopY);
          {
            ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - 15);
            if (ImGui::DragInt("##map_parallax_Sy", &m_map->parallaxSy, 0, 0, 999)) {}
          }
          ImGui::PopItemFlag();
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Show in Editor", &m_map->parallaxShow)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !m_map->parallaxShow);
          ImGui::PopItemFlag();
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          ImVec2 cursorPos = ImGui::GetCursorPos();
          ImGuiInputTextFlags flags = ImGuiInputTextFlags_CtrlEnterForNewLine;
          // Move back up a couple couple pixels
          cursorPos.y -= 4.f;
          ImGui::SetCursorPos(cursorPos);
          ImGui::Text("Note");

          strncpy(buf, m_map->note.c_str(), 4096);
          if (ImGui::InputTextMultiline("##map_note", buf, 2048, ImVec2(ImGui::GetContentRegionMax().x - 15, 400),
                                        flags)) {
            m_map->note = buf;
          }
        }
        ImGui::Dummy(ImVec2(0.0f, 15.0f));
        ImGui::EndGroup();
      }
      ImGui::EndGroupPanel();
    }
  }
  ImGui::End();

#if 0
  if (m_map) {
    const Tileset* tileset = m_parent->tileset(m_map->tilesetId);
    if (tileset && !tileset->name.empty() && !tileset->flags.empty()) {
      if (ImGui::Begin("Tile Debugger")) {
        ImGui::BeginGroup();
        {
          for (int z = 0; z < 6; ++z) {
            int tileCellIndex = (z * m_map->height + m_tileCellY) * m_map->width + m_tileCellX;
            if (tileCellIndex > m_map->data.size()) {
              continue;
            }
            ImGui::Text("Tile %i, layer %i, cell {%i, %i}, absolute {%i, %i}", tileCellIndex, z, m_tileCellX,
                        m_tileCellY, m_tileCellX * 48, m_tileCellY * 48);
            int tileId = m_map->data[tileCellIndex];
            if (tileId >= tileset->flags.size()) {
              continue;
            }
            int flags = tileset->flags[tileId];
            ImGui::Text(
                "Tile ID %i, Flags 0x%04X, IsAutoTile %i, IsA1Tile %i, IsA2Tile %i, IsA3Tile %i, IsA4Tile %i, IsA5Tile "
                "%i",
                tileId, flags, MapRenderer::isAutoTile(tileId), MapRenderer::isTileA1(tileId),
                MapRenderer::isTileA2(tileId), MapRenderer::isTileA3(tileId), MapRenderer::isTileA4(tileId),
                MapRenderer::isTileA5(tileId));
          }
        }
        ImGui::EndGroup();
        // ImGui::BeginGroup();
        // {
        //   ImGuiWindow* win = ImGui::GetCurrentWindow();
        //   for (int z = 0; z < 6; ++z) {
        //     int tileId = m_mapRenderer.tileId(m_tileCellX, m_tileCellY, z);
        //     if (MapRenderer::isAutoTile(tileId)) {
        //       auto tileRects = m_mapRenderer.getTileRect(tileId, m_tileCellX, m_tileCellY);
        //
        //       for (auto tile : tileRects) {
        //         Texture tex = m_resourceManager->loadTilesetImage(tileset->tilesetNames[tile.setNum]);
        //         if (tex) {
        //           win->DrawList->AddImage(
        //               tex.get(), win->ContentRegionRect.Min + ImVec2{tile.x * tile.tileWidth, tile.y *
        //               tile.tileHeight}, win->ContentRegionRect.Min + ImVec2{(tile.x * tile.tileWidth) +
        //               tile.tileWidth,
        //                                                   (tile.y * tile.tileHeight) + tile.tileHeight},
        //               {tile.u / tex.width(), tile.v / tex.height()},
        //               {(tile.u + tile.tileWidth) / tex.width(), (tile.v + tile.tileHeight) / tex.height()});
        //         }
        //       }
        //     }
        //   }
        // }
        // ImGui::EndGroup();
      }
    }
    ImGui::End();
  }

#endif
  // if (ImGui::BeginPopupContextWindow()) {
  //   if (ImGui::BeginPopupContextItem(""))
  // }
}