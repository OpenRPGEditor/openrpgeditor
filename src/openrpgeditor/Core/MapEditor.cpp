#include "Core/MapEditor.hpp"

#include "Core/DPIHandler.hpp"
#include "Core/Project.hpp"

#include "Core/ImGuiExt/ImGuiUtils.hpp"

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

void MapEditor::setMap(MapInfo* info) {
  if (info == nullptr) {
    m_mapRenderer.setMap(nullptr, nullptr);
  } else {
    m_mapRenderer.setMap(info->map(), m_parent->tileset(info->map()->tilesetId));
  }

  m_eventEditors.clear();
  m_mapInfo = info;

  if (map() && map()->parallaxShow && !map()->parallaxName.empty()) {
    m_parallaxTexture = ResourceManager::instance()->loadParallaxImage(map()->parallaxName);
  } else {
    m_parallaxTexture = Texture();
  }
}

int MapEditor::tileSize() { return m_parent->system().tileSize; }

void MapEditor::drawParallax(ImGuiWindow* win) {
  if (!m_parallaxTexture) {
    return;
  }
  // TODO: Proper parallax implementation
  win->DrawList->AddImage(m_parallaxTexture.get(), win->ContentRegionRect.Min + ImVec2{0.f, 0.f},
                          win->ContentRegionRect.Min + (ImVec2{static_cast<float>(m_parallaxTexture.width()),
                                                               static_cast<float>(m_parallaxTexture.height())} *
                                                        m_mapScale));
}

void MapEditor::drawGrid(ImGuiWindow* win) {
  for (int y = tileSize() * m_mapScale; y < (map()->height * tileSize()) * m_mapScale; y += tileSize() * m_mapScale) {
    win->DrawList->AddLine(win->ContentRegionRect.Min + ImVec2{0.f, static_cast<float>(y)},
                           win->ContentRegionRect.Min +
                               ImVec2{(map()->width * tileSize()) * m_mapScale, static_cast<float>(y)},
                           0x7f0a0a0a, 3.f);
  }

  for (int x = tileSize() * m_mapScale; x < (map()->width * tileSize()) * m_mapScale; x += tileSize() * m_mapScale) {
    win->DrawList->AddLine(win->ContentRegionRect.Min + ImVec2{static_cast<float>(x), 0.f},
                           win->ContentRegionRect.Min +
                               ImVec2{static_cast<float>(x), (map()->height * tileSize()) * m_mapScale},
                           0x7f0a0a0a, 3.f);
  }
}
float roundToNearestQuarter(float num) { return static_cast<float>(static_cast<int>(num * 4)) / 4; }

void MapEditor::handleEventDrag() {
  if (m_movingEvent) {
    /* For now we'll prevent events from occupying the same tile */
    /* TODO(phil): Implement some way to sort through events on the same tile */
    int oldX = m_movingEvent->x;
    int oldY = m_movingEvent->y;

    auto it = std::find_if(map()->events.begin(), map()->events.end(), [&](const std::optional<Event>& e) {
      return e && e->x == tileCellX() && e->y == tileCellY() && &e.value() != m_movingEvent;
    });

    m_movingEvent->x = m_tileCursor.tileX();
    m_movingEvent->y = m_tileCursor.tileY();
    if (it != map()->events.end()) {
      m_movingEvent->x = oldX;
      m_movingEvent->y = oldY;
    }
  }
}
void MapEditor::handleMouseInput(ImGuiWindow* win) {
  // if (map()) {
  //   map()->data[(tileCellY() * map()->width) + tileCellX()] = 2086;
  //   updateAutotilesAroundPoint(Point{tileCellX(), tileCellY()}, 0);
  // }

  if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow) || ImGui::IsKeyPressed(ImGuiKey_RightArrow) ||
      ImGui::IsKeyPressed(ImGuiKey_UpArrow) || ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {
    m_tileCursor.setKeyboardMode();
  } else if (std::fabs(ImGui::GetIO().MouseDelta.x) > 0.5f || std::fabs(ImGui::GetIO().MouseDelta.y) > 0.5f) {
    m_tileCursor.setMouseMode();
  }

  if (ImGui::IsWindowHovered() && m_parent->editMode() == EditMode::Map) {
    m_tileCursor.update(m_mapScale, map()->width, map()->height, m_parent->system().tileSize, win);
  } else if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left) &&
             (m_parent->editMode() == EditMode::Event || m_movingEvent)) {
    m_tileCursor.update(m_mapScale, map()->width, map()->height, m_parent->system().tileSize, win);
    handleEventDrag();
    m_scaleChanged = false;
  } else if (m_scaleChanged || m_tileCursor.mode() == MapCursorMode::Keyboard) {
    m_tileCursor.update(m_mapScale, map()->width, map()->height, m_parent->system().tileSize, win);
    handleEventDrag();
    m_scaleChanged = false;
  }

  if (m_parent->editMode() == EditMode::Event && m_tileCursor.mode() == MapCursorMode::Keyboard &&
      (ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift)) &&
      (!m_movingEvent || !m_selectedEvent)) {
    m_selectedEvent = map()->eventAt(m_tileCursor.tileX(), m_tileCursor.tileY());
    m_movingEvent = m_selectedEvent;
    if (m_movingEvent) {
      m_movingEventX = m_movingEvent->x;
      m_movingEventY = m_movingEvent->y;
    }
  } else if (m_parent->editMode() == EditMode::Event && m_tileCursor.mode() == MapCursorMode::Keyboard &&
             (ImGui::IsKeyReleased(ImGuiKey_LeftShift) || ImGui::IsKeyReleased(ImGuiKey_RightShift))) {
    if (m_movingEvent) {
      m_movingEvent->x = m_tileCursor.tileX();
      m_movingEvent->y = m_tileCursor.tileY();
    }
  }

  if (m_selectedEvent && !m_movingEvent && m_selectedEvent->x != m_tileCursor.tileX() &&
      m_selectedEvent->y != m_tileCursor.tileY()) {
    m_selectedEvent = nullptr;
  }

  if ((ImGui::IsMouseClicked(ImGuiMouseButton_Left) && m_parent->editMode() == EditMode::Event &&
       (m_movingEvent || m_selectedEvent))) {
    auto event = map()->eventAt(m_tileCursor.tileX(), m_tileCursor.tileY());
    if (event == nullptr) {
      m_movingEvent = nullptr;
      m_selectedEvent = nullptr;
    } else {
      m_selectedEvent = event;
    }
    m_hasScrolled = true;
  }

  if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered() &&
      m_parent->editMode() == EditMode::Event && m_movingEvent == nullptr) {
    m_selectedEvent = map()->eventAt(m_tileCursor.tileX(), m_tileCursor.tileY());
    m_movingEvent = m_selectedEvent;
    if (m_movingEvent) {
      m_movingEventX = m_movingEvent->x;
      m_movingEventY = m_movingEvent->y;
    }
    m_hasScrolled = true;
  }

  if ((ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) || ImGui::IsKeyPressed(ImGuiKey_Enter) ||
       ImGui::IsKeyPressed(ImGuiKey_KeypadEnter)) &&
      m_parent->editMode() == EditMode::Event && ImGui::IsWindowFocused()) {

    if (m_selectedEvent != nullptr) {
      const auto it = std::ranges::find_if(
          m_eventEditors, [&](const EventEditor& editor) { return editor.event()->id == m_selectedEvent->id; });
      if (it == m_eventEditors.end()) {
        m_eventEditors.emplace_back(m_parent, m_selectedEvent);
      }
    } else if (m_eventEditors.empty() && ImGui::IsWindowFocused()) {
      auto ev = map()->createNewEvent();
      ev->x = tileCellX();
      ev->y = tileCellY();
      m_eventEditors.emplace_back(m_parent, ev);
    }
  }

  if (((m_tileCursor.mode() == MapCursorMode::Keyboard && (ImGui::IsKeyReleased(ImGuiKey_LeftShift)) ||
        ImGui::IsKeyReleased(ImGuiKey_RightShift)) ||
       ImGui::IsMouseReleased(ImGuiMouseButton_Left)) &&
      m_parent->editMode() == EditMode::Event) {
    /* If we have a selected actor, and it's no longer in its original location, push it onto the undo stack
     * as an operation
     */
    if (m_movingEvent != nullptr && (m_movingEvent->x != m_movingEventX || m_movingEvent->y != m_movingEventY)) {
      m_parent->addUndo(std::make_shared<EventMoveUndoCommand>(m_movingEvent, m_movingEventX, m_movingEventY));
    }
    m_movingEvent = nullptr;
  }
}

void MapEditor::handleKeyboardShortcuts() {
  /* General */
  return;
  if (ImGui::IsKeyPressed(ImGuiKey_Delete) || ImGui::IsKeyPressed(ImGuiKey_Backspace)) {
    // TODO: Undo command
    if (ImGui::IsWindowFocused() && selectedEvent()) {
      map()->deleteEvent(selectedEvent()->id);
      setSelectedEvent(nullptr);
    }
  }
}

void MapEditor::updateAllAutotiles() {
  const auto rect = Rect(0, 0, map()->width, map()->height);
  updateAutotilesInRect(rect, 0);
  updateAutotilesInRect(rect, 1);
}

void MapEditor::updateAutotilesInRect(const Rect& rect, const int layer) {
  const int minX = rect.x() - 1;
  const int minY = rect.y() - 1;
  const int width = (rect.right() + 1) - minX;
  const int height = (rect.bottom() + 1) - minY;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int flags = 0x10 | 0x08 | 0x01;
      if (x != width) {
        flags = (x == 0 ? 0x40 | 0x04 | 0x02 : 0xFF);
      }

      if (y == height) {
        flags &= 0x20 | 0x02 | 0x01;
      } else if (y == 0) {
        flags &= 0x80 | 0x08 | 0x04;
      }

      Point p{minX + x, minY + y};
      if (!isMapPointValid(p)) {
        break;
      }
      updateAutotile(p, layer, flags);
    }
  }
}

void MapEditor::updateAutotile(const Point& point, const int layer, const int flags) {
  if (isRegionMode() || !isMapPointValid(point)) {
    return;
  }

  auto tileId = readMapData(point, layer);
  if (TileHelper::isFloorTypeAutotile(tileId)) {
    tileId = updateFloorTypeAutotile(point, layer, flags);
  } else if (TileHelper::isWallTypeAutotile(tileId)) {
    tileId = updateWallTypeAutotile(point, layer, flags);
  } else if (TileHelper::isWaterfallTile(tileId)) {
    tileId = updateWaterfallTypeAutotile(point, layer, flags);
  }

  writeMapData(point, layer, tileId);
  /* TODO: Flags */
  if (layer == 0 && (flags & (0x10 | 0x08 | 0x01))) {
    updateShadowData(point);
  }
}
void MapEditor::updateAutotilesAroundPoint(const Point& point, const int layer) {
  updateAutotile(point, layer, 0xFF);
  updateAutotile(Point{point.x(), point.y() - 1}, layer, 0x80 | 0x08 | 0x04);
  updateAutotile(Point{point.x() - 1, point.y()}, layer, 0x40 | 0x04 | 0x02);
  updateAutotile(Point{point.x(), point.y() + 1}, layer, 0x20 | 0x02 | 0x01);
  updateAutotile(Point{point.x() + 1, point.y()}, layer, 0x10 | 0x08 | 0x01);
  updateAutotile(Point{point.x() + 1, point.y() - 1}, layer, 0x08);
  updateAutotile(Point{point.x() - 1, point.y() - 1}, layer, 0x04);
  updateAutotile(Point{point.x() - 1, point.y() + 1}, layer, 0x02);
  updateAutotile(Point{point.x() + 1, point.y() + 1}, layer, 0x01);
}

void MapEditor::updateShadowData(const Point& point) {
  const int tileId = (isGroundTile(point, 0) && isShadowingTile(Point{point.x() - 1, point.y()}, 0) &&
                      isShadowingTile(Point{point.x() - 1, point.y() - 1}, 0))
                         ? 5
                         : 0;
  writeMapData(point, 4, tileId);
}

int MapEditor::updateFloorTypeAutotile(const Point& point, const int layer, const int flags) const {
  const auto tileId = readMapData(point, layer);
  const auto kind = TileHelper::getAutoTileKind(tileId);
  const auto shape = TileHelper::getAutoTileShape(tileId);
  const auto dir = TileHelper::floorShapeToDir(shape);
  const auto leftTop = makeDirectionBit(Point{point.x() - 1, point.y() - 1}, tileId, layer, flags & 0x01, false);
  const auto rightTop = makeDirectionBit(Point{point.x() + 1, point.y() - 1}, tileId, layer, flags & 0x02, false);
  const auto rightBottom = makeDirectionBit(Point{point.x() + 1, point.y() + 1}, tileId, layer, flags & 0x04, false);
  const auto leftBottom = makeDirectionBit(Point{point.x() - 1, point.y() + 1}, tileId, layer, flags & 0x08, false);
  const auto leftMiddle = makeDirectionBit(Point{point.x() - 1, point.y()}, tileId, layer, flags & 0x10, false);
  const auto topMiddle = makeDirectionBit(Point{point.x(), point.y() - 1}, tileId, layer, flags & 0x20, false);
  const auto rightMiddle = makeDirectionBit(Point{point.x() + 1, point.y()}, tileId, layer, flags & 0x40, false);
  const auto bottomMiddle = makeDirectionBit(Point{point.x(), point.y() + 1}, tileId, layer, flags & 0x80, false);
  const auto newShape = TileHelper::floorDirToShape(rightBottom | rightTop | leftTop | leftBottom | leftMiddle |
                                                    topMiddle | rightMiddle | bottomMiddle | (dir & ~flags));

  return TileHelper::makeAutoTileId(kind, newShape);
}

int MapEditor::updateWallTypeAutotile(const Point& point, const int layer, const int flags) const {
  const int tileId = readMapData(point, layer);
  const int autotileKind = TileHelper::getAutoTileKind(tileId);
  const int autotileShape = TileHelper::getAutoTileShape(tileId);
  const int direction = TileHelper::wallShapeToDir(autotileShape);

  const int yCoord = point.y();
  const int xCoord = point.x();

  int tmpYStart = yCoord;
  while (isSameKindTile(Point{xCoord, tmpYStart--}, layer, tileId))
    ;

  int tmpYEnd = yCoord;
  while (isSameKindTile(Point{xCoord, tmpYEnd++}, layer, tileId))
    ;

  bool hasSameKindLeft = false;
  bool hasSameKindRight = false;

  if (isSameKindTile(Point{xCoord - 1, yCoord}, layer, tileId)) {
    hasSameKindLeft = !isSameKindTile(Point{xCoord - 1, tmpYStart}, layer, tileId) ||
                      !isSameKindTile(Point{xCoord - 1, tmpYEnd}, layer, tileId);
  }

  if (isSameKindTile(Point{xCoord + 1, yCoord}, layer, tileId)) {
    hasSameKindRight = !isSameKindTile(Point{xCoord + 1, tmpYStart}, layer, tileId) ||
                       !isSameKindTile(Point{xCoord + 1, tmpYEnd}, layer, tileId);
  }

  if (TileHelper::isWallSideTile(tileId)) {
    if (!hasSameKindLeft) {
      hasSameKindLeft = !isWallOrRoofTile(Point{xCoord - 1, yCoord}, layer) ||
                        !isWallOrRoofTile(Point{xCoord - 1, tmpYStart}, layer) ||
                        !isWallOrRoofTile(Point{xCoord - 1, tmpYEnd}, layer);
    }

    if (!hasSameKindRight) {
      hasSameKindRight = !isWallOrRoofTile(Point{xCoord + 1, yCoord}, layer) ||
                         !isWallOrRoofTile(Point{xCoord + 1, tmpYStart}, layer) ||
                         !isWallOrRoofTile(Point{xCoord + 1, tmpYEnd}, layer);
    }
  }

  int updatedDirection = (direction & ~flags) & ~0x10;
  if (!hasSameKindLeft) {
    updatedDirection = (direction & ~flags) | 0x10;
  }
  updatedDirection &= ~0x40;
  if (!hasSameKindRight) {
    updatedDirection |= 0x40;
  }

  const int northDirectionBit = makeDirectionBit(Point{xCoord, yCoord - 1}, tileId, layer, flags & 0x20, true);
  const int southDirectionBit = makeDirectionBit(Point{xCoord, yCoord + 1}, tileId, layer, flags & 0x80, true);

  const int shape = TileHelper::wallDirToShape(southDirectionBit | northDirectionBit | updatedDirection);
  return TileHelper::makeAutoTileId(autotileKind, shape);
}

int MapEditor::updateWaterfallTypeAutotile(const Point& point, const int layer, const int flags) const {
  const auto tileId = readMapData(point, layer);
  const auto kind = TileHelper::getAutoTileKind(tileId);
  const auto shape = TileHelper::getAutoTileShape(tileId);
  const auto dir = TileHelper::waterfallShapeToDir(shape);
  const auto x = point.x();
  const auto y = point.y();
  const auto left = makeDirectionBit(Point{x - 1, y}, tileId, layer, flags & 0x10, false);
  const auto right = makeDirectionBit(Point{x + 1, y}, tileId, layer, flags & 0x40, false);
  return TileHelper::makeAutoTileId(kind, TileHelper::waterfallDirToShape(left | right | (dir & ~flags)));
}

int MapEditor::makeDirectionBit(const Point& nextPos, const int tileId, const int layer, const int flags,
                                const bool skipBorder) const {
  int ret = 0;
  if (flags) {
    const auto nextTileId = readMapData(nextPos, layer);
    if (nextTileId < 0) {
      return 0;
    }
    ret = flags;
    if (!skipBorder) {
      ret = 0;
      if (TileHelper::shouldCreateBorder(tileId, nextTileId)) {
        ret = flags;
      }
    }
  }
  return ret;
}

bool MapEditor::isWallOrRoofTile(const Point& p, const int layer) const {
  return TileHelper::isWallOrRoofTile(readMapData(p, layer));
}
bool MapEditor::isGroundTile(const Point& p, const int layer) const {
  return TileHelper::isGroundTile(readMapData(p, layer));
}

bool MapEditor::isShadowingTile(const Point& p, const int layer) const {
  return TileHelper::isShadowingTile(readMapData(p, layer));
}

bool MapEditor::isSameKindTile(const Point& p, const int layer, const int tileId) const {
  return TileHelper::isSameKindTile(readMapData(p, layer), tileId);
}

void MapEditor::renderLayerRects(ImGuiWindow* win, const MapRenderer::MapLayer& layer) {
  for (const auto& l : layer.tileLayers) {
    for (const auto& tile : l.rects) {
      if (!TileHelper::isAutoTile(tile.tileId)) {
        continue;
      }
      const float x0 = tile.x;
      const float x1 = tile.x + tile.tileWidth;
      const float y0 = tile.y;
      const float y1 = tile.y + tile.tileHeight;
      win->DrawList->AddRect(win->ContentRegionRect.Min + (ImVec2{x0, y0} * m_mapScale),
                             win->ContentRegionRect.Min + (ImVec2{x1, y1} * m_mapScale), 0xFF000000, 0, 0, 2.f);
    }
  }
}

void MapEditor::renderLayerTex(ImGuiWindow* win, const MapRenderer::TileLayer& tLayer) {
  if (!tLayer.tex) {
    return;
  }

  for (const TileRect& tile : tLayer.rects) {
    const float x0 = tile.x;
    const float x1 = tile.x + tile.tileWidth;
    const float y0 = tile.y;
    const float y1 = tile.y + tile.tileHeight;
    const float u0 = tile.u / tLayer.tex.width();
    const float u1 = (tile.u + tile.tileWidth) / tLayer.tex.width();
    const float v0 = tile.v / tLayer.tex.height();
    const float v1 = (tile.v + tile.tileHeight) / tLayer.tex.height();
    win->DrawList->AddImage(tLayer.tex.get(), win->ContentRegionRect.Min + (ImVec2{x0, y0} * m_mapScale),
                            win->ContentRegionRect.Min + (ImVec2{x1, y1} * m_mapScale), ImVec2{u0, v0}, ImVec2{u1, v1});
  }
}

void MapEditor::renderLayer(ImGuiWindow* win, const MapRenderer::MapLayer& layer) {
  renderLayerTex(win, layer.tileLayers[0]); // A1
  renderLayerTex(win, layer.tileLayers[1]); // A2
  renderLayerTex(win, layer.tileLayers[2]); // A3
  renderLayerTex(win, layer.tileLayers[3]); // A4
  renderLayerTex(win, layer.tileLayers[4]); // A5
  renderLayerTex(win, layer.tileLayers[5]); // B
  renderLayerTex(win, layer.tileLayers[6]); // C
  renderLayerTex(win, layer.tileLayers[7]); // D
  renderLayerTex(win, layer.tileLayers[8]); // E
  // renderLayerRects(win, layer);
}
void MapEditor::draw() {
  if (!m_checkeredBack) {
    m_checkeredBack = CheckerboardTexture(8192 * 2, 8192 * 2, CellSizes::_48, 255, 220);
  }
  // Keep mapScale to a quarter step
  if (ImGui::IsKeyDown(ImGuiKey_MouseWheelY) && ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
    m_mapScale += ImGui::GetIO().MouseWheel * 0.25f;
    m_scaleChanged = true;
  }

  m_mapScale = roundToNearestQuarter(m_mapScale);
  m_mapScale = std::clamp(m_mapScale, .25f, 4.f);

  std::erase_if(m_eventEditors, [](EventEditor& editor) { return !editor.draw(); });

  if (ImGui::Begin("Map Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar)) {
    ImGui::BeginChild("##mapcontents", ImVec2(0, ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(45.f)),
                      0, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoNav);
    // ImGui::SetScrollX(m_tileCursor.alignCoord(ImGui::GetScrollX()));
    // ImGui::SetScrollY(m_tileCursor.alignCoord(ImGui::GetScrollY()));
    if (map()) {
      ImGuiWindow* win = ImGui::GetCurrentWindow();
      ImGui::Dummy(ImVec2{(map()->width * m_parent->system().tileSize * m_mapScale),
                          (map()->height * m_parent->system().tileSize * m_mapScale)});

      const float u1 = std::clamp((static_cast<float>(map()->width * tileSize()) * m_mapScale) / (8192 * 2), 0.f, 1.f);
      const float v1 = std::clamp((static_cast<float>(map()->height * tileSize()) * m_mapScale) / (8192 * 2), 0.f, 1.f);

      win->DrawList->AddImage(m_checkeredBack.get(), win->ClipRect.Min + ImVec2{0, 0},
                              win->ClipRect.Min + (ImVec2{static_cast<float>(map()->width * tileSize()),
                                                          static_cast<float>(map()->height * tileSize())} *
                                                   m_mapScale),
                              ImVec2{0, 0}, ImVec2{u1, v1});

      handleMouseInput(win);

      if (m_tileCursor.mode() == MapCursorMode::Keyboard && !ImGui::IsWindowFocused()) {
        ImGui::FocusWindow(win, ImGuiFocusRequestFlags_UnlessBelowModal | ImGuiFocusRequestFlags_RestoreFocusedChild);
      }

      if (m_initialScrollSet) {
        ImGui::SetScrollX(m_initialScrollX);
        ImGui::SetScrollY(m_initialScrollY);
        m_initialScrollX = m_initialScrollY = 0.0;
        m_initialScrollSet = false;
      }

      if (ImGui::GetScrollX() != m_mapInfo->scrollX || ImGui::GetScrollY() != m_mapInfo->scrollY) {
        m_tileCursor.setVisible(false);
      }
      m_mapInfo->scrollX = ImGui::GetScrollX();
      m_mapInfo->scrollY = ImGui::GetScrollY();

      m_mapRenderer.update();

      drawParallax(win);

      for (int z = 0; z < 4; ++z) {
        renderLayer(win, m_mapRenderer.m_lowerLayers[z]);
        if (m_prisonMode) {
          renderLayer(win, m_mapRenderer.m_upperLayers[z]);
        }
      }

      for (int y = 0; y < map()->height; ++y) {
        for (int x = 0; x < map()->width; ++x) {
          const float dx = x * tileSize();
          const float dy = y * tileSize();
          const int bits = map()->data[m_mapRenderer.tileIdFromCoords(x, y, 4)];
          if (!(bits & 0x0F)) {
            continue;
          }

          const float w1 = tileSize() / 2;
          const float h1 = tileSize() / 2;
          for (int i = 0; i < 4; ++i) {
            if (!(bits & (1 << i))) {
              continue;
            }
            const float dx1 = dx + ((i % 2) * w1);
            const float dy1 = dy + (std::floor(i / 2) * h1);
            win->DrawList->AddRectFilled(win->ContentRegionRect.Min + (ImVec2{dx1, dy1} * m_mapScale),
                                         win->ContentRegionRect.Min + (ImVec2{dx1 + w1, dy1 + h1} * m_mapScale),
                                         0x7F000000);
          }
        }
      }

      if (m_prisonMode) {
        // drawGrid(win);
      }
      if (ImGui::IsWindowHovered() || m_parent->editMode() == EditMode::Event) {
        m_tileCursor.draw(win);
      }

      auto sortedEvents = map()->getSorted();
      for (auto& event : sortedEvents) {
        if (!event) {
          continue;
        }
        auto realEvent = map()->event(event->id);
        if (m_selectedEvent == realEvent && !m_hasScrolled) {
          ImGui::SetScrollX((win->ContentRegionRect.Min.x / 2) +
                            (((event->x * tileSize()) * m_mapScale) - (win->ContentRegionRect.Max.x / 2)));
          ImGui::SetScrollY((win->ContentRegionRect.Min.y / 2) +
                            (((event->y * tileSize()) * m_mapScale) - (win->ContentRegionRect.Max.y / 2)));
          m_hasScrolled = true;
        }
        bool isHovered = event->x == tileCellX() && event->y == tileCellY();
        MapEvent mapEvent(this, &event.value());
        mapEvent.draw(m_mapScale, isHovered, m_selectedEvent == realEvent, m_parent->editMode() != EditMode::Event,
                      win);
      }

      if (!m_prisonMode) {
        for (int z = 0; z < 6; z++) {
          renderLayer(win, m_mapRenderer.m_upperLayers[z]);
        }
      }
    }
    ImGui::EndChild();
    ImGui::BeginChild("##map_editor_bottom_panel", ImVec2{}, 0, ImGuiWindowFlags_NoBackground);
    {
      ImGui::Text("Scale:");
      ImGui::SameLine();
      ImGui::SliderFloat("##map_scale", &m_mapScale, 0.25f, 4.f);
      ImGui::SameLine();
      std::string fmt =
          std::format("Tile {}, ({}, {})", m_mapRenderer.tileId(m_tileCursor.tileX(), m_tileCursor.tileY(), 0),
                      m_tileCursor.tileX(), m_tileCursor.tileY());
      if (map()) {
        auto ev = std::find_if(map()->events.begin(), map()->events.end(), [&](const std::optional<Event>& e) {
          return e && m_tileCursor.tileX() == e->x && m_tileCursor.tileY() == e->y;
        });
        if (ev != map()->events.end()) {
          fmt += std::format(" {} ({:03})", (*ev)->name, (*ev)->id);
        }
      }
      ImGui::Text("%s", fmt.c_str());
    }
    ImGui::EndChild();
  }
  ImGui::End();
  drawMapProperties();

  // if (ImGui::BeginPopupContextWindow()) {
  //   if (ImGui::BeginPopupContextItem(""))
  // }
}

void MapEditor::drawMapProperties() {
  if (ImGui::Begin("Map Properties")) {
    char buf[4096]{};
    if (map()) {
      ImGui::BeginGroupPanel();
      {
        ImGui::SeparatorText("General Settings");
        ImGui::BeginGroup();
        {
          ImGui::Text("Name - ID: %.03i", m_mapInfo->id);
          ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - App::DPIHandler::scale_value(15));
          strncpy(buf, m_mapInfo->name.c_str(), 4096);
          if (ImGui::InputText("##map_name", buf, 4096)) {
            m_mapInfo->name = buf;
            map()->m_isDirty = true;
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
          ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - App::DPIHandler::scale_value(15));
          strncpy(buf, map()->displayName.c_str(), 4096);
          if (ImGui::InputText("##map_display_name", buf, 4096)) {
            map()->displayName = buf;
            map()->m_isDirty = true;
          }
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          ImGui::Text("Tileset");
          strncpy(buf, m_parent->currentMapInfo()->name.c_str(), 4096);
          std::string text = m_parent->tileset(map()->tilesetId)->name.empty()
                                 ? "##map_tileset_button_empty"
                                 : m_parent->tileset(map()->tilesetId)->name;
          ImGui::PushID("##map_tileset_button");
          if (ImGui::Button(text.c_str(),
                            ImVec2{ImGui::GetContentRegionMax().x / 2 - App::DPIHandler::scale_value(15), 0})) {}
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
          ImGui::SetNextItemWidth(((ImGui::GetContentRegionMax().x / 2) / 2) - App::DPIHandler::scale_value(15));
          int width = map()->width;
          if (ImGui::DragInt("##map_width", &width, 0, 0, 256)) {
            setDirty(map()->width, width, map()->m_isDirty);
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
          ImGui::SetNextItemWidth(((ImGui::GetContentRegionMax().x / 2) / 2) - App::DPIHandler::scale_value(15));
          int height = map()->height;
          if (ImGui::DragInt("##map_height", &map()->height, 0, 0, 256)) {
            setDirty(map()->height, height, map()->m_isDirty);
          }
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          ImVec2 cursorPos = ImGui::GetCursorPos();
          // Move back up a couple couple pixels
          ImGui::SetCursorPos(cursorPos);
          ImGui::Text("Scroll Type");
          ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - App::DPIHandler::scale_value(15));
          if (ImGui::BeginCombo("##map_scroll_type",
                                DecodeEnumName(magic_enum::enum_name(map()->scrollType)).c_str())) {
            for (const auto& e : magic_enum::enum_values<ScrollType>()) {
              if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(e)).c_str(), e == map()->scrollType)) {
                map()->scrollType = e;
              }
              if (e == map()->scrollType) {
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
          ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - App::DPIHandler::scale_value(15));
          if (ImGui::DragInt("##map_enc_steps", &map()->encounterStep, 0, 0, 999)) {}
        }
        ImGui::EndGroup();
      }
      ImGui::Separator();
      {
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Autoplay BGM", &map()->autoPlayBgm)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !map()->autoPlayBgm);
          {
            ImGui::PushID("##map_bgm_button");
            ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - 30);
            std::string text = map()->bgm.name.empty() ? "##map_bgm_button_empty" : map()->bgm.name;
            if (ImGui::Button(text.c_str(),
                              ImVec2{(ImGui::GetContentRegionMax().x / 2) - App::DPIHandler::scale_value(15), 0})) {}
            ImGui::PopID();
          }
          ImGui::PopItemFlag();
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Autoplay BGS", &map()->autoPlayBgs)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !map()->autoPlayBgs);
          {
            ImGui::PushID("##map_bgs_button");
            std::string text = map()->bgs.name.empty() ? "##map_bgs_button_empty" : map()->bgs.name;
            if (ImGui::Button(text.c_str(),
                              ImVec2{(ImGui::GetContentRegionMax().x / 2) - App::DPIHandler::scale_value(15), 0})) {}
            ImGui::PopID();
          }
          ImGui::PopItemFlag();
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Battleback", &map()->specifyBattleBack)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !map()->specifyBattleBack);
          {
            ImGui::PushID("##map_battleback_button");
            // TODO: Combine battleBack1Name and battleBack2Name
            std::string text = map()->bgs.name.empty() ? "##map_battleback_button_empty" : map()->battleBack1Name;
            if (ImGui::Button(text.c_str(),
                              ImVec2{(ImGui::GetContentRegionMax().x / 2) - App::DPIHandler::scale_value(15), 0})) {}
            ImGui::PopID();
          }
          ImGui::PopItemFlag();
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Disable Dashing", &map()->disableDashing)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !map()->disableDashing);
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
          std::string text = map()->parallaxName.empty() ? "##map_parallax_button_empty" : map()->parallaxName;
          if (ImGui::Button(text.c_str(),
                            ImVec2{(ImGui::GetContentRegionMax().x / 2) - App::DPIHandler::scale_value(15), 0})) {}
          ImGui::PopID();
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Loop Horizontally", &map()->parallaxLoopX)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !map()->parallaxLoopX);
          {
            ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - App::DPIHandler::scale_value(15));
            if (ImGui::DragInt("##map_parallax_Sx", &map()->parallaxSx, 0, 0, 999)) {}
          }
          ImGui::PopItemFlag();
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Loop Vertically", &map()->parallaxLoopY)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !map()->parallaxLoopY);
          {
            ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - App::DPIHandler::scale_value(15));
            if (ImGui::DragInt("##map_parallax_Sy", &map()->parallaxSy, 0, 0, 999)) {}
          }
          ImGui::PopItemFlag();
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Show in Editor", &map()->parallaxShow)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !map()->parallaxShow);
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

          strncpy(buf, map()->note.c_str(), 4096);
          if (ImGui::InputTextMultiline("##map_note", buf, 2048,
                                        ImVec2(ImGui::GetContentRegionMax().x - App::DPIHandler::scale_value(15),
                                               App::DPIHandler::scale_value(400)),
                                        flags)) {
            map()->note = buf;
          }
        }
        ImGui::Dummy(ImVec2(0.0f, 15.0f));
        ImGui::EndGroup();
      }
      ImGui::EndGroupPanel();
    }
  }
  ImGui::End();
}