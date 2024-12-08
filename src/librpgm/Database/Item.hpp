#pragma once

#include "Database/Damage.hpp"
#include "Database/Effect.hpp"
#include "Database/Globals.hpp"

class Item {
  friend class ItemsSerializer;

public:
  friend void to_json(nlohmann::ordered_json& json, const Item& item);
  friend void from_json(const nlohmann::ordered_json& json, Item& item);

  [[nodiscard]] int id() const { return m_id; }
  void setId(const int id) { m_id = id; }

  [[nodiscard]] int animationId() const { return m_animationId; }
  void setAnimationId(const int animationId) { m_animationId = animationId; }

  [[nodiscard]] bool consumable() const { return m_consumable; }
  void setConsumable(const bool consumable) { m_consumable = consumable; }

  Damage& damage() { return m_damage; }
  void setDamage(const Damage& damage) { m_damage = damage; }

  [[nodiscard]] const std::string& description() const { return m_description; }
  void setDescription(const std::string& description) { m_description = description; }

  std::vector<Effect>& effects() { return m_effects; }
  void setEffects(const std::vector<Effect>& effects) { m_effects = effects; }

  [[nodiscard]] HitType hitType() const { return m_hitType; }
  void setHitType(const HitType hitType) { m_hitType = hitType; }

  [[nodiscard]] int iconIndex() const { return m_iconIndex; }
  void setIconIndex(const int iconIndex) { m_iconIndex = iconIndex; }

  [[nodiscard]] int itypeId() const { return m_itypeId; }
  void setItypeId(const int itypeId) { m_itypeId = itypeId; }

  [[nodiscard]] const std::string& name() const { return m_name; }
  void setName(const std::string& name) { m_name = name; }

  [[nodiscard]] const std::string& note() const { return m_note; }
  void setNote(const std::string& note) { m_note = note; }

  [[nodiscard]] Occasion occasion() const { return m_occasion; }
  void setOccasion(const Occasion occasion) { m_occasion = occasion; }

  [[nodiscard]] int price() const { return m_price; }
  void setPrice(const int price) { m_price = price; }

  [[nodiscard]] int repeats() const { return m_repeats; }
  void setRepeats(const int repeats) { m_repeats = repeats; }

  [[nodiscard]] Scope scope() const { return m_scope; }
  void setScope(const Scope scope) { m_scope = scope; }

  [[nodiscard]] int speed() const { return m_speed; }
  void setSpeed(const int speed) { m_speed = speed; }

  [[nodiscard]] int successRate() const { return m_successRate; }
  void setSuccessRate(const int successRate) { m_successRate = successRate; }

  [[nodiscard]] int tpGain() const { return m_tpGain; }
  void setTpGain(const int tpGain) { m_tpGain = tpGain; }

  [[nodiscard]] bool isValid() const { return m_isValid; }
  void setValid(const bool valid) { m_isValid = valid; }

private:
  int m_id;
  int m_animationId;
  bool m_consumable;
  Damage m_damage;
  std::string m_description;
  std::vector<Effect> m_effects;
  HitType m_hitType;
  int m_iconIndex;
  int m_itypeId;
  std::string m_name;
  std::string m_note;
  Occasion m_occasion;
  int m_price;
  int m_repeats;
  Scope m_scope;
  int m_speed;
  int m_successRate;
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
void to_json(nlohmann::ordered_json& json, const Item& item);
void from_json(const nlohmann::ordered_json& json, Item& item);