#pragma once

#include "Database/Globals.hpp"
#include "Database/Trait.hpp"

#include <nlohmann/json.hpp>
#include <vector>
#include <string>

class State {
public:
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(State, id, autoRemovalTiming, chanceByDamage, iconIndex, maxTurns,
                                              message1, message2, message3, message4, name, note, overlay, priority,
                                              releaseByDamage, removeAtBattleEnd, removeByDamage, removeByRestriction,
                                              restriction, stepsToRemove, traits);
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
  StateRestriction restriction = StateRestriction::None;
  int stepsToRemove = 1000;
  std::vector<Trait> traits;
};

class States {
public:
  static States load(std::string_view filepath);
  bool serialize(std::string_view filename);

  [[nodiscard]] State* state(int id) {
    for (auto& item : m_states) {
      if (item.id == id) {
        return &item;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const State* state(int id) const {
    for (const auto& set : m_states) {
      if (set.id == id) {
        return &set;
      }
    }

    return nullptr;
  }
private:
  std::vector<State> m_states;
};
