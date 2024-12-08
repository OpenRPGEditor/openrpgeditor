#include "Database/State.hpp"

void to_json(nlohmann::ordered_json& j, const State& state) {
  j = {
      {"id", state.m_id},
      {"autoRemovalTiming", state.m_autoRemovalTiming},
      {"chanceByDamage", state.m_chanceByDamage},
      {"iconIndex", state.m_iconIndex},
      {"maxTurns", state.m_maxTurns},
      {"message1", state.m_message1},
      {"message2", state.m_message2},
      {"message3", state.m_message3},
      {"message4", state.m_message4},
      {"minTurns", state.m_minTurns},
      {"motion", state.m_motion},
      {"name", state.m_name},
      {"note", state.m_note},
      {"overlay", state.m_overlay},
      {"priority", state.m_priority},
      {"releaseByDamage", state.m_releaseByDamage},
      {"removeAtBattleEnd", state.m_removeAtBattleEnd},
      {"removeByDamage", state.m_removeByDamage},
      {"removeByRestriction", state.m_removeByRestriction},
      {"removeByWalking", state.m_removeByWalking},
      {"restriction", state.m_restriction},
      {"stepsToRemove", state.m_stepsToRemove},
      {"traits", state.m_traits},
  };
}

void from_json(const nlohmann::ordered_json& j, State& state) {
  state.m_id = j.value("id", state.m_id);
  state.m_autoRemovalTiming = j.value("autoRemovalTiming", state.m_autoRemovalTiming);
  state.m_chanceByDamage = j.value("chanceByDamage", state.m_chanceByDamage);
  state.m_iconIndex = j.value("iconIndex", state.m_iconIndex);
  state.m_maxTurns = j.value("maxTurns", state.m_maxTurns);
  state.m_message1 = j.value("message1", state.m_message1);
  state.m_message2 = j.value("message2", state.m_message2);
  state.m_message3 = j.value("message3", state.m_message3);
  state.m_message4 = j.value("message4", state.m_message4);
  state.m_minTurns = j.value("minTurns", state.m_minTurns);
  state.m_motion = j.value("motion", state.m_motion);
  state.m_name = j.value("name", state.m_name);
  state.m_note = j.value("note", state.m_note);
  state.m_overlay = j.value("overlay", state.m_overlay);
  state.m_priority = j.value("priority", state.m_priority);
  state.m_releaseByDamage = j.value("releaseByDamage", state.m_releaseByDamage);
  state.m_removeAtBattleEnd = j.value("removeAtBattleEnd", state.m_removeAtBattleEnd);
  state.m_removeByRestriction = j.value("removeByRestriction", state.m_removeByRestriction);
  state.m_removeByWalking = j.value("removeByWalking", state.m_removeByWalking);
  state.m_restriction = j.value("restriction", state.m_restriction);
  state.m_stepsToRemove = j.value("stepsToRemove", state.m_stepsToRemove);
  state.m_traits = j.value("traits", state.m_traits);
}
