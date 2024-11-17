#include "Database/Serializable/MapSerializer.hpp"

void MapSerializer::serialize(std::ofstream& os) const {
  if (os.is_open()) {
    nlohmann::json data = m_map;
    os << data.dump(4);
  }
}

void MapSerializer::deserialize(std::ifstream& is) {
  if (is.is_open()) {
    m_map = nlohmann::json::parse(is).get<Map>();
    m_map.m_isValid = true;
  }
}
