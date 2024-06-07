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
};

class Armors {
public:
  static Armors load(std::string_view filepath);
  bool serialize(std::string_view filename);

  [[nodiscard]] Armor* armor(int id) {
    for (auto& item : m_armors) {
      if (item.id == id) {
        return &item;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Armor* armor(int id) const {
    for (const auto& set : m_armors) {
      if (set.id == id) {
        return &set;
      }
    }

    return nullptr;
  }
private:
  std::vector<Armor> m_armors;
};
