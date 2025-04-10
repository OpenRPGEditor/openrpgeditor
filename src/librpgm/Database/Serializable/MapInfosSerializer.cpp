#include "Database/Serializable/MapInfosSerializer.hpp"
#include "Database/Serializable/MapSerializer.hpp"

void MapInfosSerializer::serialize(std::ofstream& outFile) const {
  nlohmann::ordered_json data{nullptr};

  for (int i = 1; i < m_data.m_mapinfos.size(); ++i) {
    data.push_back(m_data.m_mapinfos[i]);
  }

  outFile << data.dump(4);
}

void MapInfosSerializer::deserialize(std::ifstream& inFile) {
  try {
    nlohmann::ordered_json data = nlohmann::ordered_json::parse(inFile);
    m_data.mapInfos().reserve(data.size());

    for (const auto& [_, value] : data.items()) {
      auto& mapinfo = m_data.mapInfos().emplace_back();
      value.get_to(mapinfo);
    }
    m_data.mapInfos()[0].emplace();
    m_data.mapInfos().shrink_to_fit();
    m_data.buildTree();
  } catch (...) {}
}
