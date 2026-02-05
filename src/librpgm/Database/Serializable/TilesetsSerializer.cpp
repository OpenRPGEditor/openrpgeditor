#include "Database/Serializable/TilesetsSerializer.hpp"

void TilesetsSerializer::serialize(std::ofstream& os) const {
  try {
    nlohmann::ordered_json data;
    if (m_data.tilesets().empty()) {
      data.push_back(nullptr);
      os << data.dump(4);
      return;
    }

    for (const Tileset& tileset : m_data.tilesets()) {
      if (tileset.isValid()) {
        data.push_back(tileset);
      } else {
        data.push_back(nullptr);
      }
    }

    os << data.dump(4);
  } catch (...) {}
}

void TilesetsSerializer::deserialize(std::ifstream& is) {
  try {
    nlohmann::ordered_json data = nlohmann::ordered_json::parse(is);
    m_data.tilesets().clear();
    m_data.tilesets().reserve(data.size());

    int i = 0;
    for (const auto& [_, value] : data.items()) {
      Tileset& tileset = m_data.tilesets().emplace_back();
      tileset.m_isValid = value != nullptr;
      if (tileset.isValid()) {
        value.get_to(tileset);
      } else {
        tileset.m_id = i;
      }
      ++i;
    }
  } catch (...) {}
}
