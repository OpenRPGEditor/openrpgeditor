#pragma once

#include "Database/Weapon.hpp"

class Weapons final : public IModifiable {
public:
  Weapons() { m_weapons.emplace_back(); }
  std::vector<Weapon>& weapons() { return m_weapons; }
  [[nodiscard]] const std::vector<Weapon>& weapons() const { return m_weapons; }
  [[nodiscard]] Weapon* weapon(const int id) {
    for (auto& weapon : m_weapons) {
      if (weapon.id() == id && weapon.isValid()) {
        return &weapon;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Weapon* weapon(const int id) const {
    for (const auto& weapon : m_weapons) {
      if (weapon.id() == id && weapon.isValid()) {
        return &weapon;
      }
    }

    return nullptr;
  }

  std::vector<Weapon>& weaponList() { return m_weapons; }
  [[nodiscard]] const std::vector<Weapon>& weaponList() const { return m_weapons; }

  [[nodiscard]] int count() const { return static_cast<int>(m_weapons.size() - 1); }

  void resize(int newSize) {
    ++newSize;
    const int oldSize = static_cast<int>(m_weapons.size());
    m_weapons.resize(newSize);
    if (newSize > oldSize) {
      for (int i = oldSize; i < m_weapons.size(); ++i) {
        m_weapons[i].setId(i);
      }
    }
  }

  bool isModified() const override {
    return IModifiable::isModified() | std::ranges::any_of(m_weapons, [](const auto& w) { return w.isModified(); });
  }

private:
  std::vector<Weapon> m_weapons;
};
