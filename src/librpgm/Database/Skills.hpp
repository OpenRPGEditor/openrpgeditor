#pragma once

#include "Database/Damage.hpp"
#include "Database/Effect.hpp"
#include "Database/Globals.hpp"

#include <nlohmann/json.hpp>
#include <string>

class Skill {
  friend void to_json(nlohmann::ordered_json& j, const Skill& skill);
  friend void from_json(const nlohmann::ordered_json& j, Skill& skill);

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
  HitType hitType;
  int repeats;
  int requiredWtypeId1; // In System?
  int requiredWtypeId2; // "" ""
  Scope scope;
  int speed;
  int stypeId; // As defined in System
  int successRate;
  int tpCost;
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

class Skills {
public:
  static Skills load(std::string_view filepath);
  bool serialize(std::string_view filename);

  [[nodiscard]] Skill* skill(int id) {
    for (auto& skill : m_skills) {
      if (skill.id == id && skill.m_isValid) {
        return &skill;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Skill* skill(int id) const {
    for (const auto& skill : m_skills) {
      if (skill.id == id && skill.m_isValid) {
        return &skill;
      }
    }

    return nullptr;
  }

  std::vector<Skill>& skills() { return m_skills; }
  const std::vector<Skill>& skills() const { return m_skills; }

  int count() { return m_skills.size() - 1; }

  void resize(int newSize) {
    ++newSize;
    int oldSize = m_skills.size();
    m_skills.resize(newSize);
    if (newSize > oldSize) {
      for (int i = oldSize; i < m_skills.size(); ++i) {
        m_skills[i].id = i;
      }
    }
  }

private:
  std::vector<Skill> m_skills;
};

void to_json(nlohmann::ordered_json& j, const Skill& skill);
void from_json(const nlohmann::ordered_json& j, Skill& skill);