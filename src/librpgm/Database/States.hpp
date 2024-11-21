#pragma once

#include "Database/Globals.hpp"
#include "Database/Trait.hpp"

#include <nlohmann/json.hpp>
#include <vector>
#include <string>

class State {
  friend void to_json(nlohmann::ordered_json& j, const State& state);
  friend void from_json(const nlohmann::ordered_json& j, State& state);

public:
  int id = 0;
  AutoRemovalTiming autoRemovalTiming = AutoRemovalTiming::None;
  int chanceByDamage = 100;
  int iconIndex = 0;
  int maxTurns = 1;
  std::string message1;
  std::string message2;
  std::string message3;
  std::string message4;
  int minTurns = 1;
  StateMotion motion = StateMotion::Normal;
  std::string name;
  std::string note;
  StateOverlay overlay = StateOverlay::None;
  int priority = 50;
  bool releaseByDamage;
  bool removeAtBattleEnd = false;
  bool removeByDamage = false;
  bool removeByRestriction = false;
  bool removeByWalking = false;
  StateRestriction restriction = StateRestriction::None;
  int stepsToRemove = 1000;
  std::vector<Trait> traits;

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

class States {
public:
  static States load(std::string_view filepath);
  bool serialize(std::string_view filename);

  [[nodiscard]] State* state(int id) {
    for (auto& state : m_states) {
      if (state.id == id && state.m_isValid) {
        return &state;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const State* state(int id) const {
    for (auto& state : m_states) {
      if (state.id == id && state.m_isValid) {
        return &state;
      }
    }

    return nullptr;
  }

  [[nodiscard]] int count() const { return m_states.size() - 1; }

  [[nodiscard]] std::vector<State>& states() { return m_states; }
  [[nodiscard]] const std::vector<State>& states() const { return m_states; }
  int count() { return m_states.size() - 1; }

  void resize(int newSize) {
    ++newSize;
    int oldSize = m_states.size();
    m_states.resize(newSize);
    if (newSize > oldSize) {
      for (int i = oldSize; i < m_states.size(); ++i) {
        m_states[i].id = i;
      }
    }
  }

private:
  std::vector<State> m_states;
};
