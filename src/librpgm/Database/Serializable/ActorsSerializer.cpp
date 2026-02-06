#include "Database/Serializable/ActorsSerializer.hpp"

void ActorsSerializer::serialize(std::ofstream& os) const {
  try {
    nlohmann::ordered_json data;

    if (m_data.actorList().empty()) {
      data.push_back(nullptr);
      os << data.dump(4);
      return;
    }

    for (const Actor& actor : m_data.actorList()) {
      if (actor.isValid()) {
        data.push_back(actor);
      } else {
        data.push_back(nullptr);
      }
    }

    os << data.dump(4);
  } catch (...) {}
}

void ActorsSerializer::deserialize(std::ifstream& is) {
  try {
    nlohmann::ordered_json data = nlohmann::ordered_json::parse(is);
    m_data.disableSignals();
    m_data.actorList().clear();
    m_data.actorList().reserve(data.size());

    int i = 0;
    for (const auto& [_, value] : data.items()) {
      Actor& actor = m_data.actorList().emplace_back();
      actor.disableSignals();
      actor.setValid(value != nullptr);
      if (actor.isValid()) {
        value.get_to(actor);
      } else {
        actor.setId(i);
      }
      actor.enableSignals();
      ++i;
    }
    m_data.enableSignals();
  } catch (...) {}
}
