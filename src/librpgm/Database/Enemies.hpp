#pragma once

#include "Database/RPGEnums.hpp"
#include "Database/Trait.hpp"

#include <nlohmann/json.hpp>
#include <vector>
#include <string>

class Enemy {
public:
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Enemy, id, actions, battlerHue, battlerName, dropItems, exp, traits, gold, name, note,
                                 params);
  struct Action {
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Action, conditionParam1, conditionParam2, conditionType, rating, skillId);
    int conditionParam1;
    int conditionParam2;
    ActionCondition conditionType;
    int rating;
    int skillId;
  };

  struct DropItem {
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(DropItem, dataId, denominator, kind);
    int dataId;
    int denominator;
    int kind;
  };

  int id;
  std::vector<Action> actions;
  int battlerHue;
  std::string battlerName;
  std::vector<DropItem> dropItems;
  int exp;
  std::vector<Trait> traits;
  int gold;
  std::string name;
  std::string note;
  std::array<int, 8> params; // Attack, Defance, M.Attack, M.Defence, Agility, Luck, Max HP, Max MP
};

class Enemies {
public:
  static Enemies load(std::string_view filepath);
  bool serialize(std::string_view filename);
  std::vector<Enemy> m_enemies;
};