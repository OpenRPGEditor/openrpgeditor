#include "Database/Serializable/EnemiesSerializer.hpp"

void EnemiesSerializer::serialize(std::ofstream& os) const {
  try {
    nlohmann::ordered_json data;

    if (m_data.enemies().empty()) {
      data.push_back(nullptr);
      os << data.dump(4);
      return;
    }

    for (const Enemy& enemy : m_data.enemies()) {
      if (enemy.m_isValid) {
        data.push_back(enemy);
      } else {
        data.push_back(nullptr);
      }
    }

    os << data.dump(4);
  } catch (...) {}
}

void EnemiesSerializer::deserialize(std::ifstream& is) {
  try {
    nlohmann::ordered_json data = nlohmann::ordered_json::parse(is);
    m_data.enemies().reserve(data.size());

    int i = 0;
    for (const auto& [_, value] : data.items()) {
      Enemy& enemy = m_data.enemies().emplace_back();
      enemy.m_isValid = value != nullptr;
      if (enemy.m_isValid) {
        value.get_to(enemy);
      } else {
        enemy.id = i;
      }
      ++i;
    }
  } catch (...) {}
}
