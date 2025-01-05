#pragma once

#include "Database/Enemy.hpp"

class Enemies final : public IModifiable {
public:
  [[nodiscard]] Enemy* enemy(const int id) {
    for (auto& enemy : m_enemies) {
      if (enemy.id() == id && enemy.isValid()) {
        return &enemy;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Enemy* enemy(const int id) const {
    for (const auto& enemy : m_enemies) {
      if (enemy.id() == id && enemy.isValid()) {
        return &enemy;
      }
    }

    return nullptr;
  }

  [[nodiscard]] std::vector<Enemy>& enemies() { return m_enemies; }
  [[nodiscard]] const std::vector<Enemy>& enemies() const { return m_enemies; }
  int count() const { return m_enemies.size() - 1; }

  void resize(int newSize) {
    ++newSize;
    const int oldSize = m_enemies.size();
    m_enemies.resize(newSize);
    if (newSize > oldSize) {
      for (int i = oldSize; i < m_enemies.size(); ++i) {
        m_enemies[i].setId(i);
      }
    }
  }

  bool isModified() const override {
    return IModifiable::isModified() | std::ranges::any_of(m_enemies, [](const auto& enemy) { return enemy.isModified(); });
  }

private:
  std::vector<Enemy> m_enemies;
};