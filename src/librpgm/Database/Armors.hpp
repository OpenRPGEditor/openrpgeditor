#pragma once

#include "Database/Armor.hpp"
#include <nlohmann/json.hpp>

class Armors final : public IModifiable {
public:
  Armors() { m_armors.emplace_back(); }
  [[nodiscard]] std::vector<Armor>& armors() { return m_armors; }
  [[nodiscard]] const std::vector<Armor>& armors() const { return m_armors; }

  [[nodiscard]] Armor* armor(const int id) {
    for (auto& armor : m_armors) {
      if (armor.id() == id && armor.isValid()) {
        return &armor;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Armor* armor(const int id) const {
    for (const auto& armor : m_armors) {
      if (armor.id() == id && armor.isValid()) {
        return &armor;
      }
    }

    return nullptr;
  }

  std::vector<Armor>& armorList() { return m_armors; }
  const std::vector<Armor>& armorList() const { return m_armors; }
  int count() const { return m_armors.size() - 1; }

  void resize(int newSize) {
    ++newSize;
    const int oldSize = m_armors.size();
    m_armors.resize(newSize);
    if (newSize > oldSize) {
      for (int i = oldSize; i < m_armors.size(); ++i) {
        m_armors[i].setId(i);
      }
    }
    setModified();
  }

  bool isModified() const override {
    return IModifiable::isModified() | std::ranges::any_of(m_armors, [](const auto& arm) { return arm.isModified(); });
  }

private:
  std::vector<Armor> m_armors;
};
