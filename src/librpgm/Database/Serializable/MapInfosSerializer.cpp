#include "Database/Serializable/MapInfosSerializer.hpp"
#include "Database/Serializable/MapSerializer.hpp"

#include "Database/Serializable/DeserializationQueue.hpp"

void MapInfosSerializer::serialize(std::ofstream& os) const {}
void MapInfosSerializer::deserialize(std::ifstream& is) {
  try {
    nlohmann::ordered_json data = nlohmann::ordered_json::parse(is);
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
