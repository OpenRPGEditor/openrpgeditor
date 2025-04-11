#include "Database/MapInfos.hpp"

#include "Database/Database.hpp"
#include "Database/Map.hpp"
#include "Serializable/FileQueue.hpp"
#include "Serializable/MapSerializer.hpp"

#include <fstream>

using json = nlohmann::ordered_json;

MapInfos MapInfos::load(std::string_view filename) {
  std::ifstream file(filename.data());
  json data = json::parse(file);
  MapInfos mapinfos;
  mapinfos.m_mapinfos.reserve(data.size());

  for (const auto& [_, value] : data.items()) {
    auto& mapinfo = mapinfos.m_mapinfos.emplace_back();
    value.get_to(mapinfo);
  }
  mapinfos.m_mapinfos[0].emplace();
  mapinfos.m_mapinfos.shrink_to_fit();
  mapinfos.buildTree();
  return mapinfos;
}

bool MapInfos::serialize(std::string_view filename) {
  // TODO: Temporary file for atomic writes

  std::ofstream file(filename.data());
  json data{nullptr};

  for (int i = 1; i < m_mapinfos.size(); ++i) {
    data.push_back(m_mapinfos[i]);
  }

  file << data.dump(4);

  return true;
}

void recursiveSort(MapInfo& in) {
  std::ranges::sort(in.m_children, [](const MapInfo* a, const MapInfo* b) { return a->order() < b->order(); });

  for (auto& mapInfo : in.m_children) {
    recursiveSort(*mapInfo);
  }
}

void MapInfos::buildTree(const bool reset) {
  if (reset) {
    for (auto& mapInfo : m_mapinfos) {
      if (!mapInfo) {
        continue;
      }
      mapInfo->m_children.clear();
    }
  }
  for (auto& mapInfo : m_mapinfos) {
    if (!mapInfo || mapInfo->id() == 0) {
      continue;
    }
    if (mapInfo->parentId() >= 0 && mapInfo->parentId() < m_mapinfos.size()) {
      m_mapinfos[mapInfo->parentId()]->m_children.push_back(&mapInfo.value());
    }
  }

  recursiveSort(root());
}

void recursiveRebuildOrdering(MapInfo& in, int& order) {
  in.setOrder(order++);
  for (auto* mapinfo : in.children()) {
    if (mapinfo) {
      recursiveRebuildOrdering(*mapinfo, order);
    }
  }
}

void MapInfos::rebuildOrdering() {
  if (m_mapinfos.empty()) {
    return;
  }

  int order = 0;
  recursiveRebuildOrdering(m_mapinfos[0].value(), order);
}

void MapInfos::loadAllMaps() {
  for (const auto& mapinfo : m_mapinfos) {
    if (mapinfo && mapinfo->id() != 0 && !mapinfo->mapLoaded()) {
      //      if (auto map = Database::instance()->loadMap(mapinfo->id); map.m_isValid) {
      //        mapinfo->m_map = std::make_unique<Map>(map);
      //      }
      FileQueue::instance().enqueue(std::make_shared<MapSerializer>(std::format("data/Map{:03}.json", mapinfo->id()), mapinfo->id()),
                                    [this]<typename T>(T&& handle) { mapLoadCallback(std::forward<T>(handle)); });
    }
  }
}

static std::mutex MapInfosMutex;
void MapInfos::mapLoadCallback(const std::shared_ptr<ISerializable>& data) {
  if (const auto mapSerializable = std::dynamic_pointer_cast<MapSerializer>(data)) {
    std::lock_guard lock(MapInfosMutex);
    auto& mapinfo = m_mapinfos[mapSerializable->mapId()];
    if (const auto& map = mapSerializable->data(); mapinfo && map.isValid()) {
      mapinfo->m_map = std::make_unique<Map>(map);
    }
  }
}