#pragma once

#include "Database/RPGEnums.hpp"
#include "Database/Trait.hpp"
#include <nlohmann/json.hpp>

#include <string_view>

struct Weapon {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Weapon, id, animationId, description, etypeId, traits, iconIndex, name, note,
                                 params);
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
};

class Weapons {
public:
  static Weapons load(std::string_view filepath);
  bool serialize(std::string_view filename);
  std::vector<Weapon> m_weapons;
};
