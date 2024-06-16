#include "Troops.hpp"

#include <fstream>

using json = nlohmann::json;

Troops Troops::load(std::string_view filepath) {
  std::ifstream file(filepath.data());
  json data = json::parse(file);
  Troops troops;
  troops.m_troops.reserve(data.size());

  int i = 0;
  for (const auto& [_, value] : data.items()) {
    Troop& tileset = troops.m_troops.emplace_back();
    tileset.m_isValid = value != nullptr;
    if (tileset.m_isValid) {
      value.get_to(tileset);
    } else {
      tileset.id = i;
    }
    ++i;
  }
  return troops;
}

bool Troops::serialize(std::string_view filepath) { // TODO: Temporary file for atomic writes

  std::ofstream file(filepath.data());
  json data{nullptr};

  for (const Troop& troop : m_troops) {
    data.push_back(troop);
  }

  file << data;

  return true;
}