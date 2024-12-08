#include "Database/Serializable/ArmorsSerializer.hpp"

void ArmorsSerializer::serialize(std::ofstream& os) const {
  try {
    nlohmann::ordered_json data;

    if (m_data.armors().empty()) {
      data.push_back(nullptr);
      os << data.dump(4);
      return;
    }

    for (const Armor& armor : m_data.armors()) {
      if (armor.isValid()) {
        data.push_back(armor);
      } else {
        data.push_back(nullptr);
      }
    }

    os << data.dump(4);
  } catch (...) {}
}

void ArmorsSerializer::deserialize(std::ifstream& is) {
  try {
    nlohmann::ordered_json data = nlohmann::ordered_json::parse(is);
    m_data.armors().reserve(data.size());

    int i = 0;
    for (const auto& [_, value] : data.items()) {
      Armor& armor = m_data.armors().emplace_back();
      armor.m_isValid = value != nullptr;
      if (armor.isValid()) {
        value.get_to(armor);
      } else {
        armor.m_id = i;
      }
      ++i;
    }
  } catch (...) {}
}
