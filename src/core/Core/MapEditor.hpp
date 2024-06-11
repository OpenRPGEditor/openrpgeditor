#pragma once

#include "MapRenderer.hpp"

struct Project;
struct Map;
struct MapInfo;
struct MapEditor {
  MapEditor(Project* parent) : m_parent(parent) {}
  void draw();

  void setMap(Map* map, MapInfo* info);

  void clearLayers() {
    m_lowerLayer.clear();
    m_upperLayer.clear();
  }

private:
  Project* m_parent;
  Map* m_map = nullptr;
  MapInfo* m_mapInfo = nullptr;
  float m_mapScale = 1.f;
  double m_initialScrollX = 0.0;
  double m_initialScrollY = 0.0;
  bool m_initialScrollSet = false;
  int m_tileCellX{};
  int m_tileCellY{};
  int m_tileId{};
  Texture m_dummyTex;
  MapRenderer m_mapRenderer;
  std::vector<TileRect> m_lowerLayer;
  std::vector<TileRect> m_upperLayer;
};
