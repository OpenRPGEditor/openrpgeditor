#include "Database/Serializable/TroopsSerializer.hpp"

void TroopsSerializer::serialize(std::ofstream& os) const {
  try {
    nlohmann::ordered_json data;

    if (m_data.troops().empty()) {
      data.push_back(nullptr);
      os << data.dump(4);
      return;
    }

    for (const Troop& troop : m_data.troops()) {
      if (troop.m_isValid) {
        data.push_back(troop);
      } else {
        data.push_back(nullptr);
      }
    }

    os << data.dump(4);
  } catch (...) {}
}

void TroopsSerializer::deserialize(std::ifstream& is) {
  try {
    nlohmann::ordered_json data = nlohmann::json::parse(is);
    m_data.troops().reserve(data.size());

    int i = 0;
    for (const auto& [_, value] : data.items()) {
      Troop& troop = m_data.troops().emplace_back();
      troop.m_isValid = value != nullptr;
      if (troop.m_isValid) {
        value.get_to(troop);
      } else {
        troop.setId(i);
      }
      ++i;
    }
  } catch (...) {}
}
