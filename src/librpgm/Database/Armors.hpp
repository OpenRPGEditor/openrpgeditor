#pragma once

#include "Database/Globals.hpp"
#include "Database/Trait.hpp"
#include <nlohmann/json.hpp>

#include <string_view>

struct Armor {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Armor, id, atypeId, description, etypeId, traits, iconIndex, name, note,
                                              params);
  int id{};
  int atypeId{};
  std::string description;
  int etypeId{};
  std::vector<Trait> traits;
  int iconIndex{};
  std::string name;
  std::string note;
  std::array<int, 8> params{}; // Attack, Defance, M.Attack, M.Defence, Agility, Luck, Max HP, Max MP
  int price{};
};

class Armors {
public:
  static Armors load(std::string_view filepath);
  bool serialize(std::string_view filename);
  std::vector<Armor> m_armors;
};
