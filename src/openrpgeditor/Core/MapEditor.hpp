#pragma once

#include "Core/Graphics/CheckerboardTexture.hpp"
#include "Core/MapEditor/MapEvent.hpp"
#include "Core/MapEditor/MapRenderer.hpp"

#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventEditor.hpp"
#include "Database/MapInfos.hpp"
#include "MapEditor/MapCursor.hpp"
#include "OREMath/Rect.hpp"
#include "TemplateEditor/Dialog/TemplateName.hpp"

struct Project;
struct Map;
struct MapInfo;
struct MapEditor {
  MapEditor(Project* parent) : m_parent(parent), m_tileCursor(this) {}
  void draw();

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
      m_tileCursor.setPosition(event->x, event->y);
      m_hasScrolled = false;
    }
  }

  bool scrolledToEvent() const { return m_hasScrolled; }

  Event* movingEvent() { return m_movingEvent; }
  const Event* movingEvent() const { return m_movingEvent; }
  void setMovingEvent(Event* event) { m_movingEvent = event; }

  void addEventEditor(const EventEditor& editor) {
    auto it = std::find_if(m_eventEditors.begin(), m_eventEditors.end(), [editor](const EventEditor& ed) { return editor.event()->id == ed.event()->id; });
    if (it == m_eventEditors.end()) {
      m_eventEditors.push_back(editor);
    }
  }

  MapRenderer& mapRenderer() { return m_mapRenderer; }
  const MapRenderer& mapRenderer() const { return m_mapRenderer; }

  bool prisonMode() const { return m_prisonMode; }
  void togglePrisonMode() { m_prisonMode ^= 1; }
  void setPrisonMode(const bool prisonMode) { m_prisonMode = prisonMode; }

  Map* map() { return m_mapInfo ? m_mapInfo->map() : nullptr; }
  const Map* map() const { return m_mapInfo ? m_mapInfo->map() : nullptr; }

  Project* project() { return m_parent; }
  const Project* project() const { return m_parent; }

  float zoom() const { return m_mapScale; }

  Rect mapRect() const {
    if (!map()) {
      return {};
    }

    return Rect{0, 0, map()->width(), map()->height()};
  }

  bool isMapPointValid(const Point& point) const { return mapRect().contains(point); }
  bool isRegionMode() const { return m_regionMode; }

  int readMapData(const Point& point, const int layer) const { return m_mapRenderer.tileId(point.x(), point.y(), layer); }
  void writeMapData(const Point& point, const int layer, const int tileId) {
    if (!map()) {
      return;
    }
    map()->data()[m_mapRenderer.tileIdFromCoords(point.x(), point.y(), layer)] = tileId;
  }

private:
  void drawMapProperties();
  void drawParallax(ImGuiWindow* win);
  void drawGrid(ImGuiWindow* win);
  void handleEventDrag();
  void handleMouseInput(ImGuiWindow* win);
  void renderLayerRects(ImGuiWindow* win, const MapRenderer::MapLayer& layer);
  void renderLayerTex(ImGuiWindow* win, const MapRenderer::TileLayer& tLayer);
  void renderLayer(ImGuiWindow* win, const MapRenderer::MapLayer& layer);
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

  void resizeMap(int width, int height);

  Project* m_parent;
  MapInfo* m_mapInfo = nullptr;
  float m_mapScale = 1.f;
  bool m_scaleChanged = false;
  double m_initialScrollX = 0.0;
  double m_initialScrollY = 0.0;
  bool m_initialScrollSet = false;
  MapCursor m_tileCursor;

  int m_tileId{};
  Texture m_parallaxTexture;
  MapRenderer m_mapRenderer;
  std::vector<EventEditor> m_eventEditors;
  CheckerboardTexture m_checkeredBack;
  Event* m_movingEvent = nullptr;
  Event* m_selectedEvent = nullptr;
  bool m_hasScrolled = true;
  bool m_prisonMode{true};
  bool m_regionMode{false};
  bool m_templateSaving{false};

  std::optional<ObjectPicker<Template>> template_picker;
  std::optional<TemplatesEvent> eventProperties;
  std::optional<TemplateName> templateName_picker;
  std::optional<Event> m_templateEvent;

  int m_movingEventX = -1;
  int m_movingEventY = -1;
  int m_tempMapWidth = 0;
  int m_tempMapHeight = 0;
};
