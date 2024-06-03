#include "Enemies.hpp"

#include <fstream>

using json = nlohmann::json;

Enemies Enemies::load(std::string_view filename) {
  std::ifstream file(filename.data());
  json data = json::parse(file);
  Enemies enemies;
  enemies.m_enemies.reserve(data.size());

  for (const auto& [_, value] : data.items()) {
    if (value == nullptr) {
      continue;
    }

    Enemy& enemy = enemies.m_enemies.emplace_back();
    value.get_to(enemy);
  }
  return enemies;
}

bool Enemies::serialize(std::string_view filename) {
  // TODO: Temporary file for atomic writes

  std::ofstream file(filename.data());
  json data{nullptr};

  for (const Enemy& armor : m_enemies) {
    data.push_back(armor);
  }

  file << data;

  return true;
}
