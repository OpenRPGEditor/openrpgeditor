#include "Troops.hpp"

#include <fstream>

using json = nlohmann::json;

void to_json(nlohmann::json& j, const Troop::Member& m) {
  j = {
      {"enemyId", m.enemyId},
      {"x", m.x},
      {"y", m.y},
      {"hidden", m.hidden},
  };
}

void from_json(const nlohmann::json& j, Troop::Member& m) {
  m.enemyId = j.value("enemyId", m.enemyId);
  m.x = j.value("x", m.x);
  m.y = j.value("y", m.y);
  m.hidden = j.value("hidden", m.hidden);
}

void to_json(nlohmann::json& j, const Troop& t) {
  j = {
      {"id", t.id},
      {"name", t.name},
      {"members", t.members},
  };
}

void from_json(const nlohmann::json& j, Troop& t) {
  t.id = j.value("id", t.id);
  t.name = j.value("name", t.name);
  t.members = j.value("members", t.members);
}

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

  file << data.dump(4);

  return true;
}