#include "Database/Serializable/AnimationsSerializer.hpp"

void AnimationsSerializer::serialize(std::ofstream& os) const {
  try {
    nlohmann::json data;

    for (const Animation& animation : m_data.animations()) {
      if (animation.m_isValid) {
        data.push_back(animation);
      } else {
        data.push_back(nullptr);
      }
    }

    os << data.dump(4);
  } catch (...) {}
}

void AnimationsSerializer::deserialize(std::ifstream& is) {
  try {
    nlohmann::json data = nlohmann::json::parse(is);
    m_data.animations().reserve(data.size());

    int i = 0;
    for (const auto& [_, value] : data.items()) {
      Animation& animation = m_data.animations().emplace_back();
      animation.m_isValid = value != nullptr;
      if (animation.m_isValid) {
        value.get_to(animation);
      } else {
        animation.id = i;
      }
      ++i;
    }
  } catch (...) {}
}
