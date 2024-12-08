#pragma once

#include "Database/Globals.hpp"
#include "Database/Trait.hpp"

class State {
  friend class StatesSerializer;
  friend void to_json(nlohmann::ordered_json& j, const State& state);
  friend void from_json(const nlohmann::ordered_json& j, State& state);

public:
  [[nodiscard]] int id() const { return m_id; }
  void setId(const int id) { m_id = id; }

  [[nodiscard]] AutoRemovalTiming autoRemovalTiming() const { return m_autoRemovalTiming; }
  void setAutoRemovalTiming(const AutoRemovalTiming autoRemovalTiming) { m_autoRemovalTiming = autoRemovalTiming; }

  [[nodiscard]] int chanceByDamage() const { return m_chanceByDamage; }
  void setChanceByDamage(const int chanceByDamage) { m_chanceByDamage = chanceByDamage; }

  [[nodiscard]] int iconIndex() const { return m_iconIndex; }
  void setIconIndex(const int iconIndex) { m_iconIndex = iconIndex; }

  [[nodiscard]] int minTurns() const { return m_minTurns; }
  void setMinTurns(const int minTurns) { m_minTurns = minTurns; }

  [[nodiscard]] int maxTurns() const { return m_maxTurns; }
  void setMaxTurns(const int maxTurns) { m_maxTurns = maxTurns; }

  [[nodiscard]] const std::string& message1() const { return m_message1; }
  void setMessage1(const std::string& message1) { m_message1 = message1; }

  [[nodiscard]] const std::string& message2() const { return m_message2; }
  void setMessage2(const std::string& message2) { m_message2 = message2; }
  [[nodiscard]] const std::string& message3() const { return m_message3; }
  void setMessage3(const std::string& message3) { m_message3 = message3; }

  [[nodiscard]] const std::string& message4() const { return m_message4; }
  void setMessage4(const std::string& message4) { m_message4 = message4; }

  [[nodiscard]] StateMotion motion() const { return m_motion; }
  void setMotion(const StateMotion motion) { m_motion = motion; }

  [[nodiscard]] const std::string& name() const { return m_name; }
  void setName(const std::string& name) { m_name = name; }

  [[nodiscard]] const std::string& note() const { return m_note; }
  void setNote(const std::string& note) { m_note = note; }

  [[nodiscard]] StateOverlay overlay() const { return m_overlay; }
  void setOverlay(const StateOverlay overlay) { m_overlay = overlay; }

  [[nodiscard]] int priority() const { return m_priority; }
  void setPriority(const int priority) { m_priority = priority; }

  [[nodiscard]] bool releaseByDamage() const { return m_releaseByDamage; }
  void setReleaseByDamage(const bool releaseByDamage) { m_releaseByDamage = releaseByDamage; }

  [[nodiscard]] bool removeAtBattleEnd() const { return m_removeAtBattleEnd; }
  void setRemoveAtBattleEnd(const bool removeAtBattleEnd) { m_removeAtBattleEnd = removeAtBattleEnd; }

  [[nodiscard]] bool removeByDamage() const { return m_removeByDamage; }
  void setRemoveByDamage(const bool removeByDamage) { m_removeByDamage = removeByDamage; }

  [[nodiscard]] bool removeByRestriction() const { return m_removeByRestriction; }
  void setRemoveByRestriction(const bool removeByRestriction) { m_removeByRestriction = removeByRestriction; }

  [[nodiscard]] bool removeByWalking() const { return m_removeByWalking; }
  void setRemoveByWalking(const bool removeByWalking) { m_removeByWalking = removeByWalking; }

  [[nodiscard]] StateRestriction restriction() const { return m_restriction; }
  void setRestriction(const StateRestriction restriction) { m_restriction = restriction; }

  [[nodiscard]] int stepsToRemove() const { return m_stepsToRemove; }
  void setStepsToRemove(const int stepsToRemove) { m_stepsToRemove = stepsToRemove; }

  [[nodiscard]] std::vector<Trait>& traits() { return m_traits; }
  void setTraits(const std::vector<Trait>& traits) { m_traits = traits; }

  [[nodiscard]] bool isValid() const { return m_isValid; }
  void setValid(const bool isValid) { m_isValid = isValid; }

private:
  int m_id = 0;
  AutoRemovalTiming m_autoRemovalTiming = AutoRemovalTiming::None;
  int m_chanceByDamage = 100;
  int m_iconIndex = 0;
  int m_minTurns = 1;
  int m_maxTurns = 1;
  std::string m_message1;
  std::string m_message2;
  std::string m_message3;
  std::string m_message4;
  StateMotion m_motion = StateMotion::Normal;
  std::string m_name;
  std::string m_note;
  StateOverlay m_overlay = StateOverlay::None;
  int m_priority = 50;
  bool m_releaseByDamage;
  bool m_removeAtBattleEnd = false;
  bool m_removeByDamage = false;
  bool m_removeByRestriction = false;
  bool m_removeByWalking = false;
  StateRestriction m_restriction = StateRestriction::None;
  int m_stepsToRemove = 1000;
  std::vector<Trait> m_traits;

  /*!
   * @name m_isValid
   * @details
   * Indicates that this is an actual valid entry and not a dummy
   * When making a new entry make sure to set this to true or it won't be
   * serialized.
   */
  bool m_isValid{false};
};
void to_json(nlohmann::ordered_json& j, const State& state);
void from_json(const nlohmann::ordered_json& j, State& state);