#pragma once
#include <string_view>

#include "Database/Animation.hpp"

class Animations final : public IModifiable {
public:
  std::vector<Animation>& animations() { return m_animations; }
  const std::vector<Animation>& animations() const { return m_animations; }

  [[nodiscard]] Animation* animation(const int id) {
    for (auto& animation : m_animations) {
      if (animation.id() == id && animation.isValid()) {
        return &animation;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Animation* animation(const int id) const {
    for (const auto& animation : m_animations) {
      if (animation.id() == id && animation.isValid()) {
        return &animation;
      }
    }

    return nullptr;
  }
  int count() const { return m_animations.size() - 1; }

  void resize(int newSize) {
    ++newSize;
    const int oldSize = m_animations.size();
    m_animations.resize(newSize);
    if (newSize > oldSize) {
      for (int i = oldSize; i < m_animations.size(); ++i) {
        m_animations[i].setId(i);
      }
    }
    setModified();
  }

  bool isModified() const override {
    return IModifiable::isModified() | std::ranges::any_of(m_animations, [](const auto& anim) { return anim.isModified(); });
  }

private:
  std::vector<Animation> m_animations;
};
