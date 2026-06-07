#include "Database/MapInfos.hpp"

#include "Database/Database.hpp"
#include "Database/Map.hpp"
#include "Serializable/FileQueue.hpp"
#include "Serializable/MapSerializer.hpp"

#include <fstream>

using json = nlohmann::ordered_json;

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
      if (!m_mapinfos[mapInfo->parentId()]) {
        continue;
      }
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
      FileQueue::instance().enqueue(std::make_shared<MapSerializer>(std::format("data/Map{:03}.json", mapinfo->id()), mapinfo->id()),
                                    [this]<typename T>(T&& handle) { mapLoadCallback(std::forward<T>(handle)); });
    }
  }
}

MapInfo* MapInfos::createMapAt(int id, const int width, const int height, const int parent) {
  if (id <= 0) {
    // check if we have an invalid id
    id = m_mapinfos.size();
  }

  if (id >= m_mapinfos.size()) {
    m_mapinfos.resize(id + 1);
  }

  m_mapinfos[id].emplace();
  m_mapinfos[id]->setId(id);
  m_mapinfos[id]->createMap(width, height);

  if (parent != -1) {
    if (!m_mapinfos[parent]->children().empty()) {
      m_mapinfos[id]->setOrder(m_mapinfos[parent]->children().back()->order() + 1);
    }
    m_mapinfos[id]->setParentId(parent);
  }

  buildTree(true);
  rebuildOrdering();
  return &m_mapinfos[id].value();
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

void MapInfos::connectSignals() {
  for (auto& mapinfo : m_mapinfos) {
    if (!mapinfo) {
      continue;
    }
    // Disconnect just in case we already have a connection
    mapinfo->mapInfoModified().disconnect<&MapInfos::onMapInfoModified>(this);
    mapinfo->mapInfoModified().connect<&MapInfos::onMapInfoModified>(this);
  }
}

void MapInfos::onMapInfoModified(MapInfo* mapInfo, Map* map) { emit_signal(modified(), this); }
