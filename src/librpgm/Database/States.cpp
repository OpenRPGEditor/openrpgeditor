#include "States.hpp"

#include <fstream>

using json = nlohmann::json;

States States::load(std::string_view filename) {
  std::ifstream file(filename.data());
  json data = json::parse(file);
  States states;
  states.m_states.reserve(data.size());

  for (const auto& [_, value] : data.items()) {
    if (value == nullptr) {
      continue;
    }

    State& state = states.m_states.emplace_back();
    value.get_to(state);
  }
  return states;
}

bool States::serialize(std::string_view filename) {
  // TODO: Temporary file for atomic writes

  std::ofstream file(filename.data());
  json data{nullptr};

  for (const State& state : m_states) {
    data.push_back(state);
  }

  file << data;

  return true;
}
