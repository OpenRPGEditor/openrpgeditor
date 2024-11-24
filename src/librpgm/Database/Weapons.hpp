#pragma once

#include "Database/Globals.hpp"
#include "Database/Trait.hpp"
#include <nlohmann/json.hpp>

#include <string_view>

struct Weapon {
  friend void to_json(nlohmann::ordered_json& j, const Weapon& w);
  friend void from_json(const nlohmann::ordered_json& j, Weapon& w);
  int id;
  int animationId;
  std::string description;
  int etypeId;
  std::vector<Trait> traits;
  int iconIndex;
  std::string name;
  std::string note;
  std::array<int, 8> params; // Attack, Defence, M.Attack, M.Defence, Agility, Luck, Max HP, Max MP
  int price;
  int wtypeId;

  /*!
   * @name m_isValid
   * @details
   * Indicates that this is an actual valid entry and not a dummy
   * When making a new entry make sure to set this to true or it won't be
   * serialized.
   */
  bool m_isValid{false};
};

void to_json(nlohmann::ordered_json& j, const Weapon& w);
void from_json(const nlohmann::ordered_json& j, Weapon& w);

class Weapons {
public:
  static Weapons load(std::string_view filepath);
  bool serialize(std::string_view filename);

  std::vector<Weapon>& weapons() { return m_weapons; }
  const std::vector<Weapon>& weapons() const { return m_weapons; }
  [[nodiscard]] Weapon* weapon(int id) {
    for (auto& weapon : m_weapons) {
      if (weapon.id == id && weapon.m_isValid) {
        return &weapon;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Weapon* weapon(int id) const {
    for (const auto& weapon : m_weapons) {
      if (weapon.id == id && weapon.m_isValid) {
        return &weapon;
      }
    }

    return nullptr;
  }

  std::vector<Weapon>& weaponList() { return m_weapons; }
  const std::vector<Weapon>& weaponList() const { return m_weapons; }

  int count() { return m_weapons.size() - 1; }

  void resize(int newSize) {
    ++newSize;
    int oldSize = m_weapons.size();
    m_weapons.resize(newSize);
    if (newSize > oldSize) {
      for (int i = oldSize; i < m_weapons.size(); ++i) {
        m_weapons[i].id = i;
      }
    }
  }

private:
  std::vector<Weapon> m_weapons;
};
