#pragma once

#include "Database/Damage.hpp"
#include "Database/Effect.hpp"
#include "Database/Globals.hpp"

#include <nlohmann/json.hpp>
#include <string>

class Skill {
  friend class SkillsSerializer;
  friend void to_json(nlohmann::ordered_json& j, const Skill& skill);
  friend void from_json(const nlohmann::ordered_json& j, Skill& skill);

public:
  [[nodiscard]] int id() const { return m_id; }
  void setId(const int id) { m_id = id; }

  [[nodiscard]] int animationId() const { return m_animationId; }
  void setAnimationId(const int id) { m_animationId = id; }

  Damage& damage() { return m_damage; }
  void setDamage(const Damage& damage) { m_damage = damage; }

  [[nodiscard]] const std::string& description() const { return m_description; }
  void setDescription(const std::string& description) { m_description = description; }

  std::vector<Effect>& effects() { return m_effects; }
  void setEffects(const std::vector<Effect>& effects) { m_effects = effects; }

  [[nodiscard]] int iconIndex() const { return m_iconIndex; }
  void setIconIndex(const int iconIndex) { m_iconIndex = iconIndex; }

  [[nodiscard]] const std::string& message1() const { return m_message1; }
  void setMessage1(const std::string& message1) { m_message1 = message1; }

  [[nodiscard]] const std::string& message2() const { return m_message2; }
  void setMessage2(const std::string& message2) { m_message2 = message2; }

  [[nodiscard]] int mpCost() const { return m_mpCost; }
  void setMpCost(const int mpCost) { m_mpCost = mpCost; }

  [[nodiscard]] const std::string& name() const { return m_name; }
  void setName(const std::string& name) { m_name = name; }

  [[nodiscard]] const std::string& note() const { return m_note; }
  void setNote(const std::string& note) { m_note = note; }

  [[nodiscard]] Occasion occasion() const { return m_occasion; }
  void setOccasion(const Occasion occasion) { m_occasion = occasion; }

  [[nodiscard]] HitType hitType() const { return m_hitType; }
  void setHitType(const HitType hitType) { m_hitType = hitType; }

  [[nodiscard]] int repeats() const { return m_repeats; }
  void setRepeats(const int repeats) { m_repeats = repeats; }

  [[nodiscard]] int requiredWtypeId1() const { return m_requiredWtypeId1; }
  void setRequiredWtypeId1(const int requiredWtypeId1) { m_requiredWtypeId1 = requiredWtypeId1; }

  [[nodiscard]] int requiredWtypeId2() const { return m_requiredWtypeId2; }
  void setRequiredWtypeId2(const int requiredWtypeId2) { m_requiredWtypeId2 = requiredWtypeId2; }

  [[nodiscard]] Scope scope() const { return m_scope; }
  void setScope(const Scope scope) { m_scope = scope; }

  [[nodiscard]] int speed() const { return m_speed; }
  void setSpeed(const int speed) { m_speed = speed; }

  [[nodiscard]] int stypeId() const { return m_stypeId; }
  void setStypeId(const int stypeId) { m_stypeId = stypeId; }

  [[nodiscard]] int successRate() const { return m_successRate; }
  void setSuccessRate(const int successRate) { m_successRate = successRate; }

  [[nodiscard]] int tpCost() const { return m_tpCost; }
  void setTpCost(const int tpCost) { m_tpCost = tpCost; }

  [[nodiscard]] int tpGain() const { return m_tpGain; }
  void setTpGain(const int tpGain) { m_tpGain = tpGain; }

  [[nodiscard]] bool isValid() const { return m_isValid; }
  void setValid(const bool isValid) { m_isValid = isValid; }

private:
  int m_id;
  int m_animationId;
  Damage m_damage;
  std::string m_description;
  std::vector<Effect> m_effects;
  int m_iconIndex;
  std::string m_message1;
  std::string m_message2;
  int m_mpCost;
  std::string m_name;
  std::string m_note;
  Occasion m_occasion;
  HitType m_hitType;
  int m_repeats;
  int m_requiredWtypeId1; // In System?
  int m_requiredWtypeId2; // "" ""
  Scope m_scope;
  int m_speed;
  int m_stypeId; // As defined in System
  int m_successRate;
  int m_tpCost;
  int m_tpGain;

  /*!
   * @name m_isValid
   * @details
   * Indicates that this is an actual valid entry and not a dummy
   * When making a new entry make sure to set this to true or it won't be
   * serialized.
   */
  bool m_isValid{false};
};