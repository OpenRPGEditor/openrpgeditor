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
  void handleEventMouseInteraction(std::optional<Event>& event, bool isHovered);
  void drawMapProperties();
  void drawGrid(ImGuiWindow* win);
  void draw();

  void setMap(Map* map, MapInfo* info);

  void clearLayers() {
    m_lowerLayer.clear();
    m_upperLayer.clear();
  }

  int tileCellX() { return m_tileCursor.tileX(); }
  int tileCellY() { return m_tileCursor.tileY(); }
  int tileSize();

private:
  Project* m_parent;
  Map* m_map = nullptr;
  MapInfo* m_mapInfo = nullptr;
  float m_mapScale = 1.f;
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
  Event* m_selectedEvent = nullptr;

  int m_selectedEventX = -1;
  int m_selectedEventY = -1;
};
