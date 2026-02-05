#include "Database/Serializable/CommonEventsSerializer.hpp"

void CommonEventsSerializer::serialize(std::ofstream& os) const {
  try {
    nlohmann::ordered_json data;

    if (m_data.events().empty()) {
      data.push_back(nullptr);
      os << data.dump(4);
      return;
    }
    for (const auto& event : m_data.events()) {
      data.push_back(event);
    }

    os << data.dump(4);
  } catch (...) {}
}

void CommonEventsSerializer::deserialize(std::ifstream& is) {
  try {
    nlohmann::ordered_json data = nlohmann::ordered_json::parse(is);
    m_data.events().clear();
    m_data.events().reserve(data.size());
    m_data.disableSignals();

    for (const auto& [_, value] : data.items()) {
      value.get_to(m_data.events().emplace_back());
    }
    m_data.enableSignals();
  } catch (...) {}
}
