#pragma once

#include "Database/MapInfo.hpp"

class MapInfos {
public:
  static MapInfos load(std::string_view filepath);
  bool serialize(std::string_view filename);

  [[nodiscard]] MapInfo* map(int id) {
    for (auto& item : m_mapinfos) {
      if (item->id() == id) {
        return &item.value();
      }
    }
    return nullptr;
  }

  [[nodiscard]] const MapInfo* map(int id) const {
    for (const auto& set : m_mapinfos) {
      if (set->id() == id) {
        return &set.value();
      }
    }

    return nullptr;
  }

  bool empty() const { return m_mapinfos.empty(); }

  MapInfo& root() { return m_mapinfos[0].value(); }
  const MapInfo& root() const { return m_mapinfos[0].value(); }

  void buildTree(bool reset = false);

  std::vector<std::optional<MapInfo>>& mapInfos() { return m_mapinfos; }
  const std::vector<std::optional<MapInfo>>& mapInfos() const { return m_mapinfos; }

  void setCurrentMap(MapInfo* map) { m_currentMap = map; }
  MapInfo* currentMap() { return m_currentMap; }
  const MapInfo* currentMap() const { return m_currentMap; }

  void loadAllMaps();

  void rebuildOrdering();

private:
  std::vector<std::optional<MapInfo>> m_mapinfos;
  MapInfo* m_currentMap = nullptr;

  void mapLoadCallback(const std::shared_ptr<ISerializable>& data);
};
