#include "Database/Animations.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::ordered_json;

Animations Animations::load(const std::string_view filename) {
  std::ifstream file(filename.data());
  json data = json::parse(file);
  Animations animations;
  animations.m_animations.reserve(data.size());

  int i = 0;
  for (const auto& [_, value] : data.items()) {
    Animation& animation = animations.m_animations.emplace_back();
    animation.m_isValid = value != nullptr;
    if (animation.m_isValid) {
      value.get_to(animation);
    } else {
      animation.id = i;
    }
    ++i;
  }
  return animations;
}

void Animations::serialize(const std::string_view filename) {
  std::ofstream file(filename.data());
  json data;

  for (const Animation& animation : m_animations) {
    if (animation.m_isValid) {
      data.push_back(animation);
    } else {
      data.push_back(nullptr);
    }
  }

  file << data.dump(4);
}
