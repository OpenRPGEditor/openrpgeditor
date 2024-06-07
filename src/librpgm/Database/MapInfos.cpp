#include "MapInfos.hpp"

#include <fstream>

using json = nlohmann::json;

MapInfos MapInfos::load(std::string_view filename) {
  std::ifstream file(filename.data());
  json data = json::parse(file);
  MapInfos mapinfos;
  mapinfos.m_mapinfos.reserve(data.size());
  mapinfos.m_mapinfos.emplace_back();

  for (const auto& [_, value] : data.items()) {
    if (value == nullptr) {
      continue;
    }

    MapInfo& mapinfo = mapinfos.m_mapinfos.emplace_back();
    value.get_to(mapinfo);
  }
  mapinfos.buildTree();
  return mapinfos;
}

bool MapInfos::serialize(std::string_view filename) {
  // TODO: Temporary file for atomic writes

  std::ofstream file(filename.data());
  json data{nullptr};

  /* Erase the dummy entry */;
  m_mapinfos.erase(m_mapinfos.begin());

  for (const MapInfo& mapinfo : m_mapinfos) {
    data.push_back(mapinfo);
  }

  file << data;

  return true;
}

void recursiveSort(MapInfo& in) {
  std::sort(in.m_children.begin(), in.m_children.end(),
            [](const MapInfo* a, const MapInfo* b) { return a->order < b->order; });

  for (auto& mapInfo : in.m_children) {
    recursiveSort(*mapInfo);
  }
}

void MapInfos::buildTree() {
  for (auto& mapInfo : m_mapinfos) {
    if (mapInfo.id == 0) {
      continue;
    }
    m_mapinfos[mapInfo.parentId].m_children.push_back(&mapInfo);
  }

  recursiveSort(m_mapinfos[0]);
}
