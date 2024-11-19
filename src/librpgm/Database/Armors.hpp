#pragma once

#include "Database/Globals.hpp"
#include "Database/Trait.hpp"
#include <nlohmann/json.hpp>

#include <string_view>

struct Armor {
  friend void to_json(nlohmann::json& out, const Armor& armor);
  friend void from_json(const nlohmann::json& from, Armor& armor);
  int id{};
  int atypeId{};
  std::string description;
  int etypeId{};
  std::vector<Trait> traits;
  int iconIndex{};
  std::string name;
  std::string note;
  std::array<int, 8> params{}; // Attack, Defance, M.Attack, M.Defence, Agility, Luck, Max HP, Max MP
  int price{};

  /*!
   * @name m_isValid
   * @details
   * Indicates that this is an actual valid entry and not a dummy
   * When making a new entry make sure to set this to true or it won't be
   * serialized.
   */
  bool m_isValid{false};
};
void to_json(nlohmann::json& out, const Armor& armor);
void from_json(const nlohmann::json& from, Armor& armor);

class Armors {
public:
  static Armors load(std::string_view filepath);
  bool serialize(std::string_view filename);

  [[nodiscard]] std::vector<Armor>& armors() { return m_armors; }
  [[nodiscard]] const std::vector<Armor>& armors() const { return m_armors; }

  [[nodiscard]] Armor* armor(int id) {
    for (auto& armor : m_armors) {
      if (armor.id == id && armor.m_isValid) {
        return &armor;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Armor* armor(int id) const {
    for (const auto& armor : m_armors) {
      if (armor.id == id && armor.m_isValid) {
        return &armor;
      }
    }

    return nullptr;
  }

  std::vector<Armor>& armorList() { return m_armors; }
  const std::vector<Armor>& armorList() const { return m_armors; }
  int count() { return m_armors.size() - 1; }

  void resize(int newSize) {
    ++newSize;
    int oldSize = m_armors.size();
    m_armors.resize(newSize);
    if (newSize > oldSize) {
      for (int i = oldSize; i < m_armors.size(); ++i) {
        m_armors[i].id = i;
      }
    }
  }

private:
  std::vector<Armor> m_armors;
};
