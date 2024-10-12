#include "Enemies.hpp"

#include <fstream>

using json = nlohmann::json;

Enemies Enemies::load(std::string_view filename) {
  std::ifstream file(filename.data());
  json data = json::parse(file);
  Enemies enemies;
  enemies.m_enemies.reserve(data.size());

  int i = 0;
  for (const auto& [_, value] : data.items()) {
    Enemy& enemy = enemies.m_enemies.emplace_back();
    enemy.m_isValid = value != nullptr;
    if (enemy.m_isValid) {
      value.get_to(enemy);
    } else {
      enemy.id = i;
    }
    ++i;
  }
  return enemies;
}

bool Enemies::serialize(std::string_view filename) {
  // TODO: Temporary file for atomic writes

  std::ofstream file(filename.data());
  json data;

  for (const Enemy& armor : m_enemies) {
    if (armor.m_isValid) {
      data.push_back(armor);
    } else {
      data.push_back(nullptr);
    }
  }

  file << data.dump(4);

  return true;
}
