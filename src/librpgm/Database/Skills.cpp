#include "Skills.hpp"

#include <fstream>

using json = nlohmann::json;

Skills Skills::load(std::string_view filename) {
  std::ifstream file(filename.data());
  json data = json::parse(file);
  Skills skills;
  skills.m_skills.reserve(data.size());

  for (const auto& [_, value] : data.items()) {
    if (value == nullptr) {
      continue;
    }

    Skill& skill = skills.m_skills.emplace_back();
    value.get_to(skill);
  }
  return skills;
}

bool Skills::serialize(std::string_view filename) {
  // TODO: Temporary file for atomic writes

  std::ofstream file(filename.data());
  json data{nullptr};

  for (const Skill& skill : m_skills) {
    data.push_back(skill);
  }

  file << data;

  return true;
}
