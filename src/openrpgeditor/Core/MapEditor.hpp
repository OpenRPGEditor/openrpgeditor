#pragma once

#include "Core/Graphics/CheckerboardTexture.hpp"

#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventEditor.hpp"
#include "Database/MapInfos.hpp"
#include "Graphics/ParallaxTexture.hpp"
#include "MapEditor/MapCursor.hpp"
#include "MapEditor/MapProperties.hpp"
#include "OREMath/Rect.hpp"
#include "TemplateEditor/Dialog/TemplateName.hpp"
#include "TemplateEditor/Dialog/TemplatesEvent.hpp"
#include "Tilemap/TilemapView.hpp"

struct MainWindow;
struct Map;
struct MapInfo;

struct MapEditor {
  MapEditor(MainWindow* parent)
  : m_parent(parent)
  , m_tileCursor(this) {}

  void draw(bool closeRequested = false);

  void setMap(MapInfo* info);

  void clearLayers() {}

  void scale(float scale) {
    m_mapScale += scale;
    m_scaleChanged = true;
  }

  void setScale(float scale) {
    if (scale != m_mapScale) {
      m_scaleChanged = true;
    }
    m_mapScale = scale;
  }

  int tileCellX() const { return m_tileCursor.tileX(); }
  int tileCellY() const { return m_tileCursor.tileY(); }
  int tileAbsoluteX() const { return m_tileCursor.absoluteX(); }
  int tileAbsoluteY() const { return m_tileCursor.absoluteY(); }

  int tileSize() const;

  Event* selectedEvent() { return m_selectedEvent; }
  const Event* selectedEvent() const { return m_selectedEvent; }

  void setSelectedEvent(Event* event) {
    m_selectedEvent = event;
    if (event) {
      m_tileCursor.setPosition(event->x(), event->y());
      m_hasScrolled = false;
    }
  }

  bool scrolledToEvent() const { return m_hasScrolled; }

  Event* movingEvent() { return m_movingEvent; }
  const Event* movingEvent() const { return m_movingEvent; }
  void setMovingEvent(Event* event) { m_movingEvent = event; }

  TilemapView& tilemapView() { return m_tilemapView; }
  const TilemapView& tilemapView() const { return m_tilemapView; }

  bool prisonMode() const { return m_prisonMode; }
  void togglePrisonMode() { m_prisonMode ^= 1; }
  void setPrisonMode(const bool prisonMode) { m_prisonMode = prisonMode; }

  Map* map() { return m_mapInfo ? m_mapInfo->map() : nullptr; }
  const Map* map() const { return m_mapInfo ? m_mapInfo->map() : nullptr; }

  MainWindow* project() { return m_parent; }
  const MainWindow* project() const { return m_parent; }

  float zoom() const { return m_mapScale; }

  Rect mapRect() const {
    if (!map()) {
      return {};
    }

    return Rect{0, 0, map()->width(), map()->height()};
  }

  bool isMapPointValid(const Point& point) const { return mapRect().contains(point); }
  bool isRegionMode() const { return m_regionMode; }

  int readMapData(const Point& point, const int layer) const {
    if (!map()) {
      return 0;
    }
    return map()->tileAt(point.x(), point.y(), layer);
  }

  void writeMapData(const Point& point, const int layer, const int tileId) {
    if (!map()) {
      return;
    }
    map()->setTileAt(tileId, point.x(), point.y(), layer);
    m_tilemapView.setTileDirty(point.x(), point.y());
  }

  double adjustX(double x);

  double adjustY(double y);

  double roundX(double x);

  double roundY(double y);

  double xWithDirection(double x, Direction d);

  double yWithDirection(double y, Direction d);

  double roundXWithDirection(double x, Direction d);

  double roundYWithDirection(double y, Direction d);

  bool isValid(const double x, const double y) {
    if (!map()) {
      return false;
    }

    return mapRect().contains(x, y);
  }

  bool isPassable(const double x, const double y, Direction d) const { return m_tilemapView.isPassable(x, y, static_cast<int>(d)); }

  std::vector<Event*> eventsAtNoThrough(int x, int y) {
    if (!map()) {
      return {};
    }
    return map()->eventsAtNoThrough(x, y);
  }

  double deltaX(double x1, double x2) const {
    if (!map()) {
      return 0;
    }
    double result = x1 - x2;
    if (isLoopHorizontally() && std::abs(result) > map()->width() / 2) {
      if (result < 0) {
        result += map()->width();
      } else {
        result -= map()->width();
      }
    }

    return result;
  }

  double deltaY(const double y1, const double y2) const {
    if (!map()) {
      return 0;
    }
    double result = y1 - y2;
    if (isLoopVertically() && std::abs(result) > map()->height() / 2) {
      if (result < 0) {
        result += map()->height();
      } else {
        result -= map()->height();
      }
    }

    return result;
  }

  bool isLoopHorizontally() const {
    if (!map()) {
      return false;
    }

    return map()->scrollType() == ScrollType::Loop_Horizontally || map()->scrollType() == ScrollType::Loop_Both;
  }

  bool isLoopVertically() const {
    if (!map()) {
      return false;
    }
    return map()->scrollType() == ScrollType::Loop_Vertically || map()->scrollType() == ScrollType::Loop_Both;
  }

  void preRender() { m_tilemapView.render(); }

private:
  void drawParallax(ImGuiWindow* win);

  void drawGrid(ImGuiWindow* win);

  void handleEventDrag();

  void handleMouseInput(ImGuiWindow* win);

  void handleKeyboardShortcuts();

  void updateAllAutotiles();

  void updateAutotilesInRect(const Rect& rect, int layer);

  void updateAutotile(const Point& point, int layer, int flags);

  void updateAutotilesAroundPoint(const Point& point, int layer);

  void updateShadowData(const Point& point);

  int updateFloorTypeAutotile(const Point& point, int layer, int flags) const;

  int updateWallTypeAutotile(const Point& point, int layer, int flags) const;

  int updateWaterfallTypeAutotile(const Point& point, int layer, int flags) const;

  int makeDirectionBit(const Point& nextPos, int tileId, int layer, int flags, bool skipBorder) const;

  bool isGroundTile(const Point& p, int layer) const;

  bool isShadowingTile(const Point& p, int layer) const;

  bool isSameKindTile(const Point& p, int layer, int tileId) const;

  bool isWallOrRoofTile(const Point& p, int layer) const;

  MainWindow* m_parent;
  MapInfo* m_mapInfo = nullptr;
  float m_mapScale = 1.f;
  bool m_scaleChanged = false;
  double m_initialScrollX = 0.0;
  double m_initialScrollY = 0.0;
  bool m_initialScrollSet = false;
  MapCursor m_tileCursor;

  int m_tileId{};
  ParallaxTexture m_parallaxTexture;
  CheckerboardTexture m_checkeredBack;
  Event* m_movingEvent = nullptr;
  Event* m_selectedEvent = nullptr;
  bool m_hasScrolled = true;
  bool m_prisonMode{true};
  bool m_regionMode{false};
  bool m_templateSaving{false};

  std::optional<TemplatePicker> m_templatePicker;
  std::optional<TemplatesEvent> m_eventProperties;
  std::optional<TemplateName> m_templateNamePicker;
  std::optional<Event> m_templateEvent;
  TilemapView m_tilemapView{48, 48};

  int m_movingEventX = -1;
  int m_movingEventY = -1;
  int m_tempMapWidth = 0;
  int m_tempMapHeight = 0;

  void onParallaxNameModified(Map* info, const std::string& name);
};
