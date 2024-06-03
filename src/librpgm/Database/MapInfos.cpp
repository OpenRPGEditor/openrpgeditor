#include "MapInfos.hpp"

#include <fstream>

using json = nlohmann::json;

MapInfos MapInfos::load(std::string_view filename) {
  std::ifstream file(filename.data());
  json data = json::parse(file);
  MapInfos mapinfos;
  mapinfos.m_mapinfos.reserve(data.size());

  for (const auto& [_, value] : data.items()) {
    if (value == nullptr) {
      continue;
    }

    MapInfo& mapinfo = mapinfos.m_mapinfos.emplace_back();
    value.get_to(mapinfo);
  }
  return mapinfos;
}

bool MapInfos::serialize(std::string_view filename) {
  // TODO: Temporary file for atomic writes

  std::ofstream file(filename.data());
  json data{nullptr};

  for (const MapInfo& mapinfo : m_mapinfos) {
    data.push_back(mapinfo);
  }

  file << data;

  return true;
}
