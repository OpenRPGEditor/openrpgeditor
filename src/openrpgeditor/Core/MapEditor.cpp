#include "Core/MapEditor.hpp"

#include "Core/MainWindow.hpp"

#include "Core/ImGuiExt/ImGuiNotify.hpp"
#include "Core/Tilemap/TilePalette.hpp"

#include "Database/EventParser.hpp"
#include "Utils/JavaMath.hpp"

#include "imgui.h"
#include "imgui_internal.h"

#include <format>

struct EventMoveUndoCommand : IUndoCommand {
  EventMoveUndoCommand() = delete;
  EventMoveUndoCommand(Event* event, int x, int y) : m_event(event), m_x(x), m_y(y) {}
  int type() const override { return -1; }

  bool undo() override {
    // if (m_event) {
    //   std::swap(m_event->x, m_x);
    //   std::swap(m_event->y, m_y);
    //   return true;
    // }

    return false;
  }

  std::string description() override {
    return {};
    // if (!m_event) {
    //   return "Invalid move command!";
    // }
    // if (!m_isRedo) {
    //   return std::format("Move {} from {},{} to {},{}", m_event->name.empty() ? std::to_string(m_event->id) : m_event->name, m_x, m_y, m_event->x, m_event->y);
    // }
    // return std::format("Move {} from {},{} to {},{}", m_event->name.empty() ? std::to_string(m_event->id) : m_event->name, m_event->x, m_event->y, m_x, m_y);
  }

private:
  Event* m_event = nullptr;
  int m_x = -1;
  int m_y = -1;
};

void MapEditor::setMap(MapInfo* info) {
  m_mapRenderer.setMap(nullptr, nullptr);
  m_mapInfo = info;

  if (map() && map()->parallaxShow() && !map()->parallaxName().empty()) {
    m_parallaxTexture = ResourceManager::instance()->loadParallaxImage(map()->parallaxName());
  } else {
    m_parallaxTexture = Texture();
  }
}

int MapEditor::tileSize() const { return Database::instance()->system.tileSize(); }

void MapEditor::drawParallax(ImGuiWindow* win) {
  if (!m_parallaxTexture) {
    return;
  }
  // TODO: Proper parallax implementation
  win->DrawList->AddImage(m_parallaxTexture, win->ContentRegionRect.Min + ImVec2{0.f, 0.f},
                          win->ContentRegionRect.Min + (ImVec2{static_cast<float>(m_parallaxTexture.width()), static_cast<float>(m_parallaxTexture.height())} * m_mapScale));
}

void MapEditor::drawGrid(ImGuiWindow* win) {
  for (int y = tileSize() * m_mapScale; y < (map()->height() * tileSize()) * m_mapScale; y += tileSize() * m_mapScale) {
    win->DrawList->AddLine(win->ContentRegionRect.Min + ImVec2{0.f, static_cast<float>(y)}, win->ContentRegionRect.Min + ImVec2{(map()->width() * tileSize()) * m_mapScale, static_cast<float>(y)},
                           0x7f0a0a0a, 3.f);
  }

  for (int x = tileSize() * m_mapScale; x < (map()->width() * tileSize()) * m_mapScale; x += tileSize() * m_mapScale) {
    win->DrawList->AddLine(win->ContentRegionRect.Min + ImVec2{static_cast<float>(x), 0.f}, win->ContentRegionRect.Min + ImVec2{static_cast<float>(x), (map()->height() * tileSize()) * m_mapScale},
                           0x7f0a0a0a, 3.f);
  }
}
float roundToNearestQuarter(float num) { return static_cast<float>(static_cast<int>(num * 4)) / 4; }

void MapEditor::handleEventDrag() {
  if (m_movingEvent) {
    /* For now we'll prevent events from occupying the same tile */
    /* TODO(phil): Implement some way to sort through events on the same tile */
    const int oldX = m_movingEvent->x();
    const int oldY = m_movingEvent->y();

    const auto it =
        std::ranges::find_if(map()->events(), [&](const std::optional<Event>& e) { return e && e->id() != 0 && e->x() == tileCellX() && e->y() == tileCellY() && &e.value() != m_movingEvent; });

    m_movingEvent->setX(m_tileCursor.tileX());
    m_movingEvent->setY(m_tileCursor.tileY());
    if (it != map()->events().end()) {
      m_movingEvent->setX(oldX);
      m_movingEvent->setY(oldY);
    }
  }
}
void MapEditor::handleMouseInput(ImGuiWindow* win) {
  // if (map()) {
  //   map()->data[(tileCellY() * map()->width) + tileCellX()] = 2086;
  //   updateAutotilesAroundPoint(Point{tileCellX(), tileCellY()}, 0);
  // }

  if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow) || ImGui::IsKeyPressed(ImGuiKey_RightArrow) || ImGui::IsKeyPressed(ImGuiKey_UpArrow) || ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {
    m_tileCursor.setKeyboardMode();
  } else if (std::fabs(ImGui::GetIO().MouseDelta.x) > 0.5f || std::fabs(ImGui::GetIO().MouseDelta.y) > 0.5f) {
    m_tileCursor.setMouseMode();
  }

  if (ImGui::IsWindowHovered() && m_parent->editMode() == EditMode::Map) {
    m_tileCursor.update(m_mapScale, map()->width(), map()->height(), Database::instance()->system.tileSize(), win);
  } else if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left) && (m_parent->editMode() == EditMode::Event || m_movingEvent)) {
    m_tileCursor.update(m_mapScale, map()->width(), map()->height(), Database::instance()->system.tileSize(), win);
    handleEventDrag();
    m_scaleChanged = false;
  } else if (m_scaleChanged || m_tileCursor.mode() == MapCursorMode::Keyboard) {
    m_tileCursor.update(m_mapScale, map()->width(), map()->height(), Database::instance()->system.tileSize(), win);
    handleEventDrag();
    m_scaleChanged = false;
  }

  if (m_parent->editMode() == EditMode::Event && m_tileCursor.mode() == MapCursorMode::Keyboard && (ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift)) &&
      (!m_movingEvent || !m_selectedEvent)) {
    m_selectedEvent = map()->eventAt(m_tileCursor.tileX(), m_tileCursor.tileY());
    m_movingEvent = m_selectedEvent;
    if (m_movingEvent) {
      m_movingEventX = m_movingEvent->x();
      m_movingEventY = m_movingEvent->y();
    }
  } else if (m_parent->editMode() == EditMode::Event && m_tileCursor.mode() == MapCursorMode::Keyboard && (ImGui::IsKeyReleased(ImGuiKey_LeftShift) || ImGui::IsKeyReleased(ImGuiKey_RightShift))) {
    if (m_movingEvent) {
      m_movingEvent->setX(m_tileCursor.tileX());
      m_movingEvent->setY(m_tileCursor.tileY());
    }
  }

  if (m_selectedEvent && !m_movingEvent && m_selectedEvent->x() != m_tileCursor.tileX() && m_selectedEvent->y() != m_tileCursor.tileY()) {
    m_selectedEvent = nullptr;
  }

  if ((ImGui::IsMouseClicked(ImGuiMouseButton_Left) && m_parent->editMode() == EditMode::Event && (m_movingEvent || m_selectedEvent))) {
    auto event = map()->eventAt(m_tileCursor.tileX(), m_tileCursor.tileY());
    if (event == nullptr) {
      m_movingEvent = nullptr;
      m_selectedEvent = nullptr;
    } else {
      m_selectedEvent = event;
    }
    m_hasScrolled = true;
  }

  if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered() && m_parent->editMode() == EditMode::Event && m_movingEvent == nullptr) {
    m_selectedEvent = map()->eventAt(m_tileCursor.tileX(), m_tileCursor.tileY());
    m_movingEvent = m_selectedEvent;
    if (m_movingEvent) {
      m_movingEventX = m_movingEvent->x();
      m_movingEventY = m_movingEvent->y();
    }
    m_hasScrolled = true;
  }

  if ((ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) || ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter)) && m_parent->editMode() == EditMode::Event &&
      ImGui::IsWindowFocused()) {

    if (m_selectedEvent != nullptr) {
      m_selectedEvent->editor()->open();
    } else if (ImGui::IsWindowFocused()) {
      /* TODO: Use temporary event object */
      const auto ev = map()->createNewEvent();
      ev->setX(tileCellX());
      ev->setY(tileCellY());
      ev->editor()->open();
    }
  }

  if ((ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseDragging(ImGuiMouseButton_Left)) && m_parent->editMode() == EditMode::Map &&
      (ImGui::IsWindowFocused() && ImGui::IsWindowHovered()) && map()) {
    const auto& penData = m_parent->tilesetPicker().penData();
    int layer = 0;
    if (!penData.empty()) {
      if (m_parent->tilesetPicker().isRegionMode()) {
        map()->setTileAt(penData[0][0], m_tileCursor.tileX(), m_tileCursor.tileY(), 5);
      } else {
        switch (m_parent->drawTool()) {
        case DrawTool::Pencil: {
          if (layer == -1) {
            for (int i = 0; i < 4; ++i) {
              if (penData[0][i] == -1) {
                continue;
              }
              map()->setTileAt(penData[0][i], m_tileCursor.tileX(), m_tileCursor.tileY(), i);
              updateAutotilesAroundPoint({m_tileCursor.tileX(), m_tileCursor.tileY()}, i);
            }
          } else {
            int tileId = -1;
            for (int i = 1; i < 4; ++i) {
              if (penData[0][i] != -1) {
                tileId = penData[0][i];
                break;
              }
            }
            if (tileId != -1) {
              map()->setTileAt(penData[0][layer], m_tileCursor.tileX(), m_tileCursor.tileY(), layer);
              if (TileHelper::isAutoTile(tileId)) {
                updateAutotilesAroundPoint({m_tileCursor.tileX(), m_tileCursor.tileY()}, layer);
              }
            }
          }
        } break;
        case DrawTool::Rectangle:
          break;
        case DrawTool::Ellipse:
          break;
        case DrawTool::Flood_Fill:
          break;
        case DrawTool::Shadow_Pen:
          break;
        case DrawTool::Eraser:
          if (m_parent->tilesetPicker().isRegionMode()) {
            // Clear region tiles
            map()->setTileAt(0, m_tileCursor.tileX(), m_tileCursor.tileY(), 5);
          } else {
            for (int i = 0; i < 5; ++i) {
              map()->setTileAt(0, m_tileCursor.tileX(), m_tileCursor.tileY(), i);
              updateAutotilesAroundPoint({m_tileCursor.tileX(), m_tileCursor.tileY()}, i);
            }
          }
          break;
        }
      }
    }
  }

  if (((m_tileCursor.mode() == MapCursorMode::Keyboard && (ImGui::IsKeyReleased(ImGuiKey_LeftShift)) || ImGui::IsKeyReleased(ImGuiKey_RightShift)) || ImGui::IsMouseReleased(ImGuiMouseButton_Left)) &&
      m_parent->editMode() == EditMode::Event) {
    /* If we have a selected actor, and it's no longer in its original location, push it onto the undo stack
     * as an operation
     */
    if (m_movingEvent != nullptr && (m_movingEvent->x() != m_movingEventX || m_movingEvent->y() != m_movingEventY)) {
      // TODO: Actually implement undo
      // m_parent->addUndo(std::make_shared<EventMoveUndoCommand>(m_movingEvent, m_movingEventX, m_movingEventY));
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
      map()->deleteEvent(selectedEvent()->id());
      setSelectedEvent(nullptr);
    }
  }
}

void MapEditor::updateAllAutotiles() {
  const auto rect = Rect(0, 0, map()->width(), map()->height());
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
  const int tileId = (isGroundTile(point, 0) && isShadowingTile(Point{point.x() - 1, point.y()}, 0) && isShadowingTile(Point{point.x() - 1, point.y() - 1}, 0)) ? 5 : 0;
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
  const auto newShape = TileHelper::floorDirToShape(rightBottom | rightTop | leftTop | leftBottom | leftMiddle | topMiddle | rightMiddle | bottomMiddle | (dir & ~flags));

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
    hasSameKindLeft = !isSameKindTile(Point{xCoord - 1, tmpYStart}, layer, tileId) || !isSameKindTile(Point{xCoord - 1, tmpYEnd}, layer, tileId);
  }

  if (isSameKindTile(Point{xCoord + 1, yCoord}, layer, tileId)) {
    hasSameKindRight = !isSameKindTile(Point{xCoord + 1, tmpYStart}, layer, tileId) || !isSameKindTile(Point{xCoord + 1, tmpYEnd}, layer, tileId);
  }

  if (TileHelper::isWallSideTile(tileId)) {
    if (!hasSameKindLeft) {
      hasSameKindLeft = !isWallOrRoofTile(Point{xCoord - 1, yCoord}, layer) || !isWallOrRoofTile(Point{xCoord - 1, tmpYStart}, layer) || !isWallOrRoofTile(Point{xCoord - 1, tmpYEnd}, layer);
    }

    if (!hasSameKindRight) {
      hasSameKindRight = !isWallOrRoofTile(Point{xCoord + 1, yCoord}, layer) || !isWallOrRoofTile(Point{xCoord + 1, tmpYStart}, layer) || !isWallOrRoofTile(Point{xCoord + 1, tmpYEnd}, layer);
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

int MapEditor::makeDirectionBit(const Point& nextPos, const int tileId, const int layer, const int flags, const bool skipBorder) const {
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

bool MapEditor::isWallOrRoofTile(const Point& p, const int layer) const { return TileHelper::isWallOrRoofTile(readMapData(p, layer)); }
bool MapEditor::isGroundTile(const Point& p, const int layer) const { return TileHelper::isGroundTile(readMapData(p, layer)); }

bool MapEditor::isShadowingTile(const Point& p, const int layer) const { return TileHelper::isShadowingTile(readMapData(p, layer)); }

bool MapEditor::isSameKindTile(const Point& p, const int layer, const int tileId) const { return TileHelper::isSameKindTile(readMapData(p, layer), tileId); }

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
      win->DrawList->AddRect(win->ContentRegionRect.Min + (ImVec2{x0, y0} * m_mapScale), win->ContentRegionRect.Min + (ImVec2{x1, y1} * m_mapScale), 0xFF000000, 0, 0, 2.f);
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
    win->DrawList->AddImage(tLayer.tex, win->ContentRegionRect.Min + (ImVec2{x0, y0} * m_mapScale), win->ContentRegionRect.Min + (ImVec2{x1, y1} * m_mapScale), ImVec2{u0, v0}, ImVec2{u1, v1});
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
  if (m_mapInfo != nullptr && m_mapInfo->map() != nullptr && m_mapRenderer.map() != m_mapInfo->map()) {
    m_mapRenderer.setMap(m_mapInfo->map(), Database::instance()->tilesets.tileset(m_mapInfo->map()->tilesetId()));
    m_tempMapWidth = m_mapInfo->map()->width();
    m_tempMapHeight = m_mapInfo->map()->height();
  }

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

  if (map()) {
    std::ranges::for_each(map()->events(), [&](auto& event) {
      if (!event || event->id() == 0) {
        return;
      }
      event->editor()->draw();
    });
  }

  if (ImGui::Begin(trNOOP("Map Editor"), nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoTitleBar)) {
    ImGui::BeginChild("##mapcontents", ImVec2(0, ImGui::GetContentRegionAvail().y - (ImGui::CalcTextSize("S").y + (ImGui::GetStyle().FramePadding.y * 2) + ImGui::GetStyle().ItemSpacing.y)), 0,
                      ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoNav);
    // ImGui::SetScrollX(m_tileCursor.alignCoord(ImGui::GetScrollX()));
    // ImGui::SetScrollY(m_tileCursor.alignCoord(ImGui::GetScrollY()));
    if (map()) {
      ImGuiWindow* win = ImGui::GetCurrentWindow();
      ImGui::Dummy(ImVec2{(map()->width() * Database::instance()->system.tileSize() * m_mapScale), (map()->height() * Database::instance()->system.tileSize() * m_mapScale)});

      const float u1 = std::clamp((static_cast<float>(map()->width() * tileSize()) * m_mapScale) / (8192 * 2), 0.f, 1.f);
      const float v1 = std::clamp((static_cast<float>(map()->height() * tileSize()) * m_mapScale) / (8192 * 2), 0.f, 1.f);

      win->DrawList->AddImage(static_cast<ImTextureID>(m_checkeredBack), win->ClipRect.Min + ImVec2{0, 0},
                              win->ClipRect.Min + (ImVec2{static_cast<float>(map()->width() * tileSize()), static_cast<float>(map()->height() * tileSize())} * m_mapScale), ImVec2{0, 0},
                              ImVec2{u1, v1});

      handleMouseInput(win);

      if (m_selectedEvent) {
        if (ImGui::BeginPopupContextWindow()) {
          ImGui::BeginDisabled(true);
          if (ImGui::MenuItem(trNOOP("Insert template..."))) {}
          ImGui::EndDisabled();
          if (ImGui::MenuItem(trNOOP("Save as template..."))) {
            m_templateSaving = true;
            template_picker = ObjectPicker(trNOOP("Templates"), Database::instance()->templates.templateList(Template::TemplateType::Event), 0);
            template_picker->setNoSelectionMeansAdd(true);
            template_picker->setOpen(true);
          }
          ImGui::EndPopup();
        }
      } else {
        // No event selected, but it should still show if the map cursor is on the screen
        if (ImGui::BeginPopupContextWindow()) {
          if (ImGui::MenuItem(trNOOP("Insert template..."))) {
            m_templateSaving = false;
            template_picker = ObjectPicker(trNOOP("Templates"), Database::instance()->templates.templateList(Template::TemplateType::Event), 0);
            template_picker->setNoSelectionMeansAdd(false);
            template_picker->setOpen(true);
          }
          ImGui::BeginDisabled(true);
          if (ImGui::MenuItem(trNOOP("Save as template..."))) {}
          ImGui::EndDisabled();
          ImGui::EndPopup();
        }
      }
      if (template_picker) {
        auto [closed, confirmed] = template_picker->draw();
        if (closed) {
          if (confirmed) {
            EventParser parser;
            if (m_templateSaving) {
              nlohmann::ordered_json eventJson;
              EventParser::serialize(eventJson, *m_selectedEvent);
              if (template_picker.value().selection() == 0) {
                Database::instance()->templates.addTemplate(Template(Database::instance()->templates.templates.size() + 1,
                                                                     tr("New Event Template") + " " + std::to_string(Database::instance()->templates.templates.size() + 1), "",
                                                                     Template::TemplateType::Event, eventJson.dump(), {}));
                templateName_picker = TemplateName(&Database::instance()->templates.templates.back(), nullptr);
              } else {
                Database::instance()->templates.templates.at(template_picker.value().selection() - 1).setCommands(eventJson.dump());
                templateName_picker = TemplateName(&Database::instance()->templates.templates.at(template_picker.value().selection() - 1), nullptr);
              }
              Database::instance()->templates.templates.back().setCommands(eventJson.dump());

            } else {
              nlohmann::ordered_json eventJson = nlohmann::ordered_json::parse(Database::instance()->templates.templates.at(template_picker->selection() - 1).commands());

              m_templateEvent.emplace(parser.parse(eventJson));
              m_templateEvent.value().setId(map()->events().size() - 1);
              eventProperties = TemplatesEvent(&m_templateEvent.value(), map(), nullptr, tileCellX(), tileCellY());
            }
          }
          template_picker.reset();
        }
      }
      if (eventProperties) {
        eventProperties->draw();
        if (eventProperties->hasChanges()) {
          eventProperties.reset();
        }
      }
      if (templateName_picker) {
        templateName_picker->draw();
        if (templateName_picker->hasChanges()) {
          if (Database::instance()->templates.serialize(Database::instance()->basePath + "data/Templates.json")) {
            ImGui::InsertNotification(ImGuiToast{ImGuiToastType::Success, trNOOP("Saved event as template successfully!")});
          } else {
            ImGui::InsertNotification(ImGuiToast{ImGuiToastType::Error, trNOOP("Failed to saved event as template!")});
          }
          templateName_picker.reset();
        }
      }

      if (m_tileCursor.mode() == MapCursorMode::Keyboard && !ImGui::IsWindowFocused()) {
        ImGui::FocusWindow(win, ImGuiFocusRequestFlags_UnlessBelowModal | ImGuiFocusRequestFlags_RestoreFocusedChild);
      }

      if (m_initialScrollSet) {
        ImGui::SetScrollX(m_initialScrollX);
        ImGui::SetScrollY(m_initialScrollY);
        m_initialScrollX = m_initialScrollY = 0.0;
        m_initialScrollSet = false;
      }

      if (ImGui::GetScrollX() != m_mapInfo->scrollX() || ImGui::GetScrollY() != m_mapInfo->scrollY()) {
        m_tileCursor.setVisible(false);
      }
      m_mapInfo->setScrollX(ImGui::GetScrollX());
      m_mapInfo->setScrollY(ImGui::GetScrollY());

      m_mapRenderer.update();

      drawParallax(win);

      for (int z = 0; z < 4; ++z) {
        renderLayer(win, m_mapRenderer.m_lowerLayers[z]);
        if (m_prisonMode) {
          renderLayer(win, m_mapRenderer.m_upperLayers[z]);
        }
      }

      for (int y = 0; y < map()->height(); ++y) {
        for (int x = 0; x < map()->width(); ++x) {
          auto tile = map()->data()[m_mapRenderer.tileIdFromCoords(x, y, 4)];
          if (!tile) {
            continue;
          }
          const float dx = x * tileSize();
          const float dy = y * tileSize();
          const int bits = *tile;
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
            win->DrawList->AddRectFilled(win->ContentRegionRect.Min + (ImVec2{dx1, dy1} * m_mapScale), win->ContentRegionRect.Min + (ImVec2{dx1 + w1, dy1 + h1} * m_mapScale), 0x7F000000);
          }
        }
      }

      if (m_prisonMode) {
        // drawGrid(win);
      }
      if (ImGui::IsWindowHovered() || m_parent->editMode() == EditMode::Event) {
        m_tileCursor.draw(win);
      }

      auto sortedEvents = prisonMode() ? map()->getSorted() : map()->getRenderSorted();

      // for (const auto& event : sortedEvents) {
      //   EventPage* page = !prisonMode() ? event->page(0) : event->renderer()->page();
      //   if (!event || event->id() == 0 || page->priorityType != EventPriority::Below_character) {
      //     continue;
      //   }
      //   if (m_selectedEvent == event && !m_hasScrolled) {
      //     ImGui::SetScrollX((win->ContentRegionRect.Min.x / 2) + (((event->x() * tileSize()) * m_mapScale) - (win->ContentRegionRect.Max.x / 2)));
      //     ImGui::SetScrollY((win->ContentRegionRect.Min.y / 2) + (((event->y() * tileSize()) * m_mapScale) - (win->ContentRegionRect.Max.y / 2)));
      //     m_hasScrolled = true;
      //   }
      //   bool isHovered = event->x() == tileCellX() && event->y() == tileCellY();
      //   if (auto* renderer = static_cast<MapEvent*>(event->renderer())) {
      //     renderer->setMapEditor(this);
      //     renderer->draw(m_mapScale, isHovered, m_selectedEvent == event, m_parent->editMode() != EditMode::Event);
      //   }
      // }

      for (const auto& event : sortedEvents) {
        EventPage* page = !prisonMode() ? event->page(0) : event->renderer()->page();
        if (!event || event->id() == 0 /*|| page->priorityType != EventPriority::Same_as_characters*/) {
          continue;
        }
        if (m_selectedEvent == event && !m_hasScrolled) {
          ImGui::SetScrollX((win->ContentRegionRect.Min.x / 2) + (((event->x() * tileSize()) * m_mapScale) - (win->ContentRegionRect.Max.x / 2)));
          ImGui::SetScrollY((win->ContentRegionRect.Min.y / 2) + (((event->y() * tileSize()) * m_mapScale) - (win->ContentRegionRect.Max.y / 2)));
          m_hasScrolled = true;
        }
        bool isHovered = event->x() == tileCellX() && event->y() == tileCellY();
        if (auto* renderer = static_cast<MapEvent*>(event->renderer())) {
          renderer->setMapEditor(this);
          renderer->draw(m_mapScale, isHovered, m_selectedEvent == event, m_parent->editMode() != EditMode::Event);
        }
      }

      if (!m_prisonMode) {
        for (int z = 0; z < 6; z++) {
          renderLayer(win, m_mapRenderer.m_upperLayers[z]);
        }
      }

      // for (auto& event : sortedEvents) {
      //   EventPage* page = !prisonMode() ? event->page(0) : event->renderer()->page();
      //   if (!event || event->id() == 0 || page->priorityType != EventPriority::Above_characters) {
      //     continue;
      //   }
      //   auto realEvent = map()->event(event->id());
      //   if (m_selectedEvent == realEvent && !m_hasScrolled) {
      //     ImGui::SetScrollX((win->ContentRegionRect.Min.x / 2) + (((event->x() * tileSize()) * m_mapScale) - (win->ContentRegionRect.Max.x / 2)));
      //     ImGui::SetScrollY((win->ContentRegionRect.Min.y / 2) + (((event->y() * tileSize()) * m_mapScale) - (win->ContentRegionRect.Max.y / 2)));
      //     m_hasScrolled = true;
      //   }
      //   bool isHovered = event->x() == tileCellX() && event->y() == tileCellY();
      //   if (auto* renderer = static_cast<MapEvent*>(event->renderer())) {
      //     renderer->setMapEditor(this);
      //     renderer->draw(m_mapScale, isHovered, m_selectedEvent == realEvent, m_parent->editMode() != EditMode::Event);
      //   }
      // }
    }
    ImGui::EndChild();
    ImGui::BeginChild("##map_editor_bottom_panel", ImVec2{}, 0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    {
      ImGui::Separator();
      ImGui::Text("%s", trNOOP("Scale:"));
      ImGui::SameLine();
      ImGui::SliderFloat("##map_scale", &m_mapScale, 0.25f, 4.f);
      ImGui::SameLine();
      // TL-NOTE: The braces denote the tile ID x and y positions, they get replaced at runtime with those values
      std::string fmt = trFormat("Tile {}, ({}, {})", m_mapRenderer.tileId(m_tileCursor.tileX(), m_tileCursor.tileY(), 0), m_tileCursor.tileX(), m_tileCursor.tileY());
      if (map()) {
        if (const auto ev = std::ranges::find_if(map()->events(), [&](const std::optional<Event>& e) { return e && e->id() != 0 && m_tileCursor.tileX() == e->x() && m_tileCursor.tileY() == e->y(); });
            ev != map()->events().end()) {
          fmt += std::format(" {} ({:03})", (*ev)->name(), (*ev)->id());
        }
      }
      ImGui::Text("%s", fmt.c_str());
    }
    ImGui::EndChild();
  }
  ImGui::End();
}

double MapEditor::adjustX(double x) { return x + ((ImGui::GetCurrentWindow()->ContentRegionRect.Min.x / tileSize()) * m_mapScale); }
double MapEditor::adjustY(double y) { return y + ((ImGui::GetCurrentWindow()->ContentRegionRect.Min.y / tileSize()) * m_mapScale); }
double MapEditor::roundX(const double x) { return isLoopHorizontally() ? Math::mod(x, map()->width()) : x; }
double MapEditor::roundY(const double y) { return isLoopVertically() ? Math::mod(y, map()->height()) : y; }
double MapEditor::xWithDirection(const double x, const Direction d) { return x + (d == Direction::Right ? 1 : d == Direction::Left ? -1 : 0); }
double MapEditor::yWithDirection(const double y, const Direction d) { return y + (d == Direction::Down ? 1 : d == Direction::Up ? -1 : 0); }
double MapEditor::roundXWithDirection(const double x, const Direction d) { return roundX(x + (d == Direction::Right ? 1 : d == Direction::Left ? -1 : 0)); }
double MapEditor::roundYWithDirection(const double y, const Direction d) { return roundY(y + (d == Direction::Down ? 1 : d == Direction::Up ? -1 : 0)); }
