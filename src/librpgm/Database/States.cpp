#include "States.hpp"

#include <fstream>

using json = nlohmann::json;

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
