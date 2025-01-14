#pragma once

#include "Database/Troop.hpp"

#include "nlohmann/json.hpp"

class Troops final : IModifiable {
public:
  static Troops load(std::string_view filepath);

  bool serialize(std::string_view filename);

  [[nodiscard]] Troop* troop(int id) {
    for (auto& troop : m_troops) {
      if (troop.id() == id && troop.isValid()) {
        return &troop;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Troop* troop(int id) const {
    for (const auto& troop : m_troops) {
      if (troop.id() == id && troop.isValid()) {
        return &troop;
      }
    }

    return nullptr;
  }
  std::vector<Troop>& troops() { return m_troops; }
  const std::vector<Troop>& troops() const { return m_troops; }
  int count() const { return m_troops.size() - 1; }

  void resize(int newSize) {
    ++newSize;
    const int oldSize = m_troops.size();
    m_troops.resize(newSize);
    if (newSize > oldSize) {
      for (int i = oldSize; i < m_troops.size(); ++i) {
        m_troops[i].setId(i);
      }
    }
  }

  bool isModified() const override {
    return IModifiable::isModified() | std::ranges::any_of(m_troops, [](const auto& troop) { return troop.isModified(); });
  }

private:
  std::vector<Troop> m_troops;
};
