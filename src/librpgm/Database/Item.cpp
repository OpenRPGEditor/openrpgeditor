#include "Database/Item.hpp"
void to_json(nlohmann::ordered_json& json, const Item& item) {
  json = {
      {"id", item.m_id},           {"animationId", item.m_animationId}, {"consumable", item.m_consumable}, {"damage", item.m_damage},   {"description", item.m_description},
      {"effects", item.m_effects}, {"hitType", item.m_hitType},         {"iconIndex", item.m_iconIndex},   {"itypeId", item.m_itypeId}, {"name", item.m_name},
      {"note", item.m_note},       {"occasion", item.m_occasion},       {"price", item.m_price},           {"repeats", item.m_repeats}, {"scope", item.m_scope},
      {"speed", item.m_speed},     {"successRate", item.m_successRate}, {"tpGain", item.m_tpGain},
  };
}
void from_json(const nlohmann::ordered_json& json, Item& item) {
  item.m_id = json.value("id", item.m_id);
  item.m_animationId = json.value("animationId", item.m_animationId);
  item.m_consumable = json.value("consumable", item.m_consumable);
  item.m_damage = json.value("damage", item.m_damage);
  item.m_description = json.value("description", item.m_description);
  item.m_effects = json.value("effects", item.m_effects);
  item.m_hitType = json.value("hitType", item.m_hitType);
  item.m_iconIndex = json.value("iconIndex", item.m_iconIndex);
  item.m_itypeId = json.value("itypeId", item.m_itypeId);
  item.m_name = json.value("name", item.m_name);
  item.m_note = json.value("note", item.m_note);
  item.m_occasion = json.value("occasion", item.m_occasion);
  item.m_price = json.value("price", item.m_price);
  item.m_repeats = json.value("repeats", item.m_repeats);
  item.m_scope = json.value("scope", item.m_scope);
  item.m_speed = json.value("speed", item.m_speed);
  item.m_successRate = json.value("successRate", item.m_successRate);
  item.m_tpGain = json.value("tpGain", item.m_tpGain);
}
