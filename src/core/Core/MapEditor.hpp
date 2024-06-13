#pragma once

#include "Core/MapEditor/MapRenderer.hpp"
#include "Core/MapEditor/MapEvent.hpp"

#include "Core/EventEditor.hpp"
#include "MapEditor/MapCursor.hpp"

struct Project;
struct Map;
struct MapInfo;
struct MapEditor {
  MapEditor(Project* parent) : m_parent(parent), m_tileCursor(this) {}
  void draw();

  void setMap(Map* map, MapInfo* info);

  void clearLayers() {
    m_lowerLayer.clear();
    m_upperLayer.clear();
  }

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
  int tileSize();

  Event* selectedEvent() { return m_selectedEvent; }
  const Event* selectedEvent() const { return m_selectedEvent; }
  void setSelectedEvent(Event* event) {
    m_selectedEvent = event;
    if (event) {
      m_tileCursor.setPosition(event->x, event->y);
    }
    m_hasScrolled = false;
  }

  bool scrolledToEvent() const { return m_hasScrolled; }

  Event* movingEvent() { return m_movingEvent; }
  const Event* movingEvent() const { return m_movingEvent; }
  void setMovingEvent(Event* event) { m_movingEvent = event; }

  void addEventEditor(const EventEditor& editor) {
    auto it = std::find_if(m_eventEditors.begin(), m_eventEditors.end(),
                           [editor](const EventEditor& ed) { return editor.event()->id == ed.event()->id; });
    if (it == m_eventEditors.end()) {
      m_eventEditors.push_back(editor);
    }
  }

  bool prisonMode() const { return m_prisonMode; }
  void togglePrisonMode() { m_prisonMode ^= 1; }
  void setPrisonMode(const bool prisonMode) { m_prisonMode = prisonMode; }

  Map* map() { return m_map; }
  const Map* map() const { return m_map; }

  Project* project() { return m_parent; }
  const Project* project() const { return m_parent; }

private:
  void drawMapProperties();
  void drawGrid(ImGuiWindow* win);
  void handleEventDrag();
  void handleMouseInput(ImGuiWindow* win);
  Project* m_parent;
  Map* m_map = nullptr;
  MapInfo* m_mapInfo = nullptr;
  float m_mapScale = 1.f;
  bool m_scaleChanged = false;
  double m_initialScrollX = 0.0;
  double m_initialScrollY = 0.0;
  bool m_initialScrollSet = false;
  MapCursor m_tileCursor;

  int m_tileId{};
  Texture m_dummyTex;
  MapRenderer m_mapRenderer;
  std::vector<TileRect> m_lowerLayer;
  std::vector<TileRect> m_upperLayer;
  std::vector<EventEditor> m_eventEditors;
  Event* m_movingEvent = nullptr;
  Event* m_selectedEvent = nullptr;
  bool m_hasScrolled = true;
  bool m_prisonMode{true};

  int m_movingEventX = -1;
  int m_movingEventY = -1;
};
