#include "Database/Serializable/ClassesSerializer.hpp"

void ClassesSerializer::serialize(std::ofstream& os) const {
  try {
    nlohmann::ordered_json data;

    for (const Class& cls : m_data.classes()) {
      if (cls.m_isValid) {
        data.push_back(cls);
      } else {
        data.push_back(nullptr);
      }
    }

    os << data.dump(4);
  } catch (...) {}
}

void ClassesSerializer::deserialize(std::ifstream& is) {
  try {
    nlohmann::ordered_json data = nlohmann::ordered_json::parse(is);
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
