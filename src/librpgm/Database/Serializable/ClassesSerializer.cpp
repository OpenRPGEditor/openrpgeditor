#include "Database/Serializable/ClassesSerializer.hpp"

void ClassesSerializer::serialize(std::ofstream& os) const {
  try {
    nlohmann::json data;

    for (const Class& actor : m_data.classes()) {
      if (actor.m_isValid) {
        data.push_back(actor);
      } else {
        data.push_back(nullptr);
      }
    }

    os << data.dump(4);
  } catch (...) {}
}

void ClassesSerializer::deserialize(std::ifstream& is) {
  try {
    nlohmann::json data = nlohmann::json::parse(is);
    m_data.classes().reserve(data.size());

    int i = 0;
    for (const auto& [_, value] : data.items()) {
      Class& cls = m_data.classes().emplace_back();
      cls.m_isValid = value != nullptr;
      if (cls.m_isValid) {
        value.get_to(cls);
      } else {
        cls.id = i;
      }
      ++i;
    }
  } catch (...) {}
}
