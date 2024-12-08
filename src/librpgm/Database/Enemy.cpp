#include "Enemy.hpp"

void to_json(nlohmann::ordered_json& to, const Enemy& enemy) {
  to = {
      {"id", enemy.m_id},
      {"actions", enemy.m_actions},
      {"battlerHue", enemy.m_battlerHue},
      {"battlerName", enemy.m_battlerName},
      {"dropItems", enemy.m_dropItems},
      {"exp", enemy.m_exp},
      {"traits", enemy.m_traits},
      {"gold", enemy.m_gold},
      {"name", enemy.m_name},
      {"note", enemy.m_note},
      {"params", enemy.m_params},
  };
}
void from_json(const nlohmann::ordered_json& from, Enemy& enemy) {
  enemy.m_id = from.value("id", enemy.m_id);
  enemy.m_actions = from.value("actions", enemy.m_actions);
  enemy.m_battlerHue = from.value("battlerHue", enemy.m_battlerHue);
  enemy.m_battlerName = from.value("battlerName", enemy.m_battlerName);
  enemy.m_dropItems = from.value("dropItems", enemy.m_dropItems);
  enemy.m_exp = from.value("exp", enemy.m_exp);
  enemy.m_traits = from.value("traits", enemy.m_traits);
  enemy.m_gold = from.value("gold", enemy.m_gold);
  enemy.m_name = from.value("name", enemy.m_name);
  enemy.m_note = from.value("note", enemy.m_note);
  enemy.m_params = from.value("params", enemy.m_params);
}
void to_json(nlohmann::ordered_json& to, const Enemy::Action& action) {
  to = {
      {"conditionParam1", action.conditionParam1}, {"conditionParam2", action.conditionParam2}, {"conditionType", action.conditionType}, {"rating", action.rating}, {"skillId", action.skillId},
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