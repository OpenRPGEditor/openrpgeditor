#include "Database/Serializable/WeaponsSerializer.hpp"

void WeaponsSerializer::serialize(std::ofstream& os) const {
  try {
    nlohmann::json data;

    for (const Weapon& weapon : m_data.weapons()) {
      if (weapon.m_isValid) {
        data.push_back(weapon);
      } else {
        data.push_back(nullptr);
      }
    }

    os << data.dump(4);
  } catch (...) {}
}

void WeaponsSerializer::deserialize(std::ifstream& is) {
  try {
    nlohmann::json data = nlohmann::json::parse(is);
    m_data.weapons().reserve(data.size());

    int i = 0;
    for (const auto& [_, value] : data.items()) {
      Weapon& weapon = m_data.weapons().emplace_back();
      weapon.m_isValid = value != nullptr;
      if (weapon.m_isValid) {
        value.get_to(weapon);
      } else {
        weapon.id = i;
      }
      ++i;
    }
  } catch (...) {}
}
