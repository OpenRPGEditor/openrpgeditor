#pragma once

#include "nlohmann/json.hpp"
#include "Database/Map.hpp"
#include <string>

struct MapInfo {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(MapInfo, expanded, id, name, order, parentId, scrollX, scrollY);
  bool expanded;
  int id;
  std::string name;
  int order;
  int parentId;
  double scrollX;
  double scrollY;

  std::vector<MapInfo*>& children() { return m_children; }
  const std::vector<MapInfo*>& children() const { return m_children; }

  Map* map() { return m_map.get(); }
  const Map* map() const { return m_map.get(); }

  Event* event(int id) { return m_map->event(id); }
  const Event* event(int id) const { return m_map->event(id); }

private:
  friend void recursiveSort(MapInfo& in);
  friend class MapInfos;
  std::vector<MapInfo*> m_children;
  std::unique_ptr<Map> m_map;
};

class MapInfos {
public:
  static MapInfos load(std::string_view filepath);
  bool serialize(std::string_view filename);

  [[nodiscard]] MapInfo* map(int id) {
    for (auto& item : m_mapinfos) {
      if (item->id == id) {
        return &item.value();
      }
    }
    return nullptr;
  }

  [[nodiscard]] const MapInfo* map(int id) const {
    for (const auto& set : m_mapinfos) {
      if (set->id == id) {
        return &set.value();
      }
    }

    return nullptr;
  }

  bool empty() const {
    // If the vector is actually empty or we only have the dummy root node we have no maps
    // This allows for cases when a MapInfos.json only has it's root node
    // That being said the editor should *not* allow the user to delete their last map
    return m_mapinfos.empty() || m_mapinfos.size() == 1;
  }

  MapInfo& root() { return m_mapinfos[0].value(); }
  const MapInfo& root() const { return m_mapinfos[0].value(); }

  void buildTree(bool reset = false);

  std::vector<std::optional<MapInfo>>& mapInfos() { return m_mapinfos; }
  const std::vector<std::optional<MapInfo>>& mapInfos() const { return m_mapinfos; }

  void setCurrentMap(MapInfo* map) { m_currentMap = map; }
  MapInfo* currentMap() { return m_currentMap; }
  const MapInfo* currentMap() const { return m_currentMap; }

private:
  std::vector<std::optional<MapInfo>> m_mapinfos;
  MapInfo* m_currentMap = nullptr;
};
