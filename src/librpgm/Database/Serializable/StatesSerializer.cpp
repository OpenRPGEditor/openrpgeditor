#include "Database/Serializable/StatesSerializer.hpp"

void StatesSerializer::serialize(std::ofstream& os) const {
  try {
    nlohmann::ordered_json data;
    if (m_data.states().empty()) {
      data.push_back(nullptr);
      os << data.dump(4);
      return;
    }

    for (const State& state : m_data.states()) {
      if (state.isValid()) {
        data.push_back(state);
      } else {
        data.push_back(nullptr);
      }
    }

    os << data.dump(4);
  } catch (...) {}
}

void StatesSerializer::deserialize(std::ifstream& is) {
  try {
    nlohmann::ordered_json data = nlohmann::ordered_json::parse(is);
    m_data.states().reserve(data.size());
    m_data.disableSignals();

    int i = 0;
    for (const auto& [_, value] : data.items()) {
      State& state = m_data.states().emplace_back();
      state.m_isValid = value != nullptr;
      state.disableSignals();
      if (state.isValid()) {
        value.get_to(state);
      } else {
        state.m_id = i;
      }
      ++i;
      state.enableSignals();
    }
    m_data.disableSignals();
  } catch (...) {}
}
