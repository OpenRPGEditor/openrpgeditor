#include "Database/Serializable/MapInfosSerializer.hpp"
#include "Database/Serializable/MapSerializer.hpp"

#include "Database/Serializable/DeserializationQueue.hpp"

void MapInfosSerializer::serialize(std::ofstream& os) const {}
void MapInfosSerializer::deserialize(std::ifstream& is) {
  try {
    nlohmann::json data = nlohmann::json::parse(is);
    m_data.mapInfos().reserve(data.size());

    for (const auto& [_, value] : data.items()) {
      auto& mapinfo = m_data.mapInfos().emplace_back();
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
    m_data.mapInfos()[0].emplace();
    m_data.mapInfos().shrink_to_fit();
    m_data.buildTree();
  } catch (...) {}
}
