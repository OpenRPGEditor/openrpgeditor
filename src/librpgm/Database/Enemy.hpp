#pragma once
#include "Database/Globals.hpp"
#include "Database/Trait.hpp"

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

class Enemy {
  friend class EnemiesSerializer;

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

  [[nodiscard]] int id() const { return m_id; }
  void setId(const int id) { m_id = id; }

  std::vector<Action>& actions() { return m_actions; }
  const std::vector<Action>& actions() const { return m_actions; }
  void setActions(const std::vector<Action>& actions) { m_actions = actions; }

  [[nodiscard]] int battlerHue() const { return m_battlerHue; }
  void setBattlerHue(const int battlerHue) { m_battlerHue = battlerHue; }

  const std::string& battlerName() const { return m_battlerName; }
  void setBattlerName(const std::string& battlerName) { m_battlerName = battlerName; }

  std::vector<DropItem>& dropItems() { return m_dropItems; }
  const std::vector<DropItem>& dropItems() const { return m_dropItems; }
  void setDropItems(const std::vector<DropItem>& dropItems) { m_dropItems = dropItems; }

  [[nodiscard]] int exp() const { return m_exp; }
  void setExp(const int exp) { m_exp = exp; }

  std::vector<Trait>& traits() { return m_traits; }
  void setTraits(const std::vector<Trait>& traits) { m_traits = traits; }

  [[nodiscard]] int gold() const { return m_gold; }
  void setGold(const int gold) { m_gold = gold; }

  const std::string& name() const { return m_name; }
  void setName(const std::string& name) { m_name = name; }

  const std::string& note() const { return m_note; }
  void setNote(const std::string& note) { m_note = note; }

  const std::array<int, 8>& params() const { return m_params; }
  void setParams(const std::array<int, 8>& params) { m_params = params; }
  [[nodiscard]] int param(const int idx) const {
    assert(idx >= 0 && idx < m_params.size());
    return m_params[idx];
  }
  void setParam(const int idx, const int param) {
    assert(param >= 0 && param < m_params.size());
    m_params[idx] = param;
  }

  bool isValid() const { return m_isValid; }
  void setIsValid(const bool isValid) { m_isValid = isValid; }

private:
  int m_id;
  std::vector<Action> m_actions;
  int m_battlerHue;
  std::string m_battlerName;
  std::vector<DropItem> m_dropItems;
  int m_exp;
  std::vector<Trait> m_traits;
  int m_gold;
  std::string m_name;
  std::string m_note;
  std::array<int, 8> m_params; // Attack, Defence, M.Attack, M.Defence, Agility, Luck, Max HP, Max MP
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