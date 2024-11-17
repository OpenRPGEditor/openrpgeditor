#include "Database/Serializable/ActorsSerializer.hpp"

void ActorsSerializer::serialize(std::ofstream& os) const {
  try {
    nlohmann::json data;

    for (const Actor& actor : m_data.actorList()) {
      if (actor.m_isValid) {
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
    nlohmann::json data = nlohmann::json::parse(is);
    m_data.actorList().reserve(data.size());

    int i = 0;
    for (const auto& [_, value] : data.items()) {
      Actor& actor = m_data.actorList().emplace_back();
      actor.m_isValid = value != nullptr;
      if (actor.m_isValid) {
        value.get_to(actor);
      } else {
        actor.id = i;
      }
      ++i;
    }
  } catch (...) {}
}
