#pragma once
#include <string_view>

#include "Database/Animation.hpp"

class Animations {
public:
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Animations, m_animations);
  Animations();

  static Animations load(std::string_view filename);
  void serialize(std::string_view filename);

  [[nodiscard]] Animation* animation(int id) {
    for (auto& item : m_animations) {
      if (item.id == id) {
        return &item;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Animation* animation(int id) const {
    for (const auto& set : m_animations) {
      if (set.id == id) {
        return &set;
      }
    }

    return nullptr;
  }

private:
  std::vector<Animation> m_animations;
};
