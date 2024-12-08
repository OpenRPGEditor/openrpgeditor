#pragma once

#include "Database/Damage.hpp"
#include "Database/Effect.hpp"
#include "Database/Globals.hpp"

#include <nlohmann/json.hpp>
#include <string>

class Item {
public:
  friend void to_json(nlohmann::ordered_json& json, const Item& item);
  friend void from_json(const nlohmann::ordered_json& json, Item& item);
  int id;
  int animationId;
  bool consumable;
  Damage damage;
  std::string description;
  std::vector<Effect> effects;
  HitType hitType;
  int iconIndex;
  int itypeId;
  std::string name;
  std::string note;
  Occasion occasion;
  int price;
  int repeats;
  Scope scope;
  int speed;
  int successRate;
  int tpGain;

  /*!
   * @name m_isValid
   * @details
   * Indicates that this is an actual valid entry and not a dummy
   * When making a new entry make sure to set this to true or it won't be
   * serialized.
   */
  bool m_isValid{false};
};
void to_json(nlohmann::ordered_json& json, const Item& item);
void from_json(const nlohmann::ordered_json& json, Item& item);

class Items {
public:
  static Items load(std::string_view filename);
  void serialize(std::string_view filename);

  [[nodiscard]] Item* item(int id) {
    for (auto& item : m_items) {
      if (item.id == id && item.m_isValid) {
        return &item;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Item* item(int id) const {
    for (const auto& item : m_items) {
      if (item.id == id && item.m_isValid) {
        return &item;
      }
    }

    return nullptr;
  }
  std::vector<Item>& items() { return m_items; }
  const std::vector<Item>& items() const { return m_items; }

  int count() { return m_items.size() - 1; }

  void resize(int newSize) {
    ++newSize;
    int oldSize = m_items.size();
    m_items.resize(newSize);
    if (newSize > oldSize) {
      for (int i = oldSize; i < m_items.size(); ++i) {
        m_items[i].id = i;
      }
    }
  }

private:
  std::vector<Item> m_items;
};
