#pragma once

#include "Database/MapInfo.hpp"

class MapInfos final : public IModifiable {
public:
  MapInfos() { m_mapinfos.emplace_back(); }
  friend class MapInfosSerializer;
  static MapInfos load(std::string_view filepath);
  bool serialize(std::string_view filename);

  [[nodiscard]] MapInfo* map(int id) {
    for (auto& item : m_mapinfos) {
      if (item && item->id() == id) {
        return &item.value();
      }
    }
    return nullptr;
  }

  [[nodiscard]] const MapInfo* map(int id) const {
    for (const auto& set : m_mapinfos) {
      if (set && set->id() == id) {
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

  bool isModified() const override {
    return std::ranges::any_of(m_mapinfos, [](const auto& item) { return item && item->isModified(); });
  }

  void closeAllUnmodified() {
    for (auto& info : m_mapinfos) {
      if (!info) {
        continue;
      }
      info->closeIfNotModified();
    }
  }

  void connectSignals();

private:
  std::vector<std::optional<MapInfo>> m_mapinfos;
  MapInfo* m_currentMap = nullptr;

  void mapLoadCallback(const std::shared_ptr<ISerializable>& data);
  void onMapInfoModified(MapInfo* mapInfo, Map* map);
};
