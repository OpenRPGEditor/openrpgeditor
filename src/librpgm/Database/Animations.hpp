#pragma once
#include <string_view>

#include "Database/Animation.hpp"

class Animations {
public:
  static Animations load(std::string_view filename);
  void serialize(std::string_view filename);

  std::vector<Animation>& animations() { return m_animations; }
  const std::vector<Animation>& animations() const { return m_animations; }

  [[nodiscard]] Animation* animation(int id) {
    for (auto& animation : m_animations) {
      if (animation.id == id && animation.m_isValid) {
        return &animation;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Animation* animation(int id) const {
    for (const auto& animation : m_animations) {
      if (animation.id == id && animation.m_isValid) {
        return &animation;
      }
    }

    return nullptr;
  }
  int count() { return m_animations.size() - 1; }

  void resize(int newSize) {
    ++newSize;
    int oldSize = m_animations.size();
    m_animations.resize(newSize);
    if (newSize > oldSize) {
      for (int i = oldSize; i < m_animations.size(); ++i) {
        m_animations[i].id = i;
      }
    }
  }

private:
  std::vector<Animation> m_animations;
};
