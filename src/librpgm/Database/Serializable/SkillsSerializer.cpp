#include "Database/Serializable/SkillsSerializer.hpp"

#include <iostream>

void SkillsSerializer::serialize(std::ofstream& os) const {
  try {
    nlohmann::ordered_json data;
    if (m_data.skills().empty()) {
      data.push_back(nullptr);
      os << data.dump(4);
      return;
    }

    for (const Skill& skill : m_data.skills()) {
      if (skill.m_isValid) {
        data.push_back(skill);
      } else {
        data.push_back(nullptr);
      }
    }

    os << data.dump(4);
  } catch (...) {}
}

void SkillsSerializer::deserialize(std::ifstream& is) {
  try {
    nlohmann::ordered_json data = nlohmann::ordered_json::parse(is);
    m_data.skills().reserve(data.size());

    int i = 0;
    for (const auto& [_, value] : data.items()) {
      Skill& skill = m_data.skills().emplace_back();
      skill.m_isValid = value != nullptr;
      if (skill.m_isValid) {
        value.get_to(skill);
      } else {
        skill.id = i;
      }
      ++i;
    }
  } catch (...) {}
}
