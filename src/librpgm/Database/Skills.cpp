#include "Skills.hpp"

#include <fstream>

using json = nlohmann::json;

Skills Skills::load(std::string_view filename) {
  std::ifstream file(filename.data());
  json data = json::parse(file);
  Skills skills;
  skills.m_skills.reserve(data.size());

  int i = 0;
  for (const auto& [_, value] : data.items()) {
    Skill& skill = skills.m_skills.emplace_back();
    skill.m_isValid = value != nullptr;
    if (skill.m_isValid) {
      value.get_to(skill);
    } else {
      skill.id = i;
    }
    ++i;
  }
  return skills;
}

bool Skills::serialize(std::string_view filename) {
  // TODO: Temporary file for atomic writes

  std::ofstream file(filename.data());
  json data{nullptr};

  for (const Skill& skill : m_skills) {
    if (skill.m_isValid) {
      data.push_back(skill);
    } else {
      data.push_back(nullptr);
    }
  }

  file << data;

  return true;
}
