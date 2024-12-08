#include "Database/Troop.hpp"
#include "Database/CommandParser.hpp"

void to_json(nlohmann::ordered_json& j, const Troop::Conditions& c) {
  j = {
      {"actorHp", c.actorHp},   {"actorId", c.actorId},         {"actorValid", c.actorValid}, {"enemyHp", c.enemyHp}, {"enemyIndex", c.enemyIndex}, {"enemyValid", c.enemyValid},
      {"switchId", c.switchId}, {"switchValid", c.switchValid}, {"turnA", c.turnA},           {"turnB", c.turnB},     {"turnEnding", c.turnEnding}, {"turnValid", c.turnValid},
  };
}
void from_json(const nlohmann::ordered_json& j, Troop::Conditions& c) {
  c.actorHp = j.value("actorHp", c.actorHp);
  c.actorId = j.value("actorId", c.actorId);
  c.actorValid = j.value("actorValid", c.actorValid);
  c.enemyHp = j.value("enemyHp", c.enemyHp);
  c.enemyIndex = j.value("enemyIndex", c.enemyIndex);
  c.enemyValid = j.value("enemyValid", c.enemyValid);
  c.switchId = j.value("switchId", c.switchId);
  c.switchValid = j.value("switchValid", c.switchValid);
  c.turnA = j.value("turnA", c.turnA);
  c.turnB = j.value("turnB", c.turnB);
  c.turnEnding = j.value("turnEnding", c.turnEnding);
  c.turnValid = j.value("turnValid", c.turnValid);
}

void to_json(nlohmann::ordered_json& j, const Troop::Page& p) {
  j["conditions"] = p.conditions;
  nlohmann::ordered_json list;
  CommandParser::serialize(list, p.list);
  j["list"] = list;
  j["span"] = p.span;
}

void from_json(const nlohmann::ordered_json& j, Troop::Page& p) {
  p.conditions = j.value("conditions", p.conditions);
  CommandParser cmdParser;
  p.list = cmdParser.parse(j["list"]);
  p.span = j.value("span", p.span);
}

void to_json(nlohmann::ordered_json& j, const Troop::Member& m) {
  j = {
      {"enemyId", m.enemyId},
      {"x", m.x},
      {"y", m.y},
      {"hidden", m.hidden},
  };
}

void from_json(const nlohmann::ordered_json& j, Troop::Member& m) {
  m.enemyId = j.value("enemyId", m.enemyId);
  m.x = j.value("x", m.x);
  m.y = j.value("y", m.y);
  m.hidden = j.value("hidden", m.hidden);
}

void to_json(nlohmann::ordered_json& j, const Troop& t) {
  j = {
      {"id", t.m_id},
      {"members", t.m_members},
      {"name", t.m_name},
      {"pages", t.m_pages},
  };
}

void from_json(const nlohmann::ordered_json& j, Troop& t) {
  t.m_id = j.value("id", t.m_id);
  t.m_members = j.value("members", t.m_members);
  t.m_name = j.value("name", t.m_name);
  t.m_pages = j.value("pages", t.m_pages);
}