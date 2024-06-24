#pragma once

#include "Database/Globals.hpp"
#include "Database/Trait.hpp"
#include <nlohmann/json.hpp>

#include <string_view>

struct Armor {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Armor, id, atypeId, description, etypeId, traits, iconIndex, name, note,
                                              params);
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
private:
  std::vector<Armor> m_armors;
};
