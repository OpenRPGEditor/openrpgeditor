#include "Enemies.hpp"

#include <fstream>

using json = nlohmann::ordered_json;

void to_json(nlohmann::ordered_json& to, const Enemy& enemy) {
  to = {
      {"id", enemy.id},
      {"actions", enemy.actions},
      {"battlerHue", enemy.battlerHue},
      {"battlerName", enemy.battlerName},
      {"dropItems", enemy.dropItems},
      {"exp", enemy.exp},
      {"traits", enemy.traits},
      {"gold", enemy.gold},
      {"note", enemy.note},
      {"params", enemy.params},
  };
}
void from_json(const nlohmann::ordered_json& from, Enemy& enemy) {
  enemy.id = from.value("id", enemy.id);
  enemy.actions = from.value("actions", enemy.actions);
  enemy.battlerHue = from.value("battlerHue", enemy.battlerHue);
  enemy.battlerName = from.value("battlerName", enemy.battlerName);
  enemy.dropItems = from.value("dropItems", enemy.dropItems);
  enemy.exp = from.value("exp", enemy.exp);
  enemy.traits = from.value("traits", enemy.traits);
  enemy.gold = from.value("gold", enemy.gold);
  enemy.name = from.value("name", enemy.name);
  enemy.note = from.value("note", enemy.note);
  enemy.params = from.value("params", enemy.params);
}
void to_json(nlohmann::ordered_json& to, const Enemy::Action& action) {
  to = {
      {"conditionParam1", action.conditionParam1},
      {"conditionParam2", action.conditionParam2},
      {"conditionType", action.conditionType},
      {"rating", action.rating},
      {"skillId", action.skillId},
  };
}
void from_json(const nlohmann::ordered_json& from, Enemy::Action& action) {
  action.conditionParam1 = from.value("conditionParam1", action.conditionParam1);
  action.conditionParam2 = from.value("conditionParam2", action.conditionParam2);
  action.conditionType = from.value("conditionType", action.conditionType);
  action.rating = from.value("rating", action.rating);
  action.skillId = from.value("skillId", action.skillId);
}

void to_json(nlohmann::ordered_json& to, const Enemy::DropItem& dropitem) {
  to = {
      {"dataId", dropitem.dataId},
      {"denominator", dropitem.denominator},
      {"kind", dropitem.kind},
  };
}
void from_json(const nlohmann::ordered_json& from, Enemy::DropItem& dropitem) {
  dropitem.dataId = from.value("dataId", dropitem.dataId);
  dropitem.denominator = from.value("denominator", dropitem.denominator);
  dropitem.kind = from.value("kind", dropitem.kind);
}

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
