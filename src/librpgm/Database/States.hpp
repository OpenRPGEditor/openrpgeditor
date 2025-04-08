#pragma once

#include "Database/State.hpp"

#include <vector>

class States final : public IModifiable {
public:
  [[nodiscard]] State* state(int id) {
    for (auto& state : m_states) {
      if (state.id() == id && state.isValid()) {
        return &state;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const State* state(int id) const {
    for (auto& state : m_states) {
      if (state.id() == id && state.isValid()) {
        return &state;
      }
    }

    return nullptr;
  }

  [[nodiscard]] int count() const { return m_states.size() - 1; }

  [[nodiscard]] std::vector<State>& states() { return m_states; }
  [[nodiscard]] const std::vector<State>& states() const { return m_states; }

  void resize(int newSize) {
    ++newSize;
    const int oldSize = m_states.size();
    m_states.resize(newSize);
    if (newSize > oldSize) {
      for (int i = oldSize; i < m_states.size(); ++i) {
        m_states[i].setId(i);
      }
    }
  }

  bool isModified() const override {
    return IModifiable::isModified() | std::ranges::any_of(m_states, [](const State& state) { return state.isModified(); });
  }

private:
  std::vector<State> m_states;
};
