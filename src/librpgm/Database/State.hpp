#pragma once

#include "Database/Globals.hpp"
#include "Database/Trait.hpp"

class State final : public IModifiable {
  friend class StatesSerializer;
  friend void to_json(nlohmann::ordered_json& j, const State& state);
  friend void from_json(const nlohmann::ordered_json& j, State& state);

public:
  State() = default;
  State(const State& other);
  State& operator=(const State& other);
  State(State&& other) noexcept;
  State& operator=(State&& other) noexcept;

  [[nodiscard]] int id() const;
  void setId(int id);

  [[nodiscard]] AutoRemovalTiming autoRemovalTiming() const;
  void setAutoRemovalTiming(AutoRemovalTiming autoRemovalTiming);

  [[nodiscard]] int chanceByDamage() const;
  void setChanceByDamage(int chanceByDamage);

  [[nodiscard]] int iconIndex() const;
  void setIconIndex(int iconIndex);

  [[nodiscard]] int minTurns() const;
  void setMinTurns(int minTurns);

  [[nodiscard]] int maxTurns() const;
  void setMaxTurns(int maxTurns);

  [[nodiscard]] const std::string& message1() const;
  void setMessage1(const std::string& message1);

  [[nodiscard]] const std::string& message2() const;
  void setMessage2(const std::string& message2);
  [[nodiscard]] const std::string& message3() const;
  void setMessage3(const std::string& message3);

  [[nodiscard]] const std::string& message4() const;
  void setMessage4(const std::string& message4);

  [[nodiscard]] StateMotion motion() const;
  void setMotion(StateMotion motion);

  [[nodiscard]] const std::string& name() const;
  void setName(const std::string& name);

  [[nodiscard]] const std::string& note() const;
  void setNote(const std::string& note);

  [[nodiscard]] StateOverlay overlay() const;
  void setOverlay(StateOverlay overlay);

  [[nodiscard]] int priority() const;
  void setPriority(int priority);

  [[nodiscard]] bool releaseByDamage() const;
  void setReleaseByDamage(bool releaseByDamage);

  [[nodiscard]] bool removeAtBattleEnd() const;
  void setRemoveAtBattleEnd(bool removeAtBattleEnd);

  [[nodiscard]] bool removeByDamage() const;
  void setRemoveByDamage(bool removeByDamage);

  [[nodiscard]] bool removeByRestriction() const;
  void setRemoveByRestriction(bool removeByRestriction);

  [[nodiscard]] bool removeByWalking() const;
  void setRemoveByWalking(bool removeByWalking);

  [[nodiscard]] StateRestriction restriction() const;
  void setRestriction(StateRestriction restriction);

  [[nodiscard]] int stepsToRemove() const;
  void setStepsToRemove(int stepsToRemove);

  [[nodiscard]] std::vector<Trait>& traits();
  void setTraits(const std::vector<Trait>& traits);

  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;

  bool isModified() const override {
    return IModifiable::isModified() | std::ranges::any_of(m_traits, [](const Trait& trait) { return trait.isModified(); });
  }

  [[nodiscard]] bool isValid() const { return m_isValid; }
  void setValid(const bool isValid) { m_isValid = isValid; }

  rpgmutils::signal<void(State*, int)>& idModified();
  rpgmutils::signal<void(State*, AutoRemovalTiming)>& autoRemovalTimingModified();
  rpgmutils::signal<void(State*, int)>& chanceByDamageModified();
  rpgmutils::signal<void(State*, int)>& iconIndexModified();
  rpgmutils::signal<void(State*, int)>& minTurnsModified();
  rpgmutils::signal<void(State*, int)>& maxTurnsModified();
  rpgmutils::signal<void(State*, const std::string&)>& message1Modified();
  rpgmutils::signal<void(State*, const std::string&)>& message2Modified();
  rpgmutils::signal<void(State*, const std::string&)>& message3Modified();
  rpgmutils::signal<void(State*, const std::string&)>& message4Modified();
  rpgmutils::signal<void(State*, StateMotion)>& motionModified();
  rpgmutils::signal<void(State*, const std::string&)>& nameModified();
  rpgmutils::signal<void(State*, const std::string&)>& noteModified();
  rpgmutils::signal<void(State*, StateOverlay)>& overlayModified();
  rpgmutils::signal<void(State*, int)>& priorityModified();
  rpgmutils::signal<void(State*, bool)>& releaseByDamageModified();
  rpgmutils::signal<void(State*, bool)>& removeAtBattleEndModified();
  rpgmutils::signal<void(State*, bool)>& removeByDamageModified();
  rpgmutils::signal<void(State*, bool)>& removeByRestrictionModified();
  rpgmutils::signal<void(State*, bool)>& removeByWalkingModified();
  rpgmutils::signal<void(State*, StateRestriction)>& restrictionModified();
  rpgmutils::signal<void(State*, int)>& stepsToRemoveModified();
  rpgmutils::signal<void(State*, const std::vector<Trait>&)>& traitsModified();

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

  std::optional<int> m_oldid;
  std::optional<AutoRemovalTiming> m_oldautoRemovalTiming;
  std::optional<int> m_oldchanceByDamage;
  std::optional<int> m_oldiconIndex;
  std::optional<int> m_oldminTurns;
  std::optional<int> m_oldmaxTurns;
  std::optional<std::string> m_oldmessage1;
  std::optional<std::string> m_oldmessage2;
  std::optional<std::string> m_oldmessage3;
  std::optional<std::string> m_oldmessage4;
  std::optional<StateMotion> m_oldmotion;
  std::optional<std::string> m_oldname;
  std::optional<std::string> m_oldnote;
  std::optional<StateOverlay> m_oldoverlay;
  std::optional<int> m_oldpriority;
  std::optional<bool> m_oldreleaseByDamage;
  std::optional<bool> m_oldremoveAtBattleEnd;
  std::optional<bool> m_oldremoveByDamage;
  std::optional<bool> m_oldremoveByRestriction;
  std::optional<bool> m_oldremoveByWalking;
  std::optional<StateRestriction> m_oldrestriction;
  std::optional<int> m_oldstepsToRemove;
  std::optional<std::vector<Trait>> m_oldtraits;

  std::optional<rpgmutils::signal<void(State*, int)>> m_idModified;
  std::optional<rpgmutils::signal<void(State*, AutoRemovalTiming)>> m_autoRemovalTimingModified;
  std::optional<rpgmutils::signal<void(State*, int)>> m_chanceByDamageModified;
  std::optional<rpgmutils::signal<void(State*, int)>> m_iconIndexModified;
  std::optional<rpgmutils::signal<void(State*, int)>> m_minTurnsModified;
  std::optional<rpgmutils::signal<void(State*, int)>> m_maxTurnsModified;
  std::optional<rpgmutils::signal<void(State*, const std::string&)>> m_message1Modified;
  std::optional<rpgmutils::signal<void(State*, const std::string&)>> m_message2Modified;
  std::optional<rpgmutils::signal<void(State*, const std::string&)>> m_message3Modified;
  std::optional<rpgmutils::signal<void(State*, const std::string&)>> m_message4Modified;
  std::optional<rpgmutils::signal<void(State*, StateMotion)>> m_motionModified;
  std::optional<rpgmutils::signal<void(State*, const std::string&)>> m_nameModified;
  std::optional<rpgmutils::signal<void(State*, const std::string&)>> m_noteModified;
  std::optional<rpgmutils::signal<void(State*, StateOverlay)>> m_overlayModified;
  std::optional<rpgmutils::signal<void(State*, int)>> m_priorityModified;
  std::optional<rpgmutils::signal<void(State*, bool)>> m_releaseByDamageModified;
  std::optional<rpgmutils::signal<void(State*, bool)>> m_removeAtBattleEndModified;
  std::optional<rpgmutils::signal<void(State*, bool)>> m_removeByDamageModified;
  std::optional<rpgmutils::signal<void(State*, bool)>> m_removeByRestrictionModified;
  std::optional<rpgmutils::signal<void(State*, bool)>> m_removeByWalkingModified;
  std::optional<rpgmutils::signal<void(State*, StateRestriction)>> m_restrictionModified;
  std::optional<rpgmutils::signal<void(State*, int)>> m_stepsToRemoveModified;
  std::optional<rpgmutils::signal<void(State*, const std::vector<Trait>&)>> m_traitsModified;

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