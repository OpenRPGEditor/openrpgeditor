#pragma once

#include "Database/Globals.hpp"
#include "Database/Damage.hpp"
#include "Database/Effect.hpp"

#include <string>
#include <nlohmann/json.hpp>

class Skill {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Skill, id, animationId, damage, description, effects, iconIndex, message1,
                                              message2, mpCost, name, note, occasion, repeats, requiredWtypeId1,
                                              requiredWtypeId2, scope, speed, stypeId, successRate, tpCost, tpGain);

public:
  int id;
  int animationId;
  Damage damage;
  std::string description;
  std::vector<Effect> effects;
  int iconIndex;
  std::string message1;
  std::string message2;
  int mpCost;
  std::string name;
  std::string note;
  Occasion occasion;
  int repeats;
  int requiredWtypeId1; // In System?
  int requiredWtypeId2; // "" ""
  Scope scope;
  int speed;
  int stypeId; // As defined in System
  int successRate;
  int tpCost;
  int tpGain;
};

class Skills {
public:
  static Skills load(std::string_view filepath);
  bool serialize(std::string_view filename);

  [[nodiscard]] Skill* skill(int id) {
    for (auto& item : m_skills) {
      if (item.id == id) {
        return &item;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Skill* skill(int id) const {
    for (const auto& set : m_skills) {
      if (set.id == id) {
        return &set;
      }
    }

    return nullptr;
  }

private:
  std::vector<Skill> m_skills;
};
