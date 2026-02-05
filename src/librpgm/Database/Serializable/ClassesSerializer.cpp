#include "Database/Serializable/ClassesSerializer.hpp"

void ClassesSerializer::serialize(std::ofstream& os) const {
  try {
    nlohmann::ordered_json data;

    if (m_data.classes().empty()) {
      data.push_back(nullptr);
      os << data.dump(4);
      return;
    }

    for (const Class& cls : m_data.classes()) {
      if (cls.isValid()) {
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
    m_data.classes().clear();
    m_data.classes().reserve(data.size());
    m_data.disableSignals();

    int i = 0;
    for (const auto& [_, value] : data.items()) {
      Class& cls = m_data.classes().emplace_back();
      cls.m_isValid = value != nullptr;
      cls.disableSignals();
      if (cls.isValid()) {
        value.get_to(cls);
      } else {
        cls.m_id = i;
      }
      cls.enableSignals();
      ++i;
    }
    m_data.disableSignals();
  } catch (...) {}
}
