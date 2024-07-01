#pragma once

#include "Database/Globals.hpp"
#include "Database/Trait.hpp"

#include <nlohmann/json.hpp>
#include <vector>
#include <string>

class Enemy {
public:
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Enemy, id, actions, battlerHue, battlerName, dropItems, exp, traits, gold,
                                              name, note, params);
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
  /*!
   * @name m_isValid
   * @details
   * Indicates that this is an actual valid entry and not a dummy
   * When making a new entry make sure to set this to true or it won't be
   * serialized.
   */
  bool m_isValid{false};
};

class Enemies {
public:
  static Enemies load(std::string_view filepath);
  bool serialize(std::string_view filename);

  [[nodiscard]] Enemy* enemy(int id) {
    for (auto& enemy : m_enemies) {
      if (enemy.id == id && enemy.m_isValid) {
        return &enemy;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Enemy* enemy(int id) const {
    for (const auto& enemy : m_enemies) {
      if (enemy.id == id && enemy.m_isValid) {
        return &enemy;
      }
    }

    return nullptr;
  }

  [[nodiscard]] std::vector<Enemy>& enemies() { return m_enemies; }
  [[nodiscard]] const std::vector<Enemy>& enemies() const { return m_enemies; }

private:
  std::vector<Enemy> m_enemies;
};