#include "Database/MapInfos.hpp"

#include "Database/Database.hpp"
#include "Database/Map.hpp"
#include "Serializable/DeserializationQueue.hpp"
#include "Serializable/MapSerializer.hpp"

#include <fstream>

using json = nlohmann::json;

MapInfos MapInfos::load(std::string_view filename) {
  std::ifstream file(filename.data());
  json data = json::parse(file);
  MapInfos mapinfos;
  mapinfos.m_mapinfos.reserve(data.size());

  for (const auto& [_, value] : data.items()) {
    auto& mapinfo = mapinfos.m_mapinfos.emplace_back();
    value.get_to(mapinfo);
    if (mapinfo && mapinfo->id != 0) {
      DeserializationQueue::instance().enqueue(
          std::make_shared<MapSerializer>(std::format("data/Map{:03}.json", mapinfo->id)),
          [&mapinfo](const std::shared_ptr<ISerializable>& serializer) {
            if (auto map = std::dynamic_pointer_cast<MapSerializer>(serializer)->data(); map.m_isValid) {
              mapinfo->m_map = std::make_unique<Map>(map);
            }
          });
    }
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
  std::ranges::sort(in.m_children, [](const MapInfo* a, const MapInfo* b) { return a->order < b->order; });

  for (auto& mapInfo : in.m_children) {
    recursiveSort(*mapInfo);
  }
}

void MapInfos::buildTree(const bool reset) {
  if (reset) {
    for (auto& mapInfo : m_mapinfos) {
      mapInfo->m_children.clear();
    }
  }
  for (auto& mapInfo : m_mapinfos) {
    if (!mapInfo || mapInfo->id == 0) {
      continue;
    }
    if (mapInfo->parentId >= 0 && mapInfo->parentId < m_mapinfos.size()) {
      m_mapinfos[mapInfo->parentId]->m_children.push_back(&mapInfo.value());
    }
  }

  recursiveSort(root());
}
