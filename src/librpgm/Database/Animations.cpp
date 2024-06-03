#include "Database/Animations.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
Animations::Animations() {}

Animations Animations::load(std::string_view filename) {
  std::ifstream file(filename.data());
  json data = json::parse(file);
  Animations animations;
  animations.m_animations.reserve(data.size());

  for (const auto& [_, value] : data.items()) {
    if (value == nullptr) {
      continue;
    }

    Animation& animation = animations.m_animations.emplace_back();
    value.get_to(animation);
  }
  return animations;
}

void Animations::serialize(std::string_view filename) {
  std::ofstream file(filename.data());
  json data{nullptr};

  for (const Animation& animation : m_animations) {
    data.push_back(animation);
  }

  file << data;
}
