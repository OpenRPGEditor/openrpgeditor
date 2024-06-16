#include "Weapons.hpp"

#include <fstream>

using json = nlohmann::json;

Weapons Weapons::load(std::string_view filename) {
  std::ifstream file(filename.data());
  json data = json::parse(file);
  Weapons weapons;
  weapons.m_weapons.reserve(data.size());

  int i = 0;
  for (const auto& [_, value] : data.items()) {
    Weapon& weapon = weapons.m_weapons.emplace_back();
    weapon.m_isValid = value != nullptr;
    if (weapon.m_isValid) {
      value.get_to(weapon);
    } else {
      weapon.id = i;
    }
    ++i;
  }
  return weapons;
}

bool Weapons::serialize(std::string_view filename) {
  // TODO: Temporary file for atomic writes

  std::ofstream file(filename.data());
  json data{nullptr};

  for (const Weapon& weapon : m_weapons) {
    data.push_back(weapon);
  }

  file << data;

  return true;
}
