#include "States.hpp"

#include <fstream>

using json = nlohmann::ordered_json;
void to_json(nlohmann::ordered_json& j, const State& state) {
  j = {
      {"id", state.id},
      {"autoRemovalTiming", state.autoRemovalTiming},
      {"chanceByDamage", state.chanceByDamage},
      {"iconIndex", state.iconIndex},
      {"maxTurns", state.maxTurns},
      {"message1", state.message1},
      {"message2", state.message2},
      {"message3", state.message3},
      {"message4", state.message4},
      {"name", state.name},
      {"note", state.note},
      {"overlay", state.overlay},
      {"priority", state.priority},
      {"releaseByDamage", state.releaseByDamage},
      {"removeAtBattleEnd", state.removeAtBattleEnd},
      {"removeByDamage", state.removeByDamage},
      {"removeByRestriction", state.removeByRestriction},
      {"restriction", state.restriction},
      {"stepsToRemove", state.stepsToRemove},
      {"traits", state.traits},
  };
}

void from_json(const nlohmann::ordered_json& j, State& state) {
  state.id = j.value("id", state.id);
  state.autoRemovalTiming = j.value("autoRemovalTiming", state.autoRemovalTiming);
  state.chanceByDamage = j.value("chanceByDamage", state.chanceByDamage);
  state.iconIndex = j.value("iconIndex", state.iconIndex);
  state.maxTurns = j.value("maxTurns", state.maxTurns);
  state.message1 = j.value("message1", state.message1);
  state.message2 = j.value("message2", state.message2);
  state.message3 = j.value("message3", state.message3);
  state.message4 = j.value("message4", state.message4);
  state.name = j.value("name", state.name);
  state.note = j.value("note", state.note);
  state.overlay = j.value("overlay", state.overlay);
  state.priority = j.value("priority", state.priority);
  state.releaseByDamage = j.value("releaseByDamage", state.releaseByDamage);
  state.removeAtBattleEnd = j.value("removeAtBattleEnd", state.removeAtBattleEnd);
  state.removeByRestriction = j.value("removeByRestriction", state.removeByRestriction);
  state.restriction = j.value("restriction", state.restriction);
  state.stepsToRemove = j.value("stepsToRemove", state.stepsToRemove);
  state.traits = j.value("traits", state.traits);
}

States States::load(std::string_view filename) {
  std::ifstream file(filename.data());
  json data = json::parse(file);
  States states;
  states.m_states.reserve(data.size());

  int i = 0;
  for (const auto& [_, value] : data.items()) {
    State& state = states.m_states.emplace_back();
    state.m_isValid = value != nullptr;
    if (state.m_isValid) {
      value.get_to(state);
    } else {
      state.id = i;
    }
    ++i;
  }
  return states;
}

bool States::serialize(std::string_view filename) {
  // TODO: Temporary file for atomic writes

  std::ofstream file(filename.data());
  json data;

  for (const State& state : m_states) {
    if (state.m_isValid) {
      data.push_back(state);
    } else {
      data.push_back(nullptr);
    }
  }

  file << data.dump(4);

  return true;
}
