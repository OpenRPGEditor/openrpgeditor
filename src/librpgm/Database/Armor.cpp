#include "Database/Armor.hpp"

void to_json(nlohmann::ordered_json& out, const Armor& armor) {
  out = {
      {"id", armor.m_id},     {"atypeId", armor.m_atypeId}, {"description", armor.m_description}, {"etypeId", armor.m_etypeId}, {"traits", armor.m_traits}, {"iconIndex", armor.m_iconIndex},
      {"name", armor.m_name}, {"note", armor.m_note},       {"params", armor.m_params},           {"price", armor.m_price},
  };
}

void from_json(const nlohmann::ordered_json& from, Armor& armor) {
  armor.m_id = from.value("id", armor.m_id);
  armor.m_atypeId = from.value("atypeId", armor.m_atypeId);
  armor.m_description = from.value("description", armor.m_description);
  armor.m_etypeId = from.value("etypeId", armor.m_etypeId);
  armor.m_traits = from.value("traits", armor.m_traits);
  armor.m_iconIndex = from.value("iconIndex", armor.m_iconIndex);
  armor.m_name = from.value("name", armor.m_name);
  armor.m_note = from.value("note", armor.m_note);
  armor.m_params = from.value("params", armor.m_params);
  armor.m_price = from.value("price", armor.m_price);
}
