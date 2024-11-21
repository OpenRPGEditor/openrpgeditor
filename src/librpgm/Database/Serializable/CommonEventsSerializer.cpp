#include "Database/Serializable/CommonEventsSerializer.hpp"

void CommonEventsSerializer::serialize(std::ofstream& os) const {
  try {
    nlohmann::ordered_json data;

    for (const auto& event : m_data.events()) {
      data.push_back(event);
    }

    os << data.dump(4);
  } catch (...) {}
}

void CommonEventsSerializer::deserialize(std::ifstream& is) {
  try {
    nlohmann::ordered_json data = nlohmann::ordered_json::parse(is);
    CommonEvents commonEvents;
    m_data.events().reserve(data.size());

    for (const auto& [_, value] : data.items()) {
      value.get_to(m_data.events().emplace_back());
    }
  } catch (...) {}
}
