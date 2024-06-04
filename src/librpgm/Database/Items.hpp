#pragma once

#include "Database/Globals.hpp"
#include "Database/Damage.hpp"
#include "Database/Effect.hpp"

#include <nlohmann/json.hpp>
#include <string>

class Item {
public:
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Item, id, animationId, consumable, damage, description, effects, hitType,
                                              iconIndex, itypeId, name, note, occasion, price, scope, speed,
                                              successRate, tpGain);
  int id;
  int animationId;
  bool consumable;
  Damage damage;
  std::string description;
  std::vector<Effect> effects;
  ItemType hitType;
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
};

class Items {
public:
  static Items load(std::string_view filename);
  void serialize(std::string_view filename);

  std::vector<Item> m_items;
};
