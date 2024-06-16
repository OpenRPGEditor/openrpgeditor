#pragma once

#include "Database/Globals.hpp"
#include "Database/Trait.hpp"
#include <nlohmann/json.hpp>

#include <string_view>

struct Weapon {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Weapon, id, animationId, description, etypeId, traits, iconIndex, name, note, params,
                                 price, wtypeId);
  int id;
  int animationId;
  std::string description;
  int etypeId;
  std::vector<Trait> traits;
  int iconIndex;
  std::string name;
  std::string note;
  std::array<int, 8> params; // Attack, Defance, M.Attack, M.Defence, Agility, Luck, Max HP, Max MP
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

class Weapons {
public:
  static Weapons load(std::string_view filepath);
  bool serialize(std::string_view filename);

  std::vector<Weapon>& weapons() { return m_weapons; }
  const std::vector<Weapon>& weapons() const { return m_weapons; }
  [[nodiscard]] Weapon* weapon(int id) {
    for (auto& item : m_weapons) {
      if (item.id == id && item.m_isValid) {
        return &item;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Weapon* weapon(int id) const {
    for (const auto& set : m_weapons) {
      if (set.id == id && set.m_isValid) {
        return &set;
      }
    }

    return nullptr;
  }

private:
  std::vector<Weapon> m_weapons;
};
