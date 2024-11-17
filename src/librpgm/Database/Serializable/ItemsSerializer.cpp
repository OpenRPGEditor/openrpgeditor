#include "Database/Serializable/ItemsSerializer.hpp"

void ItemsSerializer::serialize(std::ofstream& os) const {
  try {
    nlohmann::json data;

    for (const Item& item : m_data.items()) {
      if (item.m_isValid) {
        data.push_back(item);
      } else {
        data.push_back(nullptr);
      }
    }

    os << data.dump(4);
  } catch (...) {}
}

void ItemsSerializer::deserialize(std::ifstream& is) {
  try {
    nlohmann::json data = nlohmann::json::parse(is);
    m_data.items().reserve(data.size());

    int i = 0;
    for (const auto& [_, value] : data.items()) {
      Item& item = m_data.items().emplace_back();
      item.m_isValid = value != nullptr;
      if (item.m_isValid) {
        value.get_to(item);
      } else {
        item.id = i;
      }
      ++i;
    }
  } catch (...) {}
}
