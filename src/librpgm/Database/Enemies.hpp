#pragma once

#include "Database/Globals.hpp"
#include "Database/Trait.hpp"

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

class Enemy {
public:
  friend void to_json(nlohmann::ordered_json& to, const Enemy& enemy);
  friend void from_json(const nlohmann::ordered_json& from, Enemy& enemy);
  struct Action {
    friend void to_json(nlohmann::ordered_json& to, const Action& action);
    friend void from_json(const nlohmann::ordered_json& from, Action& action);
    int conditionParam1;
    int conditionParam2;
    ActionCondition conditionType;
    int rating;
    int skillId;
  };

  struct DropItem {
    friend void to_json(nlohmann::ordered_json& to, const DropItem& dropitem);
    friend void from_json(const nlohmann::ordered_json& from, DropItem& dropitem);
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
void to_json(nlohmann::ordered_json& to, const Enemy& enemy);
void from_json(const nlohmann::ordered_json& from, Enemy& enemy);
void to_json(nlohmann::ordered_json& to, const Enemy::Action& action);
void from_json(const nlohmann::ordered_json& from, Enemy::Action& action);
void to_json(nlohmann::ordered_json& to, const Enemy::DropItem& dropitem);
void from_json(const nlohmann::ordered_json& from, Enemy::DropItem& dropitem);

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
  int count() { return m_enemies.size() - 1; }

  void resize(int newSize) {
    ++newSize;
    int oldSize = m_enemies.size();
    m_enemies.resize(newSize);
    if (newSize > oldSize) {
      for (int i = oldSize; i < m_enemies.size(); ++i) {
        m_enemies[i].id = i;
      }
    }
  }

private:
  std::vector<Enemy> m_enemies;
};