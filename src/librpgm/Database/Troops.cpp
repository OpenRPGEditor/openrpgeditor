#include "Troops.hpp"

#include <fstream>

using json = nlohmann::json;

Troops Troops::load(std::string_view filepath) {
  std::ifstream file(filepath.data());
  json data = json::parse(file);
  Troops troops;
  troops.m_troops.reserve(data.size());

  for (const auto& [_, value] : data.items()) {
    if (value == nullptr) {
      continue;
    }

    Troop& tileset = troops.m_troops.emplace_back();
    value.get_to(tileset);
  }
  return troops;
}

bool Troops::serialize(std::string_view filepath) { // TODO: Temporary file for atomic writes

  std::ofstream file(filepath.data());
  json data{nullptr};

  for (const Troop& tileset : m_troops) {
    data.push_back(tileset);
  }

  file << data;

  return true;
}