#include "Database/Serializable/ItemsSerializer.hpp"

void ItemsSerializer::serialize(std::ofstream& os) const {
  try {
    nlohmann::ordered_json data;

    if (m_data.items().empty()) {
      data.push_back(nullptr);
      os << data.dump(4);
      return;
    }

    for (const Item& item : m_data.items()) {
      if (item.isValid()) {
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
    nlohmann::ordered_json data = nlohmann::ordered_json::parse(is);
    m_data.items().reserve(data.size());

    int i = 0;
    for (const auto& [_, value] : data.items()) {
      Item& item = m_data.items().emplace_back();
      item.m_isValid = value != nullptr;
      if (item.isValid()) {
        value.get_to(item);
      } else {
        item.m_id = i;
      }
      ++i;
    }
  } catch (...) {}
}
