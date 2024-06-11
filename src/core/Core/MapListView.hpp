#pragma once

struct Project;
struct MapInfos;
struct MapInfo;
struct MapListView {
  MapListView(Project* parent) : m_parent(parent) {}

  void setMapInfos(MapInfos* mapInfos) { m_mapInfos = mapInfos; }

  void draw();

  MapInfo* currentMapInfo();
  const MapInfo* currentMapInfo() const;


  void reset() {
    m_mapInfos = nullptr;
    m_selectedMapId = -1;
  }

  void setCurrentMapId(int id) { m_selectedMapId = id; }
private:
  void recursiveDrawTree(MapInfo& in);
  Project* m_parent = nullptr;
  MapInfos* m_mapInfos = nullptr;
  int m_selectedMapId = -1;
  bool m_mapTreeStale{false};
};
